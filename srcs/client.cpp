/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntardy <ntardy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 13:20:00 by audrye            #+#    #+#             */
/*   Updated: 2024/09/04 19:25:39 by ntardy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/client.hpp"
#include "../incs/server.hpp"

Client::Client(int socket, struct sockaddr_in address)
    : socket(socket), address(address), nickname(""), username(""), password(""), log(false), admin(false), state(false), channel(NULL) {
    commandMap["PRIVMSG"] = &Server::privateMessage;
    commandMap["KICK"] = &Server::kick;
    commandMap["INVITE"] = &Server::invite;
    commandMap["TOPIC"] = &Server::topic;
    commandMap["MODE"] = &Server::mode;
    commandMap["CREATE"] = &Server::create;
    commandMap["JOIN"] = &Server::join;
    commandMap["QUIT"] = &Server::quit;
    commandMap["EXIT"] = &Server::exitServ;
}

Client::~Client() {
    close(socket);
}

int Client::getSocket() const {
    return socket;
}

std::string Client::getNickname() const {
    return nickname;
}

std::string Client::getUsername() const {
    return username;
}



std::string Client::getPassword() const {
    return password;
}

bool Client::getLog() const {
    return log;
}

bool Client::getAdmin() const {
    return admin;
}

bool Client::getState() const {
    return state;
}

std::string Client::getPotentialChannel() const {
    return potentialChannel;
}

Channel* Client::getChannel() const {
    return channel;
}

std::map<std::string, CommandFunction> Client::getCommandMap() const {
    return commandMap;
}





void Client::setLog(bool log) {
    this->log = log;
}

void Client::setNickname(const std::string& nickname) {
    this->nickname = nickname;
}

void Client::setChannel(Channel* channel) {
    this->channel = channel;
}

void Client::setAdmin(bool admin) {
    this->admin = admin;
}

void Client::setState(bool state) {
    this->state = state;
}

void Client::setPotentialChannel(const std::string& channelName) {
    this->potentialChannel = channelName;
}

void Client::setUsername(const std::string& username) {
    this->username = username;
}

void Client::setPassword(const std::string& password) {
    this->password = password;
}

std::string Client::getIPAddress() const {
    return inet_ntoa(address.sin_addr);
}

int Client::getPort() const {
    return ntohs(address.sin_port);
}
