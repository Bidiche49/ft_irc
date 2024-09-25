/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntardy <ntardy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 18:22:34 by audrye            #+#    #+#             */
/*   Updated: 2024/09/04 19:26:14 by ntardy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/utils.hpp"

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> result;
    std::string current;
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (s[i] != delimiter && s[i] != '\n') {
            current += s[i];
        } else {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        }
    }
    if (!current.empty()) {
        result.push_back(current);
    }
    return result;
}

std::string concatenateTokens(const std::vector<std::string> &tokens, int start) {
    if (tokens.size() <= 1) {
        return "";
    }

    std::string result;
    for (std::vector<std::string>::size_type i = start; i < tokens.size(); ++i) {
        if (i > 1) {
            result += " ";
        }
        result += tokens[i];
    }
    return result;
}

bool isCommand(const std::string &input) {
    std::vector<std::string> commands;
    commands.push_back("PRIVMSG");
    commands.push_back("KICK");
    commands.push_back("INVITE");
    commands.push_back("TOPIC");
    commands.push_back("MODE");
    commands.push_back("CREATE");
    commands.push_back("JOIN");
    commands.push_back("QUIT");
    commands.push_back("EXIT");
    std::vector<std::string> tokens = split(input, ' ');

    if (tokens.empty()) {
        return false;
    }

    std::string firstWord = tokens[0];
    for (size_t i = 0; i < commands.size(); ++i) {
        if (firstWord == commands[i]) {
            return true;
        }
    }
    return false;
}

std::string changeColor(const char* color, const std::string& text) {
    return std::string(color) + text + COLOR_RESET;
}

void    Server::connection(std::string buf, Client& client, std::string password)
{
    std::vector<std::string> result;

    result = split(buf, ' ');
    if(result.size() == 2 && result[0] == "PASS")
    {
        if (!password.compare(result[1]))
            client.setPassword(result[1]);
        else
            sendErrorMessage(WRONG_PASSWORD, client);
    }
    else if(result.size() == 2 && result[0] == "NICK" && !client.getPassword().empty())
    {
        if (!isChannelExist(result[1]) && !isUserExist(result[1]))
            client.setNickname(result[1]);
        else
            sendErrorMessage(NICKNAME_NOT_FREE, client);
    }
    else if(result.size() > 1 && result[0] == "USER" && !client.getNickname().empty())
    {
        client.setUsername(concatenateTokens(result, 1));
        client.setLog(true);
        std::string ret = "You are login as " + std::string(client.getNickname()) + "\n";
        send(client.getSocket(), ret.c_str(), ret.size(), 0);
    }
    else {
        std::string ret = "need a PASS -> NICK -> USER\n";
        send(client.getSocket(), ret.c_str(), ret.size(), 0);
    }
}

void    Server::execCommand(std::string buffer, Client &client) {
    std::vector<std::string> tokens = split(buffer, ' ');
    Command command;

    if (client.getCommandMap().find(tokens[0]) != client.getCommandMap().end()) {
        CommandFunction func = client.getCommandMap()[tokens[0]];
        (this->*func)(tokens, client);
    } else {
        std::cout << ":Unknown command: " << buffer << std::endl;
    }
}
