/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 16:08:22 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/31 16:08:25 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



//TO DEL, REPLACED BY heredoc file shenanigans

char	**sep_input(char	*str)
{
	char	**ret;
	char	*tmp;
	int		i;

	ret = ft_split(str, "\n");
	i = -1;
	while (ret[++i])
	{
		tmp = ft_strdup(ret[i]);
		free(ret[i]);
		if (!tmp)
			//ERROR
		ret[i] = ft_strjoin(tmp, "\n");
		free(tmp);
		if (!ret[i])
			//ERROR
	}
	return (ret);
}

char	**append_to_cmds(char **cmd, char **split_input, int nb_of_input)
{

		//delimiter vars
	char	*hd_input;
	
	int		i;
	int		j;
	char	*tmp;
	char	**ret;

	i = 0;
	while (cmd[i])
		i++;
	ret = malloc(sizeof(char *) * (i + nb_of_input + 1));
	j = -1;
	while (cmd[++j])
	{
		ret[j] = ft_strdup(cmd[j]);
		if (ret[j])
			//ERROR
		free(cmd[j]);
	}
	j = 0;
	while (split_input[j])
		ret[i++] = split_input[j++];
	//LOOP FREE CMD HERE ??
	return (ret);
}

	//int	get_hd_input(char **str);
		// Block of text down = fct

		if (shell->cmds[i].redir_in.doc)
		{
			while (shell->cmds[i].redir_in.doc)
			{
				input = readline(">");
				if (!ft_strncmp(input, shell->cmds[i].redir_in.doc->delimiter, ft_strlen(input)))
				{
					if (shell->cmds[i].redir_in.doc->next == NULL)
					{
						free(input);
						break;
					}
					else
					{
						shell->cmds[i].redir_in.doc = shell->cmds[i].redir_in.doc->next;
						free(here_doc);
						here_doc = NULL;
						cmds_here_doc = 0;
					}
				}
				if (!here_doc)
					here_doc = ft_strdup("");
				tmp = ft_strdup(here_doc);
				free(here_doc);
				here_doc = ft_strjoin(here_doc, input);	
				free(tmp);
				//repeat strjoin with \n
				tmp = ft_strdup(here_doc);
				free(here_doc);
				here_doc = ft_strjoin(here_doc, "\n");	
				free(tmp);
				cmds_here_doc++;
		
				free(input);
			}







			//FREE CMDS->AV ? Or in append ?
			//append_to_cmds(cmd, sep_input(hd_input));