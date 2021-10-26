/**
 * File name: ipv4_prtcl_table.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of ipv4_prtcl_table.c
 *
 * Function list:
 * 		int ipv4_prtcl_reg(int(*func_ptr)(), int prtcl):
 *			the interface of function ipv4_ifc_reg() in ipv4_ifc_table.c
 *			To register protocol functions.
 *
 *		int ipv4_ifc_call(int ID, void *data, int len, int gateway):
 *			the interface of function ipv4_ifc_call() in ipv4_ifc_table.c
 *			To call protocol functions.
 *			called in ipv4_rcv_helper() in ipv4_rcv.c
 *
 **/

#ifndef __IPV4_PRTCL_TABLE_H__
#define __IPV4_PRTCL_TABLE_H__

int ipv4_prtcl_reg(int(*func_ptr)(), int prtcl);
int ipv4_prtcl_call(int prtcl, void *data, int len, void *ipData);

#endif