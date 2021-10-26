/**
 * File name: ipv4_checksum.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of ipv4_checksum.c
 *
 * Function list:
 * 		int ipv4_checksum_justify (void *ipData);:
 *			the interface of function ipv4_checksum_justify() in ipv4_checksum.c
 *			To justify the correctness of IP datagram checksum field.
 *
 *		unsigned int ipv4_checksum_calculate (void *ipData):
 *			the interface of function ipv4_checksum_calculate() in ipv4_checksum.c
 *			To calculate the checksum according to IP datagram header.
 *
 **/

#ifndef __IPV4_CHECKSUM__
#define __IPV4_CHECKSUM__

int ipv4_checksum_justify (void *ipData);
unsigned int ipv4_checksum_calculate (void *ipData);

#endif