/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:16:48 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/15 14:17:30 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# define PATH_MAX 4096
# define ARG_MAX 262144
# define READ 0
# define WRITE 1

typedef struct s_token
{
	char			*command;
	char			*args;
	pid_t			pid;
	int				pre_fds[2];
	int				new_fds[2];
	struct s_token	*prev;
	struct s_token	*next;

	//TODO - infile/outfile
	int				state; //enum builtin, redirection etc.
	char			*infile;
	char			*outfile;
}	t_token;

typedef struct s_deque
{
	t_token	*head;
	t_token	*tail;
	int		cnt;
}	t_deque;

typedef struct s_file
{
	char *filepath;
	char **env;
	int pre_fds[2];
	int new_fds[2];
	t_token *cur_com;
} t_file;

typedef struct s_matrix
{
	int row;
	int column;
}	t_matrix;


typedef enum e_flag {
	CLOSED
}	t_flag;

typedef enum e_state {
    BUILTIN,
    GENERAL
}   t_state;

// quoting parsing tmp
char	**parse_command_tmp(char *av);
void	get_size_tmp(char **arguments, char *av);
int		count_rows_tmp(char *argument);
void	get_size_step2_tmp(char **arguments, char *av, int *size, int *index);
void	get_size_step1_tmp(char *av, char *quote, int *size);
void	assign_argument_tmp(char **str, char *av);

// deque
void	init_deque(t_deque *deque);
void	init_element(t_token *element, char **parsed);
// void	append_front(t_deque *deque, char *command);
void	append_back(t_deque *deque, t_token *token);
// t_token	*pop_front(t_deque *deque);
t_token	*pop_back(t_deque *deque);
void	free_deque(t_deque *deque);
void	print_deque(t_deque *deque); //delete

// parsing
char	*join_all(char **strs, int idx);
char	**ft_pipe_split(char *str);
void	syntax_check(char *str);
void	make_cmdlst(char *str, t_deque *cmd_deque, char **env);

// builtins
int		exec_pwd(void);
int		exec_exit(t_token *token);
void	assign_argument(char **str, char *av);
void	get_size(char **arguments, char *av);
void	get_size_step1(char *av, char *quote, int *size);
void	get_size_step2(char **arguments, char *av, int *size, int *index);
int		count_rows(char *argument);
int		check_option(char **parsed);
char	**parse_args(char *av);
int		exec_echo(t_token *echo);
int		exec_builtins(t_token *token);
void	print_args(char **arguments, int target_idx);

// pipe
void	make_pipefork(t_deque *com_deque, t_file *file, char **environ);
void	make_comdeque(t_deque *com_deque, int ac, char **av);
void	split_comopt(t_deque *com_deque, char *str, int fd);
void	parents_proc(t_file *file, pid_t *pid);
void	child_proc(t_file *file);
void	firchild_proc(t_file *file, char **arr, int *open_fd);
void	secchild_proc(t_file *file, char **arr, int *open_fd);
void	prefds_proc(t_file *file);
void	wait_processes(int len);
void	find_execpath(t_file *file, char **arr);
char	*matching_path(char **path_str, char **arr);
int		exec_builtins(t_token *token);
void	assign_argument(char **str, char *av);
void	get_size(char **arguments, char *av);
void	get_size_step1(char *av, char *quote, int *size);
void	get_size_step2(char **arguments, char *av, int *size, int *index);
int		count_rows(char *argument);

#endif