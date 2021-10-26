/**
 * File name: ipv4_to_uint32.c
 *
 * Date: July, 21, 2021
 *
 * Author: Keting Chen
 *
 * Description:
 * This module transform IP address in "X.X.X.X" into integer.
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int DEBUG;

/*
 * This function transforms IP address in "X.X.X.X" into integer.
 *
 * param char *ip: the received IP address
 *
 * return: unsigned int transformed IP address
 *		   -2 for transformation problem.
 *
 */
unsigned int ipv4_to_uint32(char *ip) {
    
    unsigned int ret = 0;
    int count = 0;
    int security = 0; // used to determine whether the input is in "X.X.X.X" form
    char concat[1] = ".";
    // add a "." at the last, easy to calculate
    strncat(ip, concat, 1);

    for (int i = 0; i < strlen(ip); ++i) {
        if (ip[i] == '.') {
            security += 1;
            char *temp;
            temp = malloc(i - count);
            if (temp == NULL) {
                printf("Error: ipv4_to_unit32()\n line %d: malloc temp == NULL", __LINE__ - 1);
                return -2;
            }
            strncpy(temp, ip + count, i - count);
            count = i + 1;
            // the new numbers should be added at the tail
            ret = ret << 8;
            ret += atoi(temp);
        }
    }
    
    // check the detected dots number
    if (security != 4) {
        printf("Error: ipv4_to_unit32()\n line %d: invalid input", __LINE__ - 1);
        return -2;
    }
    
    if (DEBUG) {
        printf("OK: ipv4_to_uint32()\n line %d: ipv4 to uint32 calculation complete.\n", __LINE__ - 1);
    }

    return ret;
}
