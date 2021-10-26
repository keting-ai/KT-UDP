/**
 * File name: ipv4_send.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module realizes the IP send functionality.
 * Data transmitted from upper layer(TCP, UDP, etc.) are then processed
 * and sent by this module.
 *
 **/

#include <stdio.h>

#include <ipv4.h>
#include <ipv4_ecplt.h>
#include <ipv4_checksum.h>
#include <routeTable.h>
#include <ipv4_ifc_table.h>

extern int DEBUG;

int ipv4_send_helper(void *ipData, int length, int gw, int ifc);

/*
 * This function sends IP datagram.
 * It receives the IP data and encapsulates it into IP datagram. Then sends to lower layers.
 *
 * param void *data: the received IP data
 * param int length: the length of the IP data
 * param unsigned int sAddr: the source IP address
 * param unsigned int dAddr: the destination IP address
 * param int prtcl: the upper layer protocol number
 *
 * return: 0 for successfully sending to lower layers.
 *		   -1 for sending problem.
 *
 */
int ipv4_send(void *data, int length, unsigned int sAddr,
              unsigned int dAddr, int prtcl) {

    // NULL pointer exception
    if (data == NULL) {
        printf("Error: ipv4_send()\n Line %d: data == NULL(No data).\n", __LINE__ - 1);
        return -1;
    }

    unsigned int *rt;
    void *ipData;
    int retVal;
    // ipData is encapsulated by ipv4_ecplt() in ipv4_ecplt.c
    if (DEBUG) {
        printf("OK: ipv4_send()\n line %d: call ipv4_ecplt().\n", __LINE__ - 1);
    }
    ipData = ipv4_ecplt(data, length, sAddr, dAddr, prtcl);
    if (ipData == NULL) {
        printf("Error: ipv4_send()\n Line %d: ipData == NULL(encapsulate problem).\n", __LINE__ - 1);
        return -1;
    }

    // get route from routeTable_get() in routeTable.c
    if (DEBUG) {
        printf("OK: ipv4_send()\n line %d: call routeTable_get().\n", __LINE__ - 1);
    }
    rt = routeTable_get(dAddr);
    if (rt == NULL) {
        printf("Error: ipv4_send()\n Line %d: can't get route. Drop packet\n", __LINE__ - 1);
        return -1;
    }

    // ipv4_send_helper() sends the datagram to lower layers
    if (DEBUG) {
        printf("OK: ipv4_send()\n line %d: call ipv4_send_helper().\n", __LINE__ - 1);
    }
    retVal = ipv4_send_helper(ipData, length + IPV4_HEADLEN, rt[3], rt[4]);
    if (retVal == -1) {
        printf("Error: ipv4_send()\n Line %d: Drop packet\n", __LINE__ - 1);
    }

    return 0;
}

/*
 * This function helps send IP datagram.
 * It receives the IP datagram and sends to lower layers.
 *
 * param void *ipData: the received IP datagram
 * param int length: the length of the IP datagram
 * param int gw: the gateway
 * param int ifc: the lower layer interface
 *
 * return: 0 for successfully sending to lower layers.
 *
 */
int ipv4_send_helper(void *ipData, int length, int gw, int ifc) {

    ipv4_head *head = ipData;
    int retVal;
    // host to internet byte sequence to calulate the checksum
    head -> len = (head -> len << 8) | (head -> len >> 8);
    head -> ident = (head -> ident << 8) | (head -> ident >> 8);
    head -> fragoffset = (head -> fragoffset << 8) | (head -> fragoffset >> 8);
    head -> srcAddr = ((head -> srcAddr & 0x000000ff) << 24 ) + ((head -> srcAddr & 0x0000ff00) << 8)
                      + ((head -> srcAddr & 0x00ff0000) >> 8) + ((head -> srcAddr & 0xff000000 ) >> 24);
    head -> dstAddr = ((head -> dstAddr & 0x000000ff) << 24 ) + ((head -> dstAddr & 0x0000ff00) << 8)
                      + ((head -> dstAddr & 0x00ff0000) >> 8) + ((head -> dstAddr & 0xff000000 ) >> 24);

    void *ptr;
    ptr = head;
    head -> checksum = ipv4_checksum_calculate(ptr);
    // change the checksum to internet byte field
    head -> checksum = (head -> checksum << 8) + (head -> checksum >> 8);
    // call function in the register interface list
    if (DEBUG) {
        printf("OK: ipv4_send_helper()\n line %d: call ipv4_ifc_call().\n", __LINE__ - 1);
    }

    retVal = ipv4_ifc_call(ifc, ipData, length, gw);
    if (retVal == -1) {
        printf("Error: ipv4_send_helper()\n Line %d: call interface failed\n", __LINE__ - 1);
        return -1;
    }

    return 0;
}
