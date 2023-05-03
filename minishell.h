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
# define BUFFER_SIZE 1024

typedef enum e_flag {
	ENVIRON = -2,
	BLANK,
	CLOSED,
	FREE //del
}	t_flag;

typedef enum e_state {
	BUILTIN,
	GENERAL
}	t_state;

typedef enum e_ftype {
	INFILE,
	OUTFILE,
	APPEND,
	DELIMITER,
	Q_DELIMITER
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

// deque
void	init_deque(t_deque *deque);
void	init_element(t_token *element, char **parsed);
// void	append_front(t_deque *deque, char *command);
void	append_back(t_deque *deque, t_token *token);
t_token	*pop_front(t_deque *deque);
t_token	*pop_back(t_deque *deque);
void	free_deque(t_deque *deque);
int		deque_is_empty(t_deque *pdeque);
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

// file list
int		check_redir(char *str, t_token *token);
int		get_filename(char *str, t_fdata *new, t_token *token);
void	append_file(t_fdata **head, t_fdata *new);
void	free_files(t_fdata **lst);
void	print_filelst(t_deque *cmd_lst); // delete

// parsing
char	**ft_pipe_split(char *str);
char	**parse_command(char *str, t_token *token);
int		syntax_error(char *str);
void	make_cmdlst(char *str, t_deque *cmd_deque, t_edeque *envp);
int		is_blank(char c);

// environ
char	*expand_environ(char *str, t_token *token, int quote);
int		env_trans(char *str, t_edeque *envp, char *buf);
int		search_env(char **str, char *buf, t_edeque *envp, int quote);

// builtins
int		exist_args(t_token *token);
t_env	*find_value(t_edeque *envp, char *key);
char	*ft_getenv(t_edeque *envp, char *key);
void	change_env(t_token *token, char *dest);
int		exec_pwd(t_token *token);
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
void	print_args(t_token *token, int target_idx);
int		is_builtin(char *cmd);

// pipe
void	parents_process(t_deque *cmd_deque);
void	check_file(t_token *line);
int		check_infile(char *filename);
int		check_outfile(char *filename);
void	wait_child(int count, t_deque *cmd_deque);
void	find_child(t_deque *cmd_deque, int status, pid_t pid);
void	close_pipe(int (*fd)[2], int count);
void	create_child(t_deque *cmd_deque, int (*fd)[2]);
int		(*create_pipe(t_deque *cmd_deque))[2];
void	child_process(t_token *line, int count, int total, int (*fd)[2]);
void	manage_pipe(int count, int total, int (*fd)[2]);
void	manage_file(t_token *line);

void	open_infile(char *filename, int *infile_fd);
void	open_outfile(char *filename, int *outfile_fd, int append_flag);

void	assign_argument(char **str, char *av);
void	get_size(char **arguments, char *av);
void	get_size_step1(char *av, char *quote, int *size);
void	get_size_step2(char **arguments, char *av, int *size, int *index);
int		count_rows(char *argument);
char	**strs_trim(char **before, int row);

char	**make_envstrs(t_token *token);
void	execute_line(t_token *line, char **env);
void	manage_io(t_token *line, int count, int total, int (*fd)[2]);

//here_doc
void	find_here_doc(t_deque *cmd_deque);
void	open_here_doc(t_fdata *cur_file, int count);
char	*get_next_line(int fd);

#endif
