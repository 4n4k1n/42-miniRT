/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:13:51 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 17:05:11 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include <ifaddrs.h>

/**
 * Query a public IP service to obtain the machine's public IPv4 address.
 * Uses curl to contact ifconfig.me with a short timeout. Returns a pointer to
 * a static buffer containing the NUL-terminated IP string or NULL on error.
 * @return pointer to static string with public IP, or NULL
 */
char	*get_public_ip(void)
{
	FILE		*fp;
	static char	ip[64];

	fp = popen("curl -s -4 --max-time 2 ifconfig.me 2>/dev/null", "r");
	if (!fp)
		return (NULL);
	if (fgets(ip, sizeof(ip), fp) == NULL)
	{
		pclose(fp);
		return (NULL);
	}
	pclose(fp);
	ip[ft_strlen(ip) - 1] = '\0';
	return (ip);
}

/**
 * Discover a local non-loopback IPv4 address for the host.
 * Iterates network interfaces via getifaddrs and returns the first IPv4
 * address that is not in the 127.0.0.0/8 range. If none are found returns
 * "127.0.0.1".
 * @return malloc-free C string pointer to the chosen IP (may be ephemeral)
 */
char	*get_ip_address(void)
{
	struct ifaddrs		*ifaddr;
	struct ifaddrs		*ifa;
	struct sockaddr_in	*sa;
	char				*ip;

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
