/**
 * File name: udp_port_table.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of udp_port_table.c
 *
 * Function list:
 * 		int udp_port_reg(int(*port_ptr)(), char *name, int portNum):
 *			the interface of function udp_port_reg() in udp_port_table.c
 *			To register port functions.
 *
 *		int udp_port_call(int portNum, void *data, int len):
 *			the interface of function udp_port_call() in udp_port_table.c
 *			To call port functions.
 *			called in udp_send() in udp_send.c
 *
 **/

#ifndef __UDP_PORT_TABLE_H__
#define __UDP_PORT_TABLE_H__

int udp_port_reg(int(*port_ptr)(), char *name, int portNum);
int udp_port_call(int portNum, void *data, int len);

#endif