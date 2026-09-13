/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cmp_ping.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 10:52:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/08/03 10:52:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef FT_PING_BIN
# define FT_PING_BIN "./ft_ping"
#endif

#ifndef SYS_PING_BIN
# define SYS_PING_BIN "/bin/ping"
#endif

# define C_RESET   "\033[0m"
# define C_BOLD    "\033[1m"
# define C_DIM     "\033[2m"
# define C_RED     "\033[31m"
# define C_GREEN   "\033[32m"
# define C_YELLOW  "\033[33m"
# define C_BLUE    "\033[34m"
# define C_CYAN    "\033[36m"

typedef struct s_capture
{
	char	*data;
	size_t	len;
	int		status;
}	t_capture;

static int	g_passed;
static int	g_failed;
static int	g_total;

static void	assert_ok(int cond, const char *msg)
{
	if (!msg)
		return ;

	g_total++;
	if (cond) {
		g_passed++;
		printf("  " C_GREEN C_BOLD "[OK]" C_RESET "   %s\n", msg);
	} else {
		g_failed++;
		printf("  " C_RED C_BOLD "[FAIL]" C_RESET " %s\n", msg);
	}
}

/////////////////////////////////////
//
//			CAPTURE (fork + pipe)
//
////////////////////////////////////

static t_capture	run_capture(char *const argv[])
{
	t_capture	cap;
	int			pfd[2] = { 0, 0 };
	pid_t		pid = 0;
	char		buf[4096];
	ssize_t		n = 0;
	size_t		cap_size = 0;

	memset(&cap, 0, sizeof(cap));
	memset(buf, 0, sizeof(buf));
	
	if (pipe(pfd) < 0) {
		perror("pipe");
		cap.status = -1;
		return (cap);
	}
	
	pid = fork();

	if (pid < 0) {
		perror("fork");
		close(pfd[0]);
		close(pfd[1]);
		cap.status = -1;
		return (cap);
	}
	
	if (pid == 0) {
		int	devnull = 0;

		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		
		devnull = open("/dev/null", O_WRONLY);
		
		if (devnull >= 0) {
			dup2(devnull, STDERR_FILENO);
			close(devnull);
		}

		close(pfd[1]);
		execv(argv[0], argv);
		_exit(127);
	}
	
	close(pfd[1]);
	cap_size = 0;
	
	while ((n = read(pfd[0], buf, sizeof(buf))) > 0) {
		char	*tmp = NULL;

		tmp = realloc(cap.data, cap_size + (size_t)n + 1);
		if (!tmp) {
			free(cap.data);
			cap.data = NULL;
			cap.status = -1;
			close(pfd[0]);
			waitpid(pid, NULL, 0);
			return (cap);
		}

		cap.data = tmp;
		memcpy(cap.data + cap_size, buf, (size_t)n);
		cap_size += (size_t)n;
		cap.data[cap_size] = '\0';
	}
	
	close(pfd[0]);
	cap.len = cap_size;
	waitpid(pid, &cap.status, 0);
	
	if (!cap.data) {
		cap.data = strdup("");
		cap.len = 0;
	}

	return (cap);
}

static void	free_capture(t_capture *c)
{
	if (!c)
		return ;
	free(c->data);
	c->data = NULL;
	c->len = 0;
}

/////////////////////////////////////
//
//			NORMALIZE
//
////////////////////////////////////

/*
 * Subject: ignore RTT line + reverse DNS, tolerate time deltas.
 * We normalize output so structural comparison is possible vs system ping.
 */
static char	*normalize_ping_output(const char *in)
{
	char	*out = NULL;
	char	*dst = NULL;
	size_t	len = 0;
	size_t	i = 0;

	if (!in)
		return (strdup(""));

	len = strlen(in);
	out = calloc(1, len * 2 + 1);

	if (!out)
		return (NULL);
	
	dst = out;
	i = 0;
	while (in[i]) {
		if (!strncmp(in + i, "time=", 5)) {
			const char	*p = NULL;

			memcpy(dst, "time=TIME", 9);
			dst += 9;
			p = in + i + 5;
			while (*p && (isdigit((unsigned char)*p) || *p == '.'))
				p++;
			while (*p == ' ')
				p++;
			if (!strncmp(p, "ms", 2))
				p += 2;
			i = (size_t)(p - in);
			continue ;
		}

		if (!strncmp(in + i, ", time ", 7)) {
			const char	*p = NULL;

			p = in + i + 7;
			while (*p && (isdigit((unsigned char)*p)))
				p++;
			if (!strncmp(p, "ms", 2))
				p += 2;
			i = (size_t)(p - in);
			continue ;
		}

		if (!strncmp(in + i, "rtt min/", 8))
		{
			while (in[i] && in[i] != '\n')
				i++;
			if (in[i] == '\n')
				i++;
			continue ;
		}

		if (!strncmp(in + i, "ttl=", 4))
		{
			const char	*p;

			memcpy(dst, "ttl=TTL", 7);
			dst += 7;
			p = in + i + 4;
			while (*p && isdigit((unsigned char)*p))
				p++;
			i = (size_t)(p - in);
			continue ;
		}

		*dst++ = in[i++];
	}

	*dst = '\0';

	return (out);
}

static int	count_substr(const char *s, const char *needle)
{
	int			n = 0;
	const char	*p = NULL;

	n = 0;
	
	if (!s || !needle || !*needle)
		return (0);

	p = s;
	while ((p = strstr(p, needle))) {
		n++;
		p += strlen(needle);
	}

	return (n);
}

static int	has_line_prefix(const char *s, const char *prefix)
{
	const char	*p = NULL;
	size_t		len = 0;

	if (!s || !prefix)
		return (0);

	len = strlen(prefix);
	p = s;

	while (*p) {
		if (!strncmp(p, prefix, len))
			return (1);
		while (*p && *p != '\n')
			p++;
		if (*p == '\n')
			p++;
	}

	return (0);
}

/////////////////////////////////////
//
//			TESTS
//
////////////////////////////////////

static void	test_help_version(void)
{
	t_capture	cap;
	char		*av_help[] = {FT_PING_BIN, "--help", NULL};
	char		*av_ver[] = {FT_PING_BIN, "-V", NULL};

	memset(&cap, 0, sizeof(t_capture));

	printf("\n" C_CYAN C_BOLD "== help / version ==" C_RESET "\n");
	cap = run_capture(av_help);
	assert_ok(cap.data && strstr(cap.data, "usage: ft_ping"),
		"ft_ping --help shows usage");
	free_capture(&cap);

	cap = run_capture(av_ver);
	assert_ok(WIFEXITED(cap.status) && WEXITSTATUS(cap.status) == 0,
		"ft_ping -V exits 0");
	assert_ok(cap.data && strstr(cap.data, "Version ft_ping"),
		"ft_ping -V shows version");
	free_capture(&cap);
}

static void	test_parse_cli(void)
{
	t_capture	cap;
	char		*av_nohost[] = {FT_PING_BIN, NULL};
	char		*av_bad[] = {FT_PING_BIN, "-c", "xyz", "127.0.0.1", NULL};
	char		*av_ttl[] = {FT_PING_BIN, "-t", "999", "127.0.0.1", NULL};

	memset(&cap, 0, sizeof(t_capture));

	printf("\n" C_CYAN C_BOLD "== cli errors ==" C_RESET "\n");
	cap = run_capture(av_nohost);
	assert_ok(WIFEXITED(cap.status) && WEXITSTATUS(cap.status) != 0,
		"missing host -> non-zero exit");
	free_capture(&cap);

	cap = run_capture(av_bad);
	assert_ok(WIFEXITED(cap.status) && WEXITSTATUS(cap.status) != 0,
		"invalid -c -> non-zero exit");
	free_capture(&cap);

	cap = run_capture(av_ttl);
	assert_ok(WIFEXITED(cap.status) && WEXITSTATUS(cap.status) != 0,
		"ttl out of range -> non-zero exit");
	free_capture(&cap);
}

static void	compare_localhost(void)
{
	t_capture	ours;
	t_capture	sys;
	char		*norm_ours = NULL;
	char		*norm_sys = NULL;
	char		*av_ft[] = {FT_PING_BIN, "-c", "2", "-n", "127.0.0.1", NULL};
	char		*av_sys[] = {SYS_PING_BIN, "-c", "2", "-n", "127.0.0.1", NULL};
	int			ours_replies = 0;
	int			sys_replies = 0;

	memset(&ours, 0, sizeof(t_capture));
	memset(&sys, 0, sizeof(t_capture));

	printf("\n" C_CYAN C_BOLD
		"== compare vs system ping (127.0.0.1 -c 2 -n) ==" C_RESET "\n");
	printf(C_DIM "  (needs CAP_NET_RAW / sudo for raw socket)\n" C_RESET);

	ours = run_capture(av_ft);
	sys = run_capture(av_sys);

	if (WIFEXITED(ours.status) && WEXITSTATUS(ours.status) == 127)
	{
		assert_ok(0, "ft_ping binary missing (build first)");
		free_capture(&ours);
		free_capture(&sys);
		return ;
	}
	if (!ours.data || strstr(ours.data, "Operation not permitted")
		|| (WIFEXITED(ours.status) && WEXITSTATUS(ours.status) != 0
			&& !strstr(ours.data, "PING")))
	{
		printf(C_YELLOW C_BOLD "  [SKIP]" C_RESET
			" ft_ping needs root/capabilities — run: sudo make test_cmp\n");
		free_capture(&ours);
		free_capture(&sys);
		return ;
	}

	assert_ok(has_line_prefix(ours.data, "PING "),
		"ft_ping prints PING banner");
	assert_ok(has_line_prefix(sys.data, "PING "),
		"system ping prints PING banner");

	ours_replies = count_substr(ours.data, "bytes from");
	sys_replies = count_substr(sys.data, "bytes from");
	assert_ok(ours_replies >= 1, "ft_ping got at least 1 reply");
	assert_ok(sys_replies >= 1, "system ping got at least 1 reply");
	assert_ok(ours_replies == sys_replies
		|| abs(ours_replies - sys_replies) <= 1,
		"reply count close to system ping");

	assert_ok(strstr(ours.data, "packets transmitted") != NULL,
		"ft_ping prints packet statistics");
	assert_ok(strstr(sys.data, "packets transmitted") != NULL,
		"system ping prints packet statistics");

	norm_ours = normalize_ping_output(ours.data);
	norm_sys = normalize_ping_output(sys.data);
	assert_ok(norm_ours && norm_sys, "normalize outputs");
	assert_ok(has_line_prefix(norm_ours, "PING "),
		"normalized ft_ping still has PING");
	assert_ok(strstr(norm_ours, "time=TIME") != NULL
		|| ours_replies == 0,
		"ft_ping reply times normalized");

	printf("\n" C_DIM "--- ft_ping (raw) ---\n" C_RESET "%s", ours.data);
	printf(C_DIM "--- system ping (raw) ---\n" C_RESET "%s", sys.data);

	free(norm_ours);
	free(norm_sys);
	free_capture(&ours);
	free_capture(&sys);
}

static void	test_deadline(void)
{
	t_capture	cap;
	char		*av[] = {FT_PING_BIN, "-w", "2", "-n", "127.0.0.1", NULL};

	memset(&cap, 0, sizeof(t_capture));

	printf("\n" C_CYAN C_BOLD "== deadline -w 2 ==" C_RESET "\n");
	cap = run_capture(av);
	if (!cap.data || strstr(cap.data, "Operation not permitted")
		|| !strstr(cap.data, "PING"))
	{
		printf(C_YELLOW C_BOLD "  [SKIP]" C_RESET " needs root\n");
		free_capture(&cap);
		return ;
	}
	assert_ok(strstr(cap.data, "packets transmitted") != NULL,
		"-w 2 produces statistics");
	free_capture(&cap);
}

int	main(void)
{
	printf(C_BOLD C_BLUE
		"======== ft_ping comparison / integration tests ========"
		C_RESET "\n");
	printf("ft_ping: %s\n", FT_PING_BIN);
	printf("system:  %s\n", SYS_PING_BIN);

	test_help_version();
	test_parse_cli();
	compare_localhost();
	test_deadline();

	printf("\n" C_BOLD "======== summary ========" C_RESET "\n");
	
	printf(C_GREEN "passed: %d" C_RESET "\n", g_passed);
	
	if (g_failed)
		printf(C_RED C_BOLD "failed: %d" C_RESET "\n", g_failed);
	else
		printf(C_DIM "failed: %d" C_RESET "\n", g_failed);
	
	printf("total:  %d\n", g_total);
	
	if (g_failed) {
		printf(C_RED C_BOLD "\nRESULT: FAIL\n" C_RESET);
		return (1);
	}

	printf(C_GREEN C_BOLD "\nRESULT: PASS\n" C_RESET);
	return (0);
}
