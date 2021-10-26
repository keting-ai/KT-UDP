#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ipv4.h>
#include <ipv4_rcv.h>
#include <ipv4_ecplt.h>
#include <ipv4_forward.h>
#include <ipv4_checksum.h>
#include <ipv4_send.h>
#include <routeTable.h>

int testsend (void *data, int length) {
    unsigned char *p = data;

    printf("%d\n", length);

    for (int i = 0; i < length; ++i) {
        printf("%x ", p[i]);
    }

    return 0;
}

int main (void) {
    unsigned char data1[] = {0x11,0x22,0x02,0x33,0x12};
    char *p;
    char name1[80];
    struct routeTable_array *arr1;

    unsigned int ipaddr1 = 0xBFA80000; //192.168.0.0
    unsigned int mask1 = 0xFFFF0000; // 255.255.0.0
    unsigned int gateway1 = 0x0A000001; // 10.0.0.1
    unsigned int infc1 = 0xC0A86308; // 192.168.99.8
    int matric1 = 1;

    unsigned int ipaddr2 = 0xBFA80000; //191.168.0.0
    unsigned int mask2 = 0xFFFF0000; // 255.255.0.0
    unsigned int gateway2 = 0x0A000002; // 10.0.0.2
    unsigned int infc2 = 0xC0A86308; // 192.168.99.8
    int matric2 = 10;

    unsigned int ipaddr3 = 0xBFA80000; //165.193.0.0
    unsigned int mask3 = 0xFFFF0000; // 255.255.0.0
    unsigned int gateway3 = 0x0A000003; // 10.0.0.3
    unsigned int infc3 = 0xC0A86308;
    int matric3 = 5;

    name1[0] = '1';
    arr1 = routeTable_iniArray(name1);
    routeTable_store(ipaddr1, mask1, gateway1, infc1, matric1, arr1, 0);
    routeTable_store(ipaddr2, mask2, gateway2, infc2, matric2, arr1, 0);
    routeTable_store(ipaddr3, mask3, gateway3, infc3, matric3, arr1, 0);

    ipv4_ecplt_encapsulate(data1, 5, 0x11111111, 0x22222222, 6, arr1, testsend);

}