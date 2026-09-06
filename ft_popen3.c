/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slayer <slayer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 09:40:13 by slayer            #+#    #+#             */
/*   Updated: 2026/09/06 09:52:42 by slayer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>


int popen_part(const char *file, char *const argv[], char type)
{
	int	fd[2];
	int	pid;
	int	child_end;
	int	child_fd;
	int	return_end;


	if(pipe(fd) < 0)
		return (-1);

	if(type == 'r')
	{
		child_end = fd[1];
		return_end = fd[0];
		child_fd = 1;
	}
	else
	{
		child_end = fd[0];
		return_end = fd[1];
		child_fd = 0;
	}

	pid = fork();
	
	if(pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	
	if(pid == 0)
	{
		dup2(child_end, child_fd);
		close(child_end);
		close(return_end);
		execvp(file, argv);
		exit(1);
	}
	close(child_end);
	return (return_end);
}

int ft_popen(const char *file, char *const argv[], char type)
{
	if(!file || !argv)
		return (-1);
	if(type != 'r' && type != 'w')
		return (-1);
	return (popen_part(file, argv, type));
}