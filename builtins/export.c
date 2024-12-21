/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mota <mota@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:08:39 by mota              #+#    #+#             */
/*   Updated: 2024/12/21 02:35:09 by mota             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_valid_identifier(const char *arg)
{
    int i;

    if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
        return (0);
    i = 0;
    while (arg[i] && arg[i] != '=' && arg[i] != '+')
    {
        if (!ft_isalnum(arg[i]) && arg[i] != '_')
            return (0);
        i++;
    }
    if (arg[i] == '+' && arg[i + 1] != '=')
        return (0);
    return (1);
}

static void handle_export_var(t_sh *sh, const char *arg)
{
    char *equal_sign;
    char *var_name;
    char *value_to_add;
    char *search_var;
    int var_pos;
    char *existing_value;
    char *temp_value;
    char *new_var;

    equal_sign = ft_strchr(arg, '=');
    if (!equal_sign)
        return ;
    if (*(equal_sign - 1) == '+')
    {
        var_name = ft_substr(arg, 0, equal_sign - arg - 1);
        value_to_add = equal_sign + 1;
        search_var = ft_strjoin(var_name, "=");
        var_pos = get_var_pos(sh, search_var);
        if (var_pos >= 0)
        {
            existing_value = ft_strchr(sh->envp[var_pos], '=') + 1;
            temp_value = ft_strjoin(existing_value, value_to_add);
            new_var = ft_strjoin(search_var, temp_value);
            free(sh->envp[var_pos]);
            sh->envp[var_pos] = new_var;
            free(temp_value);
        }
        else 
        {
            new_var = ft_strjoin(search_var, value_to_add);
            update_var(sh, new_var);
            free(new_var);
        }
        free(var_name);
        free(search_var);
    }
    else 
    {
        update_var(sh, (char *)arg);
    }
}

void ft_export(t_sh *sh, char **args)
{
    int i;
    int status;

    i = 1;
    status = 0;
    if (!args[1]) 
    {
        display_exported_vars(sh);
        return;
    }
    while (args[i])
    {
        if (!is_valid_identifier(args[i]))
        {
            ft_putstr_fd("export: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd("`: not a valid identifier\n", 2);
            status = 1;
        }
        else 
        {
            handle_export_var(sh, args[i]);
        }
        i++;
    }
    sh->error.exit_error = (status != 0);
}