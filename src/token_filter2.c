/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_filter2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcosta-c <rcosta-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:52:02 by rcosta-c          #+#    #+#             */
/*   Updated: 2025/01/20 22:59:20 by rcosta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	filter_file(t_sh *sh, int n)
{
	int	x;

	x = 0;
	while (sh->tokens[n].tokens[x])
	{
		if (sh->tokens[n].tokens[x] == '.')
		{
			if (sh->tokens[n].tokens[x + 1] == '.')
				break ;
			sh->tokens[n].file = true;
			break ;
		}
		x++;
	}
	if (n > 0 && (sh->tokens[n - 1].r_in == true
			|| sh->tokens[n - 1].r_out == true
			|| sh->tokens[n - 1].r_heredoc == true
			|| sh->tokens[n - 1].r_outappend == true))
	{
		sh->tokens[n].file = true;
		sh->tokens[n].arg = false;
	}
}

static void	check_filter_redir(t_sh *sh, int n)
{
	if (sh->tokens[n].tokens[0] == '>')
	{
		if (!sh->tokens[n].tokens[1])
		{
			sh->tokens[n].r_out = true;
			sh->vars.redir_tot++;
		}
		if (sh->tokens[n].tokens[1] == '>'
			&& ft_strlen(sh->tokens[n].tokens) == 2)
		{
			sh->tokens[n].r_outappend = true;
			sh->vars.redir_tot++;
		}
	}
}

void	filter_pipes_redir(t_sh *sh, int n)
{
	if (sh->tokens[n].d_quote == false && sh->tokens[n].s_quote == false)
	{
		if (sh->tokens[n].tokens[0] == '|'
			&& ft_strlen(sh->tokens[n].tokens) == 1)
		{
			sh->tokens[n].pipe = true;
			sh->vars.pipe_num++;
		}
		if (sh->tokens[n].tokens[0] == '<')
		{
			if (!sh->tokens[n].tokens[1])
			{
				sh->tokens[n].r_in = true;
				sh->vars.redir_tot++;
			}
			if (sh->tokens[n].tokens[1] == '<')
			{
				sh->tokens[n].r_heredoc = true;
				sh->vars.redir_tot++;
				sh->vars.heredoc_num++;
			}
		}
		check_filter_redir(sh, n);
	}
}

void	filter_quotes(t_sh *sh, int n)
{
	int	len;
	int	counter_d;
	int	counter_s;
	int	x;

	x = 0;
	counter_d = 0;
	counter_s = 0;
	len = ft_strlen(sh->tokens[n].tokens);
	while (x < len)
	{
		if (sh->tokens[n].tokens[x] == 34)
			counter_d++;
		if (sh->tokens[n].tokens[x] == 39)
			counter_s++;
		x++;
	}
	filter_quotes_helper(sh, n, counter_s, counter_d);
}
