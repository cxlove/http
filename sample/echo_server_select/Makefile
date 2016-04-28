CPPFLAGS=-Wall -g -std=c++11 -lglog -stdlib=libstdc++ 
all:
	$(CXX) client.cc ../../obj/socklib.o -o client $(CPPFLAGS)
	$(CXX) server.cc ../../obj/socklib.o -o server $(CPPFLAGS)

clean:
	rm client server

