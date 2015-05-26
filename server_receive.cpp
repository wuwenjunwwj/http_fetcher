#include "server_receive.hpp"
#include <iostream>
#include <queue>
#define LISTEN_BACKLOG 50

//----todo end the thread
//
//
using namespace std;
int Server_Recv::create_listen(int server_port){
	int serv_fd = socket(AF_INET, SOCK_STREAM , 0);
	if(-1 == serv_fd){
		fprintf(stderr,"create socket failed\n");
		return -1;
		}
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(server_port);
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	if  (bind(serv_fd,( struct  sockaddr * ) & servaddr, sizeof (servaddr)) < 0 ){
		fprintf(stderr, " bind to port %d failure!\n " ,server_port);
		return  -1 ;
		}
	Serv_Fd = serv_fd;
	if (listen(Serv_Fd, LISTEN_BACKLOG) == -1){
		 fprintf(stderr,"listen error\n");
		 return -1 ;
		}
		
	return 0;
	}
void Server_Recv::set_sock(int fd){
	
	}
void* Server_Recv::accept_thread(void* context){
	Server_Recv* server_recv = (Server_Recv*) context;
	server_recv -> accept_newConn(server_recv->Serv_Fd);
	return NULL;
}
void Server_Recv::accept_newConn(int Serv_Fd){
	struct sockaddr_in peeraddr;
	max_fd = -1;
	socklen_t peer_addr_size = sizeof(struct sockaddr_un);
	while(1){
		int new_fd = accept(Serv_Fd, (struct sockaddr *) &peeraddr, &peer_addr_size);
		if (new_fd == -1){
			 fprintf(stderr,"accept error\n");
			 break;
			 }
		if(new_fd > max_fd ) max_fd = new_fd;
		FD_SET(new_fd,&rset);
		
		fprintf(stderr,"accept a new connection\n");
		struct conn* conn_;
		conn_ = (conn*)malloc (sizeof(conn));
		conn_ ->fd = new_fd;
		conn_list.push_back(*conn_);
	}
}
void* Server_Recv::begin(){
	fprintf(stderr,"server_recv begin\n");;
	struct timeval timeout;
	timeout.tv_sec = 30;
	timeout.tv_usec = 1000 * 2000;
	FD_ZERO(&rset); 
	FD_ZERO(&wset);
	pthread_t pid;
	int error = pthread_create(&pid, NULL, Server_Recv::accept_thread, this);
	if(0 != error){
		fprintf(stderr, "create thread failed,exit\n");
		exit(-1);
	}
	while(1){
		cout<<"max_fd: " <<max_fd <<endl;
		int n = select((max_fd+1), &rset, NULL, NULL, &timeout);
		if(n > 0){
			fprintf(stderr,"select\n");;
			for(list<conn>::iterator it=conn_list.begin(); it != conn_list.end(); ++it){
				int flag;
				flag = FD_ISSET(it->fd, &rset); //judge 
				if(flag){
					char* buf;
					buf = (char*) malloc (100);
					int len = 100;

					if( -1 == recv(it->fd, buf, len, 0)){
						//recv error,close the socket(the client may be leave)
						if(errno != EINTR){
							cout<<"erase the element"<<endl;
							close(it->fd); //need ? no
							//it = conn_list.erase (it);
							conn_list.erase (it);
							
							}
						
						}
					// recv successfully
					else{

						std::cout<<"receive data: buf "<<buf<<endl;
						Http_Request request;
						request.url = buf;
						if(http_fetcher_){
							http_fetcher_ -> put_request(request);
						}
						else
							continue; 
					}
					std::cout<<"receive data: buf "<<buf<<endl;
				}

				}
			// ret the rset, add the connetion¡s fd to the se
			// for keep-alive connection; 
			FD_ZERO(&rset);
			for(list<conn>::iterator it=conn_list.begin(); it != conn_list.end(); ++it){
				 FD_SET(it->fd,&rset);
				}

			break;
		}
	}
	return NULL;
}
void Server_Recv::set_httpFetcher(http_fetcher* fetcher){
	if(fetcher){
		http_fetcher_ = fetcher;
	}
	http_fetcher_ = NULL;
}

