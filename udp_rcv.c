/**
 * File name: ipv4_rcv.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module realizes the UDP receive functionality.
 * Data transmitted from lower layer(IP) are then processed
 * by this module.
 *
 **/

#include <stdio.h>
#include <string.h>

#include <udp_head.h>
#include <ipv4.h>
#include <udp_checksum.h>
#include <udp_port_table.h>

extern int DEBUG;

/*
 * This function receives UDP datagram.
 * It justifies the legality of UDP datagram and transmits it to upper layers(SQL, NFS, etc.)
 *
 * param void *data: the transmitted UDP datagram
 * param int length: the length of the UDP datagram
 * param void *ipData: the IP datagram for checksum calculation
 *
 * return: 0 for successfully transmitted to upper layer.
 *		   -1 for transmission problem.
 *
 */
int udp_rcv(void *data, int length, void *ipData) {

    if (data == NULL) {
        printf("Error: udp_rcv()\n line %d: data == NULL(No data).\n", __LINE__ - 1);
        return -1;
    }

    ipv4_head *ipHead;
    udp_head_pseudo *udp_phdr;
    udp_head_real *udp_rhdr;
    unsigned char *dt;
    unsigned int sAddr;
    unsigned int dAddr;
    unsigned int prtcl;
    unsigned int iplen;

    ipHead = ipData;
    sAddr = ipHead -> srcAddr;
    dAddr = ipHead -> dstAddr;
    prtcl = ipHead -> protocol;
    iplen = ipHead -> len;

    dt = data;
    // initialize udp header field
    void *temp = dt - 12;
    udp_phdr = temp;
    // set the pseudo header field zero
    memset(udp_phdr, 0, 12);
    udp_rhdr = data;

    // set udp header fields
    udp_phdr -> udp_srcAddr = sAddr;
    udp_phdr -> udp_dstAddr = dAddr;
    udp_phdr -> zeros = 0;
    udp_phdr -> udp_protocol = prtcl;
    // here, if we use length, udp_phdr -> udp_len is not in internet byte order
    udp_phdr -> udp_len = udp_rhdr -> udpLen;
    // now udp_phdr and udp_rhdr are in internet byte order

    // justify the checksum
    if(!udp_checksum_justify(udp_phdr, length + UDP_HEAD_PSEUDO_LEN)) {
        printf("Error: udp_rcv()\n line %d: checksum incorrect.\n", __LINE__ - 1);
        return -1;
    }

    // internet to host byte order
    udp_phdr -> udp_srcAddr = ((udp_phdr -> udp_srcAddr & 0x000000ff) << 24 ) | ((udp_phdr -> udp_srcAddr & 0x0000ff00) << 8)
                              | ((udp_phdr -> udp_srcAddr & 0x00ff0000) >> 8) | ((udp_phdr -> udp_srcAddr & 0xff000000 ) >> 24);
    udp_phdr -> udp_dstAddr = ((udp_phdr -> udp_dstAddr & 0x000000ff) << 24 ) | ((udp_phdr -> udp_dstAddr & 0x0000ff00) << 8)
                              | ((udp_phdr -> udp_dstAddr & 0x00ff0000) >> 8) | ((udp_phdr -> udp_dstAddr & 0xff000000 ) >> 24);
    udp_phdr -> udp_len = (udp_phdr -> udp_len << 8) | (udp_phdr -> udp_len >> 8);
    udp_rhdr -> srcPort = (udp_rhdr -> srcPort << 8) | (udp_rhdr -> srcPort >> 8);
    udp_rhdr -> dstPort = (udp_rhdr -> dstPort << 8) | (udp_rhdr -> dstPort >> 8);
    udp_rhdr -> udpLen = (udp_rhdr -> udpLen << 8) | (udp_rhdr -> udpLen >> 8);
    udp_rhdr -> udpCks = (udp_rhdr -> udpCks << 8) | (udp_rhdr -> udpCks >> 8);
    // now they are host byte order

    // check the correctness of length
    if (udp_rhdr -> udpLen != length || udp_rhdr -> udpLen < UDP_HEAD_REAL_LEN) {
        printf("Error: udp_rcv()\n line %d: length field incorrect.\n", __LINE__ - 1);
        return -1;
    }

    // check the protocol correctness
    if (udp_phdr -> udp_protocol != UDP_HEAD_PRTCL) {
        printf("Error: udp_rcv()\n line %d: protocol incorrect.\n", __LINE__ - 1);
        return -1;
    }

    // send to upper layer
    if (DEBUG) {
        printf("OK: udp_rcv()\n line %d: call udp_port_call().\n", __LINE__ - 1);
    }
    int ret;
    ret = udp_port_call(udp_rhdr -> dstPort, dt + UDP_HEAD_REAL_LEN, udp_rhdr -> udpLen - UDP_HEAD_REAL_LEN);
    if (ret == -1) {
        printf("Error: udp_rcv()\n line %d: call UDP port failed.\n", __LINE__ - 1);
        return -1;
    }

    return 0;
}
