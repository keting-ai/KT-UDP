#include <stdio.h>
#include <stdlib.h>
#include <routeTable.h>

int main (void) {
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
    unsigned int infc3 = 0xC0A86308; // 192.168.99.8
    int matric3 = 5;

    unsigned int ipaddr4 = 0xBFA80000; //165.193.0.0
    unsigned int mask4 = 0xFFFF0000; // 255.255.0.0
    unsigned int gateway4 = 0x0A000003; // 10.0.0.3
    unsigned int infc4 = 0xC0A86308; // 192.168.99.8
    int matric4 = 10;

    unsigned int ipaddr5 = 0xBFA80000; //165.193.0.0
    unsigned int mask5 = 0xFFFF0000; // 255.255.0.0
    unsigned int gateway5 = 0x0A000003; // 10.0.0.3
    unsigned int infc5 = 0xC0A86308; // 192.168.99.8
    int matric5 = 10;

    name1[0] = '1';
    arr1 = routeTable_iniArray(name1);
    routeTable_store(ipaddr1, mask1, gateway1, infc1, matric1, arr1, 1);
    routeTable_store(ipaddr2, mask2, gateway2, infc2, matric2, arr1, 0);
    routeTable_store(ipaddr3, mask3, gateway3, infc3, matric3, arr1, 0);
    routeTable_store(ipaddr4, mask4, gateway4, infc4, matric4, arr1, 0);
    routeTable_store(ipaddr5, mask5, gateway5, infc5, matric5, arr1, 0);
    //routeTable_display(arr1);
    //routeTable_delete(ipaddr1, gateway1, arr1);
    //routeTable_display(arr1);

    //routeTable_display(arr1);

    unsigned int * getroute;
    getroute = routeTable_get(0x00001, arr1);
    if (getroute == NULL) {
        printf("NULL");
    } else {
        for (int i = 0; i < getroute[0]; ++i) {
            printf("%x ", getroute[5 * i + 1]);
            printf("%x ", getroute[5 * i + 2]);
            printf("%x ", getroute[5 * i + 3]);
            printf("%x ", getroute[5 * i + 4]);
            printf("%x ", getroute[5 * i + 5]);
            //printf("%x ", getroute[i]);
        }
    }
}
