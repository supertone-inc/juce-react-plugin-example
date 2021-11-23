#pragma once

#define ASIO_STANDALONE

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

typedef websocketpp::server<websocketpp::config::asio> WebsocketEndpoint;
typedef websocketpp::connection_hdl ClientConnection;

using websocketpp::lib::bind;
using websocketpp::lib::asio::error_code;
using websocketpp::lib::asio::ip::tcp;
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

    template <typename Callback> void addConnectHandler(Callback handler)
    {
        asio::post(server.get_io_service(), [this, handler]() { connectHandlers.push_back(handler); });
    }

    template <typename Callback> void addMessageHandler(Callback handler)
    {
        asio::post(server.get_io_service(), [this, handler]() { messageHandlers.push_back(handler); });
    }

    template <typename Callback> void addDisconnectHandler(Callback handler)
    {
        asio::post(server.get_io_service(), [this, handler]() { disconnectHandlers.push_back(handler); });
    }

    void start(uint16_t port)
    {
        server.listen(port);
        server.start_accept();
        serverThread = std::thread([this]() { server.run(); });
    }

    void send(ClientConnection connection, const std::string &message)
    {
        server.send(connection, message, websocketpp::frame::opcode::text);
    }

    void broadcast(const std::string &message)
    {
        std::lock_guard<std::mutex> lock(connectionsMutex);

        for (auto connection : connections)
        {
            send(connection, message);
        }
    }

    void stop()
    {
        server.stop();

        if (serverThread.joinable())
        {
            serverThread.join();
        }
    }

    tcp::endpoint getLocalEndpoint()
    {
        error_code errorCode;

        tcp::endpoint localEndpoint = server.get_local_endpoint(errorCode);

        if (errorCode)
        {
            throw std::runtime_error(errorCode.message());
        }

        return localEndpoint;
    }

  private:
    void onOpen(ClientConnection connection)
    {
        {
            std::lock_guard<std::mutex> lock(connectionsMutex);
            connections.insert(connection);
        }

        for (auto handler : connectHandlers)
        {
            handler(connection);
        }
    }

    void onMessage(ClientConnection connection, WebsocketEndpoint::message_ptr message)
    {
        for (auto handler : messageHandlers)
        {
            handler(connection, message->get_payload());
        }
    }

    void onClose(ClientConnection connection)
    {
        {
            std::lock_guard<std::mutex> lock(connectionsMutex);
            connections.erase(connection);
        }

        for (auto handler : disconnectHandlers)
        {
            handler(connection);
        }
    }

    WebsocketEndpoint server;
    std::set<ClientConnection, std::owner_less<ClientConnection>> connections;
    std::mutex connectionsMutex;
    std::thread serverThread;

    std::vector<std::function<void(ClientConnection)>> connectHandlers;
    std::vector<std::function<void(ClientConnection, const std::string &)>> messageHandlers;
    std::vector<std::function<void(ClientConnection)>> disconnectHandlers;
};
