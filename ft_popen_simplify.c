#include <unistd.h>
#include <stdlib.h>

static int	popen_part(const char *file, char *const argv[], char type)
{
	int	p[2];
	int	pid;
	int	child_end;
	int	child_std;

	if (pipe(p) < 0)
		return (-1);
	if (type == 'r')
	{
		child_end = p[1];
		child_std = STDOUT_FILENO;
	}
	else
	{
		child_end = p[0];
		child_std = STDIN_FILENO;
	}
	pid = fork();
	if (pid < 0)
	{
		close(p[0]);
		close(p[1]);
		return (-1);
	}
	if (pid == 0)
	{
		dup2(child_end, child_std);
		close(p[0]);
		close(p[1]);
		execvp(file, argv);
		exit(1);
	}
	close(child_end);
	if (type == 'r')
		return (p[0]);
	else
		return (p[1]);
}

int	ft_popen(const char *file, char *const argv[], char type)
{
	if (!file || !argv)
		return (-1);
	if (type != 'r' && type != 'w')
		return (-1);
	return (popen_part(file, argv, type));
}
