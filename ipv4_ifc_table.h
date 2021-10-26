/**
 * File name: ipv4_ifc_table.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of ipv4_ifc_table.c
 *
 * Function list:
 * 		int ipv4_ifc_reg(int(*ifc_ptr)(), char *name):
 *			the interface of function ipv4_ifc_reg() in ipv4_ifc_table.c
 *			To register interfaces.
 *
 *		int ipv4_ifc_call(int ID, void *data, int len, int gateway):
 *			the interface of function ipv4_ifc_call() in ipv4_ifc_table.c
 *			To call interfaces.
 *			called in ipv4_send() in ipv4_send.c
 *
 **/

#ifndef __IPV4_IFC_TABLE_H__
#define __IPV4_IFC_TABLE_H__

int ipv4_ifc_reg(int(*ifc_ptr)(), char *name);
int ipv4_ifc_call(int ID, void *data, int len, int gateway);

#endif