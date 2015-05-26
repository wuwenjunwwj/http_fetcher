#include "server_receive.hpp"
#include <unistd.h>
int funA(){
	Server* server  = new Server();
	int server_port = 8000;
	if(server -> create_listen(server_port) == -1) 
		exit(-1);
 	server->begin();
	 sleep(6000);
	return 0;
 }
 
int main(){
	funA();
	sleep(6000);
}
