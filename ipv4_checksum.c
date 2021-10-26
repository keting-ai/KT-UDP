/**
 * File name: ipv4_checksum.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module does ipv4 header checksum justification and calculation.
 *
 **/

#include <stdio.h>
extern int DEBUG;

/*
 * This function helps justify the IP datagram checksum.
 *
 * param void *ipData: the received IP datagram
 *
 * return 1 for checksum correct, 0 otherwise.
 *
 */
int ipv4_checksum_justify (void *ipData) {

    unsigned short *head;
    unsigned int sum = 0;
    unsigned int ret = 0;

    head = ipData;

    /* According to RFC791, ipv4 calculate checksum in header field,
     * so the checksum calculation includes the first 20 bytes.
     * This program doesn't realize IP options yet, make changes when realizing IP options.
     */
    for (int i = 0; i < 10; ++i) {
        // We have to calculate checksum in internet byte order
        head[i] = (head[i] << 8) | (head[i] >> 8);
        sum += head[i];
        // If there's a carry, add it to the back
        if (sum >> 16 != 0) {
            sum += sum >> 16;
            sum = sum & 0xffff;
        }
        // Change back
        head[i] = (head[i] << 8) | (head[i] >> 8);
    }

    // sum is hopefully 0xffff, ~sum is then hopefully 0
    ret = ~sum & 0xffff;
    if (DEBUG) {
        printf("OK: ipv4_checksum_justify()\n line %d: checksum justification complete.\n", __LINE__ - 1);
    }
    return (ret == 0) ? 1 : 0;
}

/*
 * This function helps calculate the IP datagram checksum.
 *
 * param void *ipData: the received IP datagram
 *
 * return unsigned int checksum.
 *
 */
unsigned int ipv4_checksum_calculate (void *ipData) {

    unsigned short *head;
    unsigned int ret = 0;

    head = ipData;

    /* The algorithm is similar to that in ipv4_checksum_justify() */
    for (int i = 0; i < 10; ++i) {
        head[i] = (head[i] << 8) | (head[i] >> 8);
        ret += head[i];
        if (ret >> 16 != 0) {
            ret += ret >> 16;
            ret = ret & 0xffff;
        }
        head[i] = (head[i] << 8) | (head[i] >> 8);
    }

    ret = ~ret;
    ret = ret & 0xffff;
    
    if (DEBUG) {
        printf("OK: ipv4_checksum_calculate()\n line %d: checksum calculation complete.\n", __LINE__ - 1);
    }
    return ret;
}