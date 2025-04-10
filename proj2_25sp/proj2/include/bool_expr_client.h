//copyright wthacher
#ifndef IPC_DOMAIN_SOCKET_CLIENT_H_
#define IPC_DOMAIN_SOCKET_CLIENT_H_

#include <domain_socket.h>

#include <cstddef>  // using size_t
#include <cstdlib>  // exit
#include <csignal>

#include <string>
#include <iostream>


const char kSocket_path[] = "socket_example";


///
/// Domain Socket Client C++ Interface Class
///
class BoolExprClient : public DomainSocketClient {
 public:
  using DomainSocketClient::DomainSocketClient;

  void Run(std::string &treufalses);
};


#endif  // IPC_DOMAIN_SOCKET_CLIENT_H_