/**
 * File name: udp_checksum.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module does UDP datagram checksum justification and calculation.
 *
 **/

#include <stdio.h>
#include <udp_head.h>

extern int DEBUG;

/*
 * This function helps justify the UDP datagram checksum.
 *
 * param void *udpData: the received UDP datagram
 * param int len: the length of the UDP datagram
 *
 * return: 1 for checksum correct, 0 otherwise.
 *
 */
int udp_checksum_justify (void *udpData, int len) {

    unsigned short *data;
    unsigned int sum = 0;
    unsigned int ret = 0;

    data = udpData;

    /* The algorithm is similar to that in ipv4_checksum.c,
     * but different due to UDP length
     */
    for (int i = 0; i < len / 2; ++i) {
        data[i] = (data[i] << 8) | (data[i] >> 8);
        sum += data[i];
        if (sum >> 16 != 0) {
            sum += sum >> 16;
            sum = sum & 0xffff;
        }
        data[i] = (data[i] << 8) | (data[i] >> 8);
    }

    /* when len is odd, we have to process the last byte individually */
    if (len % 2 != 0) {
        unsigned char *dtOdd = udpData;
        // use char to gain the last one byte number
        unsigned int temp = dtOdd[len - 1];
        // assume there's another zero byte after the last byte
        temp = temp << 8;
        sum += temp;
        if (sum >> 16 != 0) {
            sum += sum >> 16;
            sum = sum & 0xffff;
        }
    }

    ret = ~sum & 0xffff;

    if (DEBUG) {
        printf("OK: udp_checksum_justify()\n line %d: justification complete.\n", __LINE__ - 1);
    }
    return (ret == 0) ? 1 : 0;

}

/*
 * This function helps calculate the UDP datagram checksum.
 *
 * param void *udpData: the received UDP datagram
 * param int len: the length of the UDP datagram
 *
 * return: unsigned int checksum.
 *
 */
unsigned int udp_checksum_calculate (void *udpData, int len) {

    unsigned short *data;
    unsigned int sum = 0;
    unsigned int ret = 0;

    data = udpData;

    /* The algorithm is similar to that in udp_checksum_justify() */
    for (int i = 0; i < len / 2; ++i) {
        data[i] = (data[i] << 8) | (data[i] >> 8);
        sum += data[i];
        if (sum >> 16 != 0) {
            sum += sum >> 16;
            sum = sum & 0xffff;
        }
        data[i] = (data[i] << 8) | (data[i] >> 8);
    }

    // len is odd
    if (len % 2 != 0) {
        unsigned char *dtOdd = udpData;
        unsigned int temp = dtOdd[len - 1];
        temp = temp << 8;
        sum += temp;
        if (sum >> 16 != 0) {
            sum += sum >> 16;
            sum = sum & 0xffff;
        }
    }

    ret = ~sum & 0xffff;

    if (DEBUG) {
        printf("OK: udp_checksum_calculate()\n line %d: calculation complete.\n", __LINE__ - 1);
    }
    return ret;

}