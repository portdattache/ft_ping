/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brice <brice@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 13:46:23 by brice             #+#    #+#             */
/*   Updated: 2026/02/05 10:48:43 by brice            ###   ########.fr       */
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

int     error_option(char c)
{
    fprintf(stderr, "ft_ping: invalid option -- '%c'\n", c);
    fprintf(stderr, "Try 'ft_ping -?' for more information\n");
    return (-1);
}

int     error_missing_host(void)
{
    fprintf(stderr, "ft_ping: missing host operand\n");
    fprintf(stderr, "'Try ft_ping -?' for more information\n");
    return (-1);
}

int     error_many_host(void)
{
    fprintf(stderr, "ft_pring: only one host allowed\n");
    fprintf(stderr, "Try 'ft_ping -?' for more information\n");
    return (-1);
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
            return (error_option(arg[i]));
        }
    }
    return (0);
}

int     parse_line(int ac, char **av, t_options *opt, const char **host)
{
    int host_count;

    ft_memset(opt, 0, sizeof(*opt));

    *host = NULL;
    host_count = 0;

    for (int i = 1; i < ac; i++)
    {
        if (is_option(av[i]))
        {
            if (handle_options(av[i], opt) < 0)
                return (-1);
            if (opt->help)
            {
                print_usage();
                    return (1);
            }
        }
        else
        {
            *host = av[i];
            host_count++;
        }
    }
    if (host_count == 0)
        return (error_missing_host());
    if (host_count > 1)
        return (error_many_host());
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
    hints.ai_flags = AI_CANONNAME;

    if (getaddrinfo(host, NULL, &hints, &res) !=0 || !res)
    {
        fprintf(stderr, "ft_ping: %s: Name or service not know\n", host);
        return (-1);
    }
    dst->input_host = host;
	if (res->ai_canonname)
		ft_strlcpy(dst->host_name, res->ai_canonname, sizeof(dst->host_name));
	else
		ft_strlcpy(dst->host_name, host, sizeof(dst->host_name));
    ft_memcpy(&dst->addr, res->ai_addr, res->ai_addrlen);
    if (!inet_ntop(AF_INET, &dst->addr.sin_addr, dst->ip, sizeof(dst->ip)))
    {
        freeaddrinfo(res);
        perror("inet_ntop");
        return (-1);
    }
    freeaddrinfo(res);
    return (0);
}

double time_diff(struct timeval a, struct timeval b)
{
    double  ua;
    double  ub;

    ua = (double)a.tv_sec * 1000000.0 + (double)a.tv_usec;
    ub = (double)b.tv_sec * 1000000.0 + (double)b.tv_usec;
   
    return((ua - ub) / 1000.0);
}

int     main(int ac, char **av)
{
    t_options opt;
    t_destination dst;
    const char *host_arg = NULL;
    struct timeval start, end;
    double rtt;
    int ret;

    ret = parse_line(ac, av, &opt, &host_arg);
    if (ret < 0)
        return (1);
    if (ret == 1)
        return (0);
    if (get_dest_info(&dst, host_arg) < 0)
        return (1);
    print_ping_dest(&dst, &opt);
    gettimeofday(&start, NULL);
    usleep(60000);
    gettimeofday(&end, NULL);
    rtt = time_diff(end, start);
    printf("%d bytes from %s(%s): icmp_seq=1 ttl=63 time=%.2f ms\n", PING_DATA_LEN, dst.host_name, dst.ip, rtt);
    return (0);
}
