/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audrye <audrye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 14:50:39 by audrye            #+#    #+#             */
/*   Updated: 2024/09/05 15:33:15 by audrye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../incs/channel.hpp"

Channel::Channel(const std::string&  channelName, Client& admin)
:   channelName(channelName),
    invitOnly(false),
    topic(channelName + " is a new channel ! This topic is example for the test 🧪🧪🧪"),
    topicNeedAdmin(true),
    maxUsers(-1)//-1 equals to no limit

{
    clients.push_back(&admin);
    admin.setChannel(this);
    admin.setAdmin(true);
    std::cout << COLOR_GREEN << ":Channel " << channelName << " just created by " << admin.getNickname() << COLOR_RESET << std::endl;

}

Channel::~Channel() {
}

std::string Channel::getTopic() {
    return topic;
}

std::string Channel::getPassword() {
    return password;
}

int Channel::getMaxUsers() {
    return maxUsers;
}

bool Channel::getNeedAdmin() {
    return topicNeedAdmin;
}

std::vector<Client*> Channel::getClients() {
    return clients;
}

void Channel::setTopic(const std::string& newTopic) {
    topic = newTopic;
}

void Channel::setPassword(const std::string& newPassword) {
    this->password = newPassword;
}

void Channel::setMaxUsers(const int newMaxUsers) {
    maxUsers = newMaxUsers;
}

void Channel::setNeedAdmin(const bool needAdmin) {
    topicNeedAdmin = needAdmin;
}

void sendMessageChannel(std::string message, Client& client, Channel* target) {
    if (!target)
        return ;
    std::vector<Client *>clients = target->getClients();
    if (!message.empty()) {
        size_t length = message.length();
        for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
            Client* otherClient = *it;
            if (otherClient != &client && otherClient->getLog()) {
                send(otherClient->getSocket(), message.c_str(), length, 0);
            }
        }
    }
}

void sendNotif(std::string message, Client& client) {
    if (!message.empty()) {
        size_t length = message.length();
        send(client.getSocket(), message.c_str(), length, 0);
    }
}

void Channel::addClientToChannel(const std::string& channelName, Client& client) {
    Channel *channel = client.getChannel();
    if (channel)
        channel->clients.erase(std::find(channel->clients.begin(), channel->clients.end(), &client));
    client.setChannel(this);
    clients.push_back(&client);
    sendNotif(":" + changeColor(COLOR_GREEN, "Congratulation, ")
                + changeColor(COLOR_CYAN, client.getNickname())
                + changeColor(COLOR_GREEN, " you just joined #"
                + channelName + " channel!\n"), client);
    std::cout << COLOR_GREEN << ":Added client " << client.getNickname() << " to channel " << channelName << COLOR_RESET << std::endl;
}

void Channel::removeClientFromChannel(Client& client) {
    clients.erase(std::remove(clients.begin(), clients.end(), &client), clients.end());
    std::cout << COLOR_GREEN << ":Removed client " << client.getNickname() << " from channel " << channelName << COLOR_RESET << std::endl;
}

void Channel::broadcastToChannel(const std::string& channelName, const std::string& message, Client& sender) {
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        Client* client = *it;
        if (client != &sender) {
            send(client->getSocket(), message.c_str(), message.size(), 0);
        }
    }
    std::cout << COLOR_GREEN << ":Broadcasted message in channel " << channelName << ":" << message << COLOR_RESET;
}

bool Channel::isClientInChannel(const std::string& name) {
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        Client* client = *it;
        if (client->getNickname() == name) {
            return true;
        }
    }
    return false;
}


