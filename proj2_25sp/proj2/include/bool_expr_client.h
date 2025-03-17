// Copyright wthacher 2025

#ifndef BOOL_EXPR_CLIENT_H
#define BOOL_EXPR_CLIENT_H

#include <cstdlib>
#include <cstring>
// Would not compile without libraries
#include <sys/socket.h>  // NOLINT
#include <sys/un.h>  // NOLINT
#include <unistd.h>  // NOLINT
#include <iostream>
#include <vector>
#include <string>
#include <domain_socket.h>

#define BUFFER_SIZE 256


class BoolExprClient : DomainSocketClient{
 public:
    BoolExprClient(const std::string& socket_path,
        const std::vector<std::string>& truth_values);
        
    void Run();

 private:
    std::string socket_path_;
    std::vector<std::string> truth_values_;
    int client_fd_;
    ssize_t bytes_sent_;
};

#endif 
