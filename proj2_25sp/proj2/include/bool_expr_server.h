//copyright wthacher
#ifndef IPC_DOMAIN_SOCK_SERVER_H_
#define IPC_DOMAIN_SOCK_SERVER_H_

#include <domain_socket.h>

#include <sys/socket.h>  // Unix header for sockets, using socket
#include <sys/un.h>  // defns for Unix domain sockets, using struct sockaddr_un
#include <unistd.h>  // Unix standard header, using close

#include <cassert>  // using assert
#include <cerrno>  // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // exit
#include <cstring>  // using strncpy, strerror

#include <string>
#include <iostream>
#include <unordered_map>

const char kSocket_path[] = "socket_example";

///
/// Domain Socket Server C++ Interface Class
///
class BoolExprServer : public DomainSocketServer {
 public:
  using ::DomainSocketServer::DomainSocketServer;

    explicit BoolExprServer(std::string filename, const char* socket_path, char us, char eot, bool abstract = true): DomainSocketServer(socket_path,us,eot,abstract),filename_(filename){

    }
  void Run();
  std::string evaluateExpr(std::unordered_map<char, bool> *values);
  private:
  std::string filename_;
};

#endif  // IPC_DOMAIN_SOCK_SERVER_H