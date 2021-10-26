/**
 * File name: ipv4_prtcl_table.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module realizes the IP protocol table.
 * ipv4_rcv_helper() in ipv4_rcv.c uses this module to call higher layer protocols
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int DEBUG;

/* The list structure */
typedef struct ipv4_prtcl_list {

    void *list_head;
    void *list_tail;
} ipv4_prtcl_list;

/* The structure for each item in the list */
typedef struct ipv4_prtcl_item {

    int(*reg_func_ptr)();
    int protocol;
    struct ipv4_prtcl_item *next;
} ipv4_prtcl_item;

/* The list that contains interfaces */
static ipv4_prtcl_list *prtcl_list = NULL;

/* Helps initialize the list */
static ipv4_prtcl_list* ipv4_prtcl_init(void) {

    ipv4_prtcl_list *newlist;
    newlist = malloc(sizeof(ipv4_prtcl_list));
    if (newlist == NULL) {
        printf("Error: ipv4_prtcl_init()\n line %d: malloc newlist == NULL.\n", __LINE__ - 1);
        return NULL;
    }

    newlist -> list_head = NULL;
    newlist -> list_tail = NULL;

    if (DEBUG) {
        printf("OK: ipv4_prtcl_list()\n line %d: initialize list complete.\n", __LINE__ - 1);
    }
    return newlist;
}

/*
 * This function helps register protocols into the list.
 *
 * param int(*func_ptr)(): the function pointer to the protocol
 * param int prtcl: the protocol number
 *
 * return 0 for registration succeed.
 *		   -1 for failure.
 *
 */
int ipv4_prtcl_reg(int(*func_ptr)(), int prtcl) {
    if (prtcl > 255) {
        printf("Error: ipv4_prtcl_reg()\n line %d: protocol cannot > 255.\n", __LINE__ - 1);
    }

    if (prtcl_list == NULL) {
        prtcl_list = ipv4_prtcl_init();
    }

    // create a new ipv4_prtcl_item
    ipv4_prtcl_item *newItem;
    newItem = malloc(sizeof(ipv4_prtcl_item));
    if (newItem == NULL) {
        printf("Error: ipv4_prtcl_reg()\n line %d: malloc newItem == NULL.\n", __LINE__ - 1);
        return -1;
    }
    memset(newItem, 0, sizeof(ipv4_prtcl_item));

    newItem -> reg_func_ptr = func_ptr;
    newItem -> protocol = prtcl;
    newItem -> next = NULL;

    // no item in prtcl_list
    if (prtcl_list -> list_head == NULL) {
        prtcl_list -> list_head = newItem;
        prtcl_list -> list_tail = newItem;
        
        if (DEBUG) {
            printf("OK: ipv4_prtcl_reg()\n line %d: register protocol complete.\n", __LINE__ - 1);
        }
        return 0;
    }

    // one/more item(s) in this list

    ipv4_prtcl_item *thisItem = prtcl_list -> list_tail;
    thisItem -> next = newItem;
    thisItem = newItem;
    
    if (DEBUG) {
            printf("OK: ipv4_prtcl_reg()\n line %d: register protocol complete.\n", __LINE__ - 1);
        }
    return 0;
}

/*
 * This function helps call protocols in the list.
 *
 * param int prtcl: the prtcl number
 * param void *data: the pointer to transmission data
 * param int len: the length of data
 * param void *ipData: the IP data with IP head
 *
 * return 0 for calling succeed.
 *		   -1 for failure.
 *
 */
int ipv4_prtcl_call(int prtcl, void *data, int len, void *ipData) {

    // check if prtcl_list available
    if (prtcl_list == NULL) {
        printf("Error: ipv4_prtcl_call()\n line %d: list not exit.\n", __LINE__ - 1);
        return -1;
    }

    ipv4_prtcl_item *current;
    current = prtcl_list -> list_head;

    if (current == NULL) {
        printf("Error: ipv4_prtcl_call()\n line %d: no protocol pointer found.\n", __LINE__ - 1);
        return -1;
    }

    while (current != NULL) {
        if (current -> protocol == prtcl) {
            if (DEBUG) {
                printf("OK: ipv4_ifc_call()\n line %d: call protocol.\n", __LINE__ - 1);
            }
            int ret;
            ret = current -> reg_func_ptr (data, len, ipData);
            if (ret == -1) {
                printf("Error: ipv4_prtcl_call()\n line %d: call protocol failed.\n", __LINE__ - 1);
                return -1;
            }
            return 0;
        }
        current = current -> next;
    }

    printf("Error: ipv4_prtcl_call()\n line %d: no protocol pointer found.\n", __LINE__ - 1);
    return -1;
}
