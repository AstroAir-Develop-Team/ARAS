#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include "sockpp/socket.h"
#include "sockpp/tcp_connector.h"
#include "sockpp/version.h"
#include "sockpp/tcp_socket.h"

#include "../aras.hpp"

#include <chrono>

namespace ARAS
{
    struct SOCKETINFO
    {
        std::string host = "localhost";
        int port = 4400;

        std::atomic_bool RunSocketClient;

        std::string lastmsg;
    };

    class socket
    {
        public:
            explicit socket();
            ~socket();

            virtual bool Run(std::string host,int port);
            virtual bool Send(std::string msg);
            virtual std::string Read();

        protected:
            virtual bool JsonParser(std::string msg);
            virtual bool ErrorConnect();
            virtual bool Timeout();
        private:
            SOCKETINFO info;         
            sockpp::tcp_socket con;
    };
}
#endif