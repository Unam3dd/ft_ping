/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 21:21:08 by stales            #+#    #+#             */
/*   Updated: 2026/03/08 21:22:18 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

unsigned short checksum(void *b, int len)
{
	if (!b || len < 0)
		return (0);

    unsigned short *buf = b;
    unsigned int sum = 0;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;

    if (len == 1)
        sum += *(unsigned char*)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    return (unsigned short)(~sum);
}

int main(int ac, char **av)
{
	struct icmphdr icmp_hdr;
	struct sockaddr_in sin;
	
	if (ac != 2) {
		fprintf(stderr, "%s: <ip>\n", av[0]);
		return (1);
	}

	printf("sizeof of header ICMP packet: %zd\n", sizeof(icmp_hdr));

	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (fd < 0)
		return (1);

	memset(&sin, 0, sizeof(sin));
	memset(&icmp_hdr, 0, sizeof(struct icmphdr));

	sin.sin_addr.s_addr = inet_addr(av[1]);

	icmp_hdr.type = ICMP_ECHO;        // 8 = echo request
    icmp_hdr.code = 0;
    icmp_hdr.un.echo.id = 1234;
    icmp_hdr.un.echo.sequence = 1;
	icmp_hdr.checksum = checksum(&icmp_hdr, sizeof(icmp_hdr));

	if (sendto(fd, &icmp_hdr, sizeof(icmp_hdr), 0, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
		fprintf(stderr, "Error: failed to sent ICMP packet !");
		return (1);
	}

	puts("ICMP packet sent !");

	close(fd);

	return (0);
}
