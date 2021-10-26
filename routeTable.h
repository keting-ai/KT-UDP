/**
 * File name: routeTable.h
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description: This is the header file of routeTable.c
 *
 * Function list:
 * 		int routeTable_store (unsigned int addr, unsigned int msk, unsigned int gw,
 *				unsigned int ifc, int mtc, int isdefault):
 *			the interface of function routeTable_store() in routeTable.c
 *			To store routes.
 *
 *		int routeTable_delete (unsigned int addr, unsigned int gw):
 *			the interface of routeTable_delete() in routeTable.c
 *			To call delete routes.
 *
 *		unsigned int* routeTable_get(unsigned int addr):
 *			the interface of routeTable_get() in routeTable.c
 *			To get route based on destination IP address
 *
 *		void routeTable_display(void):
 *			the interface of routeTable_display() int routeTable.c
 *			To display the current routeTable
 *
 **/

#ifndef __ROUTETABLE_H__
#define __ROUTETABLE_H__

int routeTable_store (unsigned int addr, unsigned int msk, unsigned int gw,
                      unsigned int ifc, int mtc, int isdefault);
int routeTable_delete (unsigned int addr, unsigned int gw);
unsigned int* routeTable_get(unsigned int addr);
void routeTable_display(void);

#endif




