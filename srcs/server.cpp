/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audrye <audrye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 16:26:50 by audrye            #+#    #+#             */
/*   Updated: 2024/09/05 15:51:22 by audrye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/server.hpp"

Server::Server(int port, const std::string& password)
    : port(port), password(password), max_clients(10), addrlen(sizeof(address))
{

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    #ifdef __linux__
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
    }
    #endif

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, max_clients) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "IRC Server started on port " << port << std::endl;
}

Server::~Server()
{
    close(server_fd);
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        delete *it;
    }
    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
        delete *it;
    }
}

void Server::start() {
    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);

        FD_SET(server_fd, &readfds);
        int max_sd = server_fd;

        FD_SET(fileno(stdin), &readfds);
        if (fileno(stdin) > max_sd) {
            max_sd = fileno(stdin);
        }


        for (size_t i = 0; i < clients.size(); ++i) {
            int sd = clients[i]->getSocket();
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0 && errno != EINTR) {
            std::cerr << "Select error: " << strerror(errno) << std::endl;
            break;
        }

        if (FD_ISSET(fileno(stdin), &readfds)) {
            std::string serverCommand;
            std::cin >> serverCommand;
            if (serverCommand == "EXIT") {
                std::cout << "IRC server just shutdown\n";
                return ;
            }
        }

        if (FD_ISSET(server_fd, &readfds)) {
            handleNewConnection();
        }

        for (size_t i = 0; i < clients.size(); ++i) {
            Client* client = clients[i];
            if (FD_ISSET(client->getSocket(), &readfds)) {
                handleClientData(*client);
            }
        }
    }
}


void Server::handleNewConnection()
{
    int new_socket;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror(":Accept failed");
    }

    if (fcntl(new_socket, F_SETFL, O_NONBLOCK) == -1) {
        perror(":fcntl F_SETFL failed");
        close(new_socket);
        return;
    }

    std::cout << COLOR_CYAN << ":New connection, socket FD: " << new_socket << ", IP: "
            << inet_ntoa(address.sin_addr) << ", Port: "
            << ntohs(address.sin_port) << COLOR_RESET << std::endl;

    if (static_cast<int>(clients.size()) < max_clients) {
        Client* new_client = new Client(new_socket, address);
        clients.push_back(new_client);
        std::cout << COLOR_CYAN << ":Client added. Total clients: " << clients.size() << COLOR_RESET << std::endl;
    } else {
        std::cerr << COLOR_ORANGE << ":Max clients reached. Connection rejected.\n" << COLOR_RESET << std::endl;
        close(new_socket);
    }
}

void Server::handleInvitationResponse(Client& client, const std::string& response)
{
    std::string trimmedResponse = response;
    trimmedResponse.erase(trimmedResponse.find_last_not_of(" \n\r\t") + 1);
    std::string channelName = client.getPotentialChannel();

    if (trimmedResponse == "Y" || trimmedResponse == "y") {
        client.setState(false);
        client.setPotentialChannel("");
        Channel* channel = findChannelByName(channelName);
        channel->addClientToChannel(channelName, client);
        std::string joinMessage = ":" + changeColor(COLOR_CYAN, client.getNickname()) + changeColor(COLOR_GREEN, " has joined the channel #" + channelName) + ".\n";
        client.getChannel()->broadcastToChannel(channelName, joinMessage, client);
    } else if (trimmedResponse == "N" || trimmedResponse == "n") {
        client.setState(false);
        client.setPotentialChannel("");
        std::string declineMessage = ":You declined the invitation to join the channel " + channelName + ".\n";
        send(client.getSocket(), declineMessage.c_str(), declineMessage.size(), 0);
    } else {
        std::string errorMessage = ":Invalid response. Please reply with 'Y' or 'N'.";
        std::cout << errorMessage << std::endl;
        send(client.getSocket(), errorMessage.c_str(), errorMessage.size(), 0);//TODO: join only if there is only Y char AND print confirmation
    }
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void Server::keepClientAlive(Client& client)
{
    std::vector<std::string> messages;
    messages.push_back(":" + client.getNickname() + " :Welcome to the IRC server!\r\n");
    messages.push_back(":" + client.getNickname() + " :This server was created 24/08/2024\r\n");
    messages.push_back(":" + client.getNickname() + " :Your host is " + client.getIPAddress() + ", running version 1.0\r\n");

    for (std::vector<std::string>::const_iterator it = messages.begin(); it != messages.end(); ++it) {
        ssize_t bytesSent = 0;
        const char* ptr = it->c_str();
        size_t remaining = it->length();

        while (remaining > 0) {
            bytesSent = send(client.getSocket(), ptr, remaining, 0);
            if (bytesSent < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    continue;
                }
                logClientActivity(client, "Failed to send message: " + std::string(strerror(errno)));
                return;
            }
            ptr += bytesSent;
            remaining -= bytesSent;
        }
    }
}


void Server::logClientActivity(const Client& client, const std::string& activity)
{
    std::cout << ":[Client " << client.getSocket() << " - " << client.getIPAddress()
              << ":" << client.getPort() << "] " << activity << std::endl;
}

void Server::handleClientConnection(Client& client, const std::string& buffer)
{
    std::vector<std::string> commands = split(buffer, '\n');
    for (size_t i = 0; i < commands.size(); ++i)
    {
        std::string trimmedCommand = trim(commands[i]);
        if (!trimmedCommand.empty())
        {
            logClientActivity(client, "Processing command: " + trimmedCommand);
            connection(trimmedCommand, client, this->password);
            if (client.getLog())
            {
                keepClientAlive(client);
                logClientActivity(client, "Successfully authenticated");
                return;
            }
        }
    }
    if (!client.getUsername().empty())
        logClientActivity(client, "Failed to authenticate");
}



void Server::handleClientData(Client& client)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int valread = recv(client.getSocket(), buffer, sizeof(buffer) - 1, 0);

    if (valread > 0) {
        buffer[valread] = '\0';
        clientBuffers[client.getSocket()] += buffer;
        std::string& clientBuffer = clientBuffers[client.getSocket()];
        size_t newlinePos;
        while ((newlinePos = clientBuffer.find('\n')) != std::string::npos) {
            std::string completeCommand = clientBuffer.substr(0, newlinePos + 1);
            clientBuffer.erase(0, newlinePos + 1);
            if (!client.getLog()) {
                handleClientConnection(client, completeCommand);
            } else if (isCommand(completeCommand)) {
                execCommand(completeCommand, client);
            } else {
                if (client.getState()) {
                    handleInvitationResponse(client, completeCommand);
                } else if (client.getChannel()) {
                    std::string message = ":" + changeColor(COLOR_GREEN, "#" + client.getChannel()->channelName) + ": " +changeColor(COLOR_CYAN, client.getNickname()) + ": " + completeCommand;
                    std::cout << ":Message from " << client.getChannel()->channelName << " : " << client.getNickname() << ":" << completeCommand;
                    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
                        Client* otherClient = *it;
                        if (otherClient != &client && otherClient->getLog() && client.getChannel() != NULL && otherClient->getChannel() == client.getChannel()) {
                            send(otherClient->getSocket(), message.c_str(), message.size(), 0);
                        }
                    }
                }
            }
        }
    } else if (valread == 0) {
        logClientActivity(client, "Disconnected normally");
        handleClientDisconnection(client.getSocket());
    } else {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        }
        logClientActivity(client, "Error reading from socket: " + std::string(strerror(errno)));
        handleClientDisconnection(client.getSocket());
    }
}

struct ClientDisconnector {
    int clientSocket;
    Server* server;
    ClientDisconnector(int socket, Server* server) : clientSocket(socket), server(server) {}

    bool operator()(Client* client) {
        if (client->getSocket() == clientSocket) {
            std::cout << ":Client disconnected, IP: " << client->getIPAddress() << ", Port: " << client->getPort() << std::endl;
            Channel* channel = client->getChannel();
            if (channel) {
                std::string joinMessage = ":" + changeColor(COLOR_CYAN, client->getNickname())
                            + changeColor(COLOR_GREEN, " leaved the channel #"
                            + channel->channelName) + ".\n";
                channel->broadcastToChannel(channel->channelName, joinMessage, *client);
                channel->removeClientFromChannel(*client);
                if (channel->getClients().size() == 0) {
                    server->channels.erase(std::remove(server->channels.begin(), server->channels.end(), channel), server->channels.end());
                    delete channel;
                }
            }
            delete client;
            return true;
        }
        return false;
    }
};

void Server::handleClientDisconnection(int clientSocket)
{
    std::cout << "start handleClientDisconnection"<< std::endl;
    std::vector<Client*>::iterator it = std::remove_if(clients.begin(), clients.end(), ClientDisconnector(clientSocket, this));
    clients.erase(it, clients.end());
}
