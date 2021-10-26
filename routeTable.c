/**
 * File name: routeTable.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module realizes a hash map to store, delete, get routes.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROUTETABLE_MAX 128
#define ROUTETABLE_LEN (ROUTETABLE_MAX - 1)
#define ROUTETABLE_NAME 128

extern int DEBUG;
extern int DEFAULT_RT;

/* The routeTable structure */
static struct routeTable_array {

    // the default route array
    unsigned int *default_route;
    // the array that each item points to a list
    void **arr;
};

/* The list structure in the hash map routeTable */
static struct routeTable_list {

    // the number of items stored in the list
    int num;
    void *head;
    void *tail;
};

/* The item structure */
static struct routeTable_item {

    unsigned int ipaddr;
    unsigned int mask;
    unsigned int gateway;
    unsigned int infc;
    unsigned int metric;
    unsigned int key;
    struct routeTable_item *next;
};

/* The structure that contains mask array */
static struct routeTable_mask_arr {

    int mask[64];
    int mask_num;
};

typedef struct routeTable_array routeTableArray;
typedef struct routeTable_list routeTable_list;
typedef struct routeTable_item routeTable_item;
typedef struct routeTable_mask_arr routeTable_mask_arr;

/* The overall routeTable */
static routeTableArray *hashArray = NULL;

/* The mask structure pointer */
static routeTable_mask_arr *mskArr = NULL;

/* This function helps initialize the routeTable */
static routeTableArray* routeTable_ini(void) {

    routeTableArray *newarr;
    void *p;

    /*	The memory is arraged as follows
     *  -------------------------------------------------------------------------------------------
     * | *default_route | **arr | *routeTable_list#1 |......| *routeTable_list#127 | int |...| int |
     *	-------------------------------------------------------------------------------------------
     */
    p = malloc(sizeof(routeTableArray) + ROUTETABLE_MAX * sizeof(routeTable_list *) + 6 * sizeof(int));
    if (p == NULL) {
        printf("Error: routeTable_ini()\n line %d: malloc p == NULL.\n", __LINE__ - 1);
        return NULL;
    }
    memset(p, 0, sizeof(routeTableArray) + ROUTETABLE_MAX * sizeof(routeTable_list *) + 6 * sizeof(int));

    newarr = p;

    /* arr points to the routeTable_list pointer array */
    newarr -> arr = p + sizeof(routeTableArray);
    if (newarr -> arr == NULL) {
        printf("Error: routeTable_ini()\n line %d: newarr -> arr == NULL.\n", __LINE__ - 1);
        return NULL;
    }

    /* default_route points to int array */
    newarr -> default_route = p + sizeof(routeTableArray) + ROUTETABLE_MAX * sizeof(routeTable_list *);
    if (newarr -> default_route == NULL) {
        printf("Error: routeTable_ini()\n line %d: newarr -> default_route == NULL.\n", __LINE__ - 1);
        return NULL;
    }

    if (DEBUG) {
        printf("OK: routeTable_ini()\n line %d: initiate routeTable complete.\n", __LINE__ - 1);
    }
    return newarr;
}

/* This function helps create a new item in the list */
static routeTable_item* routeTable_createItem(unsigned int addr, unsigned int gw,
        unsigned int msk, unsigned int ifc, int mtc) {

    routeTable_item *new;

    new = malloc(sizeof(routeTable_item));
    if (new == NULL) {
        printf("Error: routeTable_createItem()\n line %d: malloc new == NULL.\n", __LINE__ - 1);
        return NULL;
    }
    memset(new, 0, sizeof(routeTable_item));

    unsigned int thiskey;
    thiskey = addr & msk;
    
    /* check the legality of inputs */
    if (thiskey != addr) {
        printf("Error: routeTable_createItem()\n line %d: Re-entry a valid ip address and mask.\n", __LINE__ - 1);
        return NULL;
    }

    /* initialize fields in the new item */
    new -> key = thiskey;
    new -> ipaddr = addr;
    new -> gateway = gw;
    new -> mask = msk;
    new -> infc = ifc;
    new -> metric = mtc;
    new -> next = NULL;

    if (DEBUG) {
        printf("OK: routeTable_createItem()\n line %d: create new item complete.\n", __LINE__ - 1);
    }
    return new;
}

/* do hash by taking mod of 127(prime number) */
static int routeTable_dohash(routeTable_item *item) {

    return (item -> key % ROUTETABLE_LEN);
}

/* This is the compare function to use qsort */
static int cmpfunc (const void * a, const void * b)
{
   return -( *(int*)a - *(int*)b );
}

/*
 * This function helps store routes into the hash map.
 *
 * param unsigned int addr: the IP address
 * param unsigned int msk: the address mask
 * param unsigned int gw: the next hop gateway
 * param unsigned int ifc: the sending interface
 * param int mtc: the matric of this route
 * param int isdefault: 1 for default route, 0 otherwise
 *
 * return 0 for storing succeed.
 *		   -1 for failure.
 *
 */
int routeTable_store(unsigned int addr, unsigned int msk, unsigned int gw,
                     unsigned int ifc, int mtc, int isdefault) {

    if (hashArray == NULL) {
        hashArray = routeTable_ini();
    }

    routeTable_item *newItem;
    routeTable_list *thislist;
    int index;

    /* If the input route is default */
    if (isdefault) {
        hashArray -> default_route[0] = 0;
        hashArray -> default_route[1] = addr;
        hashArray -> default_route[2] = msk;
        hashArray -> default_route[3] = gw;
        hashArray -> default_route[4] = ifc;
        hashArray -> default_route[5] = mtc;
        
        if (DEBUG) {
            printf("OK: routeTable_store()\n line %d: store default route complete.\n", __LINE__ - 1);
        }
        return 0;
    }
    
    /* initialize mskArr if it is NULL */
    if (mskArr == NULL) {
        mskArr = malloc(sizeof(routeTable_mask_arr));
        if (mskArr == NULL) {
            printf("Error: routeTable_store()\n line %d: malloc mskArr == NULL.\n", __LINE__ - 1);
            return -1;
        }
        memset(mskArr, 0, sizeof(routeTable_mask_arr));
        mskArr -> mask_num = 0;
    }
    
     /* Store the masks */
    int cal_ones = 0;
    unsigned int x = msk;
    
    /* Calculate how many ones in the mask */
    while (x) {
        cal_ones++;
        x = x & (x - 1);
    }
    
    /* Check the redundancy */
    for (int i = 0; i < mskArr -> mask_num; ++i) {
        /* If there is one same mask exists, don't store this mask */
        if (cal_ones == mskArr -> mask[i]) {
            goto CREATEITEM;
        }
    }

    /* Store the mask info into the structure */
    mskArr -> mask[mskArr -> mask_num] = cal_ones;
    mskArr -> mask_num += 1;
    qsort(mskArr -> mask, mskArr -> mask_num, sizeof(int), cmpfunc);

CREATEITEM:
    /* We create a new item */
    newItem = routeTable_createItem(addr, gw, msk, ifc, mtc);
    if (newItem == NULL) {
        printf("Error: routeTable_store()\n line %d: newItem == NULL.\n", __LINE__ - 1);
        return -1;
    }
    index = routeTable_dohash(newItem);
    thislist = hashArray -> arr[index];

    /* Initiate the list */
    if (thislist == NULL) {

        thislist = malloc(sizeof(routeTable_list));
        if (thislist == NULL) {
            printf("Error: routeTable_store()\n line %d: malloc thislist == NULL.\n", __LINE__ - 1);
            return -1;
        }

        thislist -> head = NULL;
        thislist -> tail = NULL;
        thislist -> num = 0;
        hashArray -> arr[index] = thislist;
    }

    /* Store this item in the corresponding list */
    if (thislist -> head == NULL && thislist -> tail == NULL) {
        thislist -> head = newItem;
        thislist -> tail = newItem;
        thislist -> num += 1;
        if (DEBUG) {
            printf("OK: routeTable_store()\n line %d: store route complete.\n", __LINE__ - 1);
        }
        return 0;
    } else {
        routeTable_item *thisitem = thislist -> tail;
        thisitem -> next = newItem;
        thislist -> tail = newItem;
        thislist -> num += 1;
        if (DEBUG) {
            printf("OK: routeTable_store()\n line %d: store route complete.\n", __LINE__ - 1);
        }
        return 0;
    }

    printf("Error: routeTable_store()\n line %d: item storing failed.\n", __LINE__ - 1);
    return -1;
}

/*
 * This function helps delete routes into the hash map.
 *
 * param unsigned int addr: the IP address
 * param unsigned int gw: the next hop gateway
 *
 * return 0 for deletion succeed.
 *		   -1 for failure.
 * 
 */
int routeTable_delete (unsigned int addr, unsigned int gw) {

    routeTable_list *thislist;
    routeTable_item *current;
    int index;
    index = addr % ROUTETABLE_LEN;
    thislist = hashArray -> arr[index]; // find the corresponding list

    // list has no item
    if (thislist == NULL || thislist -> head == NULL) {
        printf("Error: routeTable_delete()\n line %d: ip address not found.\n", __LINE__ - 1);
        return -1;
    }

    /* List has only one item */
    if (thislist -> head == thislist -> tail) {

        routeTable_item *thisitem = thislist -> tail;

        if (thisitem -> ipaddr != addr || thisitem -> gateway != gw) {
            printf("Error: line %d: ip address not found.\n", __LINE__ - 1);
            return -1;
        }

        free(thislist -> head);
        thislist -> head = NULL;
        thislist -> tail = NULL;
        thislist -> num -= 1;
        if (DEBUG) {
            printf("OK: routeTable_delete()\n line %d: delete route complete.\n", __LINE__ - 1);
        }
        return 0;
    }

    // list has more than one item
    current = thislist -> head;
    // delete the first one
    if (current -> ipaddr = addr && current -> gateway == gw) {
        thislist -> num -= 1;
        thislist -> head = current -> next;
        free(current);
        if (DEBUG) {
            printf("OK: routeTable_delete()\n line %d: delete route complete.\n", __LINE__ - 1);
        }
        return 0;
    }

    while (current != NULL && current -> next != NULL) {
        if (current -> next -> ipaddr == addr && current -> next -> gateway == gw) {
            thislist -> num -= 1;
            free(current -> next);
            current -> next = current -> next -> next;
            if (DEBUG) {
                printf("OK: routeTable_delete()\n line %d: delete route complete.\n", __LINE__ - 1);
            }
            return 0;
        }
        current = current -> next;
    }

    printf("Error: routeTable_delete()\n line %d: ip address/gateway not found.\n", __LINE__ - 1);
    return -1;
}

/*
 * This function helps get routes from the hash map.
 *
 * param unsigned int addr: the IP address
 *
 * return an unsigned int array that stores route information
 *
 */
unsigned int* routeTable_get(unsigned int addr) {

    routeTable_list *thislist;
    routeTable_item *current;
    unsigned int *retarr;
    int index;
    int count;

    count = 0;
    for (int i = 0; i < mskArr -> mask_num; ++i) {
        unsigned int mask = 0;
        mask = (int)pow(2.0, (double)(mskArr -> mask[i]));
        mask -= 1;
        mask = mask << (32 - mskArr -> mask[i]);
        index = (addr & mask) % ROUTETABLE_LEN;
        thislist = hashArray -> arr[index];
        if (thislist != NULL) goto GETROUTE;
    }

    /* if thislist == NULL, there's no matched route, return the default route */
    if (thislist == NULL) {
        if (DEFAULT_RT) {
            retarr = hashArray -> default_route;
            if (DEBUG) {
                printf("OK: routeTable_get()\n line %d: return default route.\n", __LINE__ - 1);
            }
            if (hashArray -> default_route[0] == 0) {
                printf("Error: routeTable_get()\n line %d: no default route.\n", __LINE__ - 1);
            }
            if (DEBUG) {
                printf("OK: routeTable_get()\n line %d: get route complete.\n", __LINE__ - 1);
            }
            return retarr;
        } else {
            printf("Error: routeTable_get()\n line %d: no matched route found.\n", __LINE__ - 1);
            return NULL;
        }
    }

GETROUTE:
    /* To save memory, use one dimensional array */
    retarr = (int*)malloc(6 * sizeof(int) * thislist -> num);
    if (retarr == NULL) {
        printf("Error: routeTable_get()\n line %d: malloc retarr == NULL.\n", __LINE__ - 1);
        return NULL;
    }
    memset(retarr, 0, 6 * sizeof(int) * thislist -> num);

    current = thislist -> head;
    retarr[0] = thislist -> num; // the first one specifies how many routes are there
    while (current != NULL) {
        if (current -> ipaddr == addr) {
            retarr[count * 5 + 1] = current -> ipaddr;
            retarr[count * 5 + 2] = current -> mask;
            retarr[count * 5 + 3] = current -> gateway;
            retarr[count * 5 + 4] = current -> infc;
            retarr[count * 5 + 5] = current -> metric;
        }
        current = current -> next;
        count += 1;
    }
    
    if (DEBUG) {
        printf("OK: routeTable_get()\n line %d: get route complete.\n", __LINE__ - 1);
    }
    return retarr;
}

/* This function displays the routeTable*/
void routeTable_display(void) {

    for (int i = 1; i <= ROUTETABLE_MAX; ++i) {
        routeTable_list *thislist;
        routeTable_item *current;

        thislist = hashArray -> arr[i - 1];
        if (thislist == NULL) {
            printf("%d: ",i);
            goto PRINTLINE;
        }
        current = thislist -> head;

        printf("%d: ",i);
        while (current != NULL) {
            printf("%x -> ", current -> ipaddr);
            current = current -> next;
        }

PRINTLINE:
        printf("\n");
        for (int j = 0; j < 60; ++j) {
            printf("-");
        }
        printf("\n");
    }
    printf("\n");
}
