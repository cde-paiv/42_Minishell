/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcosta-c <rcosta-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:54:13 by rcosta-c          #+#    #+#             */
/*   Updated: 2024/12/27 13:44:43 by rcosta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	ft_if_redir(t_sh *sh, int x)
{
	if(sh->tokens[x].r_in  == true && sh->tokens[x + 1].file == false)
		return(false);
	else if(sh->tokens[x].r_out == true  && sh->tokens[x + 1].file == false)
		return(false);
	else if(sh->tokens[x].r_heredoc == true && !sh->tokens[x + 1].tokens)
		return(false);
	else if(sh->tokens[x].r_outappend == true && sh->tokens[x + 1].file == false )
		return(false);	
	else if(sh->tokens[x].r_in  == true && sh->tokens[x + 1].file == true )
		return(true);
	else if(sh->tokens[x].r_out == true  && sh->tokens[x + 1].file == true )
		return(true);
	else if(sh->tokens[x].r_heredoc == true && sh->tokens[x + 1].tokens)
		return(true);
	else if(sh->tokens[x].r_outappend == true && sh->tokens[x + 1].file == true )
		return(true);
	else
		return(false);
}

void	ft_count_redirs(t_sh *sh, int x, int n_cmd)
{
	while(sh->tokens[x].pipe == false && x < sh->vars.tk_num)
	{
		if(sh->tokens[x].r_in == true || sh->tokens[x].r_heredoc == true
			|| sh->tokens[x].r_out == true || sh->tokens[x].r_outappend == true)
		{
			sh->comands[n_cmd].n_redir++;	
		}
		x++;	
	}	
//	printf("\nnumero de redires=%d\n\n", sh->comands[n_cmd].n_redir);
}


int		parse_with_args(t_sh *sh, int n_cmd, int x)
{
	int counter;
	int	narg;
	int	xtemp;
	
	narg = 0;
	xtemp = x;
//printf("\n\n NARGS=%d\nSISTEMA=%d\n", narg, sh->comands[n_cmd].n_args);
	sh->comands[n_cmd].arg = malloc(sizeof(char **) * (sh->comands[n_cmd].n_args + 2));
	sh->comands[n_cmd].cmd = ft_strdup(sh->tokens[x - 1].tokens);
	sh->comands[n_cmd].arg[narg++] = ft_strdup(sh->tokens[x - 1].tokens);
	while(sh->tokens[x].pipe == false && x < sh->vars.tk_num)
	{
		if(sh->tokens[x].arg == true)
			sh->comands[n_cmd].arg[narg++] = ft_strdup(sh->tokens[x].tokens);
		x++;
	}
	if (sh->comands[n_cmd].n_args > 0)
		sh->comands[n_cmd].arg[narg] = NULL;
	else
		sh->comands[n_cmd].arg[1] = NULL;
	x = xtemp;	
	ft_count_redirs(sh, x, n_cmd);
	if(sh->comands[n_cmd].n_redir > 0)
	{
		counter = 0;
		sh->comands[n_cmd].redir = true;
		while(sh->tokens[x].pipe == false && x < sh->vars.tk_num)
		{
			if(sh->tokens[x - 1].r_in  == true)
			{
				if(sh->tokens[x].file == true )
					sh->comands[n_cmd].infile = ft_strdup(sh->tokens[x].tokens);
				else
					sh->comands[n_cmd].errors.empty_redir = true;
				counter++;
			}
			else if(sh->tokens[x - 1].r_out == true)
			{	if(sh->tokens[x].file == true)
					sh->comands[n_cmd].outfile = ft_strdup(sh->tokens[x].tokens);
				else
					sh->comands[n_cmd].errors.empty_redir = true;
				counter++;
			}
			else if(sh->tokens[x - 1].r_heredoc == true)
			{
				if(sh->tokens[x].tokens)
					sh->comands[n_cmd].inheredoc_file = ft_strdup(sh->tokens[x].tokens);
				else
					sh->comands[n_cmd].errors.empty_redir = true;
				counter++;
			}
			else if(sh->tokens[x - 1].r_outappend == true)
			{
				if(sh->tokens[x].file == true)
					sh->comands[n_cmd].outappendfile = ft_strdup(sh->tokens[x].tokens);
				else
					sh->comands[n_cmd].errors.empty_redir = true;
				counter++;
			}
			x++;
		}
	}
//	printf("\nvai retornar x=%d\n\n", x);
	return(x);
}

int		parse_no_args(t_sh *sh, int n_cmd, int x)
{
	int counter;
	
//	printf("\n\nHEY HEY ESTOU AQUI NO ARGS!!x=%d!!\n\n", x);
	sh->comands[n_cmd].arg = malloc(sizeof(char **) * 2); 
	sh->comands[n_cmd].cmd = ft_strdup(sh->tokens[x].tokens); 
	sh->comands[n_cmd].arg[0] = ft_strdup(sh->tokens[x].tokens);
	sh->comands[n_cmd].arg[1] = NULL;
	sh->comands[n_cmd].n_args = 0;
	x++;
	ft_count_redirs(sh, x, n_cmd);
	if(sh->comands[n_cmd].n_redir > 0)
	{
		counter = 0;
		sh->comands[n_cmd].redir = true;
		while(counter < sh->comands[n_cmd].n_redir && x < sh->vars.tk_num)
		{
			if(sh->tokens[x - 1].r_in  == true)
			{
				if(sh->tokens[x].file == true )
					sh->comands[n_cmd].infile = ft_strdup(sh->tokens[x].tokens);
				else
					sh->comands[n_cmd].errors.empty_redir = true;
				counter++;
			}
			else if(sh->tokens[x - 1].r_out == true)
			{	if(sh->tokens[x].file == true)
					sh->comands[n_cmd].outfile = ft_strdup(sh->tokens[x].tokens);
				else
					sh->comands[n_cmd].errors.empty_redir = true;
				counter++;
			}
			else if(sh->tokens[x - 1].r_heredoc == true)
			{
				if(sh->tokens[x].tokens)
					sh->comands[n_cmd].inheredoc_file = ft_strdup(sh->tokens[x].tokens);
				else
					sh->comands[n_cmd].errors.empty_redir = true;
				counter++;
			}
			else if(sh->tokens[x - 1].r_outappend == true)
			{
				if(sh->tokens[x].file == true)
					sh->comands[n_cmd].outappendfile = ft_strdup(sh->tokens[x].tokens);
				else
					sh->comands[n_cmd].errors.empty_redir = true;
				counter++;
				
			}
			x++;
		}
	}
	return(x);
}

int		parse_no_cmds(t_sh *sh, int n_cmd, int x)
{
	int		narg;
//	int		counter;
	
	printf("\n\nestou no cmds\n x=%d     ncmd=%d\n\n", x, n_cmd);
	narg = 0;
	sh->vars.cmds_num = 1;
	init_cmds(sh, n_cmd);
	sh->comands[n_cmd].arg = malloc(sizeof(char **) * (sh->vars.tk_num + 1)); 
	sh->comands[n_cmd].cmd = ft_strdup(sh->tokens[x].tokens);
	if(sh->vars.tk_num > 1)
	{
		while(x < sh->vars.tk_num)
			sh->comands[n_cmd].arg[narg++] = ft_strdup(sh->tokens[x++].tokens);
		sh->comands[n_cmd].arg[narg] = NULL;
		sh->comands[n_cmd].n_args = narg - 1;
		x++;
	}
	else
	{
		sh->comands[n_cmd].arg[0] = ft_strdup(sh->tokens[x].tokens);
		sh->comands[n_cmd].arg[1] = NULL;
		sh->comands[n_cmd].n_args = 0;
		x++;
	}

	return(x);
}

int	parse_utils(t_sh *sh, int x, int n_cmd)
{
	int	counter;
	int xtemp;

	counter = 0;
   	if(sh->tokens[x].cmd) 
	{
		xtemp = x;
		while(sh->tokens[x].pipe == false && x < sh->vars.tk_num)
		{
			if(sh->tokens[x].arg == true)
				sh->comands[n_cmd].n_args++;
			x++;
		}
		x = xtemp;	
		if(x < sh->vars.tk_num && sh->comands[n_cmd].n_args >= 1)
		{
			x++;
			x = parse_with_args(sh, n_cmd, x);		
		}
		else
			x = parse_no_args(sh, n_cmd, x);
	}
	else
		x = parse_no_cmds(sh, n_cmd, x);
	return(x);
}

/*int	parse_utils(t_sh *sh, int x, int n_cmd)
{

   	if(sh->tokens[x].cmd) 
	{	
		if(x < sh->vars.tk_num && sh->tokens[x + 1].arg == true)
		{
			x++;
			while(sh->tokens[x].arg == true && x < sh->vars.tk_num)
			{
				sh->comands[n_cmd].n_args++;
				x++;
			}
			if(sh->comands[n_cmd].n_args > 0)
				x -= sh->comands[n_cmd].n_args;
			x = parse_with_args(sh, n_cmd, x);		
		}
		else
			x = parse_no_args(sh, n_cmd, x);
	}
	else
		x = parse_no_cmds(sh, n_cmd, x);
	return(x);
}*/

int	parse_pipes(t_sh *sh, int z, int n_cmd)
{
	int	x;

	x = z;
	if (sh->vars.pipe_num == 0)
		return (x);
	if(sh->tokens[x].pipe)
	{
		sh->comands[n_cmd].pipes = true; // Marca que este comando usa pipe
		x++;
	}
	return(x);
}