/**
 * File name: ipv4_ecplt.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of ipv4_ecplt.c
 *
 * Function list:
 * 		void* ipv4_ecplt(void *data, int len, unsigned int sAddr,
 *				unsigned int dAddr, int ptcl):
 *			the interface of function ipv4_ecplt() in ipv4_ecplt.c.
 *			called by ipv4_send() in ipv4_send.c
 *
 **/

#ifndef __IPV4_ENCPLT_H__
#define __IPV4_ENCPLT_H__

void* ipv4_ecplt(void *data, int len, unsigned int sAddr,
                 unsigned int dAddr, int ptcl);

#endif