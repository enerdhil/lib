#ifndef TEST_H
# define TEST_H

# include <morphux.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>

# define DUP_ALL_OUTPUTS(name) dup2(name[1], 1); dup2(name[1], 2); close(name[1]);
# define WAIT_AND_CLOSE(pid, status, fd) pid = waitpid(pid, &status, 0); close(fd[1]);
# define OPT_STR_SIZE 150
# define OPT_DEF(val) { \
	{'q', "qwerty", "qwerty", val, &callback_q}, \
	{'w', "wertyu", "wertyu", val, &callback_w}, \
	{'e', "ertyui", "rtyuio", val, &callback_e}, \
	{'r', "rtyuio", "tyuiop", val, &callback_r}, \
	{'t', "tyuiop", "tyuiop", val, &callback_t}, \
	{'y', "yuiop[", "yuiop[", val, &callback_y}, \
	ARGS_EOL \
}

void	register_infos_tests(void);
void	register_args_tests(void);
void	register_list_tests(void);
void	register_tests_tests(void);
void	register_print_tests(void);
void		callback_q(const char *s);
void		callback_w(const char *s);
void		callback_e(const char *s);
void		callback_r(const char *s);
void		callback_t(const char *s);
void		callback_y(const char *s);
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
