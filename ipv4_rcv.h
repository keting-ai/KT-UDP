/**
 * File name: ipv4_rcv.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of ipv4_rcv.c
 *
 * Function list:
 * 		int ipv4_rcv(void *ipData, int length, unsigned int thisAddr):
 *			the interface of function ipv4_rcv() in ipv4_rcv.c
 *			To receive IP datagram and send to upper layers
 *
 **/

#ifndef __IPV4_RCV_H__
#define __IPV4_RCV_H__

int ipv4_rcv(void *ipData, int length, unsigned int thisAddr);

#endif