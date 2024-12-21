/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcosta-c <rcosta-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:54:43 by rcosta-c          #+#    #+#             */
/*   Updated: 2024/12/21 01:14:49 by rcosta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    init_tk_flag1(t_sh *sh, int x)
{
    sh->tokens[x].cmd = false;
	sh->tokens[x].arg = false;
	sh->tokens[x].pipe = false;
	sh->tokens[x].r_in = false;
	sh->tokens[x].r_out = false;
	sh->tokens[x].r_heredoc = false;
	sh->tokens[x].r_outappend = false;
	sh->tokens[x].s_quote = false;
	sh->tokens[x].d_quote = false;
	sh->tokens[x].f_quote = false;
	sh->tokens[x].envp = false;
	sh->tokens[x].file = false;
	sh->tokens[x].exp_e = false;
	sh->tokens[x].exp_t = false;
	sh->tokens[x].exp_empty = false;
}

void    init_cmds(t_sh *sh, int x)
{
	sh->comands[x].n_redir = 0;
    sh->comands[x].pipes = false;
	sh->comands[x].redir = false;
	sh->comands[x].infile = NULL;
	sh->comands[x].infile_fd = -1;
	sh->comands[x].inheredoc_fd = -1;
	sh->comands[x].inheredoc_file = NULL;
	sh->comands[x].outfile = NULL;
	sh->comands[x].outfile_fd = -1;
	sh->comands[x].outappend_fd = -1;
	sh->comands[x].outappendfile = NULL;
	sh->comands[x].inbackup = -1;
	sh->comands[x].outbackup = -1;
	sh->comands[x].arg = NULL;
	sh->comands[x].cmd = NULL;
	sh->comands[x].n_args = 0;
    sh->comands[x].errors.cmd_not_found = false;
    sh->comands[x].errors.empty_pipe = false;
    sh->comands[x].errors.infile_noaccess = false;
    sh->comands[x].errors.infile_notvalid = false;
    sh->comands[x].errors.outfile_noaccess = false;
    sh->comands[x].errors.outfile_notvalid = false;
	sh->comands[x].errors.empty_redir = false;
}

void	init_vars(t_sh *sh)
{
	sh->vars.tk_num = 0;
	sh->vars.cmds_num = 0;
	sh->vars.pipe_num = 0;
	sh->vars.redir_num = 0;
	sh->vars.heredoc_num = 0;
}

void	init_prompt_utils(t_sh *sh)
{
	sh->vars.sh_user = search_envp(sh, "USER");
	sh->vars.sh_host = find_my_host(sh);
	sh->vars.sh_pwd = search_envp(sh, "PWD");
	sh->vars.sh_pwd = verify_home(sh, sh->vars.sh_pwd);
	
}

