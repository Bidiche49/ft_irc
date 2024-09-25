/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntardy <ntardy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 03:06:05 by ntardy            #+#    #+#             */
/*   Updated: 2024/09/04 21:35:08 by ntardy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/errors.hpp"

int err(std::string str)
{
	std::cerr << str << std::endl;
	return 1;
}
void sendErrorMessage(ErrorCode error, Client& target) {
	const char* errorMessage = getErrorMessage(error);
	if (errorMessage) {
		size_t length = std::strlen(errorMessage);
		send(target.getSocket(), errorMessage, length, 0);
	}
}

const char* getErrorMessage(ErrorCode error) {
	switch (error) {
		case PERMISSION_DENIED:			return ERROR_PERMISSON_DENIED;
		case USER_NOT_FOUND:			return ERROR_USER_NOT_FOUND;
		case CANNOT_KICK_SELF:			return ERROR_CANNOT_KICK_SELF;
		case CANNOT_SEND_SELF:			return ERROR_CANNOT_SEND_SELF;
		case USER_NOT_IN_CHANNEL:		return ERROR_USER_NOT_IN_CHANNEL;
		case NOT_ENOUGH_ARGUMENTS:		return ERROR_NOT_ENOUGH_ARGUMENTS;
		case TOO_MANY_ARGUMENTS:		return ERROR_TOO_MANY_ARGUMENTS;
		case BAD_ARGUMENTS:				return ERROR_BAD_ARGUMENTS;
		case WRONG_PASSWORD:			return ERROR_WRONG_PASSWORD;
		case NICKNAME_NOT_FREE:			return ERROR_NICKNAME_NOT_FREE;
		case DELETE_OWN_PRIVILEGE:		return ERROR_DELETE_OWN_PRIVILEGE;
		case USER_HAVE_ALREADY_CHANNEL:	return ERROR_USER_HAVE_ALREADY_CHANNEL;
		case MAX_USERS_REACHED:			return ERROR_MAX_USERS_REACHED;
		case INVITE_ONLY:				return ERROR_INVITE_ONLY;
		case NEED_PASSWORD:				return ERROR_NEED_PASSWORD;
		case CHANNEL_DOES_NOT_EXIST:	return ERROR_CHANNEL_DOES_NOT_EXIST;
		case UNAVAILABLE_NAME:			return ERROR_UNAVAILABLE_NAME;
		case USER_DOESNT_HAVE_CHANNEL:	return ERROR_USER_DOESNT_HAVE_CHANNEL;
		default:						return NULL;
	}
}

ErrorCode checkKickConditions(std::vector<std::string> input, Client& client, Client* targetClient) {
	if (!client.getChannel()) {
		return USER_DOESNT_HAVE_CHANNEL;
	}
	if (!client.getAdmin()) {
		return PERMISSION_DENIED;
	}
	if (!targetClient) {
		return USER_NOT_FOUND;
	}
	if (!targetClient->getLog()) {
		return USER_NOT_FOUND;
	}
	if (targetClient == &client) {
		return CANNOT_KICK_SELF;
	}
	if (!client.getChannel()->isClientInChannel(input[1])) {
		return USER_NOT_IN_CHANNEL;
	}
	return NONE;
}

ErrorCode checkInviteConditions(std::vector<std::string> input, Client& client, Channel* channel, bool userExist) {
	if (!channel) {
		return CHANNEL_DOES_NOT_EXIST;
	}
	if (!client.getAdmin()) {
		return PERMISSION_DENIED;
	}
	if (input.size() != 2) {
		return NOT_ENOUGH_ARGUMENTS;
	}
    if (!userExist) {
		return USER_NOT_FOUND;
	}
	if (channel->getMaxUsers() == (int)(channel->getClients().size()))
		return MAX_USERS_REACHED;
	return NONE;
}

ErrorCode checkTopicConditions(std::vector<std::string> input, Client& client) {
	if (!client.getChannel()) {
		return PERMISSION_DENIED;
	}
	if (input.size() < 2) {
		return NOT_ENOUGH_ARGUMENTS;
	}
	if (input[1]!= "-p" && input[1] != "--print"
			&& input[1] != "-s" && input[1] != "--set") {
		return BAD_ARGUMENTS;
	}
	if ((input[1] == "-s" || input[1] == "--set")
		&& !client.getAdmin()
		&& client.getChannel()
		&& client.getChannel()->getNeedAdmin()) {
		return PERMISSION_DENIED;
	}
	if ((input[1] == "-p" || input[1] == "--print") && input.size() != 2) {
		return TOO_MANY_ARGUMENTS;
	}
	if ((input[1] == "-s" || input[1] == "--set") && input.size() < 3) {
		return NOT_ENOUGH_ARGUMENTS;
	}
	return NONE;
}

ErrorCode checkSetDeleteOptions(std::vector<std::string> input) {
	if (input.size() < 3) {
		return NOT_ENOUGH_ARGUMENTS;
	}
	const std::string& option = input[2];
	if (option != "-d" && option != "-s") {
		return BAD_ARGUMENTS;
	}

	if ((option == "-d" && input.size() != 3) ||
		(option == "-s" && input.size() != 4)) {
		return TOO_MANY_ARGUMENTS;
	}

	return NONE;
}

ErrorCode checkModeConditions(const std::vector<std::string>& input, Client& client, Server* server) {
	if (!client.getAdmin() || !client.getChannel()) {
		return PERMISSION_DENIED;
	}

	if (input.size() < 2) {
		return NOT_ENOUGH_ARGUMENTS;
	}

	const std::string& mode = input[1];
	if (mode != "-i" && mode != "-t" && mode != "-k"
			&& mode != "-o" && mode != "-l") {
		return BAD_ARGUMENTS;
	}

	if ((mode == "-i" || mode == "-t") && input.size() != 2) {
		return TOO_MANY_ARGUMENTS;
	}

	if (mode == "-k") {
		return checkSetDeleteOptions(input);
	}

	if (mode == "-l") {
		ErrorCode error = checkSetDeleteOptions(input);
		if (error != NONE)
			return error;
		if (input.size() == 4) {
			if (atoi(input[3].c_str()) <= 0) {
				return BAD_ARGUMENTS;
			}
		}
	}

	if (mode == "-o") {
		if (input.size() != 3) {
			return input.size() < 3 ? NOT_ENOUGH_ARGUMENTS : TOO_MANY_ARGUMENTS;
		}
		if (input[2] == client.getNickname()) {
			return DELETE_OWN_PRIVILEGE;
		}
		if (!server->isUserExist(input[2])) {
			return USER_NOT_FOUND;
		}
		if (!client.getChannel()->isClientInChannel(input[2])) {
			return USER_NOT_IN_CHANNEL;
		}
	}

	return NONE;
}

ErrorCode checkJoinConditions(const std::vector<std::string>& input, Channel* channel, Client& client){
	if (!channel) {
		return CHANNEL_DOES_NOT_EXIST;
	}
	if (client.getChannel()) {
		return USER_HAVE_ALREADY_CHANNEL;
	}
	if (input.size() < 2) {
		return NOT_ENOUGH_ARGUMENTS;
	}
	if (input.size() > 3) {
		return TOO_MANY_ARGUMENTS;
	}
	if (channel->invitOnly){
		return INVITE_ONLY;
	}
	std::string password = channel->getPassword();
	if (!password.empty() && input.size() != 3) {
		return NEED_PASSWORD;
	}
	if (!password.empty() && password != input[2]) {
		return WRONG_PASSWORD;
	}
	if (channel->getMaxUsers() == (int)channel->getClients().size()) {
		return MAX_USERS_REACHED;
	}
	return NONE;
}
