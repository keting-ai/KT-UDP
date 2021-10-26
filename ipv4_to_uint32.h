/**
 * File name: ipv4_to_uint32.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: the header file of ipv4_to_uint32.c
 *
 * Function list:
 *      unsigned int ipv4_to_uint32(char *ip):
 *          the interface of function ipv4_to_uint32() in ipv4_to_uint32.c
 *          To transform IP address from "X.X.X.X" to integer
 *
 **/

#ifndef __IPV4_TO_UINT32_H__
#define __IPV4_TO_UINT32_H__

unsigned int ipv4_to_uint32(char *ip);

#endif