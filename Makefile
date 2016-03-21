INCLUDE_DIR = ./include
OBJ_DIR = ./obj
SRC_DIR = ./src
BIN_DIR = ./bin

CPPFLAGS=-Wall -g -std=c++11 -lglog -stdlib=libstdc++ -I${INCLUDE_DIR}

COMMON_OBJS=$(OBJ_DIR)/http_request.o $(OBJ_DIR)/http_response.o $(OBJ_DIR)/http_method.o $(OBJ_DIR)/http_protocol.o $(OBJ_DIR)/http_status.o $(OBJ_DIR)/http.o 
SERVER_MAIN=$(OBJ_DIR)/http_server_main.o 
CLIENT_MAIN=$(OBJ_DIR)/http_client_main.o
SVR_OBJS=$(OBJ_DIR)/http_server.o $(OBJ_DIR)/http_client.o
OBJS=$(SVR_OBJS) $(COMMON_OBJS) 
SERVER_BIN=$(BIN_DIR)/http_server_main
CLIENT_BIN=$(BIN_DIR)/http_client_main

all: chkobjdir chkbindir $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(OBJS) $(SERVER_MAIN)
	$(CXX) -o $@ $(OBJS) $(SERVER_MAIN) $(CPPFLAGS)

$(CLIENT_BIN): $(OBJS) $(CLIENT_MAIN)
	$(CXX) -o $@ $(OBJS) $(CLIENT_MAIN) $(CPPFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc
	$(CXX) $(CPPFLAGS) -c -o $@ $<

chkobjdir :
	@if test ! -d $(OBJ_DIR) ; \
	then \
		mkdir $(OBJ_DIR) ; \
	fi

chkbindir :
	@if test ! -d $(BIN_DIR) ; \
	then \
		mkdir $(BIN_DIR) ; \
	fi

clean:
	rm -rf bin obj
	rm -rf *.dSYM
