#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <list>
#include <string>
#include "http_fetcher.hpp"

struct conn{
	int fd;
	int status;
};

class Server_Recv{
	public:
		int create_listen(int server_port);
		void set_sock(int fd);
		void* begin();
		static void* accept_thread(void* context);
		void accept_newConn(int Serv_Fd);
		void set_httpFetcher(http_fetcher* fetcher);
	//	std::list* getUrlList();
	private:
		int Serv_Port;
		int Serv_Fd;
		fd_set rset;
		fd_set wset;
		int max_fd;
		std::list<conn> conn_list;
		http_fetcher* http_fetcher_;

	};
