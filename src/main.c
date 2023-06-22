/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 21:11:43 by amenses-          #+#    #+#             */
/*   Updated: 2023/06/22 21:56:36 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

char	*ft_readline(char *prompt)
{
	char	*line;
	char	*tmp;

	ft_putstr_fd(prompt, STDOUT_FILENO);
	tmp = get_next_line(STDIN_FILENO);
	line = tmp;
	if (tmp)
		line = ft_substr(tmp, 0, ft_strlen(tmp) - 1);
	free(tmp);
	return (line);
}

static void	ctrl_d(char *line, t_app **app)
{
	if (!line)
	{
		rl_clear_history();
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		terminate(app);
		exit(g_exit_status);
	}
}

static int	mini_loop(t_app *app)
{
	char	*l;
	char	*ppt;

	sig_config();
	ppt = set_prompt();
	l = readline(ppt);
	free(ppt);
	ctrl_d(l, &app);
	add_history(l);
	app->input = ft_strtrim(l, " ");
	free(l);
	if (!app->input)
		return (mini_perr(PRE, "malloc", 1, 0));
	if (!check_quotes(app->input))
		return (ft_error(2, app));
	if (!get_tokens(app))
		return (ft_error(1, app));
	if (parser(app) != 0 || expander(app) != 0 || executor(app) != 0)
		return (reset(app), g_exit_status);
	reset(app);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_app	*app;

	(void)argc;
	(void)argv;
	rl_clear_history();
	app = ft_calloc(1, sizeof(t_app));
	if (!app)
		return (mini_perr(PRE, "malloc", 1, 0));
	fill_env_list(app, envp);
	while (1)
	{
		mini_loop(app);
		reset(app);
	}
	terminate(&app);
	return (0);
}
