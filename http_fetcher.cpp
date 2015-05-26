
#include "http_fetcher.hpp"
using namespace std;
int http_fetcher::put_request(Http_Request request){
	fetch_queue.push(request);
	return 0;
	}
int http_fetcher::begin(){
	while(1){
		if(fetch_queue.size()>0){
			cout<<"fetch_queue.size(): "<< fetch_queue.size()<<endl;
			fetch_queue.pop();
			}

	}
	return 0;
}

