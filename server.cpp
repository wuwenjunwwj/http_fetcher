#include "server_receive.hpp"
#include <unistd.h>
using namespace std;
int funA(){
	cout<<"in func A"<<endl;
	Server_Recv* server_recv  = new Server_Recv();
	int server_port = 8000;
	if(server_recv -> create_listen(server_port) == -1) 
		exit(-1);
	http_fetcher* fetcher =  new http_fetcher();
	server_recv -> set_httpFetcher(fetcher);
 	server_recv -> begin();
	fetcher ->begin();
	 sleep(6000);
	return 0;
 }
 
int main(){
	funA();
	sleep(6000);
}
