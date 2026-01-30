/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brice <brice@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 13:37:08 by brice             #+#    #+#             */
/*   Updated: 2026/01/29 10:53:38 by brice            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <stdbool.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include "lib/libft.h"

# define PING_DATA_LEN 64

typedef struct s_options
{
    bool    verbose;
    bool    help;
}   t_options;

typedef struct s_destination
{
    const char          *input_host;
    char                ip[INET_ADDRSTRLEN];
    char                host_name[NI_MAXHOST];
    struct sockaddr_in  addr;
}   t_destination;

int     parse_line(int ac, char **av, t_options *opt, const char **host);
int     init_destination(t_destination *dst, const char *host);
void    print_ping_dest(const t_destination *dst, const t_options *opt);

#endif