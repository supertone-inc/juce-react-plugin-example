#pragma once

#define ASIO_STANDALONE

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <set>
#include <thread>

typedef websocketpp::server<websocketpp::config::asio> WebsocketEndpoint;
typedef websocketpp::connection_hdl ClientConnection;

using websocketpp::lib::bind;
using websocketpp::lib::asio::error_code;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class WebSocketServer
{
  public:
    WebSocketServer()
    {
        server.init_asio();

        server.set_open_handler(bind(&WebSocketServer::onOpen, this, ::_1));
        server.set_message_handler(bind(&WebSocketServer::onMessage, this, ::_1, ::_2));
        server.set_close_handler(bind(&WebSocketServer::onClose, this, ::_1));
    }

    virtual ~WebSocketServer()
    {
        stop();
    }

    void start(uint16_t port)
    {
        serverThread = std::thread([=]() {
            server.listen(port);

            error_code ec;
            auto endpoint = server.get_local_endpoint(ec);
            std::cout << "WebSocketServer listening on " << endpoint << std::endl;

            server.start_accept();
            server.run();
        });
    }

    void stop()
    {
        server.stop();

        if (serverThread.joinable())
        {
            serverThread.join();
        }
    }

  private:
    void onOpen(ClientConnection connection)
    {
        connections.insert(connection);
    }

    void onMessage(ClientConnection connection, WebsocketEndpoint::message_ptr message)
    {
        for (auto conn : connections)
        {
            server.send(conn, message);
        }
    }

    void onClose(ClientConnection connection)
    {
        connections.erase(connection);
    }

    WebsocketEndpoint server;
    std::set<ClientConnection, std::owner_less<ClientConnection>> connections;
    std::thread serverThread;
};
