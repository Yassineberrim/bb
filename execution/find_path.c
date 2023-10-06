/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberrim <yberrim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:18:37 by yberrim           #+#    #+#             */
/*   Updated: 2023/10/02 16:31:30 by yberrim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_abs_path(char *cmd)
{
	char *raw_path;
	char **path_arr;
	int i;

	i = 0;
	raw_path = getenv("PATH");
	path_arr = ft_split(raw_path, ':');
	if (cmd && cmd[0] == '/' && ft_strlen(cmd) > 1)
	{
		if (access(cmd, F_OK) == 0)
			return (cmd);
		return (NULL);
	}
	if (cmd && cmd[0] == '.' && cmd[1] == '/' && ft_strlen(cmd) > 2)
	{
		if (access(cmd, F_OK) == 0)
			return (cmd);
		return (NULL);
	}
	while (path_arr[i])
	{
		char *fwd_slash = ft_strjoin(path_arr[i], "/");
		char *abs_path = ft_strjoin(fwd_slash, cmd);
		if (access(abs_path, F_OK) == 0)
		{
			free(path_arr);
			return (abs_path);
		}
		free(abs_path);
		i++;
	}
	return (NULL);
}