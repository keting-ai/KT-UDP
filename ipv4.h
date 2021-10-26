/**
 * File name: ipv4.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This file defines the ipv4 head structure and important macros.
 *				The overall IP functionality is shown by the figure below
 *
 *					--------------------------------------------
 *				   |    Upper layer protocols(TCP, UDP, etc.)   |
 *					--------------------------------------------
 *						   ^						     |
 *						   |						     v
 *					   ---------	   ---------       ------
 *					  | receive |---->| forward |---->| send |
 *					   ---------	   ---------	   ------
 *						   ^							 |
 *						   |							 v
 *					--------------------------------------------
 *				   |    Lower layer protocols(PPP, eth, etc.)	|
 *				    --------------------------------------------
 *
 */

#ifndef __IPV4_H__
#define __IPV4_H__

#define IPV4_VERS 4
#define IPV4_MIN_IHL 5
#define IPV4_CONTROL 0 /* AKA ToS, type of service. This program doesn't realize ToS */
#define IPV4_MAX_PROTOCOL 255 /* Due to the length of the protocol bytefield, it cannot exceed 255 */
#define IPV4_UNFRAG_OFFSET 0x4000 /* This program doesn't fragement */
#define IPV4_FRAG_OFFSET 0x0000
#define IPV4_TTL 64 /* Same as Linux OS */
#define IPV4_HEADLEN 20 /* Fixed head length since this program doesn't realize IP option */

/* Structure of ipv4 header, without option */
typedef struct ipv4_head {

    /* Little endian bytefield machine, ihl has to be at the first */
    unsigned ihl : 4; /* Header length, count in 4 bytes */
    unsigned version : 4;
    unsigned control : 8; /* AKA type of servise*/
    unsigned len : 16;
    unsigned ident : 16;
    unsigned fragoffset : 16; /* Fragement offset field*/
    unsigned ttl : 8; /* Time to Live */
    unsigned protocol : 8;
    unsigned checksum : 16;
    unsigned srcAddr : 32;
    unsigned dstAddr : 32;
} ipv4_head;

#endif
