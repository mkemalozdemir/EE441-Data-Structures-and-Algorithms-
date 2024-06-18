#include <iostream>
#include <cstdlib>  //To use std::rand()
#include <ctime>
#include "adhoc.h"

int counter;
std::vector<int> holdSource;
std::vector<int> holdDest;

size_t _hash(int sourceId, int destId, std::string message);

size_t _hash(int sourceId, int destId, std::string message) {   //Random hash function
    size_t holder = 0;
    size_t holdID;

    holdID = 11*static_cast<size_t>(sourceId) + 17*static_cast<size_t>(destId); //Used prime numbers to obtain good hash function

    for (char ch : message) {
        holder += static_cast<size_t>(static_cast<int>(ch)); //Sum all the characters in the message according to their ascii representations
    }

    holder %= 100;
    holder += holdID;

    return holder;
}

///************************************* PACKET CLASS ******************************************************

Packet::Packet(int sourceId, int destId, std::string message)
    : sourceId(sourceId), destId(destId), message(message), hash(_hash(sourceId, destId, message)) {}

Packet::Packet() : sourceId(0), destId(0), message(""), hash(0) {}

int Packet::getSourceId() {
    return sourceId;
}

int Packet::getDestId() {
    return destId;
}

std::string Packet::getMessage() {
    return message;
}

bool Packet::checkIntegrity() {
    return hash == _hash(sourceId, destId, message);
}

void Packet::corrupt() {
    if (!message.empty()) {
        int index = std::rand() % message.size();
        int randomAscii = std::rand() % 223;

        while (randomAscii == 127 || randomAscii == 255) {
            randomAscii = std::rand() % 223;
        }

        randomAscii += 32;  // from the 32nd character to the 254th character
        message[index] = static_cast<char>(randomAscii);
    }
}

///************************************* MACPACKET CLASS ******************************************************

MacPacket::MacPacket(){
    type = MacPacketType::Empty;
}

MacPacket::MacPacket(MacPacketType type, int macSourceId,
            int macDestId, std::vector<int>& path, Packet& packet): type(type), macSourceId(macSourceId), macDestId(macDestId),
            path(path), packet(packet){}

MacPacket MacPacket::createMessagePacket(int macSourceId, int macDestId,std::vector<int>& path, Packet& packet){
    return MacPacket(MacPacketType::Message, macSourceId, macDestId, path, packet);
}
MacPacket MacPacket::createSuccessPacket(int macSourceId, int macDestId, std::vector<int>& path) {
    Packet a;
    return MacPacket(MacPacketType::Success, macSourceId, macDestId, path, a);
}

MacPacket MacPacket::createFailurePacket(int macSourceId, int macDestId, std::vector<int>& path) {
    Packet b;
    return MacPacket(MacPacketType::Failure, macSourceId, macDestId, path, b);
}


MacPacketType MacPacket::getType(){
    return type;
}

int MacPacket::getMacSourceId(){
    return macSourceId;
}

int MacPacket::getMacDestId(){
    return macDestId;
}

std::vector<int>& MacPacket::getPath(){
    return path;
}

Packet& MacPacket::getPacket(){
    return packet;
}

///************************************* NODE CLASS ******************************************************

Node::Node() : id(0), neighbors({}) {}

Node::Node(int id, std::vector<int>& neighbors):id(id), neighbors(neighbors){}

int Node::getId(){
    return id;
}

std::vector<int>& Node::getNeighbors(){
    return neighbors;
}

MacPacket Node::receive(MacPacket packet) {

    if(packet.getType() == MacPacketType::Message){ //If the received message type is message
            if(!packet.getPacket().checkIntegrity()){ //Check the corruption
                std::cout << "[NODE "<< packet.getMacDestId() <<"] Packet corrupted: \"" << packet.getPacket().getMessage() << "\". Requesting resend from Node " <<packet.getMacSourceId()<<"..."<<std::endl;
                MacPacket request = MacPacket::createFailurePacket(packet.getMacDestId(),packet.getMacSourceId(), packet.getPath());
                return request;
            }
            else{
                if(packet.getMacDestId()==packet.getPacket().getDestId()){  //Check whether the destination node is reached
                    std::cout << "[NODE "<< packet.getMacDestId() <<"] Message successfully received: \"" << packet.getPacket().getMessage() << "\". Sending acknowledgment to Node " <<packet.getMacSourceId()<<"..."<<std::endl;
                    MacPacket ack = MacPacket::createSuccessPacket(packet.getMacDestId(),packet.getMacSourceId(),packet.getPath());
                    return ack;
                }
                else{
                    int nextVertex;
                    for(int i=0; i<packet.getPath().size();i++){
                        if(packet.getPath()[i] == packet.getMacDestId()){
                            nextVertex = packet.getPath()[i+1];
                            break;
                        }
                    }
                    buffer = packet;
                    std::cout << "[NODE "<< packet.getMacDestId() <<"]  Packet received. Sending to Node " <<nextVertex<<"..."<<std::endl;
                    MacPacket newmsg = MacPacket::createMessagePacket(packet.getMacDestId(),nextVertex,packet.getPath(), packet.getPacket());
                    return newmsg;
                }
            }
    }

    else if(packet.getType() == MacPacketType::Success){//If the received message type is success
            if(packet.getMacDestId() == packet.getPath()[0]){ //If the source node is reached
                std::cout << "[NODE "<< packet.getMacDestId() <<"] Acknowledgment received."<<std::endl;
                MacPacket _empty = MacPacket();
                return _empty;
            }
            else{
                int nextVertex;
                for(int i=0; i<packet.getPath().size();i++){
                    if(packet.getPath()[i] == packet.getMacDestId()){
                        nextVertex = packet.getPath()[i-1];
                        break;
                    }
                }
                std::cout << "[NODE "<< packet.getMacDestId() <<"]  Acknowledgment received. Sending to Node " <<nextVertex<<"..."<<std::endl;
                MacPacket ack = MacPacket::createSuccessPacket(packet.getMacDestId(),nextVertex,packet.getPath());
                return ack;
            }
    }

    else if(packet.getType() == MacPacketType::Failure){ //If the received message type is failure
        std::cout << "[NODE "<< packet.getMacDestId() <<"]  Resending previous packet to Node " <<packet.getMacSourceId()<<"..."<<std::endl;
        MacPacket resend =MacPacket::createMessagePacket(packet.getMacDestId(),packet.getMacSourceId(),packet.getPath(),buffer.getPacket());
        return resend;
    }
}


///************************************* NETWORK CLASS ******************************************************

Network::Network(float corruptionRate):corruptionRate(corruptionRate) {} //constructor of the network class

void Network::addNode(int id, std::vector<int>& neighbors){
    nodes.push_back(Node(id,neighbors));            //Add nodes in the nodes vector
}
void Network::removeNode(int id) {                  //Remove Node from the graph
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].getId() == id) {
            nodes.erase(nodes.begin() + i);
            break;
        }
    }
}
std::vector<int> Network::calculatePath(int source, int destination) {
    std::vector<bool> visited(nodes.size(), false); //Determine the nodes visited
    std::vector<int> previous(nodes.size(), -1); //To determine the path
    std::vector<int> path;  //Gives the path
    std::vector<int> _queue; //instead of include queue header, I used a vector as a queue named queue

    _queue.push_back(source);   //Insert the source node in queue
    visited[source - 1] = true;  //Mark the source as visited

    while (!_queue.empty()) {               //Until que become empty
        int current = _queue.front();
        _queue.erase(_queue.begin());       //Delete from queue

        for (size_t i = 0; i < nodes[current - 1].getNeighbors().size(); ++i) {
            int neighborId = nodes[current - 1].getNeighbors()[i];

            if (!visited[neighborId - 1]) {
                visited[neighborId - 1] = true;
                previous[neighborId - 1] = current;
                _queue.push_back(neighborId); //Insert the neighbors of source to the queue

                if (neighborId == destination) {
                    // Reconstruct the path from destination to source
                    int temp = destination;

                    while (temp != -1) {
                        path.insert(path.begin(), temp); //By traversing the previous nodes in reverse order create path
                        temp = previous[temp - 1];
                    }

                    return path;
                }
            }
        }
    }

    return std::vector<int>();  // No path found
}

void Network::simulate(Packet packet) {
    int nextVertex;
    std::vector<int> PATH = calculatePath(packet.getSourceId(),packet.getDestId()); //Calculate minimum length of path
    Node* currentNode = getNode(packet.getSourceId());// Choose current node as source
    MacPacket newmsg = MacPacket::createMessagePacket(0,packet.getSourceId(),PATH,packet); // Create message and send it to source node
    MacPacket currentmsg;
    std::cout<<"Message to send: \""<<packet.getMessage()<<"\". Path: ";
    for(int i=0; i<PATH.size();i++){
        std::cout<< "-> "<<PATH[i];
    }
    std::cout<<std::endl;
    while(true){
        currentmsg = currentNode->receive(newmsg); // Update currentmsg
        packet = currentmsg.getPacket();           // Update packet for corruption situations.
        if(currentmsg.getType() == MacPacketType::Empty){ //Finish the simulation
            break;
        }
        else if(currentmsg.getType() == MacPacketType::Message){ //If the type of the message is message
            if((((float)std::rand())/RAND_MAX)< corruptionRate){ //Corrupt the message with a probability
                packet.corrupt();
            }
            for(int i=0; i<PATH.size();i++){
                if(PATH[i] == currentNode->getId()){    //Find the next node from path
                    nextVertex = PATH[i+1];
                    break;
                }
            }
            newmsg = MacPacket::createMessagePacket(currentNode->getId(),nextVertex,PATH,packet);    //From current node to next node send message
            currentNode = getNode(nextVertex);  //Update the current node as next node
        }
        else if(currentmsg.getType() == MacPacketType::Success){ //Received message type is success create success messages
            for(int i=0; i<PATH.size();i++){
                if(PATH[i] == currentNode->getId()){
                    nextVertex = PATH[i-1];
                    break;
                }
            }
            newmsg=MacPacket::createSuccessPacket(currentNode->getId(),nextVertex,PATH);
            currentNode = getNode(nextVertex);
        }
        else if(currentmsg.getType() == MacPacketType::Failure){//Received message type is failure create failure messages
            for(int i=0; i<PATH.size();i++){
                if(PATH[i] == currentNode->getId()){
                    nextVertex = PATH[i-1];
                    break;
                }
            }
            newmsg=MacPacket::createFailurePacket(currentNode->getId(),nextVertex,PATH);
            currentNode = getNode(nextVertex);
        }

    }
}



Node* Network::getNode(int id) {
    for (Node& node : nodes) {          //Search node by node to find node that has same id with function input
        if (node.getId() == id) {
            return &node;
        }
    }
    return nullptr;
}

