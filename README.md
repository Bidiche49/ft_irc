
---

# ft_irc

**Developed with @augustin06130 @spookier**

## Description

`ft_irc` is an implementation of an Internet Relay Chat (IRC) server in C. The project aims to create a functional IRC server that allows users to connect, communicate in channels, and manage basic user commands. This project serves as an exercise in networking, multithreading, and protocol design.

## Features

- **User Connections**: Allows multiple users to connect to the server simultaneously.
- **Channel Management**: Users can create, join and leave channels, facilitating group communication.
- **Private Messaging**: Supports sending private messages between users.
- **Command Handling**: Implements basic IRC commands such as JOIN, QUIT, PRIVMSG, CREATE, MODE, and JOIN.

## Key Technologies

- **C++98 Programming Language**: The primary language used for developing the IRC server.
- **Sockets**: Utilizes TCP sockets for network communication between clients and the server.
- **Makefile**: Automates the compilation process for easier project management.

## Learning Outcomes

- Gained experience in implementing network protocols and server-client architecture.
- Learned to handle user input and manage communication in a networked environment.

Here’s the updated README for your `ft_irc` project, reflecting the specified changes:

---

# ft_irc

## Description

`ft_irc` is an implementation of an Internet Relay Chat (IRC) server in C. The project aims to create a functional IRC server that allows users to connect, communicate in channels, and manage basic user commands. This project serves as an exercise in networking, multithreading, and protocol design.

## Features

- **User Connections**: Allows multiple users to connect to the server simultaneously.
- **Channel Management**: Users can join and leave channels, facilitating group communication.
- **Private Messaging**: Supports sending private messages between users.

## Command Handling

Implements essential IRC commands, including:
- **PRIVMSG**: Sends a message to a specified user or channel.
- **KICK**: Removes a user from a channel.
- **INVITE**: Invites a user to join a specific channel.
- **CREATE**: Creates a new channel.
- **MODE**: Changes the mode of a channel or user.
- **JOIN**: Allows a user to join a specified channel.
- **QUIT**: Disconnects a user from the server.

## Key Technologies

- **C Programming Language**: The primary language used for developing the IRC server.
- **Sockets**: Utilizes TCP sockets for network communication between clients and the server.
- **Multithreading**: Uses threads to handle multiple user connections concurrently.
- **Makefile**: Automates the compilation process for easier project management.

## Learning Outcomes

- Gained experience in implementing network protocols and server-client architecture.
- Developed skills in multithreading and concurrent programming.
- Learned to handle user input and manage communication in a networked environment.

## How to Use

1. Clone the repository:
   ```bash
   git clone https://github.com/Bidiche49/ft_irc
   cd ft_irc
   ```
2. Compile the project:
   ```bash
   make
   ```
3. Run the server:
   ```bash
   ./ft_irc
   ```

4. Launch clients in another terminal using one of the provided bash scripts:
   ```bash
   ./alex.sh
   ```
   or
   ```bash
   ./aug.sh
   ```
   or
   ```bash
   ./bidiche.sh
   ```

## Example Usage

To connect a client, simply run one of the bash scripts in another terminal.

---
