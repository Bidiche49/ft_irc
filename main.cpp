/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntardy <ntardy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:25:38 by audrye            #+#    #+#             */
/*   Updated: 2024/09/01 03:21:03 by ntardy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <cctype>
#include <vector>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "./incs/server.hpp"

typedef struct s_client
{
    int         port;
    std::string password;
}   t_client;

bool isOnlyDigits(const std::string &str)
{
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        if (!std::isdigit(*it))
            return false;
    }
    return true;
}


t_client parsing(char **argv)
{

    std::stringstream convert;
    t_client input;


    if (!isOnlyDigits(argv[1]))
    {
        std::cerr << "Only number accept for a port" << std::endl;
        exit(1);
    }

    convert << argv[1];
    convert >> input.port;
    if (input.port < 1024 || input.port > 65536)
    {
        std::cerr << "Requer a valid port - 1024 <-> 65536" << std::endl;
        exit(1);
    }

    input.password = argv[2];

    return input;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    t_client client = parsing(argv);

    Server server(client.port, client.password);
    server.start();

    return 0;
}
