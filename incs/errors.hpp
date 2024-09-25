/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntardy <ntardy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 02:19:26 by ntardy            #+#    #+#             */
/*   Updated: 2024/09/01 02:21:40 by ntardy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>
#include "./server.hpp"

class Client;
class Server;


#define ERROR_WRONG_PASSWORD			COLOR_RED "Error: Wrong password, retry!\n" COLOR_RESET
#define ERROR_PERMISSON_DENIED			COLOR_RED "Error: You do not have the required permissions.\n" COLOR_RESET
#define ERROR_USER_NOT_FOUND			COLOR_RED "Error: User not found.\n" COLOR_RESET
#define ERROR_CHANNEL_DOES_NOT_EXIST	COLOR_RED "Error: Channel does not exist.\n" COLOR_RESET
#define ERROR_UNAVAILABLE_NAME			COLOR_RED "Error: This name is unavailable.\n" COLOR_RESET
#define ERROR_USER_HAVE_ALREADY_CHANNEL	COLOR_RED "Error: You have already a channel.\n" COLOR_RESET
#define ERROR_USER_DOESNT_HAVE_CHANNEL	COLOR_RED "Error: You don't have a channel yet.\n" COLOR_RESET
#define ERROR_CHANNEL_NOT_FOUND			COLOR_RED "Error: Channel not found\n" COLOR_RESET
#define ERROR_NICKNAME_NOT_FREE			COLOR_RED "Error: This nickname isn't available yet.\n" COLOR_RESET
#define ERROR_USER_NOT_IN_CHANNEL		COLOR_RED "Error: User is not in the channel.\n" COLOR_RESET
#define ERROR_CANNOT_KICK_SELF			COLOR_RED "Error: You cannot kick yourself.\n" COLOR_RESET
#define ERROR_CANNOT_SEND_SELF			COLOR_RED "Error: You cannot send a message to yourself.\n" COLOR_RESET
#define ERROR_NOT_ENOUGH_ARGUMENTS 		COLOR_RED "Error: Not enough arguments provided.\n" COLOR_RESET
#define ERROR_TOO_MANY_ARGUMENTS 		COLOR_RED "Error: Too many arguments provided.\n" COLOR_RESET
#define ERROR_BAD_ARGUMENTS 			COLOR_RED "Error: Bad arguments provided.\n" COLOR_RESET
#define ERROR_DELETE_OWN_PRIVILEGE		COLOR_RED "Error: You cannot delete your own privileges.\n" COLOR_RESET
#define ERROR_MAX_USERS_REACHED			COLOR_RED "Error: The max users is reached.\n" COLOR_RESET
#define ERROR_INVITE_ONLY				COLOR_RED "Error: This channel is in invite only mode.\n" COLOR_RESET
#define ERROR_NEED_PASSWORD				COLOR_RED "Error: This channel need a password.\n" COLOR_RESET

enum ErrorCode {
	NONE,
	WRONG_PASSWORD,
	CHANNEL_DOES_NOT_EXIST,
	PERMISSION_DENIED,
	USER_NOT_FOUND,
	NICKNAME_NOT_FREE,
	CANNOT_KICK_SELF,
	CANNOT_SEND_SELF,
	USER_NOT_IN_CHANNEL,
	TOO_MANY_ARGUMENTS,
	NOT_ENOUGH_ARGUMENTS,
	BAD_ARGUMENTS,
	DELETE_OWN_PRIVILEGE,
	USER_HAVE_ALREADY_CHANNEL,
	MAX_USERS_REACHED,
	INVITE_ONLY,
	NEED_PASSWORD,
	UNAVAILABLE_NAME,
	USER_DOESNT_HAVE_CHANNEL,
};

int err(std::string str);
const char* getErrorMessage(ErrorCode error);
void sendErrorMessage(ErrorCode error, Client& client);

ErrorCode checkKickConditions(std::vector<std::string> input, Client& client, Client* targetClient);
ErrorCode checkTopicConditions(std::vector<std::string> input, Client& client);
ErrorCode checkModeConditions(const std::vector<std::string>& input, Client& client, Server* server);
ErrorCode checkInviteConditions(std::vector<std::string> input, Client& client, Channel* channel, bool userExist);
ErrorCode checkJoinConditions(const std::vector<std::string>& input, Channel* channel, Client& client);


#endif
