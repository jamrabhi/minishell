/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamrabhi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 21:30:50 by jamrabhi          #+#    #+#             */
/*   Updated: 2023/01/12 21:30:55 by jamrabhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_rt(char **rt, char *tmp)
{
	char	*rt_cpy;

	if (*rt)
	{
		rt_cpy = ft_strdup(*rt);
		free(*rt);
		*rt = ft_strjoin(rt_cpy, tmp);
		free(rt_cpy);
	}
	else
		*rt = ft_strdup(tmp);
}

static int	check_which_case(char *str, char **rt, int i, t_msh *msh)
{
	while (str[i] && is_whitespace(str[i]))
		i++;
	if (is_pipe_redir(str[i]))
		i = pipe_redir_case(str, rt, i, msh);
	else
	{
		while (str[i] && !is_whitespace(str[i]) && !is_pipe_redir(str[i]))
		{
			if (str[i] != '\'' && str[i] != '"' && str[i] != '$'
				&& !is_whitespace(str[i]))
				i = string_case(str, rt, i, msh);
			else if (str[i] == '\'')
				i = single_quote_case(str, rt, i, msh);
			else if (str[i] == '"')
				i = double_quote_case(str, rt, i, msh);
			else if (str[i] == '$')
				i = dollar_case(str, rt, i, msh);
		}
	}
	return (i);
}

char	**parse_prompt(char *str, t_msh *msh)
{
	int		i;
	int		j;
	char	**rt;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	rt = malloc(sizeof(rt));
	if (!rt)
		return (NULL);
	while (str[i])
	{
		rt[j] = NULL;
		i = check_which_case(str, &rt[j], i, msh);
		j++;
		rt = ft_realloc(rt, j * sizeof(rt), (j + 1) * sizeof(rt));
		if (!rt)
			return (NULL);
	}
	rt[j] = 0;
	return (rt);
}
