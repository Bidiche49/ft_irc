/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audrye <audrye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 16:26:15 by audrye            #+#    #+#             */
/*   Updated: 2024/09/05 15:34:47 by audrye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <algorithm>
#include <unistd.h>
#include <cstdio>
#include <arpa/inet.h>
#include <cerrno>
#include <sstream>
#include <cstring>
#include <vector>
#include <string>
#include <fcntl.h>
#include "./client.hpp"
#include "./utils.hpp"
#include "./channel.hpp"
#include "./errors.hpp"

#define COLOR_RESET     "\033[0m"
#define COLOR_CYAN      "\033[36m"
#define COLOR_RED       "\033[31m"
#define COLOR_ORANGE    "\033[1;33m"
#define COLOR_GREEN     "\033[32m"

class Client;
class Channel;

class Server {
public:
    Server(int port, const std::string& password);
    ~Server();
    void start();

    void privateMessage(std::vector<std::string> input, Client& client);
    void kick(std::vector<std::string> input, Client& client);
    void invite(std::vector<std::string> input, Client& client);
    void topic(std::vector<std::string> input, Client& client);
    void mode(std::vector<std::string> input, Client& client);
    void join(std::vector<std::string> input, Client& client);
    void quit(std::vector<std::string> input, Client& client);
    void exitServ(std::vector<std::string> input, Client& client);
    void execCommand(std::string buffer, Client &client);
    Client* findClientByName(const std::string& name);
    bool isChannelExist(const std::string& channelName);
    bool isUserExist(const std::string& channelName);
    void create(std::vector<std::string> input, Client& client);
    Channel* findChannelByName(const std::string& name);
    void handleClientConnection(Client& client, const std::string& buffer);
    void connection(std::string buf, Client& client, std::string password);
    void keepClientAlive(Client& client);
    void logClientActivity(const Client& client, const std::string& activity);
    void modeOperator(std::vector<std::string> input, Client& client);
    std::vector<Client*> clients;
    std::vector<Channel*> channels;


protected:
    void handleNewConnection();
    void handleClientData(Client& client);
    void handleClientDisconnection(int clientSocket);
    void handleInvitationResponse(Client& client, const std::string& response);


    int server_fd;
    int port;
    std::string password;
    struct sockaddr_in address;
    int max_clients;
    int addrlen;
    fd_set readfds;

    std::map<int, std::string> clientBuffers;

};

#endif

