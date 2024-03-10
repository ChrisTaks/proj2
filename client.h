// Copyright 2024 Chris Taks

#ifndef _PROJ2_CLIENT_H_
#define _PROJ2_CLIENT_H_

#include <proj2/domain_socket.h>

#include <cstddef>
#include <cstdlib>

#include <string>
#include <iostream>

const char kSocket_path[] = "socket_example";

class DomainSocketClient : public DomainSocket {
  public:
    using DomainSocket::DomainSocket;

    void Run(int argc, char *argv[]);
};

#endif  // _PROJ2_CLIENT_H_
