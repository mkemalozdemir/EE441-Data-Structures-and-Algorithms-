#include "func.hpp"

int main() {

    Network network(0.5);  //Arrange corruption rate
    std::srand(static_cast<unsigned int>(std::time(nullptr)));  //Start seed to use std::rand()
    // Define node neighbors
    std::vector<int> nb1 = {2, 3, 4};
    std::vector<int> nb2 = {1, 3, 4, 5};
    std::vector<int> nb3 = {1, 2, 4};
    std::vector<int> nb4 = {1, 2, 3, 6};    ///Form the neighbors for each node
    std::vector<int> nb5 = {2, 6, 7};
    std::vector<int> nb6 = {4, 5, 7};
    std::vector<int> nb7 = {5, 6};

    // Add nodes to the network
    network.addNode(1, nb1);
    network.addNode(2, nb2);
    network.addNode(3, nb3);
    network.addNode(4, nb4);                ///Form the graph
    network.addNode(5, nb5);
    network.addNode(6, nb6);
    network.addNode(7, nb7);

    Packet messagePacket(1, 6, "Hello world!"); // Create the packet


    network.simulate(messagePacket); //Simulate the adhoc network

    return 0;
}
