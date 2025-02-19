/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcosta-c <rcosta-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:53:34 by rcosta-c          #+#    #+#             */
/*   Updated: 2025/01/06 13:07:43 by rcosta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_getenv(t_sh *sh, char **envp)
{
	int	x;

	x = 0;
	while (envp[x] && envp)
	{
		x++;
	}
	sh->vars.envp_total = x;
	sh->envp = malloc(sizeof(char *) * (x + 1));
	if (!sh->envp)
		return ;
	x = 0;
	while (envp[x] && x < sh->vars.envp_total)
	{
		sh->envp[x] = ft_strdup(envp[x]);
		x++;
	}
	sh->envp[x] = NULL;
}

char	*find_my_host(t_sh *sh)
{
	int		x;
	int		xx;
	char	*temp;
	char	*temp2;

	x = 0;
	xx = 0;
	temp = search_envp(sh, "SESSION_MANAGER");
	if (!temp)
		temp = ft_strdup("/:42");
	temp2 = malloc(sizeof(char *) * 20);
	while (temp[x] != '/' && temp[x])
		x++;
	x++;
	while (temp[x] != ':' && temp[x])
	{
		temp2[xx++] = temp[x++];
	}
	temp2[xx] = '\0';
	free(temp);
	temp = ft_strdup(temp2);
	free(temp2);
	return (temp);
}

bool	multiquote_checker(int counter_s, int counter_d)
{
	if (counter_d >= 2 && counter_s >= 2
		&& counter_d % 2 == 0 && counter_s % 2 == 0)
		return (true);
	else
		return (false);
}
