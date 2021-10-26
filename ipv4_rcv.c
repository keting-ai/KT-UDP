/**
 * File name: ipv4_rcv.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module realizes the IP receive functionality.
 * Data transmitted from lower layer(ethernet, PPP, etc.) are then processed
 * and sent to upper layers or resent to the internet by this module.
 *
 */

#include <stdio.h>

#include <ipv4.h>
#include <ipv4_ecplt.h>
#include <ipv4_forward.h>
#include <ipv4_checksum.h>
#include <ipv4_prtcl_table.h>

extern int DEBUG;

/*
 * This function receives IP datagram.
 * It justifies the legality of IP datagram and determines whether to transmit it to upper layers(TCP, UDP, etc.)
 * or forward the datagram to lower layers(eth, PPP, etc.).
 *
 * param void *ipData: the transmitted IP datagram
 * param int length: the length of the IP datagram
 * param unsigned int thisAddr: this machine's IP address
 *
 * return 0 for successfully transmitted to upper layer or to forward module.
 *		   -1 for transmission problem.
 *
 */
int ipv4_rcv(void *ipData, int length, unsigned int thisAddr) {

    void *ret;
    ipv4_head *head;
    head = ipData;
    unsigned char *p = ipData;
    int retVal;

    if (ipData == NULL) {
        printf("Error: ipv4_rcv()\n line %d: received message is NULL.\n", __LINE__ - 1);
        return -1;
    }

    // justify checksum
    if (ipv4_checksum_justify(ipData) == 0) {
        printf("Error: ipv4_rcv()\n line %d: checksum incorrect.\n", __LINE__ - 1);
        return -1;
    }

    // internet to host byte field for justifying IP header fields
    head -> len = (head -> len << 8) | (head -> len >> 8);
    head -> ident = (head -> ident << 8) | (head -> ident >> 8);
    head -> fragoffset = (head -> fragoffset << 8) | (head -> fragoffset >> 8);
    head -> checksum = (head -> checksum << 8) | (head -> checksum >> 8);
    head -> srcAddr = ((head -> srcAddr & 0x000000ff) << 24 ) | ((head -> srcAddr & 0x0000ff00) << 8)
                      | ((head -> srcAddr & 0x00ff0000) >> 8) | ((head -> srcAddr & 0xff000000 ) >> 24);
    head -> dstAddr = ((head -> dstAddr & 0x000000ff) << 24 ) | ((head -> dstAddr & 0x0000ff00) << 8)
                      | ((head -> dstAddr & 0x00ff0000) >> 8) | ((head -> dstAddr & 0xff000000 ) >> 24);

    if (length <= 46) {
        int count_zero = 0;
        for (int i = head -> len; i < length; ++i) {
            if (p[i] == 0) {
                count_zero++;
            }
        }
        length -= count_zero;
        p[head -> len] = '\0';
    }


    // verify correctness of IP header fields
    if (head -> version != IPV4_VERS) {
        printf("Error: ipv4_rcv()\n line %d: version != 4.\n", __LINE__ - 1);
        return -1;
    }

    if (head -> ihl < IPV4_MIN_IHL) {
        printf("Error: ipv4_rcv()\n line %d: ihl != 4.\n", __LINE__ - 1);
        return -1;
    }

    if (head -> len != length) {
        printf("Error: ipv4_rcv()\n line %d: len != length.\n", __LINE__ - 1);
        return -1;
    }

    if (head -> fragoffset != IPV4_UNFRAG_OFFSET && head -> fragoffset != IPV4_FRAG_OFFSET) {
        printf("Error: ipv4_rcv()\n line %d: flag != 2.\n", __LINE__ - 1);
        return -1;
    }

    if (head -> ttl - 1 < 0) {
        printf("Error: ipv4_rcv()\n line %d: ttl - 1 != 0.\n", __LINE__ - 1);
        return -1;
    }

    // not my package, forward the datagram
    if (head -> dstAddr != thisAddr) {
        if (DEBUG) {
            printf("OK: ipv4_rcv()\n line %d: call ipv4_forward().\n", __LINE__ - 1);
        }
        retVal = ipv4_forward(ipData, length);
        if (retVal == -1) {
            printf("Error: ipv4_rcv()\n line %d: Drop packet.\n", __LINE__ - 1);
            return -1;
        }
        return 0;
    }

    // is my package
    // send to upper layer...
    // host to internet byte sequence: to make calculating checksum and adding pseudo header in UDP/TCP easier
    head -> len = (head -> len << 8) | (head -> len >> 8);
    head -> ident = (head -> ident << 8) | (head -> ident >> 8);
    head -> fragoffset = (head -> fragoffset << 8) | (head -> fragoffset >> 8);
    head -> checksum = (head -> checksum << 8) | (head -> checksum >> 8);
    head -> srcAddr = ((head -> srcAddr & 0x000000ff) << 24 ) | ((head -> srcAddr & 0x0000ff00) << 8)
                      | ((head -> srcAddr & 0x00ff0000) >> 8) | ((head -> srcAddr & 0xff000000 ) >> 24);
    head -> dstAddr = ((head -> dstAddr & 0x000000ff) << 24 ) | ((head -> dstAddr & 0x0000ff00) << 8)
                      | ((head -> dstAddr & 0x00ff0000) >> 8) | ((head -> dstAddr & 0xff000000 ) >> 24);
	
    if (DEBUG) {
        printf("OK: ipv4_rcv()\n line %d: call ipv4_prtcl_call().\n", __LINE__ - 1);
    }
    retVal = ipv4_prtcl_call(head -> protocol, p + IPV4_HEADLEN, length - IPV4_HEADLEN, ipData);
    if (retVal == -1) {
        printf("Error: ipv4_rcv()\n line %d: Drop packet.\n", __LINE__ - 1);
        return -1;
    }
      
    return 0;
}
