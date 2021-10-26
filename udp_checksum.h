/**
 * File name: udp_checksum.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of udp_checksum.c
 *
 * Function list:
 * 		int udp_checksum_justify (void *udpData, int len);:
 *			the interface of function udp_checksum_justify() in udp_checksum.c
 *			To justify the correctness of UDP datagram checksum field.
 *
 *		unsigned int udp_checksum_calculate (void *udpData, int len):
 *			the interface of function udp_checksum_calculate() in udp_checksum.c
 *			To calculate the checksum according to the whole UDP datagram.
 *
 **/

#ifndef __UDP_CHECKSUM_H__
#define __UDP_CHECKSUM_H__

int udp_checksum_justify (void *udpData, int len);
unsigned int udp_checksum_calculate (void *udpData, int len);

#endif