/**
 * File name: udp_port_table.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module realizes the UDP port table.
 * udp_rcv() in udp_rcv.c uses this module to call higher layer ports
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UDP_PORT_TABLE_NAMELEN 128

extern int DEBUG;

/* The list structure */
typedef struct udp_port_list {

    void *list_head;
    void *list_tail;
} udp_port_list;

/* The structure for each item in the list */
typedef struct udp_port_item {

    int(*reg_port_ptr)();
    char port_name[UDP_PORT_TABLE_NAMELEN];
    int port;
    struct udp_port_item *next;
} udp_port_item;

/* The list that contains interfaces */
static udp_port_list *port_list = NULL;

/* Helps initialize the list */
static udp_port_list* udp_port_init(void) {

    udp_port_list *newlist;
    newlist = malloc(sizeof(udp_port_list));
    if (newlist == NULL) {
        printf("Error: udp_port_init()\n line %d: malloc newlist == NULL.\n", __LINE__ - 1);
        return NULL;
    }

    newlist -> list_head = NULL;
    newlist -> list_tail = NULL;

    if (DEBUG) {
        printf("OK: udp_port_init()\n line %d: initiate new list complete.\n", __LINE__ - 1);
    }
    return newlist;
}

/*
 * This function helps register ports into the list.
 *
 * param int(*port_ptr)(): the function pointer to the port
 * param char *name: the port name
 * param int portNum: the port number
 *
 * return 0 for registration succeed.
 *		   -1 for failure.
 *
 */
int udp_port_reg(int(*port_ptr)(), char *name, int portNum) {

    if (port_list == NULL) {
        port_list = udp_port_init();
    }

    // create a new udp_port_item
    udp_port_item *newItem;
    newItem = malloc(sizeof(udp_port_item));
    if (newItem == NULL) {
        printf("Error: udp_port_reg()\n line %d: malloc newItem == NULL.\n", __LINE__ - 1);
        return -1;
    }
    memset(newItem, 0, sizeof(udp_port_item));

    newItem -> reg_port_ptr = port_ptr;
    strncpy(newItem -> port_name, name, strlen(newItem -> port_name));
    newItem -> port = portNum;
    newItem -> next = NULL;

    // no item in port_list
    if (port_list -> list_head == NULL) {
        port_list -> list_head = newItem;
        port_list -> list_tail = newItem;
        if (DEBUG) {
            printf("OK: udp_port_reg()\n line %d: register new port complete.\n", __LINE__ - 1);
        }
        return 0;
    }

    // one/more item(s) in this list
    udp_port_item *thisItem = port_list -> list_tail;
    thisItem -> next = newItem;
    thisItem = newItem;
    if (DEBUG) {
        printf("OK: udp_port_reg()\n line %d: register new port complete.\n", __LINE__ - 1);
    }
    return 0;
}

/*
 * This function helps call ports in the list.
 *
 * param int portNum: the port number
 * param void *data: the pointer to transmission data
 * param int len: the length of data
 *
 * return 0 for calling succeed.
 *		   -1 for failure.
 *
 */
int udp_port_call(int portNum, void *data, int len) {

    // check if port_list available
    if (port_list == NULL) {
        printf("Error: udp_port_call()\n line %d: list not exit.\n", __LINE__ - 1);
        return -1;
    }

    udp_port_item *current;
    current = port_list -> list_head;

    if (current == NULL) {
        printf("Error: udp_port_call()\n line %d: no port pointer found.\n", __LINE__ - 1);
        return -1;
    }

    while (current != NULL) {
        if (portNum == current -> port) {
            if (DEBUG) {
                printf("OK: udp_port_call()\n line %d: call port complete.\n", __LINE__ - 1);
            }
            int ret;
            ret = current -> reg_port_ptr(data, len);
            if (ret == -1) {
                printf("Error: udp_port_call()\n line %d: port reports error.\n", __LINE__ - 1);
                return -1;
            }
            return 0;
        }
        current = current -> next;
    }

    printf("Error: udp_port_call()\n line %d: no port pointer found.\n", __LINE__ - 1);
    return -1;
}
