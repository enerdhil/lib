#ifndef TEST_H
# define TEST_H
# define COMPILE_WITH_TEST

# include <morphux.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>

# define DUP_ALL_OUTPUTS(name) dup2(name[1], 1); dup2(name[1], 2); close(name[1]);
# define WAIT_AND_CLOSE(pid, status, fd) pid = waitpid(pid, &status, 0); close(fd[1]);
# define OPT_STR_SIZE 150
# define OPT_DEF(val) { \
	{.opt = 'q', .s_opt = "qwerty", .desc = "qwerty", .take_arg = val, .callback = &callback_q}, \
	{.opt = 'w', .s_opt = "wertyu", .desc = "wertyu", .take_arg = val, .callback = &callback_w}, \
	{.opt = 'e', .s_opt = "ertyui", .desc = "rtyuio", .take_arg = val, .callback = &callback_e}, \
	{.opt = 'r', .s_opt = "rtyuio", .desc = "tyuiop", .take_arg = val, .callback = &callback_r}, \
	{.opt = 't', .s_opt = "tyuiop", .desc = "tyuiop", .take_arg = val, .callback = &callback_t}, \
	{.opt = 'y', .s_opt = "yuiop[", .desc = "yuiop[", .take_arg = val, .callback = &callback_y}, \
	ARGS_EOL \
}

void	register_infos_tests(void);
void	register_args_tests(void);
void	register_list_tests(void);
void	register_tests_tests(void);
void	register_print_tests(void);
void    register_files_tests(void);
void    register_util_tests(void);
bool		callback_q(const char *s);
bool		callback_w(const char *s);
bool		callback_e(const char *s);
bool		callback_r(const char *s);
bool		callback_t(const char *s);
bool		callback_y(const char *s);
void		reset_args(void);


struct		margs_tests {
	bool		opt_q;
	bool		opt_w;
	bool		opt_e;
	bool		opt_r;
	bool		opt_t;
	bool		opt_y;
	char		str_q[OPT_STR_SIZE];
	char		str_w[OPT_STR_SIZE];
	char		str_e[OPT_STR_SIZE];
	char		str_r[OPT_STR_SIZE];
	char		str_t[OPT_STR_SIZE];
	char		str_y[OPT_STR_SIZE];
};

#endif /* TEST_H */
