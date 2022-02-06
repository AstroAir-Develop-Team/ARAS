#include "socket.hpp"

namespace ARAS
{
    socket::socket()
    {
        spdlog::info("初始化socket通信模块");
        spdlog::info("检测到Sockpp版本为%s",sockpp::SOCKPP_VERSION);
        sockpp::socket_initializer sockInit;
    }

    socket::~socket()
    {
        if(con.is_open())
            con.close();
    }

    bool socket::Run(std::string host,int port)
    {
        info.host = host;
        info.port = port;
        con.bind(sockpp::inet_address(info.host,info.port));
        con.create();
        if(!con)
        {
            ErrorConnect();
            return false;
        }
        //spdlog::info("Connected server at %s",sockpp::inet_address(con.address()));
        if(con.read_timeout(std::chrono::seconds(5)))
        {
            Timeout();
            return false;
        }
        while(info.RunSocketClient)
        {
            std::string tmp = Read();
            if(tmp != info.lastmsg)
                JsonParser(tmp);
            info.lastmsg = tmp;
        }
        return true;
    }

    bool socket::Send(std::string msg)
    {
        if(msg.empty())
        {
            spdlog::error("Counld't send empty message");
            return false;
        }
        if(con.write(msg) != ssize_t(msg.length()))
        {
            spdlog::error("Coundn't send message");
            spdlog::error("Err: %s",con.last_error_str());
            return false;
        }
        return true;
    }

    std::string socket::Read()
    {
        char msg[512];
        con.read(msg,sizeof(msg));
        return msg;
    }

    bool socket::JsonParser(std::string msg)
    {
        configor::json j = configor::json::parse(msg);

        return true;
    }

    bool socket::ErrorConnect()
    {
        spdlog::error("Error Connecting to server" );
        spdlog::error("Err: %s",con.last_error_str());
        return true;
    }

    bool socket::Timeout()
    {
        spdlog::error("Error setting up TCP server");
        spdlog::error("Err: %s",con.last_error_str());
        return true;
    }
}