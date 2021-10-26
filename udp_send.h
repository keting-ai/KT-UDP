/**
 * File name: udp_send.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of udp_send.c
 *
 * Function list:
 * 		int udp_send(void *data, int len, unsigned int sPort, unsigned int dPort,
 *				unsigned int sAddr, unsigned int dAddr, int (*low_layer_send)()):
 *			the interface of function udp_send() in udp_send.c.
 *			To send UDP datagram to lower layer protocol(IP)
 *
 **/

#ifndef __UDP_SEND_H__
#define __UDP_SEND_H__

int udp_send(void *data, int len, unsigned int sPort, unsigned int dPort,
             unsigned int sAddr, unsigned int dAddr, int (*low_layer_send)());

#endif