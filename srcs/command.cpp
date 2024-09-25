/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audrye <audrye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:11:55 by audrye            #+#    #+#             */
/*   Updated: 2024/09/05 15:33:37 by audrye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/command.hpp"

Command::Command() : command(""), option(""), target(""), argument("") {}

Command::Command(const std::string& cmd, const std::string& tgt, const std::string& opt, const std::string& arg)
    : command(cmd), option(opt), target(tgt), argument(arg) {}

Command::Command(const Command& other)
    : command(other.command), option(other.option), target(other.target), argument(other.argument) {}

Command::~Command() {}

Command& Command::operator=(const Command& other) {
    if (this != &other) {
        command = other.command;
        option = other.option;
        target = other.target;
        argument = other.argument;
    }
    return *this;
}

std::string Command::getCommand() const {
    return command;
}

std::string Command::getOption() const {
    return option;
}

std::string Command::getTarget() const {
    return target;
}

std::string Command::getArgument() const {
    return argument;
}

void Command::setCommand(const std::string& cmd) {
    command = cmd;
}

void Command::setOption(const std::string& opt) {
    option = opt;
}

void Command::setTarget(const std::string& tgt) {
    target = tgt;
}

void Command::setArgument(const std::string& arg) {
    argument = arg;
}

void Server::privateMessage(std::vector<std::string> input, Client& client) {
    if (input.size() < 3) {
        return sendErrorMessage(NOT_ENOUGH_ARGUMENTS, client);
    }
    if (!findClientByName(input[1])) {
        return sendErrorMessage(USER_NOT_FOUND, client);
	}
    std::string message = ":"
                        + input[0]
                        + " "
                        + changeColor(COLOR_CYAN, client.getNickname())
                        + ":"
                        + concatenateTokens(input, 2)
                        + "\n";

    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        Client* otherClient = *it;

        if (client.getNickname() == input[1]) {
            return sendErrorMessage(CANNOT_SEND_SELF, client);
        } else if (clients.size() > 1 && it == clients.end()) {
            return sendErrorMessage(USER_NOT_FOUND, client);
        } else if (otherClient && otherClient->getLog() && otherClient != &client
                && otherClient->getNickname() == input[1]) {
            send(otherClient->getSocket(), message.c_str(), message.size(), 0);
            return;
        }
    }
}

void Server::create(std::vector<std::string> input, Client& client) {
    if (input.size() != 2) {
        return sendErrorMessage(input.size() == 1
            ? NOT_ENOUGH_ARGUMENTS
            : TOO_MANY_ARGUMENTS, client);
    }
    std::string channelName = input[1];
    if (client.getChannel()) {
        return(sendErrorMessage(USER_HAVE_ALREADY_CHANNEL, client));
    }
    if (isChannelExist(input[1]) || isUserExist(channelName)) {
        return(sendErrorMessage(UNAVAILABLE_NAME, client));
    }

    Channel* newChannel = new Channel(channelName, client);//TODO free later
    channels.push_back(newChannel);
    sendNotif(":" + changeColor(COLOR_GREEN, "You create the ")
        + changeColor(COLOR_CYAN, "#" + channelName)
        + changeColor(COLOR_GREEN, ". Welcome in your channel!\n"), client);
}

void Server::kick(std::vector<std::string> input, Client& client) {

    std::string userName = input[1];
    Client* targetClient = findClientByName(userName);

    ErrorCode error = checkKickConditions(input, client, targetClient);
    if (error != NONE) return sendErrorMessage(error, client);

    std::string message = ":"
                + changeColor(COLOR_CYAN, client.getNickname())
                + changeColor(COLOR_GREEN, " kicked you of channel : ")
                + changeColor(COLOR_GREEN, "#" + client.getChannel()->channelName) + "\n";

    targetClient->setState(false);
    targetClient->setPotentialChannel("");
    targetClient->setAdmin(false);
    targetClient->setChannel(NULL);
    client.getChannel()->removeClientFromChannel(*targetClient);

    send(targetClient->getSocket(), message.c_str(), message.size(), 0);
}

void Server::quit(std::vector<std::string> input, Client& client) {
    if (input.size() != 1)
        return sendErrorMessage(TOO_MANY_ARGUMENTS, client);
    Channel* channel = client.getChannel();
    if (channel == NULL)
        return sendErrorMessage(USER_DOESNT_HAVE_CHANNEL, client);
    std::string message = ":"
                + changeColor(COLOR_GREEN, "You leave ")
                + changeColor(COLOR_GREEN, "#" + channel->channelName) + "\n";

    client.setState(false);
    client.setPotentialChannel("");
    client.setAdmin(false);
    channel->removeClientFromChannel(client);
    client.setChannel(NULL);
    std::string joinMessage = ":" + changeColor(COLOR_CYAN, client.getNickname())
                                + changeColor(COLOR_GREEN, " leaved the channel #"
                                + channel->channelName) + ".\n";
    if (channel->getClients().size() == 0) {
        channels.erase(std::remove(channels.begin(), channels.end(), channel), channels.end());
        return delete channel;
    }
    send(client.getSocket(), message.c_str(), message.size(), 0);
    channel->broadcastToChannel(channel->channelName, joinMessage, client);
}

void Server::exitServ(std::vector<std::string> input, Client& client) {
    if (input.size() != 1)
        return sendErrorMessage(TOO_MANY_ARGUMENTS, client);
    handleClientDisconnection(client.getSocket());
}

void Server::invite(std::vector<std::string> input, Client& client) {
    Channel* channel = client.getChannel();
    ErrorCode error = checkInviteConditions(input, client, channel, input.size() > 1
                                                                    ? isUserExist(input[1])
                                                                    : false);
    if (error != NONE) return sendErrorMessage(error, client);


    std::string userName = input[1];
    std::string message = ":" + changeColor(COLOR_CYAN, client.getNickname()) + " invited you in this channel : " + changeColor(COLOR_GREEN, "#" + client.getChannel()->channelName) + " do you want join it ? Y/n : ";

    Client* targetClient = findClientByName(userName);
    if (targetClient && targetClient != &client && targetClient->getLog())
    {
        targetClient->setState(true);
        targetClient->setPotentialChannel(channel->channelName);
        send(targetClient->getSocket(), message.c_str(), message.size(), 0);
        sendNotif(":" + changeColor(COLOR_GREEN, "Your invitation is send !\n"), client);
    }
}

void Server::join(std::vector<std::string> input, Client& client) {
    if (input.size() < 2)
        return sendErrorMessage(NOT_ENOUGH_ARGUMENTS, client);
    Channel* channel = findChannelByName(input[1]);
    ErrorCode error = checkJoinConditions(input, channel, client);
    if (error != NONE) return sendErrorMessage(error, client);
    channel->addClientToChannel(channel->channelName, client);
    std::string joinMessage = ":" + changeColor(COLOR_CYAN, client.getNickname()) + changeColor(COLOR_GREEN, " has joined the channel #" + channel->channelName) + ".\n";
    client.getChannel()->broadcastToChannel(channel->channelName, joinMessage, client);
}


void sendTopic(std::string topic, std::string channelName, Client& target) {
    if (!topic.empty()) {
        std::string displayTopic = ":" + changeColor(COLOR_GREEN, "#" + channelName +  "'s topic: ") + topic + "\n";
        size_t length = displayTopic.length();
        send(target.getSocket(), displayTopic.c_str(), length, 0);
    }
}

void Server::topic(std::vector<std::string> input, Client& client) {
    ErrorCode error = checkTopicConditions(input, client);
    if (error != NONE) return sendErrorMessage(error, client);

    std::string opt = input[1];
    std::string newTopic = concatenateTokens(input, 2);

    if (opt == "-s" || opt == "--set") {
        client.getChannel()->setTopic(newTopic);
    } else if (opt == "-p" || opt == "--print") {
        sendTopic(client.getChannel()->getTopic(), client.getChannel()->channelName, client);
    }
}

void modeTopic(Client& client) {
    Channel* channel = client.getChannel();
    channel->setNeedAdmin(!channel->getNeedAdmin());
    sendNotif(":" + changeColor(COLOR_GREEN, channel->getNeedAdmin()
            ? "The channel's topic can be set only by admin\n"
            : "The channel's topic can be set by everyone\n"), client);

}

void modeInvitOnly(Client& client) {
    Channel* channel = client.getChannel();
    channel->invitOnly = !channel->invitOnly;
    sendNotif(":" + changeColor(COLOR_GREEN, channel->invitOnly
            ? "Invite only mode actived\n"
            : "Invite only mode desactived\n"), client);

}

void modePassword(std::vector<std::string> input, Client& client) {
    Channel* channel = client.getChannel();

    if (input.size() == 3) {
        sendNotif(":" + changeColor(COLOR_GREEN, "You've delete the channel's password\n"), client);
        return ((void)channel->setPassword(""));
    }
    sendNotif(":" + changeColor(COLOR_GREEN, "The channel's password was changed\n"), client);
    return (channel->setPassword(input[3]));
}

void Server::modeOperator(std::vector<std::string> input, Client& client) {
    Client* targetUser = findClientByName(input[2]);
    targetUser->setAdmin(!targetUser->getAdmin());
    if (targetUser->getAdmin())
        return sendNotif(":" + changeColor(COLOR_GREEN, "Congratulation, ")
                + changeColor(COLOR_CYAN, client.getNickname())
                + changeColor(COLOR_GREEN, " up you as operator!\n"), *targetUser);
    return sendNotif(":" + changeColor(COLOR_CYAN, client.getNickname())
                + changeColor(COLOR_ORANGE, " downgrade you\n"), *targetUser);
}

void modelimitUser(std::vector<std::string> input, Client& client) {
    Channel* channel = client.getChannel();
    if (input.size() == 3) {
        sendNotif(":" + changeColor(COLOR_GREEN, "You've delete the channel's limit users\n"), client);
        return (channel->setMaxUsers(-1));
    }
    sendNotif(":" + changeColor(COLOR_GREEN, "The channel's max users was changed\n"), client);
    channel->setMaxUsers(atoi(input[3].c_str()));
}


void Server::mode(std::vector<std::string> input, Client& client) {
    ErrorCode error = checkModeConditions(input, client, this);
    if (error != NONE) return sendErrorMessage(error, client);

    std::string opt = input[1];
    if (opt == "-i") return modeInvitOnly(client);
    if (opt == "-t") return modeTopic(client);
    if (opt == "-k") return modePassword(input, client);
    if (opt == "-o") return modeOperator(input, client);
    if (opt == "-l") return modelimitUser(input, client);
}

Client* Server::findClientByName(const std::string& name) {
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        Client* client = *it;
        if (client->getNickname() == name) {
            return client;
        }
    }
    return NULL;
}

Channel* Server::findChannelByName(const std::string& name) {
    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
        Channel* channel = *it;
        if (channel->channelName == name) {
            return channel;
        }
    }
    return NULL;
}

bool Server::isChannelExist(const std::string& channelName) {
    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
        Channel* otherChannel = *it;

        if (otherChannel->channelName == channelName) {
            return true;
        }
    }
    return false;
}

bool Server::isUserExist(const std::string& userName) {
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        Client* otherClient = *it;

        if (otherClient->getNickname() == userName) {
            return true;
        }
    }
    return false;
}
