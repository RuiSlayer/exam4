/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slayer <slayer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 12:14:54 by slayer            #+#    #+#             */
/*   Updated: 2026/09/05 12:26:06 by slayer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int	picoshell(char **cmds[])
{
	int	i;
	int	pid;
	int	fd[2];
	int	prev_fd;
	int	has_next;
	int	not_first;

	prev_fd = -1;
	i = 0;
	while(cmds[i])
	{
		if(has_next && pipe(fd) < 0)
		{
			if(not_first)
				close(prev_fd);
			return (1);
		}

		pid = fork();

		if (pid < 0)
		{
			if(not_first)
				close(prev_fd);
			if(has_next)
			{
				close(fd[0]);
				close(fd[1]);
			}
			return (1);
		}

		if (pid == 0)
		{
			if(not_first)
			{
				dup2(prev_fd, 0);
				close(prev_fd);
			}
			if(has_next)
			{
				close(fd[0]);
				dup2(fd[1], 1);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}

		if(not_first)
			close(prev_fd);
		if(has_next)
		{
			prev_fd = fd[0];
			close(fd[1]);
		}
		i++;
	}
	
	while(wait(NULL) < 0);
	return (0);
}