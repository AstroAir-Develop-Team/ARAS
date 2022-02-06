/*
 * wsserver.hpp
 * 
 * Copyright (C) 2020-2022 Max Qian
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/************************************************* 
 
Copyright: 2020-2022 Max Qian. All rights reserved
 
Author:Max Qian

E-mail:astro_air@126.com
 
Date:2022-1-30
 
Description:Main program of astroair websocket server
 
**************************************************/

#ifndef _WSSERVER_H_
#define _WSSERVER_H_

#include "../aras.hpp"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <set>

typedef websocketpp::server<websocketpp::config::asio> airserver;
typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> con_list;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::condition_variable;
typedef airserver::message_ptr message_ptr;

typedef enum { IDEL,CONNECTED,CONNECTING,STOPPED } ServerState;


namespace ARAS
{
    class WS
    {
        public:
            explicit WS();
            ~WS();

            virtual void on_open(websocketpp::connection_hdl hdl);
			virtual void on_close(websocketpp::connection_hdl hdl);
			virtual void on_message(websocketpp::connection_hdl hdl,message_ptr msg);
            virtual void send(std::string message);
            virtual void stop();
            virtual bool is_running();
            virtual void run(int port,configor::json config);
        
        protected:
            virtual bool SaveClientInfo(websocketpp::connection_hdl hdl);
            virtual void JsonParser(std::string message);
        
        private:
            airserver m_server;
			con_list m_connections;

            struct WsInfo
            {
                ServerState State = IDEL;
                std::atomic_int ClientCount;
                std::atomic_int MaxClientCount;
                std::atomic_int Timeout;
                std::atomic_int MaxThreadCount;
            }*Ws_Info;
    };

    extern WS *ws;
}

#endif