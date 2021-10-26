/**
 * File name: ipv4_forward.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of ipv4_forward.c
 *
 * Function list:
 * 		int ipv4_forward(void *ipData, int length):
 *			the interface of function ipv4_forward() in ipv4_forward.c
 *			called by ipv4_rcv() in ipv4_rcv.c
 *
 **/

#ifndef __IPV4_FORWARD_H__
#define __IPV4_FORWARD_H__

#include <routeTable.h>
int ipv4_forward(void *ipData, int length);

#endif