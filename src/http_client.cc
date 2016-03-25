/*************************************************************************
	> File Name: http_client.cc
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 一  3/14 20:23:41 2016
 ************************************************************************/

#include <iostream>
#include <string>
#include <glog/logging.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "http.h"
#include "http_method.h"
#include "http_response.h"
#include "http_request.h"
#include "http_client.h"

HttpClient::HttpClient(std::string file_name) {
    port_ = 1024;
    server_ip_ = "127.0.0.1";
    remote_file_name_ = local_file_name_ = file_name;
    method_ = HttpMethod();
    method_.Set("GET");
} 

HttpClient::~HttpClient(void) {
}

bool HttpClient::MergeRequest(void) {
    http_request_->SetMethod(method_.Get());  
    http_request_->SetProtocol(HTTP1_0);
    http_request_->SetUrl(remote_file_name_);
    http_request_->AddHeader("Host", server_ip_);
    http_request_->AddHeader("User-Agent", "Awesome HTTP Client");
    http_request_->AddHeader("Content-Type", ExtractMimeType(remote_file_name_));
    http_request_->AddHeader("Connection", "close");
    http_request_->MergeData();
    return true;
}

bool HttpClient::CreateSocket(void) {
    LOG(INFO) << "Start creating socket for server.";
    if((client_sockfd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG(ERROR) << "CreateSocket: Failed to create socket!";
        return false;
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if(connect(client_sockfd_, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        LOG(ERROR) << "CreateSocket: Failed to connect!";
        return false;
    }
    LOG(INFO) << "Creating socket done!";
    return true;
}

bool HttpClient::SendRequest(void) {
    size_t len = http_request_->GetLength();
    std::string *request_data_ptr = http_request_->GetDataPtr();
    char *buf = new char[len];
    memcpy(buf, request_data_ptr->data(), len);
    if(send(client_sockfd_, buf, len, 0) < 0) {
        LOG(ERROR) << "SendRequest: Failed to send request";
        return false;
    }

    delete buf;
    return true;
}

bool HttpClient::ReadResponse(void) {
    LOG(INFO) << "Start reading response from " << client_sockfd_;
    char *buf = new char[kBufSize];
    int len;
    len = recv(client_sockfd_, buf, kBufSize, 0);
    if(0 == len) {
        LOG(ERROR) << "ReadResponse: Read response failed, no data(blocking)!";
        return false;
    }
    
    http_response_->AddData(buf, len);
    while(true) {
        len = recv(client_sockfd_, buf, kBufSize, MSG_DONTWAIT);
        if(len < 0) {
            if(errno == EWOULDBLOCK || errno == EAGAIN) {
                break;
            }
            else {
                LOG(ERROR) << "Receive response failed!";
                return false;
            }
        }
        http_response_->AddData(buf, len);
        if(len < kBufSize) {
            break;
        }
    }
    LOG(INFO) << "Read response done!";
    http_response_->ShowData();
    delete buf;
    return true;
}

bool HttpClient::CompleteResponse(void) {
    std::ofstream out;
    switch(method_.Get()) {
        case GET:
            out.open(local_file_name_.c_str(), std::ofstream::out|std::ofstream::trunc);
            if(out.is_open()) {
                if(!http_response_->WriteIntoFile(out)) {
                    LOG(ERROR) << "CompleteResponse: Failed to write response into " << local_file_name_;
                    return false;
                }
            }
            else {
                LOG(ERROR) << "CompleteResponse: Failed to write response into " << local_file_name_;
                return false;
            }
            out.close();
            break;
        default:
            break;
    }
    return true;
}

bool HttpClient::HandlerResponse(void) {
    if(!ReadResponse()) {
        LOG(ERROR) << "HandlerResponse: Failed to handler response";
        return false;
    }
    if(!http_response_->Parse()) {
        LOG(ERROR) << "HandlerResponse: Failed to parse response";
        return false;
    }
    if(!CompleteResponse()) {
        LOG(ERROR) << "HandlerResponse: Failed to complete response";
        return false;
    }
    return true;
}

bool HttpClient::Run(void) {
    http_request_ = new HttpRequest();
    http_response_ = new HttpResponse();
    if(!MergeRequest()) {
        LOG(ERROR) << "Run: Request invalid!";
        return false;
    }
    http_request_->ShowData();
    if(!CreateSocket()) {
        LOG(ERROR) << "Run: Failed to create socket";
        return false;
    }
    if(!SendRequest()) {
        LOG(ERROR) << "Run: Failed to send request";
        return false;
    }
    if(!HandlerResponse()) {
        LOG(ERROR) << "Run: Failed to handler reqsponse";
        return false;
    }
    delete http_request_;
    delete http_response_;
    return true;
}
