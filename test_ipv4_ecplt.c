#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ipv4_ecplt.h>

int main (void) {
    unsigned char data1[] = {0x11,0x22,0x02,0x33,0x12};
    unsigned char data2[] = {0x11,0x22,0x02,0x33,0x12};
    void *res;
    res = ipv4_ecplt_encapsulate(data1, 5, 0x11111111, 0x22222222, 6);
    unsigned char *p = res;
    for (int i = 0; i< 25; ++i) {
        printf("%x ", p[i]);
    }
    printf("\n");
    res = ipv4_ecplt_encapsulate(data2, 5, 0x11111111, 0x22222222, 6);
    p = res;
    for (int i = 0; i< 25; ++i) {
        printf("%x ", p[i]);
    }
}