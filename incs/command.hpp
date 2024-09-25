/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audrye <audrye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:12:30 by audrye            #+#    #+#             */
/*   Updated: 2024/09/05 15:34:30 by audrye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>
#include <iostream>
#include "./utils.hpp"
#include "./server.hpp"
#include "./channel.hpp"

class Command {
private:
    std::string command;
    std::string option;
    std::string target;
    std::string argument;

public:
    Command();
    Command(const std::string& cmd, const std::string& tgt, const std::string& opt = "", const std::string& arg = "");
    Command(const Command& other);
    ~Command();
    Command& operator=(const Command& other);

    std::string getCommand() const;
    std::string getOption() const;
    std::string getTarget() const;
    std::string getArgument() const;

    void setCommand(const std::string& cmd);
    void setOption(const std::string& opt);
    void setTarget(const std::string& tgt);
    void setArgument(const std::string& arg);

};

#endif
