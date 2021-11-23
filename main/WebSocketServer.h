#pragma once

#define ASIO_STANDALONE

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <set>
#include <thread>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::bind;
using websocketpp::lib::asio::error_code;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class WebSocketServer
{
public:
    WebSocketServer()
    {
        m_server.init_asio();

        m_server.set_open_handler(bind(&WebSocketServer::on_open, this, ::_1));
        m_server.set_close_handler(bind(&WebSocketServer::on_close, this, ::_1));
        m_server.set_message_handler(bind(&WebSocketServer::on_message, this, ::_1, ::_2));
    }

    virtual ~WebSocketServer()
    {
        stop();
    }

    void on_open(connection_hdl hdl)
    {
        m_connections.insert(hdl);
    }

    void on_close(connection_hdl hdl)
    {
        m_connections.erase(hdl);
    }

    void on_message(connection_hdl hdl, server::message_ptr msg)
    {
        for (auto it : m_connections)
        {
            m_server.send(it, msg);
        }
    }

    void start(uint16_t port)
    {
        m_server_thread = std::thread([=]()
                                      {
                                          m_server.listen(port);

                                          error_code ec;
                                          auto endpoint = m_server.get_local_endpoint(ec);
                                          std::cout << "WebSocketServer listening on " << endpoint << std::endl;

                                          m_server.start_accept();
                                          m_server.run();
                                      });
    }

    void stop()
    {
        m_server.stop();

        if (m_server_thread.joinable())
        {
            m_server_thread.join();
        }
    }

private:
    typedef std::set<connection_hdl, std::owner_less<connection_hdl> > con_list;

    server m_server;
    con_list m_connections;
    std::thread m_server_thread;
};
