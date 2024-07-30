# Network Bandwidth Allocation Using Shortest Path Algorithms

## Project Description

This project is a C++ application designed to optimize bandwidth allocation across a network of nodes using shortest path algorithms. The application uses Dijkstra's Algorithm and Yen's K-Shortest Paths Algorithm to determine the most efficient routes for data transmission between nodes, while dynamically allocating spectrum resources to minimize congestion and maximize network efficiency.

## Key Features

- **Routing:** The process of determining the most efficient paths for data transmission across a network. In this project, routing is achieved using Dijkstra's Algorithm for finding the shortest path between nodes, and Yen's K-Shortest Paths Algorithm for identifying alternative paths.

- **Spectrum Allocation:** The distribution of available frequency bands (spectrum) across network connections. This project dynamically allocates up to 50 bands per connection, ensuring optimal use of spectrum resources and reducing interference.

- **Network Optimization:** Techniques and algorithms used to improve network performance. This project focuses on optimizing bandwidth allocation and routing paths to achieve a 95% success rate in optimal band assignments, ensuring efficient data flow and reducing congestion.

## Getting Started

To run this project, you will need a C++ compiler and standard development tools. You can compile the project using the following commands:

```bash
g++ -o network_bandwidth main.cpp -std=c++11
./network_bandwidth
```
Ensure that you have all necessary dependencies installed and configured.

## Usage

The application calculates the shortest paths and allocates spectrum resources based on the specified network configuration. The results include allocated bands, routes for each connection, and identification of any blocked connections.

### License

This project is licensed under the MIT License. See the LICENSE file for more details.
