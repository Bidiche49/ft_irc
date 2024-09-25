/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audrye <audrye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 18:22:00 by audrye            #+#    #+#             */
/*   Updated: 2024/09/05 15:35:15 by audrye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include "./client.hpp"

class Client;

std::vector<std::string> split(const std::string &s, char delimiter);
std::string concatenateTokens(const std::vector<std::string> &tokens, int start);
bool isCommand(const std::string &input);
std::string changeColor(const char* color, const std::string& text);
void sendNotif(std::string message, Client& client);

#endif
