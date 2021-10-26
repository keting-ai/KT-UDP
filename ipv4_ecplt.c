/**
 * File name: ipv4_ecplt.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module realizes the IP encapsulate functionality.
 * This module adds IP header to IP data.
 *
 */

#include <stdio.h>
#include <string.h>

#include <ipv4.h>

#define IPV4_ECPLT_MTU 1500 // MTU is set to 1500

extern int DEBUG;
unsigned int ipv4_ecplt_time = 0; // the timer to set the IP identification field

/*
 * This function encapsulates IP data.
 * It justifies the legality of IP data and encapsulates the IP header
 *
 * param void *ipData: the IP data without header
 * param int length: the length of the IP data without head
 * param unsigned int sAddr: the source IP address
 * param unsigned int dAddr: the destination IP address
 * param int prtcl: the upper layer protocol number
 *
 * return the void pointer of the encapsulated IP datagram.
 *		   NULL for encapsulating failure.
 *
 */
void* ipv4_ecplt(void *data, int len, unsigned int sAddr,
                 unsigned int dAddr, int prtcl) {

    unsigned char *p;
    void *ipData;
    ipv4_head *head;

    // legality check
    if (prtcl > IPV4_MAX_PROTOCOL) {
        printf("Error: ipv4_ecplt_encapsulate().\n line %d: protocol > 255.\n", __LINE__ - 1);
        return NULL;
    }

    if (len + IPV4_HEADLEN > IPV4_ECPLT_MTU) {
        printf("Error: ipv4_ecplt_encapsulate().\n line %d: length too large.\n", __LINE__ - 1);
        return NULL;
    }

    p = data;
    ipData = p - IPV4_HEADLEN;
    memset(ipData, 0, 20); // set header fields zero first
    head = ipData;

    head -> version = IPV4_VERS;
    head -> ihl = IPV4_MIN_IHL;

    head -> control = IPV4_CONTROL;
    head -> len = len + IPV4_HEADLEN;

    // the counter is 16 bits recurrent. Set to zero when reaching 0xffff.
    if (ipv4_ecplt_time == 0xffff) {
        ipv4_ecplt_time = 0;
    }
    head -> ident = ipv4_ecplt_time;
    ipv4_ecplt_time += 1;

    // this program haven't realized fragement yet. Add something to realize fragement.
    head -> fragoffset = IPV4_UNFRAG_OFFSET;

    head -> ttl = IPV4_TTL;
    head -> protocol = prtcl;
    head -> srcAddr = sAddr;
    head -> dstAddr = dAddr;

    // calculate checksum after turnning into internet byte sequence
    // in ipv4_send_helper() in file ipv4_send.c
    
    if (DEBUG) {
        printf("OK: ipv4_ecplt()\n line %d: IP datagram encapsulate complete.\n", __LINE__ - 1);
    }
    return ipData;

}
