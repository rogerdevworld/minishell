/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXECUTOR_H
# define EXECUTOR_H

void	ft_check_executor(t_command *cmd, t_token *token, char **envp);
void	ft_exec_cmd(t_command *cmd, char **envp);
void	ft_pipe_and_fork(t_command *cmd, char **env);

#endif