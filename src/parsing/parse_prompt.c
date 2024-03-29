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

static void	*syntax_error_free(char **str)
{
	int	i;

	i = 0;
	printf(SYNTAX_ERR_QUOTES);
	if (str)
	{
		while (str[i])
		{
			free(str[i]);
			i++;
		}
		*str = NULL;
		if (str)
			free(str);
	}
	return (NULL);
}

static int	check_which_case(char *str, char **rt, int i)
{
	while (i != -1 && str[i] && is_whitespace(str[i]))
		i++;
	if (is_pipe_redir(str[i]))
		i = pipe_redir(str, rt, i);
	else
	{
		while (i != -1 && str[i] && !is_whitespace(str[i])
			&& !is_pipe_redir(str[i]))
		{
			if (str[i] != '\'' && str[i] != '"' && !is_whitespace(str[i]))
				i = string(str, rt, i);
			else if (str[i] == '\'')
				i = single_quote(str, rt, i);
			else if (str[i] == '"')
				i = double_quote(str, rt, i);
		}
	}
	return (i);
}

static char	**parse_expanded_prompt(char *expanded_prompt)
{
	char	**rt;
	int		i;
	int		j;

	i = 0;
	j = 0;
	rt = malloc(sizeof(rt));
	if (!rt)
		return (NULL);
	while (i != -1 && expanded_prompt[i])
	{
		rt[j] = 0;
		i = check_which_case(expanded_prompt, &rt[j++], i);
		rt = ft_realloc(rt, j * sizeof(rt), (j + 1) * sizeof(rt));
		if (!rt)
			return (NULL);
	}
	rt[j] = 0;
	if (i == -1)
		return (syntax_error_free(rt));
	return (rt);
}

char	**parse_prompt(char *str, t_msh *msh)
{
	char	*expanded_prompt;
	char	**rt;

	if (!str)
		return (NULL);
	expanded_prompt = expand_prompt(str, msh);
	rt = parse_expanded_prompt(expanded_prompt);
	free(expanded_prompt);
	return (rt);
}
