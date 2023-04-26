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
# define TRUE 1
# define FALSE 0

typedef enum e_flag {
	CLOSED
}	t_flag;

typedef enum e_state {
	BUILTIN,
	GENERAL
}	t_state;

typedef enum e_ftype {
	INFILE,
	OUTFILE,
	APPEND,
	DELIMITER
}	t_ftype;

typedef struct s_fdata
{
	char			filename[PATH_MAX];
	int				type;
	struct	s_fdata	*next;
}	t_fdata;

// delete
typedef struct s_fd
{
	char	filename[PATH_MAX];
	int		type;
}	t_fd;

typedef struct s_env
{
	char			*key;
	char			*val;
	struct s_env	*prev;
	struct s_env	*next;
}t_env;

typedef struct s_edeque
{
	t_env	*head;
	t_env	*tail;
	int		cnt;
}	t_edeque;

typedef struct s_token
{
	char			**command;
	pid_t			pid;
	int				pre_fds[2];
	int				new_fds[2];
	struct s_token	*prev;
	struct s_token	*next;

	//TODO - infile/outfile
	int				state; // builtin?
	int				status; // exit code
	t_fdata			*files;
	t_edeque		*envp;
}	t_token;

typedef struct s_deque
{
	t_token	*head;
	t_token	*tail;
	int		cnt;
}	t_deque;

typedef struct s_file
{
	char 	*filepath;
	char	**env;
	int		pre_fds[2];
	int		new_fds[2];
	t_token *cur_com;
} t_file;

typedef struct s_matrix
{
	int row;
	int column;
}	t_matrix;

// quoting
// void	assign_argument(char **str, char *av);
// void	get_size(char **arguments, char *av);
// void	get_size_step1(char *av, char *quote, int *size);
// void	get_size_step2(char **arguments, char *av, int *size, int *index);
// int		count_rows(char *argument);

// deque
void	init_deque(t_deque *deque);
void	init_edeque(t_edeque *deque);
void	init_element(t_token *element, char **parsed);

// void	append_front(t_deque *deque, char *command);
void	append_back(t_deque *deque, t_token *token);
void	append_back_env(t_edeque *deque, t_env *env);
t_token	*pop_front(t_deque *deque);
t_env	*pop_front_env(t_edeque *deque);
t_token	*pop_back(t_deque *deque);
t_env	*pop_back_env(t_edeque *deque);
void	free_deque(t_deque *deque);
void	free_edeque(t_edeque *deque);
void	print_deque(t_deque *deque); //delete
void	print_edeque(t_edeque *deque); // delete
int		deque_is_empty(t_deque *pdeque);

// parsing
char	*join_all(char **strs, int idx);
char	**ft_pipe_split(char *str);
void	syntax_check(char *str);
void	make_cmdlst(char *str, t_deque *cmd_deque, char **env);
int		is_blank(char c);
void	parse_command(char *str, t_token *token);
int		env_trans(char *str, int *idx, char *buf, t_edeque *envp);


// builtins
int		exec_pwd(t_token *token);
void	exec_exit(t_token *token);
int		chdir_home(void);
char	*make_dirstr(char *str);
int		exec_cd(t_token *token);
int		exec_export(t_token *token);
int		check_option(char **parsed);
int		exec_echo(t_token *echo);
int		exist_args(t_token *token);
int		init_validkeyflag(t_token *token, int idx, char *tmp, int *flag);
int		exist_validkey(t_token *token);
void	print_invalidargserror(t_token *token);
void	print_envlist(t_token *token);
int		exec_env(t_token *token);
int		exec_builtins(t_token *token);
void	print_args(char **arguments, int target_idx);
int		is_builtin(char *cmd);

// pipe
void	parents_process(t_deque *cmd_deque);
void	wait_child(int count);
void	close_pipe(int (*fd)[2], int count);
void	create_child(t_deque *cmd_deque, int (*fd)[2]);
int		(*create_pipe(t_deque *cmd_deque))[2];
void	child_process(t_token *line, int count, int total, int (*fd)[2]);
void	manage_pipe(int count, int total, int (*fd)[2]);

void	assign_argument(char **str, char *av);
void	get_size(char **arguments, char *av);
void	get_size_step1(char *av, char *quote, int *size);
void	get_size_step2(char **arguments, char *av, int *size, int *index);
int		count_rows(char *argument);
char	**strs_trim(char **before, int row);

#endif