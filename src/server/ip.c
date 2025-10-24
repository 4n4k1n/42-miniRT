/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:13:51 by anakin            #+#    #+#             */
/*   Updated: 2025/10/24 15:24:20 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include <ifaddrs.h>

char *get_ip_address(void)
{
	struct ifaddrs *ifaddr;
	struct ifaddrs *ifa;
	struct sockaddr_in *sa;
	char *ip;

	if (getifaddrs(&ifaddr) == -1)
		return (perror("getifaddrs error"), NULL);
	ifa = ifaddr;
	while (ifa != NULL)
	{
		if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET)
		{
			sa = (struct sockaddr_in *)ifa->ifa_addr;
			ip = inet_ntoa(sa->sin_addr);
			if (ft_strncmp(ip, "127.", 4) != 0)
			{
				freeifaddrs(ifaddr);
				return (ip);
			}
		}
		ifa = ifa->ifa_next;
	}
	freeifaddrs(ifaddr);
	return ("127.0.0.1");
}
