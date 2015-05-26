#include <iostream>
#include <string>
#include <queue>
struct Http_Request{
	std::string url;

	};
class http_fetcher{
	public:
		int put_request(Http_Request request);
		int begin();
	private:
		std::queue<Http_Request> fetch_queue;

};


