/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slayer <slayer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 18:38:04 by slayer            #+#    #+#             */
/*   Updated: 2026/09/03 10:40:32 by slayer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int	picoshell(char **cmds[])
{
	int	i;
	int	fd[2];
	int	pid;
	int	prev_fd;

	i = 0;
	prev_fd = -1;
	while(cmds[i] != NULL)
	{
		if (cmds[i + 1] != NULL)
		{
			if (pipe(fd) < 0)
			{
				if (prev_fd != -1)
					close(prev_fd);
				return (1);
			}
		}

		pid = fork();
		if (pid < 0)
		{
			if (cmds[i + 1] != NULL)
			{
				close(fd[0]);
				close(fd[1]);
			}
			if (prev_fd != -1)
				close(prev_fd);
			return(1);
		}

		else if (pid == 0)
		{
			if(i != 0)
				dup2(prev_fd, STDIN_FILENO);
			if (cmds[i + 1] != NULL)
   				 dup2(fd[1], STDOUT_FILENO);
			if (prev_fd != -1)
				close(prev_fd);
			if (cmds[i + 1] != NULL)
			{
				close(fd[0]);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}

		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cmds[i + 1] != NULL)
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
			i++;
		}
	}

	while (wait(NULL) > 0)
	;
	return (0);

}
