#ifndef _SOCKET_HPP
#define _SOCKET_HPP

#include <sys/stat.h>
#include <sys/un.h>

struct unix_domain_socket {
	static const mode_t mode_666;
	static const mode_t mode_660;

	int listenfd;
	struct sockaddr_un svname;
	bool listened;
	bool binded;

	unix_domain_socket() : listened(false) {}

	void listen(char const *addr, bool do_chmod = false, mode_t mode = 0,
		    char const *new_owner = NULL, char const *new_group = NULL);

	~unix_domain_socket();

	int get_fd();
};

#endif /* _SOCKET_HPP */
