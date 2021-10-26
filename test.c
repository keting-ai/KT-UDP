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
#include <ipv4_prtcl_table.h>
#include <ipv4_ifc_table.h>
#include <udp_head.h>
#include <udp_rcv.h>
#include <udp_port_table.h>
#include <udp_send.h>
#include <ipv4_to_uint32.h>

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

int DEBUG = 0;

int debug(char *arg) {
    char dbug[8] = "DEBUG";
    if (strcmp(arg, dbug) == 0) {
        DEBUG = 1;
    }
    return 0;
}

int DEFAULT_RT = 1;

int dft_rt_off(char *arg) {
    char off[32] = "DEFAULT_RT_OFF";
    if (strcmp(arg, off) == 0) {
        DEFAULT_RT = 0;
    }
    return 0;
}

int ens33_send (void *data, int length, int gw) {

    printf("\nSending packet......\n");
    
    struct sockaddr_ll stTagAddr;
    struct ifreq req;
    int ret;
    int sd;
    int send_len;
    unsigned char *sendbuff = data;
    
    sendbuff = sendbuff - ETH_HLEN;
    memset(&stTagAddr, 0 , sizeof(stTagAddr));

    sd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW);//这个sd就是用来获取ens33的index，完了就关闭
    strncpy(req.ifr_name, "ens33", IFNAMSIZ - 1);//通过设备名称获取index
    
    ret = ioctl(sd,SIOCGIFINDEX,&req); // gain device index, store in req
   
    if (ret==-1) {
       printf("Get ens33 index err \n");
    }
    
    stTagAddr.sll_family    = AF_PACKET;//填写AF_PACKET,不再经协议层处理
    stTagAddr.sll_protocol  = htons(ETH_P_IP);
    stTagAddr.sll_ifindex   = req.ifr_ifindex;//网卡eth33的index，非常重要，系统把数据往哪张网卡上发，就靠这个标识
    stTagAddr.sll_pkttype   = PACKET_OTHERHOST;//标识包的类型为发出去的包
    stTagAddr.sll_halen     = ETH_ALEN;    //MAC address length

    /* filling in destination mac address */
    /*stTagAddr.sll_addr[0]   = 0xF0;
    stTagAddr.sll_addr[1]   = 0x18;
    stTagAddr.sll_addr[2]   = 0x98;
    stTagAddr.sll_addr[3]   = 0x2C;
    stTagAddr.sll_addr[4]   = 0xAB;
    stTagAddr.sll_addr[5]   = 0x7D;*/
    
    stTagAddr.sll_addr[0]   = 0x8C;
    stTagAddr.sll_addr[1]   = 0x8D;
    stTagAddr.sll_addr[2]   = 0x28;
    stTagAddr.sll_addr[3]   = 0xF4;
    stTagAddr.sll_addr[4]   = 0x08;
    stTagAddr.sll_addr[5]   = 0x3F;
    
    
    struct ethhdr *eth = (struct ethhdr *)(sendbuff);
 
    eth->h_source[0] = 0x00;
    eth->h_source[1] = 0x0C;
    eth->h_source[2] = 0x29;
    eth->h_source[3] = 0x7A;
    eth->h_source[4] = 0xA5;
    eth->h_source[5] = 0x6E;
 
    /* filling destination mac. */
    /*eth->h_dest[0] = 0xF0;
    eth->h_dest[1] = 0x18;
    eth->h_dest[2] = 0x98;
    eth->h_dest[3] = 0x2C;
    eth->h_dest[4] = 0xAB;
    eth->h_dest[5] = 0x7D;*/
    
    eth->h_dest[0] = 0x8C;
    eth->h_dest[1] = 0x8D;
    eth->h_dest[2] = 0x28;
    eth->h_dest[3] = 0xF4;
    eth->h_dest[4] = 0x08;
    eth->h_dest[5] = 0x3F;
 
    eth->h_proto = htons(ETH_P_IP); //means next header will be IP header
    /*for (int i = 0; i < length; ++i) {
        printf("%x ",sendbuff[i]);    
    }*/
 
    send_len = sendto(sd, sendbuff, length + ETH_HLEN, 0, (const struct sockaddr *)&stTagAddr, sizeof(struct sockaddr_ll));
    
    if(send_len<0) {
        printf("error in sending....sendlen=%d\n",send_len);
        return -1;
    }

    return 0;
}

int test_port_rcv (void *data, int length) {
    printf("port rcv: ");
    unsigned char *p = data;
    for (int i = 0; i < length; ++i) {
        printf("%c", p[i]);
    }
    printf("\n");
    return 0;
}

int main (int argc, char* argv[]) {
    
    if (argv[1] != NULL) {
        dft_rt_off(argv[1]);      
    }
    
    if (argv[2] != NULL) {
        debug(argv[2]);
    }
    
    // first initialize routeTable
    /*printf("Initialize routeTable...");
    do {
        unsigned char input[64];
        unsigned int ipaddr = 0;
        unsigned int mask = 0;
        unsigned int gateway = 0;
        unsigned int infc = 0;
        int matric = 0;
        int isdefault = 0;
        
        printf("\nEnter destination IP address: ");
        fgets(input, 17, stdin);
	    input[strcspn(input, "\n")] = '\0';
        //input[strlen(input) - 1] = '\0';
        ipaddr = ipv4_to_uint32(input);
        if (ipaddr == -2) continue;
	    fflush(stdin);
        
        printf("Enter IP address mask: ");
        fgets(input, 17, stdin);
	    input[strcspn(input, "\n")] = '\0';
        //input[strlen(input) - 1] = '\0';
        mask = ipv4_to_uint32(input);
        if (mask == -2) continue;
        fflush(stdin);
        
        printf("Enter next hop gateway: ");
        fgets(input, 17, stdin);
	    input[strcspn(input, "\n")] = '\0';
        //input[strlen(input) - 1] = '\0';
        gateway = ipv4_to_uint32(input);
        if (gateway == -2) continue;
        //fflush(stdin);
        
        printf("Enter sending interface: ");
        scanf("%u", &infc);
        //fflush(stdin);
        
        printf("Enter route matric: ");
        scanf("%d", &matric);
        //fflush(stdin);
        
        printf("Is the route default(1 for yes, 0 for no)? ");
        scanf("%d", &isdefault);
        //fflush(stdin);
	    getchar();
        
        routeTable_store(ipaddr, mask, gateway, infc, matric, isdefault);
        printf("Press 'q' to quit, press other keys to continue: ");
        fgets(input, 4, stdin);
	    input[strcspn(input, "\n")] = '\0';
	    //input[strlen(input) - 1] = '\0';
        fflush(stdin);

        if (input[0] == 'q') {
            break;
        }
    } while (1);*/

    unsigned int ipaddr1 = 0x0A3334Cf; // 10.51.52.207
    unsigned int mask1 = 0xFFFFFFFF; // 255.255.255.255
    unsigned int gateway1 = 0x0A33355f; // 10.51.53.95
    unsigned int infc1 = 0x1; // the first one
    int matric1 = 1;

    unsigned int ipaddr2 = 0xBFA80000; //191.168.0.0
    unsigned int mask2 = 0xFFFF0000; // 255.255.0.0
    unsigned int gateway2 = 0x0A000002; // 10.0.0.2
    unsigned int infc2 = 0x1; // 192.168.99.8
    int matric2 = 10;

    unsigned int ipaddr3 = 0xCFA80000; //192.168.0.0
    unsigned int mask3 = 0xFFFF0000; // 255.255.0.0
    unsigned int gateway3 = 0x0A000003; // 10.0.0.3
    unsigned int infc3 = 0x1; // 192.168.99.8
    int matric3 = 5;
    
    routeTable_store(ipaddr1, mask1, gateway1, infc1, matric1, 0);
    routeTable_store(ipaddr2, mask2, gateway2, infc2, matric2, 0);
    routeTable_store(ipaddr3, mask3, gateway3, infc3, matric3, 0);

    for (int i = 0; i < 30; ++i) {
        unsigned int ipaddr = i + 0xCFA80001; //192.168.0.0
        unsigned int mask = 0xFFFFFFFF; // 255.255.0.0
        unsigned int gateway = 0x0A000003; // 10.0.0.3
        unsigned int infc = 0x1; // 192.168.99.8
        int matric = 5;
        routeTable_store(ipaddr, mask, gateway, infc, matric, 0);
    }

    char *testsendname;
    testsendname = "ens33_send";
    char *test_udp_port_name;
    test_udp_port_name = "udp_port";

    ipv4_ifc_reg(ens33_send, testsendname);
    ipv4_prtcl_reg(udp_rcv, 17);
    udp_port_reg(test_port_rcv, test_udp_port_name, 2000);
    
    /*printf("Sending UDP datagram...");
    while(1) {
        unsigned char data1 [1024] = {0};
        unsigned char *data = data1 + 100;
        unsigned int srcPort = 0;
        unsigned int dstPort = 0;
        unsigned char input[64];
        unsigned int srcAddr;
        unsigned int dstAddr;
        
        printf("\nEnter sending message: ");
        fgets(data, 925, stdin);
        data[strlen(data) - 1] = '\0';
        //fflush(stdin);
        
        printf("Enter source port: ");
        scanf("%d", &srcPort);
        //fflush(stdin);
        
        printf("Enter destination port: ");
        scanf("%d", &dstPort);
        //fflush(stdin);
	    getchar();
        
        printf("Enter source IP address: ");
        fgets(input, 17, stdin);
        input[strlen(input) - 1] = '\0';
        srcAddr = ipv4_to_uint32(input);
        if (srcAddr == -2) continue;
        fflush(stdin);
        
        printf("Enter destination IP address: ");
        fgets(input, 17, stdin);
        input[strlen(input) - 1] = '\0';
        dstAddr = ipv4_to_uint32(input);
        if (dstAddr == -2) continue;
        fflush(stdin);
        
        udp_send(data, strlen(data), srcPort, dstPort, srcAddr, dstAddr, ipv4_send);
        printf("\nEnter q to quit, others to continue: ");
        fgets(input, 2, stdin);        
        if (input[0] == 'q') {
            break;
        }
        fflush(stdin);
    }*/
    
    
    unsigned char data1 [1024] = {0};
    unsigned char *data = data1 + 100;
    printf("Enter sending message: ");
    fgets(data, 925, stdin);
    data[strlen(data) - 1] = '\0';
    
    udp_send(data, strlen(data), 2000, 2000, 0x0A33347A, 0x0A3334Cf, ipv4_send);

    while(1) {
        printf("\nReceiving packet......\n");

        struct sockaddr saddr;
        int SockFd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
        int recvLen;
        int ret;
        int addr_len = sizeof(saddr);
        unsigned char *szBuff = (unsigned char *) malloc(2048);
        if (szBuff == NULL) {
            printf("malloc failed.\n");
            return -1;
        }
        memset(szBuff, 0, 2048);

        if (-1 == SockFd) {
            printf("create socket error.\n");
            return -1;
        }
        
        recvLen = recvfrom(SockFd, szBuff, 2048, 0, &saddr, (socklen_t *)&addr_len);
        if ( recvLen < 14 ) {
            //接收的数据还不到一个帧头
            printf("recvfrom returns %d \n", recvLen);
            return -1;
        }

        /*for (int i = 0; i < recvLen; ++i) {
            printf("%x ", szBuff[i]);
        }
        printf("\n%d\n", recvLen);*/

        ret = ipv4_rcv(szBuff + 14, recvLen - 14, 0x0A33347A); // VM ip: 10.51.52.122
        if (ret == 0) {
            break;    
        }
        free(szBuff);
    }
    
    return 0;
    
    /* 本机ip: 10.51.53.95 
       虚拟机ip: 10.51.52.122 */
}
