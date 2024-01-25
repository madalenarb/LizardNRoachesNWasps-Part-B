# LizardsNroachesNwasps: Multiplayer Network Game

## Project Overview

"LizardsNroachesNwasps" is an innovative multiplayer game, a creative variation of the classic snake game, developed for the Systems Programming course (2023/2024). This project is a testament to the skills in network programming, concurrent systems, and game design. It involves lizards navigating a field, eating cockroaches, and avoiding wasps, managed by a server and operated by various clients.

## Game Mechanics

- **Players control lizards**, trying to eat cockroaches and avoid wasps in a dynamic field.
- **Cockroaches** (denoted by digits) and **wasps** (represented by '#') move randomly.
- **Lizards' scores** change by eating cockroaches (+value of the cockroach) or being stung by wasps (-10 points).
- A **lizard wins** at 50 points and **loses** at a negative score.

## Distributed Architecture

- **Lizard-client**: Manages lizard movements based on user input.
- **lizardsNroachesNwasps-server**: Central server handling game logic, field updates, and client communication.
- **Roaches-client**: Controls up to 10 roaches with random movements.
- **Wasps-client**: Similar to Roaches-client but for controlling wasps.

## User Interfaces

- **Lizard-client** and **server** use NCurses for display.
- **Display-app** is integrated into Lizard-client for real-time field visualization.
- **Roaches-client** and **Wasps-client** don't require special interfaces but may output movements to the console.

## Interactivity and Networking

- Uses **ZeroMQ TCP sockets** for client-server communication.
- Protocols like **Lizard_connect**, **Lizard_movement**, **Roaches_movement**, **Wasps_movement**, and **Field_update** facilitate gameplay.
- The server is **multi-threaded**, handling multiple lizard clients and roach/wasp clients concurrently.

## Lifecycle of Game Elements

- **Cockroaches**: Appear/disappear based on Roaches-client's connection/disconnection. Eaten cockroaches reappear after 5 seconds.
- **Wasps**: Managed similar to cockroaches by the Wasps-client.
- **Lizards**: Have their lifecycle controlled by Lizard-client connections and movements.

## Error Handling and Security

- Robust **error validation** and **cheating prevention** mechanisms in place.
- Ensures game integrity against protocol manipulation.

## Development and Technologies

- Developed in **standard C (ANSI or C99)**.
- Multithreading without resorting to non-blocking communication or active wait.
- Utilizes **Protocol Buffers** for message encoding.

# Building and Running
Certainly, here's the section for your README.md file in Markdown format:

## How to Run "LizardsNroachesNwasps"

### Prerequisites
- Ensure you have a C compiler (like GCC) and Make installed for compiling the C programs.
- For running the Wasps-client, Python 3.x is required.
- ZeroMQ libraries must be installed on your system.
- Protocol Buffers (protobuf) should be installed for message encoding.

### Compilation
To compile all components of the game, including the server, lizard client, roaches client, and protocol buffers, use the provided Makefile:

```bash
cd LizardsNroachesNwasps
make all
```

This will compile the server, all clients, and any necessary libraries.

### Running the Server
Start the server by navigating to the server directory and executing the server binary:

```bash
cd src/server
./lizardsNroachesNwasps-server
```

### Running the Lizard Client
To run a lizard client, use the following command, replacing `<server_address>` and `<port>` with the appropriate values:

```bash
cd src/lizard-client
./Lizard-client <server_address> <port>
```

### Running the Roaches Client
Similarly, start the roaches client by navigating to its directory and executing the binary:

```bash
cd src/roaches-client
./Roaches-client <server_address> <port>
```

### Running the Wasps Client
The Wasps-client is a Python script. Ensure you are in the correct directory and run the script:

```bash
cd src/wasps-client
python Wasps-client.py <server_address> <port>
```

### Stopping the Game
To stop the game, you can simply terminate the server and clients using Ctrl+C in their respective terminal windows.

### Cleaning Up
After you're done, you can clean up all compiled files by running:

```bash
make clean
```

This will remove all object files and binaries, preparing the directory for a fresh compilation.
