#ifndef SYNTAX_H
# define SYNTAX_H

int	check_unclosed_quotes(char *line);
void	resolve_command_path(t_command *cmd, char **env);

#endif
