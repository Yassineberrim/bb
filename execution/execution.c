/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberrim <yberrim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:43:17 by yberrim           #+#    #+#             */
/*   Updated: 2023/10/04 23:18:06 by yberrim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exec(t_cmd *cmd, char **env, int **pipe_fd)
{
	if (cmd->fd_out != 1)
	{
		dup2(cmd->fd_out, 1);
		close(cmd->fd_out); 
	}
	if (cmd->fd_in != 0)
	{
		dup2(cmd->fd_in, 0);
		close(cmd->fd_in);
	}
	close(*pipe_fd[0]);
	close(*pipe_fd[1]);
	execve(cmd->cmd_path, cmd->cmd, env);
	exit(g_exit_status = 127);
	return (g_exit_status = 127);
}

static void	ft_close(t_cmd *cmd, int **pipe_fd)
{
	if (cmd->out_redir_type != IN_NONE || cmd->in_redir_type != OUT_NONE)
		close(*pipe_fd[1]);
	if (cmd->fd_out != 1)
		close(cmd->fd_out);
	if (cmd->fd_in != 0)
		close(cmd->fd_in);
}

int	built_in(t_cmd *cmd)
{
	if (is_buildin(cmd))
	{
		execution_builtin(cmd, 0);
		return (g_exit_status);
	}
	else if (!cmd->cmd_path)
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		g_exit_status = 127;
		return (g_exit_status);
	}
	return (-1);
}

int	execution_proto(t_cmd *cmd, char **env)
{
	int	*pipe_fd;

	while (cmd)
	{
		pipe_fd = malloc(sizeof(int) * 2);
		cmd->cmd_path = find_abs_path(cmd->cmd[0]);
		if (cmd->next)
		{
			pipe(pipe_fd);
			cmd->fd_out = pipe_fd[1];
			cmd->next->fd_in = pipe_fd[0];
		}
		else if ((is_buildin(cmd) || !cmd->cmd_path) && built_in(cmd) != -1)
			return (g_exit_status);
		check_redirections(cmd);
		cmd->child_pid = fork();
		if (cmd->child_pid == 0)
			return (exec(cmd, env, &pipe_fd));
		ft_close(cmd, &pipe_fd);
		free(pipe_fd);
		cmd = cmd->next;
	}
	while (wait (&g_exit_status) > 0)
		;
	return (g_exit_status);
}
