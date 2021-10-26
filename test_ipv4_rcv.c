#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ipv4_rcv.h>

static struct test_ipv4_rcv_ip {

    unsigned version : 4;
    unsigned IHL : 4;
    unsigned control : 8;
    unsigned len : 16;
    unsigned ident : 16;
    unsigned fragoffset : 16;
    unsigned TTL : 8;
    unsigned protocol : 8;
    unsigned checksum : 16;
    unsigned srcAddr : 32;
    unsigned dstAddr : 32;
} ;

typedef struct test_ipv4_rcv_ip test_ipv4_rcv_ip;

int main (void) {

    void *p;
    test_ipv4_rcv_ip *ip;
    char *rcv;
    p = malloc(sizeof(test_ipv4_rcv_ip) + 2);
    memset(p, 0, sizeof(test_ipv4_rcv_ip));

    ip = p;
    ip -> version = 4;
    ip -> IHL = 5;
    ip -> len = 25;
    ip -> fragoffset = 0x0400;
    ip -> TTL = 5;
    ip -> protocol = 17;

    char *body = p + sizeof(test_ipv4_rcv_ip);
    body[0] = 'a';
    body[1] = 'b';

    rcv = ipv4_rcv_receive(ip, 25);

    for (int i = 0; i < 2; ++i) {
        printf("%c ",rcv[i]);
    }
}