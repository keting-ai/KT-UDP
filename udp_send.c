/**
 * File name: udp_send.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module realizes the UDP send functionality.
 * Data transmitted from upper layer(SQL, NFS, etc.) are then processed
 * and sent by this module.
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <udp_head.h>
#include <udp_checksum.h>

extern int DEBUG;

/*
 * This function sends UDP datagram.
 * It receives the UDP data and encapsulates it into UDP datagram. Then sends to lower layers.
 *
 * param void *data: the received UDP data
 * param int length: the length of the UDP data
 * param unsigned int sPort: the source port
 * param unsigned int dPort: the destination port
 * param unsigned int sAddr: the source IP address
 * param unsigned int dAddr: the destination IP address
 * param int (*low_layer_send)(): the pointer to the lower layer(IP) receive function
 *
 * return: 0 for successfully sending to lower layers.
 *		   -1 for sending problem.
 *
 */
int udp_send(void *data, int len, unsigned int sPort, unsigned int dPort,
             unsigned int sAddr, unsigned int dAddr, int (*low_layer_send)()) {

    if (data == NULL) {
        printf("Error: udp_send()\n line %d: data == NULL(No data).\n", __LINE__ - 1);
        return -1;
    }
    
    if (len < 0) {
        printf("Error: udp_send()\n line %d: length too short.\n", __LINE__ - 1);
        return -1;
    }

    unsigned char *p;
    void *temp;
    udp_head_pseudo *phead;
    udp_head_real *rhead;
    int retVal;

    // initialize pointers to pseudo head and real head
    p = data;
    temp = p - UDP_HEAD_REAL_LEN;
    rhead = temp;
    temp = p - UDP_HEAD_REAL_LEN - UDP_HEAD_PSEUDO_LEN;
    phead = temp;
    // set the pseudo head and real head fields to zero
    memset(temp, 0, 20);

    phead -> udp_srcAddr = sAddr;
    phead -> udp_dstAddr = dAddr;
    phead -> zeros = 0;
    phead -> udp_protocol = UDP_HEAD_PRTCL;
    phead -> udp_len = len + UDP_HEAD_REAL_LEN;

    rhead -> srcPort = sPort;
    rhead -> dstPort = dPort;
    rhead -> udpLen = len + UDP_HEAD_REAL_LEN;
    rhead -> udpCks = 0;

    // host to internet byte sequence
    phead -> udp_srcAddr = ((phead -> udp_srcAddr & 0x000000ff) << 24 ) | ((phead -> udp_srcAddr & 0x0000ff00) << 8)
                           | ((phead -> udp_srcAddr & 0x00ff0000) >> 8) | ((phead -> udp_srcAddr & 0xff000000 ) >> 24);
    phead -> udp_dstAddr = ((phead -> udp_dstAddr & 0x000000ff) << 24 ) | ((phead -> udp_dstAddr & 0x0000ff00) << 8)
                           | ((phead -> udp_dstAddr & 0x00ff0000) >> 8) | ((phead -> udp_dstAddr & 0xff000000 ) >> 24);
    phead -> udp_len = (phead -> udp_len << 8) | (phead -> udp_len >> 8);
    rhead -> srcPort = (rhead -> srcPort << 8) | (rhead -> srcPort >> 8);
    rhead -> dstPort = (rhead -> dstPort << 8) | (rhead -> dstPort >> 8);
    rhead -> udpLen = (rhead -> udpLen << 8) | (rhead -> udpLen >> 8);
    // checksum is calculated in internet byte sequence, so we have to change first

    rhead -> udpCks = udp_checksum_calculate(phead, len + UDP_HEAD_REAL_LEN + UDP_HEAD_PSEUDO_LEN);
    // now change the udpCks field to internet byte sequence
    rhead -> udpCks = (rhead -> udpCks << 8) | (rhead -> udpCks >> 8);

    // send to the lower layer protocol(IP)
    if (DEBUG) {
        printf("OK: udp_send()\n line %d: call low_layer_send().\n", __LINE__ - 1);
    }
    retVal = low_layer_send(rhead, len + UDP_HEAD_REAL_LEN, sAddr, dAddr, UDP_HEAD_PRTCL);
    if (retVal == -1) {
        printf("Error: udp_send()\n line %d: call lower layer function failed.\n", __LINE__ - 1);
        return -1;    
    }
    return 0;
}
