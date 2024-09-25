/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntardy <ntardy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 14:49:35 by audrye            #+#    #+#             */
/*   Updated: 2024/09/03 21:15:14 by ntardy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <vector>
#include <string>
#include "./server.hpp"
#include "./client.hpp"

class Client;

class Channel {
	public:
		Channel(const std::string&  channelName, Client& admin);
		~Channel();

		std::string channelName;
		bool invitOnly;

		std::string getTopic();
		std::string getPassword();
		int getMaxUsers();
		bool getNeedAdmin();
		std::vector<Client*> getClients();
		void setTopic(const std::string& newTopic);
		void setPassword(const std::string& newPassword);
		void setMaxUsers(const int newMaxUsers);
		void setNeedAdmin(const bool needAdmin);
		void addClientToChannel(const std::string& channelName, Client& client);
		void removeClientFromChannel(Client& client);
		void broadcastToChannel(const std::string& channelName, const std::string& message, Client& sender);
		bool isClientInChannel(const std::string& name);

	private:
		std::string password;
		std::vector<Client*> clients;
		std::string topic;
		bool topicNeedAdmin;
		int maxUsers;
};

#endif

