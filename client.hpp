#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h> 
#include <errno.h>
#include <queue>
//int Client::signal_;
typedef struct Connection{
	int cli_fd;
	struct sockaddr_in serv_addr;
	}Connection;
enum FETCH_TYPE{
	GET = 0,
	POST
	};
struct Http_Request{
	std::string url;
	FETCH_TYPE type;
	
	};
class Client{
	public:
		Http_Request get_request();
		static void* cli_worker(void* connection );
		void init(int server_port, int client_port);
		void ConnectToServer();
		int Begin();
		int End();
		volatile static int signal_;
	private:
		std::queue<Http_Request> request_queue;
		int put_request(Http_Request request);
		void cli_svc(Connection* conn);
		int cli_fd;
		Connection* conn;
		pthread_t pid;

};
