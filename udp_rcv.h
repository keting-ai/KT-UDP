/**
 * File name: udp_rcv.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of udp_rcv.c
 *
 * Function list:
 * 		int udp_rcv(void *data, int length, void *ipData):
 *			the interface of function udp_rcv() in ipv4_rcv.c
 *			To receive UDP datagram.
 *
 **/

#ifndef __UDP_RCV_H__
#define __UDP_RCV_H__

int udp_rcv(void *data, int length, void *ipData);

#endif