#include "client.hpp"
#define MAXLINE 4096
#define thread_num 3
#define TIME_OUT 3000
 #include <signal.h>
volatile int Client::signal_  ;
int Client::put_request(Http_Request request){
	request_queue.push(request);
	return 0;
	}
Http_Request Client::get_request(){
	Http_Request request;
	while(request_queue.size()>0){

		request = request_queue.front();
		request_queue.pop();
	}
	return request;
}
void Client::cli_svc(Connection* conn){ 
	fprintf(stderr,"send data to server\n");
	sleep(2);
	int error = send(conn->cli_fd, "http://www.baidu.com",  20, 0);
	fprintf(stderr, "send return code errno or length %d\n",error);
	}
void* Client::cli_worker(void* context ){
	Client* client =(Client*) context; 
	client->cli_svc(client->conn);
	return NULL;
}

void Client::ConnectToServer(){

	if (connect(cli_fd,( struct  sockaddr * ) & (conn->serv_addr), sizeof(conn->serv_addr))  <   0 ){
         fprintf(stderr,"connect error %d\n",errno);
		 exit(-1);
         }
	fprintf(stderr,"connect success\n");
	}

void Client::init(int server_port, int client_port){
	cli_fd = socket(AF_INET, SOCK_STREAM , 0); 
	fprintf(stderr,"hello,kitty1\n");
    if(-1 == cli_fd){
        fprintf(stderr,"create client socket failed\n");
        exit(-1);
        }
    struct sockaddr_in cli_addr;
    bzero(&cli_addr, sizeof (sockaddr_in)); //
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(client_port);
    cli_addr.sin_addr.s_addr = htons(INADDR_ANY);
	struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof (sockaddr_in));  //
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
	//socklen_t socklen  =   sizeof (serv_addr);
    if  (bind(cli_fd,( struct  sockaddr * ) & cli_addr, sizeof (cli_addr)) < 0 ){
        fprintf(stderr, " bind to port %d failure!\n " ,client_port);
        exit( -1 );
        }   
	
    fprintf(stderr, " bind to port %d success!\n " ,client_port);
	conn =  new Connection;
	conn -> cli_fd = cli_fd;
	memset(&(conn->serv_addr),0,sizeof(serv_addr));
	conn -> serv_addr =  serv_addr;
	}
int Client::Begin(){
	int error;
	ConnectToServer();
	error = pthread_create(&pid,NULL, Client::cli_worker, this);
		if(0 != error){
			fprintf(stderr, "create thread failed,exit\n");
			exit(-1);
			}
		fprintf(stderr, "create thread success\n");
	 return 0;
	}
int Client::End(){
	int error;
		error = pthread_join(pid, NULL);
		if(0 != error ){
			fprintf(stderr, "thread termained failed\n");
			}

	close (cli_fd);
	
	return 0;
	}
static void signal_handler(int nu){
	Client::signal_ = -1;
	std::cout<<"catch signal:"<< Client::signal_<< std::endl;
	}
int main(){
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	int server_port = 8000;
	int client_port = 1100;
	Client* client = new Client();
	client -> init(server_port,client_port);
	client -> Begin();
	client_port = 1200;
	Client* client1 = new Client();
	Client* client3 = new Client();
	client1 -> init(server_port,client_port);
	client1 -> Begin();
	client_port = 1300;
	client3 -> init(server_port,client_port);
	client3 -> Begin();
	while(1){
		if(Client::signal_ == -1)
			break;
		}
	std::cout<<"will eix t"<<std::endl;
	client -> End();
	client1 -> End();
	client3 -> End();
}
