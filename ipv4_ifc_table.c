/**
 * File name: ipv4_ifc_table.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module realizes the IP interface table.
 * ipv4_send() in ipv4_send.c uses this module to call lower layer interfaces
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IPV4_IFC_TABLE_NAMELEN 128

extern int DEBUG;

/* The list structure */
typedef struct ipv4_ifc_list {

    void *list_head;
    void *list_tail;
} ipv4_ifc_list;

/* The structure for each item in the list */
typedef struct ipv4_ifc_item {

    int(*reg_ifc_ptr)();
    char ifc_name[IPV4_IFC_TABLE_NAMELEN];
    int ifc_ID;
    struct ipv4_ifc_item *next;
} ipv4_ifc_item;

/* The list that contains interfaces */
static ipv4_ifc_list *ifc_list = NULL;
/* The counter that assign each interface ID*/
static int ipv4_ifc_IDcouter = 0;

/* Helps initialize the list */
static ipv4_ifc_list* ipv4_ifc_init(void) {

    ipv4_ifc_list *newlist;
    newlist = malloc(sizeof(ipv4_ifc_list));
    if (newlist == NULL) {
        printf("Error: ipv4_ifc_init()\n line %d: malloc newlist == NULL.\n", __LINE__ - 1);
        return NULL;
    }

    newlist -> list_head = NULL;
    newlist -> list_tail = NULL;
    
    if (DEBUG) {
        printf("OK: ipv4_ifc_list()\n line %d: initialize list complete.\n", __LINE__ - 1);
    }
    return newlist;
}

/*
 * This function helps register interface into the list.
 *
 * param int(*ifc_ptr)(): the function pointer to the interface
 * param char *name: the interface's name
 *
 * return 0 for storing succeed.
 *		   -1 for failure.
 *
 */
int ipv4_ifc_reg(int(*ifc_ptr)(), char *name) {

    if (ifc_list == NULL) {
        ifc_list = ipv4_ifc_init();
    }

    // create a new ipv4_ifc_item
    ipv4_ifc_item *newItem;
    newItem = malloc(sizeof(ipv4_ifc_item));
    if (newItem == NULL) {
        printf("Error: ipv4_ifc_reg()\n line %d: malloc newItem == NULL.\n", __LINE__ - 1);
        return -1;
    }
    memset(newItem, 0, sizeof(ipv4_ifc_item));

    newItem -> reg_ifc_ptr = ifc_ptr;
    strncpy(newItem -> ifc_name, name, strlen(newItem -> ifc_name));
    newItem -> ifc_ID = ++ipv4_ifc_IDcouter;
    newItem -> next = NULL;

    // no item in ifc_list
    if (ifc_list -> list_head == NULL) {
        ifc_list -> list_head = newItem;
        ifc_list -> list_tail = newItem;
        
        if (DEBUG) {
            printf("OK: ipv4_ifc_reg()\n line %d: register interface complete.\n", __LINE__ - 1);
        }
        return 0;
    }

    // one/more item(s) in this list
    ipv4_ifc_item *thisItem = ifc_list -> list_tail;
    thisItem -> next = newItem;
    thisItem = newItem;
    
    if (DEBUG) {
            printf("OK: ipv4_ifc_reg()\n line %d: register interface complete.\n", __LINE__ - 1);
        }
    return 0;
}

/*
 * This function helps call interface in the list.
 *
 * param int ID: the interface ID
 * param void *data: the pointer to transmission data
 * param int len: the length of data
 * param int gateway: the next hop gateway
 *
 * return 0 for calling succeed.
 *		   -1 for failure.
 *
 */
int ipv4_ifc_call(int ID, void *data, int len, int gateway) {

    // check if ifc_list available
    if (ifc_list == NULL) {
        printf("Error: ipv4_ifc_call()\n line %d: list not exit.\n", __LINE__ - 1);
        return -1;
    }

    ipv4_ifc_item *current;
    current = ifc_list -> list_head;
    int retVal;

    if (current == NULL) {
        printf("Error: ipv4_ifc_call()\n line %d: no ifc pointer found.\n", __LINE__ - 1);
        return -1;
    }

    while (current != NULL) {
        /* ID number is the only identification, name is not reliable */
        if (ID == current -> ifc_ID) {
            if (DEBUG) {
                printf("OK: ipv4_ifc_call()\n line %d: call interface.\n", __LINE__ - 1);
            }
            retVal = current -> reg_ifc_ptr(data, len, gateway);
            if (retVal == -1) {
                printf("Error: ipv4_ifc_call()\n line %d: interface reports error.\n", __LINE__ - 1);
                return -1;
            }
            return 0;
        }
        current = current -> next;
    }

    printf("Error: ipv4_ifc_call()\n line %d: no ifc pointer found.\n", __LINE__ - 1);
    return -1;
}
