/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slayer <slayer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 10:28:16 by slayer            #+#    #+#             */
/*   Updated: 2026/09/04 10:14:10 by slayer           ###   ########.fr       */
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
	int	has_next;
	int	not_first_cmd;

	i = 0;
	prev_fd = -1;
	while(cmds[i] != NULL)
	{
		has_next = (cmds[i + 1] != NULL);
		not_first_cmd = (prev_fd != -1);

		if (has_next && pipe(fd) < 0)
		{
			if (not_first_cmd)
				close(prev_fd);
			return (1);
		}

		pid = fork();

		if (pid < 0)
		{
			if (has_next)
			{
				close(fd[0]);
				close(fd[1]);
			}
			if (not_first_cmd)
				close(prev_fd);
			return (1);
		}

		else if (pid == 0)
		{
			if (not_first_cmd)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (has_next)
			{
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}

		else
		{
			if (not_first_cmd)
				close(prev_fd);
			if (has_next)
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

// #include <stdio.h>
// #include <stdarg.h>
// #include <string.h>
// static int	count_cmds(int argc, char **argv)
// {
// 	int	count = 1;
// 	for (int i = 1; i < argc; i++)
// 	{
// 		if (strcmp(argv[i], "|") == 0)
// 			count++;
// 	}
// 	return (count);
// }

// int	main(int argc, char **argv)
// {
// 	if (argc < 2)
// 		return (fprintf(stderr, "Usage: %s cmd1 [args] | cmd2 [args] ...\n", argv[0]), 1);

// 	int	cmd_count = count_cmds(argc, argv);
// 	char	***cmds = calloc(cmd_count + 1, sizeof(char **));
// 	if (!cmds)
// 		return (perror("calloc"), 1);

// 	int	i = 1, j = 0;
// 	while (i < argc)
// 	{
// 		int	len = 0;
// 		while (i + len < argc && strcmp(argv[i + len], "|") != 0)
// 			len++;
// 		cmds[j] = calloc(len + 1, sizeof(char *));
// 		if (!cmds[j])
// 			return (perror("calloc"), 1);
// 		for (int k = 0; k < len; k++)
// 			cmds[j][k] = argv[i + k];
// 		cmds[j][len] = NULL;
// 		i += len + 1;
// 		j++;
// 	}
// 	cmds[cmd_count] = NULL;

// 	int	ret = picoshell(cmds);

// 	// Clean up
// 	for (int i = 0; cmds[i]; i++)
// 		free(cmds[i]);
// 	free(cmds);

// 	return (ret);
// }
