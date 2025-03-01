/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcosta-c <rcosta-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:53:27 by rcosta-c          #+#    #+#             */
/*   Updated: 2025/01/09 11:11:33 by rcosta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	verify_helper(t_sh *sh, char *prompt, int x)
{
	char	*home;

	home = search_envp(sh, "HOME");
	while (prompt[x])
	{
		if (x == 0 && prompt[x] != home[x])
			return (x);
		else if (prompt[x] == home[x])
		{
			x++;
		}
		else if (prompt[x] != home[x])
			break ;
	}
	free(home);
	return (x);
}

char	*verify_home(t_sh *sh, char *prompt)
{
	int		x;
	int		n_pr;
	int		n_total;
	char	*final_prompt;

	x = 0;
	x = verify_helper(sh, prompt, x);
	n_pr = ft_strlen(prompt);
	n_total = n_pr - x;
	final_prompt = malloc(sizeof(char) * n_total + 2);
	n_total = 1;
	if (x == 1)
		final_prompt[0] = '/';
	else
		final_prompt[0] = '~';
	while (prompt[x])
	{
		final_prompt[n_total] = prompt[x];
		n_total++;
		x++;
	}
	final_prompt[n_total] = '\0';
	free(prompt);
	return (final_prompt);
}

char	*join_2_str(char *a, char *b, char *z, int option)
{
	char	*res;
	char	*temp;

	if (z)
	{
		temp = ft_strjoin(a, z);
		res = ft_strjoin(temp, b);
		free(temp);
	}
	else
		res = ft_strjoin(a, b);
	if (option == 1)
	{
		free(b);
		free(a);
	}
	if (option == 2)
		free(a);
	return (res);
}

char	*get_prompt(t_sh *sh)
{
	char	*prompt;
	char	*temp;

	prompt = join_2_str(sh->vars.sh_user, sh->vars.sh_host, "@", 0);
	temp = join_2_str(prompt, sh->vars.sh_pwd, ":", 0);
	free(prompt);
	prompt = join_2_str(temp, " ", "$", 0);
	free(temp);
	return (prompt);
}
