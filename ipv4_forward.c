/**
 * File name: ipv4_forward.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module realizes the IP forward functionality.
 * IP datagrams that are not sent to this machine are retransmitted by this module.
 *
 */

#include <stdio.h>

#include <ipv4.h>
#include <ipv4_send.h>
#include <routeTable.h>

extern int DEBUG;

/*
 * This function forwards IP datagram.
 * It receives the IP datagrams and resends to lower layers.
 *
 * param void *ipdata: the received IP datagram
 * param int length: the length of the IP datagram
 *
 * return 0 for successfully resending to ipv4_send_helper() in ipv4_send.c.
 *		   -1 for resending problem.
 *
 */
int ipv4_forward(void *ipData, int length) {

    unsigned int *rt;
    ipv4_head *head = ipData;
    void *ptr = head;
    int retVal;

    head -> ttl -= 1;
    head -> checksum = 0;

    // get route
    if (DEBUG) {
        printf("OK: ipv4_forward()\n line %d: get route.\n", __LINE__ - 1);
    }
    
    rt = routeTable_get(head -> dstAddr);
    if (rt == NULL || rt[0] == 0) {
        printf("Error: ipv4_forward()\n line %d: can't find route.\n", __LINE__ - 1);
        return -1;
    }

    // calculate checksum after turnning into internet byte sequence
    // in ipv4_send_helper() function.
    if (DEBUG) {
        printf("OK: ipv4_forward()\n line %d: call ipv4_send_helper().\n", __LINE__ - 1);
    }
    
    retVal = ipv4_send_helper(ptr, head -> len, rt[3], rt[4]);
    if (retVal == -1) {
        printf("Error: ipv4_forward()\n line %d: sending problem.\n", __LINE__ - 1);
        return -1;
    }

    return 0;
}
