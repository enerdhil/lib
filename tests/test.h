#ifndef TEST_H
# define TEST_H

# include <morphux.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

void	register_infos_tests(void);
void	register_args_tests(void);

struct		margs_tests {
	bool		opt_q;
	bool		opt_w;
	bool		opt_e;
	bool		opt_r;
	bool		opt_t;
	bool		opt_y;
};

#endif /* TEST_H */
