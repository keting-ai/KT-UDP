/**
 * File name: udp_head.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This file defines the udp head structures, including pseudo head and real head,
 *				and important macros.
 *				The overall UDP functionality is shown by the figure below
 *
 *					--------------------------------------------
 *				   |    Upper layer protocols(SQL, NFS, etc.)   |
 *					--------------------------------------------
 *						   ^						     |
 *						   |						     v
 *					   ---------	                   ------
 *					  | receive |                     | send |
 *					   ---------	            	   ------
 *						   ^							 |
 *						   |							 v
 *					--------------------------------------------
 *				   |   Lower layer protocols(IP, ICMP, etc.)	|
 *				    --------------------------------------------
 *
 */

#ifndef __UDP_HEAD_H__
#define __UDP_HEAD_H__

#define UDP_HEAD_PSEUDO_LEN 12 /* UDP pseudo head length */
#define UDP_HEAD_REAL_LEN 8 /* UDP real head length */
#define UDP_HEAD_PRTCL 17 /* UDP protocol number specified by RFC768 */

/* Structure of UDP pseudo head */
typedef struct udp_head_pseudo {

    unsigned udp_srcAddr : 32; /* source IP address */
    unsigned udp_dstAddr : 32; /* destination IP address */
    unsigned zeros : 8; /* This field is set to all zeros */
    unsigned udp_protocol : 8;
    unsigned udp_len : 16;
} udp_head_pseudo;

/* Structure of UDP real head */
typedef struct udp_head_real {

    unsigned srcPort : 16; /* source application port number */
    unsigned dstPort : 16; /* destination application port number*/
    unsigned udpLen : 16;
    unsigned udpCks : 16;
} udp_head_real;

#endif