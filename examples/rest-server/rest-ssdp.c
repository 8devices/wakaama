/*
 * MIT License
 *
 * Copyright (c) 2017 8devices
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "rest-ssdp.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>


#define SSDP_PORT "1900"
#define SSDP_GROUP "ff05::c"
#define SSDP_QUIT_SIGNAL SIGUSR1
#define SSDP_RESPONSE   "HTTP/1.1 200 OK\r\n"                               \
                        "CACHE-CONTROL: max-age=1800\r\n"                   \
                        "EXT:\r\n"                                          \
                        "LOCATION: *\r\n"                                   \
                        "SERVER: OS/0.1 UPnP/1.0 X/0.1\r\n"                 \
                        "ST: urn:8devices-com:service:lwm2m:1\r\n"          \
                        "USN: uuid:754cb08a-7ea1-485f-8202-9f16234083f2"    \
                        "::urn:8devices-com:service:lwm2m:1\r\n"            \
                        "\r\n"

static void *ssdp_run(void *arg);

static pthread_t ssdp_thread = 0; //must be used from main only (start...stop)
static volatile int ssdp_quit = 0;

ssdp_status_t ssdp_start(const char *coap_port)
{
    int status;

    if (ssdp_thread != 0)
    {
        ssdp_stop();
    }

    status = pthread_create(&ssdp_thread, NULL, &ssdp_run, (void *)coap_port);
    if (status != 0)
    {
        fprintf(stderr, "can't create thread :[%s]\n", strerror(status));
        return SSDP_ERROR;
    }

    pthread_setname_np(ssdp_thread, "ssdp_server");

    printf("Thread created successfully\n");

    return SSDP_OK;
}

void ssdp_stop(void)
{
    if (ssdp_thread != 0)
    {
        pthread_kill(ssdp_thread, SSDP_QUIT_SIGNAL);

        pthread_join(ssdp_thread, NULL);
        ssdp_thread = 0;

        if (ssdp_quit)
        {
            ssdp_quit = 0;
        }
        else
        {
            fprintf(stderr, "Can't stop ssdp thread\n");
        }
    }
}

static int parse_buf_lines(char *buf)
{
    char *line;
    char *saveptr;
    int i = 0;

    line = strtok_r(buf, "\r\n", &saveptr);

    while (line != NULL)
    {
        if (i == 0 && strcmp(line, "M-SEARCH * HTTP/1.1") != 0)
        {
            return 0;
        }

        if (strcmp(line, "ST: urn:8devices-com:service:lwm2m:1") == 0)
        {
            return 1;
        }

        line = strtok_r(NULL, "\r\n", &saveptr);
    }

    return 0;
}

static void ssdp_quit_sig_handler(int signo)
{
    ssdp_quit = 1;
}

static void *ssdp_run(void *arg)
{
    int status;
    struct sigaction sa;
    struct addrinfo hints = { 0 }; /* Hints for name lookup */
    struct addrinfo* multicastAddr = 0; /* Multicast Address */
    struct addrinfo* localAddr = 0; /* Local address to bind to */
    int sock = -1;

    ssdp_quit = 0;

    /* Register kill signal */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &ssdp_quit_sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    status = sigaction(SSDP_QUIT_SIGNAL, &sa, NULL);
    if (status != 0)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Resolve the multicast group address */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_flags = AI_NUMERICHOST;
    status = getaddrinfo(SSDP_GROUP, NULL, &hints, &multicastAddr);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        goto exit;
    }

    /*
     Get a local address with the same family (IPv4 or IPv6) as our multicast group
     This is for receiving on a certain port.
     */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = multicastAddr->ai_family;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; /* Return an address we can bind to */
    status = getaddrinfo(NULL, SSDP_PORT, &hints, &localAddr);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        goto exit;
    }

    /* Create socket for receiving datagrams */
    sock = socket(localAddr->ai_family, localAddr->ai_socktype, 0);
    if (sock < 0)
    {
        perror("socket() failed");
        goto exit;
    }

    /*
     * Enable SO_REUSEADDR to allow multiple instances of this
     * application to receive copies of the multicast datagrams.
     */
    status = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    if (status != 0)
    {
        perror("setsockopt");
        goto exit;
    }

    /* Bind the local address to the multicast port */
    status = bind(sock, localAddr->ai_addr, localAddr->ai_addrlen);
    if (status != 0)
    {
        perror("bind() failed");
        goto exit;
    }

    /* Join the multicast group. We do this seperately depending on whether we
     * are using IPv4 or IPv6.
     */
    if (multicastAddr->ai_family == PF_INET && multicastAddr->ai_addrlen == sizeof(struct sockaddr_in)) /* IPv4 */
    {
        struct ip_mreq multicastRequest; /* Multicast address join structure */

        /* Specify the multicast group */
        memcpy(&multicastRequest.imr_multiaddr, &((struct sockaddr_in *)(multicastAddr->ai_addr))->sin_addr, sizeof(multicastRequest.imr_multiaddr));

        /* Accept multicast from any interface */
        multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);

        /* Join the multicast address */
        status = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                            &multicastRequest, sizeof(multicastRequest));
        if (status != 0)
        {
            perror("setsockopt() failed");
            goto exit;
        }
    }
    else if (multicastAddr->ai_family == PF_INET6 && multicastAddr->ai_addrlen == sizeof(struct sockaddr_in6)) /* IPv6 */
    {
        struct ipv6_mreq multicastRequest; /* Multicast address join structure */

        /* Specify the multicast group */
        memcpy(&multicastRequest.ipv6mr_multiaddr, &((struct sockaddr_in6*) (multicastAddr->ai_addr))->sin6_addr, sizeof(multicastRequest.ipv6mr_multiaddr));

        /* Accept multicast from any interface */
        multicastRequest.ipv6mr_interface = 0;

        /* Join the multicast address */
        status = setsockopt(sock, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
                            &multicastRequest, sizeof(multicastRequest));
        if (status != 0)
        {
            perror("setsockopt() failed");
            goto exit;
        }
    }
    else
    {
        perror("Neither IPv4 or IPv6");
        goto exit;
    }

    printf("Listening on all addresses\n");

    /* now just enter a read-print loop */
    while (!ssdp_quit)
    {
        char buf[1024];
        int len;

        struct sockaddr_storage clientaddr;
        socklen_t addrlen = sizeof(clientaddr);

        char clienthost[NI_MAXHOST];
        char clientport[NI_MAXSERV];

        len = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&clientaddr, &addrlen);

        if (len < 0 || len >= sizeof(buf))
        {
            continue;
        }

        buf[len] = '\0';

        memset(clienthost, 0, sizeof(clienthost));
        memset(clientport, 0, sizeof(clientport));

        getnameinfo((struct sockaddr *)&clientaddr, addrlen,
                    clienthost, sizeof(clienthost),
                    clientport, sizeof(clientport),
                    NI_NUMERICHOST | NI_NUMERICSERV | NI_NOFQDN);

        printf("Received request from host=[%s] port=[%s]\n", clienthost, clientport);

        puts(buf);

        if (parse_buf_lines(buf) != 0)
        {
            len = snprintf(buf, sizeof(buf), SSDP_RESPONSE);
            puts(buf);

            len = sendto(sock, buf, len, 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
            if (len < 0)
            {
                perror("sendto error:: \n");
            }
        }
    } //while

exit:
    if (sock != -1)
    {
        close(sock);
    }

    if (localAddr)
    {
        freeaddrinfo(localAddr);
    }

    if (multicastAddr)
    {
        freeaddrinfo(multicastAddr);
    }

    return (void *)status;
}

