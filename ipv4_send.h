/**
 * File name: ipv4_send.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of ipv4_send.c
 *
 * Function list:
 * 		int ipv4_send(void *data, int length, unsigned int sAddr,
 *				unsigned int dAddr, int prtcl):
 *			the interface of function ipv4_send() in ipv4_send.c.
 *
 *		ipv4_send_helper(void *ipData, int length, int gw, int ifc):
 *			the interface of function ipv4_send_helper() in ipv4_ecplt.c.
 *			called by ipv4_send() in ipv4_send.c
 *			called by ipv4_forward() int ipv4_forward.c
 *
 **/

#ifndef __IPV4_SEND_H__
#define __IPV4_SEND_H__

int ipv4_send(void *data, int length, unsigned int sAddr,
              unsigned int dAddr, int prtcl);
int ipv4_send_helper(void *ipData, int length, int gw, int ifc);

#endif