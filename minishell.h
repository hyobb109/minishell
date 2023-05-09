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
# include <sys/stat.h>

# define PATH_MAX 4096
# define ARG_MAX 262144
# define READ 0
# define WRITE 1
# define TRUE 1
# define FALSE 0
# define BUFFER_SIZE 1024

typedef enum e_flag {
	EMPTY = -2,
	BLANK,
	CLOSED,
}	t_flag;

typedef enum e_state {
	P_BUILTIN,
	BUILTIN,
	GENERAL
}	t_state;

typedef enum e_ftype {
	INFILE,
	OUTFILE,
	APPEND,
	LIMITER,
	Q_LIMITER
}	t_ftype;

typedef struct s_fdata
{
	char			filename[PATH_MAX];
	int				type;
	struct	s_fdata	*next;
}	t_fdata;

typedef struct s_syntax
{
	char	flag;
	int		heredoc;
	int		i;
	char	*tmp;
}	t_syntax;

typedef struct s_vars
{
	int		i;
	int		len;
	int		flag;
	char	quote;
}	t_vars;

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
	int				func; // builtin?
	int				status; // exit code
	int				infile_fd;
	int				outfile_fd;
	t_fdata			*files;
	t_edeque		*envp;
}	t_token;

typedef struct s_deque
{
	t_token	*head;
	t_token	*tail;
	int		cnt;
}	t_deque;


typedef struct s_matrix
{
	int row;
	int column;
}	t_matrix;

extern int	g_exit_status;

// deque
void	init_deque(t_deque *deque);
void	init_element(t_token *element, char **parsed);
// void	append_front(t_deque *deque, char *command);
void	append_back(t_deque *deque, t_token *token);
t_token	*pop_front(t_deque *deque);
t_token	*pop_back(t_deque *deque);
void	free_deque(t_deque *deque);
void	print_deque(t_deque *deque); //delete

//edeque
void	init_edeque(t_edeque *deque);
void	free_edeque(t_edeque *deque);
void	make_envlst(t_edeque *envp, char **env);
void	append_back_env(t_edeque *deque, t_env *env);
t_env	*pop_front_env(t_edeque *deque);
t_env	*pop_back_env(t_edeque *deque);
t_env	*pop_select_env(t_edeque *deque, char *key);
void	print_edeque(t_edeque *deque); // delete

// signal
void	signal_handler(int sig);
void	signal_handler_heredoc(int sig);
void	ft_signal_set(void);

// file list
void	check_redir(char **str, t_token *token);
void	get_filename(char **str, t_fdata *new, t_token *token);
void	append_file(t_fdata **head, t_fdata *new);
void	free_files(t_fdata **lst);
void	print_filelst(t_deque *cmd_lst); // delete

// parsing
char	**ft_pipe_split(char *str);
char	**parse_command(char *str, t_token *token);
int		syntax_error(char *str);
int		quote_error(char *str);
int		empty_error(char *str);
void	make_cmdlst(char *str, t_deque *cmd_deque, t_edeque *envp);
int		is_blank(char c);

// environ
int		env_trans(char *str, t_edeque *envp, char *buf, t_vars v);
int		search_env(char **str, char *buf, t_edeque *envp, t_vars v);

// builtins
int		exist_args(t_token *token);
t_env	*find_value(t_edeque *envp, char *key);
char	*ft_getenv(t_edeque *envp, char *key);
int		change_env(t_token *token, char *cwd_name);
int		exec_pwd(void);
void	exec_exit(t_token *token);
int		chdir_home(void);
char	*make_dirstr(char *str, char *cwd_name, char *parent_dir, char *home_dir);
int		exec_cd(t_token *token);
char	**make_strmatrix(t_edeque *envp);
char	**sorting_strsarr(t_edeque *envp);
void	print_export(t_env	*print_env);
void	print_exportlist(t_token *token);
int		exec_export(t_token *token);
char	*join_all(t_token *token, int idx);
int		check_option(char **parsed);
int		exec_echo(t_token *echo);
int		init_validkeyflag(t_token *token, int idx, char *tmp, int *flag);
int		exist_validkey(t_token *token);
void	print_invalidargserror(t_token *token);
void	print_envlist(t_token *token);
int		exec_env(t_token *token);
int		exec_builtins(t_token *token);
int		is_builtin(char *cmd);
int		exec_unset(t_token *token);
char	*swapfree_strs(char *result, char *result2);
t_env	*init_envelem(char *key, char *value);
int		is_validkey(char *key);

// pipe
void	parents_process(t_deque *cmd_deque);
void	only_builtins(t_deque *cmd_deque, int (*fd)[2]);
void	check_file(t_token *line);
int		check_infile(char *filename, t_token *token);
int		check_outfile(char *filename, t_token *token);
void	wait_child(int count, t_deque *cmd_deque);
void	find_child(t_deque *cmd_deque, int status, pid_t pid);
void	close_pipe(int (*fd)[2], int count);
void	create_child(t_deque *cmd_deque, int (*fd)[2]);
int		(*create_pipe(t_deque *cmd_deque))[2];
void	child_process(t_token *line, int count, int total, int (*fd)[2]);
void	manage_pipe(int count, int total, int (*fd)[2]);
int		manage_file(t_token *line);

int		open_infile(char *filename, int *infile_fd, int func);
// int		open_outfile(char *filename, int *outfile_fd, int append_flag, int func);
int		open_outfile(t_fdata *cur_file, int *outfile_fd, int func);

void	assign_argument(char **str, char *av);
void	get_size(char **arguments, char *av);
void	get_size_step1(char *av, char *quote, int *size);
void	get_size_step2(char **arguments, char *av, int *size, int *index);
int		count_rows(char *argument);
char	**strs_trim(char **before, int row);

char	**make_envstrs(t_token *token);
void	execute_line(t_token *line, char **env);
void	manage_io(t_token *line, int count, int total, int (*fd)[2]);

pid_t	ft_fork(void);
int		ft_dup2(int old_fd, int new_fd, int func);
int		ft_close(int fd, int func);
int		ft_pipe(int fd[2]);

//here_doc
int		find_here_doc(t_deque *cmd_deque);
int		open_here_doc(t_token *cur_token, t_fdata *cur_file, int count);
char	*here_doc_make_name(int count);
void	exec_here_doc(t_token *cur_token, t_fdata *cur_file, char *here_doc_name);
void	here_doc_write(t_token *cur_token, t_fdata *cur_file, char *line, int here_doc_fd);
void	unlink_here_doc(t_deque *cmd_deque);
char	*check_env_var(char *line, t_edeque *envp);
char	*get_next_line(int fd);

#endif