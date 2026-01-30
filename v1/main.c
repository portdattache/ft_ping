/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brice <brice@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 13:46:23 by brice             #+#    #+#             */
/*   Updated: 2026/01/29 10:39:12 by brice            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void    print_usage(void)
{
    printf("Usage: ft_ping [OPTION...] HOST...\n"
           "Send ICMP ECHO_REQUEST packets to network hosts.\n\n"
           "Options:\n"
           "  -v, --verbose   Verbose output\n"
           "  -?, --help      Display help information\n");
           			fprintf(stderr, "Try 'ft_ping -?' for more information.\n");

}

void    print_ping_dest(const t_destination *dst, const t_options *opt)
{
    (void) opt;
    printf("PING %s (%s): %d data bytes\n",
            dst->host_name[0] ? dst->host_name : dst->input_host,
            dst->ip, PING_DATA_LEN);
}

bool    is_option(const char *s)
{
    return (s && s[0] == '-' && s[1] != '\0');
}

int     handle_options(const char *arg, t_options *opt)
{
    for (size_t i = 1; arg[i]; i++)
    {
        if (arg[i] == 'v')
            opt->verbose = true;
        else if (arg[i] == '?')
            opt->help = true;
        else
        {
            fprintf(stderr, "ft_ping: invalid option -- '%c'\n", arg[i]);
            fprintf(stderr, "Try 'ft-ping -? for more information\n");
            return (-1);
        }
        i++;
    }
    return (0);
}

int     parse_line(int ac, char **av, t_options *opt, const char **host)
{
    int host_count = 0;

    ft_memset(opt, 0, sizeof(*opt));
    *host = NULL;

    for (int i = 1; i < ac; i++)
    {
        if (is_option(av[i]))
        {
            if (handle_options(av[i], opt) < 0)
                return (-1);
        }
        else
        {
            *host = av[i];
            host_count++;
        }
    }
    if (opt->help)
    {
        print_usage();
        return (1);
    }
    if (host_count == 0)
    {
        fprintf(stderr, "ft_ping: missing host operand\n");
        fprintf(stderr, "Try 'ft_ping -?' for more information\n");
        return (-1);
    }
    if (host_count > 1)
    {
        fprintf(stderr, "ft_ping: only one host allowed\n");
        fprintf(stderr, "Try 'ft_ping -?' for more information\n");
        return (-1);
    }
    return (0);
}

int     get_dest_info(t_destination *dst, const char *host)
{
    struct addrinfo hints;
    struct addrinfo *res;

    ft_memset(dst, 0, sizeof(*dst));
    ft_memset(&hints, 0, sizeof(hints));
    res = NULL;
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    hints.ai_flags = AI_CANONNAME;

    if (getaddrinfo(host, NULL, &hints, &res) !=0 || !res)
    {
        fprintf(stderr, "ft_ping: %s: Name or service not know\n", host);
        return (-1);
    }
    dst->input_host = host;
	if (res->ai_canonname)
		ft_strncpy(dst->host_name, res->ai_canonname, sizeof(dst->host_name) - 1);
	else
		ft_strncpy(dst->host_name, host, sizeof(dst->host_name) - 1);
    ft_memcpy(&dst->addr, res->ai_addr, sizeof(dst->addr));
    if (!inet_ntop(AF_INET, &dst->addr.sin_addr, dst->ip, sizeof(dst->ip)))
    {
        freeaddrinfo(res);
        perror("inet_ntop");
        return (-1);
    }
    freeaddrinfo(res);
    return (0);
}

int     main(int ac, char **av)
{
    t_options opt;
    t_destination dst;
    const char *host;
    int ret;

    ret = parse_line(ac, av, &opt, &host);
    if (ret == 1)
        return (0);
    if (ret < 0)
        return (1);
    if (get_dest_info(&dst, host) < 0)
        return (1);
    print_ping_dest(&dst, &opt);
    return (0);
}
