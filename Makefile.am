include $(top_srcdir)/common.mk
bin_PROGRAMS = http_client http_server

http_client_SOURCES=client.cpp
http_client_LDADD = -lpthread

http_server_SOURCES=server.cpp server_receive.cpp http_fetcher.cpp
