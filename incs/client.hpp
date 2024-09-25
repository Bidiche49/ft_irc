/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntardy <ntardy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 13:19:38 by audrye            #+#    #+#             */
/*   Updated: 2024/09/03 14:34:36 by ntardy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "./command.hpp"
#include "./server.hpp"
#include "./channel.hpp"

class Server;
class Client;
class Channel;

typedef void (Server::*CommandFunction)(std::vector<std::string>, Client&);

class Client {
public:
    Client(int socket, struct sockaddr_in address);
    ~Client();

    int getSocket() const;
    std::string getNickname() const;
    std::string getUsername() const;
    std::string getPassword() const;
    bool getLog() const;
    Channel* getChannel() const;
    bool getAdmin() const;
    bool getState() const;
    std::string getPotentialChannel() const;
    std::map<std::string, CommandFunction> getCommandMap() const;
    void setLog(bool log);
    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);
    void setPassword(const std::string& password);
    void setChannel(Channel* channel);
    void setState(bool state);
    void setPotentialChannel(const std::string& channelName);
    std::string getIPAddress() const;
    int getPort() const;
    void setAdmin(bool admin);

private:
    int socket;
    struct sockaddr_in address;
    std::string nickname;
    std::string username;
    std::string password;
    bool log;
    bool admin;
    bool state;
    std::string potentialChannel;
    Channel* channel;
    std::map<std::string, CommandFunction> commandMap;
};


#endif
