/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slayer <slayer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 18:08:39 by slayer            #+#    #+#             */
/*   Updated: 2026/08/10 18:33:24 by slayer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

int read_part(const char *file, char *const argv[])
{
	int p[2];
	int pid;

	if (pipe(p) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(p[0]);
		close(p[1]);
		return (-1);
	}
	if (pid == 0)
	{
		dup2(p[1], STDOUT_FILENO);
		close(p[0]);
		close(p[1]);
		execvp(file, argv);
		exit(1);
	}
	close(p[1]);
	return (p[0]);
}

int write_part(const char *file, char *const argv[])
{
	int p[2];
	int pid;

	if (pipe(p) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(p[0]);
		close(p[1]);
		return (-1);
	}
	if (pid == 0)
	{
		dup2(p[0], STDIN_FILENO);
		close(p[0]);
		close(p[1]);
		execvp(file, argv);
		exit(1);
	}
	close(p[0]);
	return (p[1]);
}

int ft_popen(const char *file, char *const argv[], char type)
{
	if (!file || !argv)
		return (-1);
	if (type == 'r')
		return (read_part(file, argv));
	else if (type == 'w')
		return (write_part(file, argv));
	return (-1);
}
