/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcosta-c <rcosta-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:55:03 by rcosta-c          #+#    #+#             */
/*   Updated: 2024/12/21 02:02:23 by rcosta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *pre_expand(t_sh *sh, int *x, int n)
{
	int     xa;
	char    *a;

	xa = 0;
	while (sh->tokens[n].tokens[*x + xa] && 
			sh->tokens[n].tokens[*x + xa] != '$' && 
			sh->tokens[n].tokens[*x + xa] != '~')
		xa++;
	a = malloc(sizeof(char) * (xa + 1));
	if (!a)
		return (NULL);
	xa = 0;
	while (sh->tokens[n].tokens[*x] && 
			sh->tokens[n].tokens[*x] != '$' && 
			sh->tokens[n].tokens[*x] != '~')
	{
		a[xa] = sh->tokens[n].tokens[*x];
		(*x)++;
		xa++;
	}
	a[xa] = '\0';
	return (a);
}

char    *expand_token_seeker2(t_sh *sh, int *x, int n, char *c)
{
	char    b[500];
	int		bx;
	
	if(sh->tokens[n].tokens[*x] == '$' && 
		sh->tokens[n].tokens[*x + 1] ==  '?')
	{
		(*x)++;
		c = ft_itoa(g_status);
		(*x)++;
	}
	else if(sh->tokens[n].tokens[*x] == '$')
	{
		bx = 0;
		(*x)++;
		while(sh->tokens[n].tokens[*x] && 
				ft_isalpha(sh->tokens[n].tokens[*x]))
					b[bx++] = sh->tokens[n].tokens[(*x)++];
		b[bx] = '\0';
		c = search_envp(sh, b);
		if(!c)
		{
			c =ft_strdup(" ");
			sh->tokens[n].exp_empty = true;
			/*if(sh->vars.tk_num > 1)
			{
				sh->vars.cmds_num++;
				sh->tokens[n + 1].cmd = true;
			}*/
		}
	}
	return(c);
}

char    *expand_token_seeker(t_sh *sh, int *x, int n)
{
	char    *c;
	pid_t	pid;

	c = NULL;
	if(sh->tokens[n].tokens[*x] == '~' && 
		sh->tokens[n].d_quote == false &&
			sh->tokens[n].s_quote == false)
	{
		c = search_envp(sh, "HOME");
		(*x)++;
	}
	else if(sh->tokens[n].tokens[*x] ==  '$' &&
			sh->tokens[n].tokens[*x + 1] == '$')
	{
		(*x)++;
		pid = getpid();
		c = ft_itoa(pid);
		(*x)++;
	}
	else
		return(expand_token_seeker2(sh, x, n, c));
	return(c);
}

void    expand_token(t_sh *sh, int n)
{
	char    *z;
	int     x[1];
	int     exp_counter;
	int		counter;
	
	counter = 0;
	z = NULL;
	exp_counter = count_expands(sh, n);
	x[0] = 0;
	while(counter < sh->vars.tk_num)
	{
		z = join_2_str(z, pre_expand(sh, x, n), NULL, 1);
		if((size_t)x == ft_strlen(sh->tokens[n].tokens) - 1)
			break;
		z = join_2_str(z, expand_token_seeker(sh, x, n), NULL, 1);
		exp_counter--;
		if (exp_counter == 0)
		{
			z = expand_exit(sh, n, x[0], z);
			break;
		}
		counter++;
	}
	free(sh->tokens[n].tokens);
	sh->tokens[n].tokens = z;
}

void	search_expand(t_sh *sh)
{
	int	x;
	int n;

	n = 0;
	x = 0;
	while(n < sh->vars.tk_num)
	{
		if(sh->tokens[n].exp_e || sh->tokens[n].exp_t )
			expand_token(sh, n);
		else if(sh->tokens[n].d_quote && (sh->tokens[n].exp_e || sh->tokens[n].exp_t))
		{
			while(sh->tokens[n].tokens[x++])
				if(sh->tokens[n].tokens[x] == '$')
					expand_token(sh, n); 
		}
		else if(sh->tokens[n].file && sh->tokens[n].exp_t)
		{
			while(sh->tokens[n].tokens[x++])
			{
				if(sh->tokens[n].tokens[x] == '~')
					expand_token(sh, n);
			}
		}
		n++;
	}
}
