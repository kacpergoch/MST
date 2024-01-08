#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

struct Graph {
    size_t numberOfNodes = {};
    size_t numberOfEdges = {};

    struct Node {
        float x = {};
        float y = {};

        Node() = default;

        void setNode(float xx, float yy){
            this->x = xx;
            this->y = yy;
        }
    };

    struct Edge {
        size_t firstNode = {};
        size_t secondNode = {};
        float weight = {};

        Edge() = default;

        void setEdge(size_t fNode, size_t sNode, float w) {
            this->firstNode = fNode;
            this->secondNode = sNode;
            this->weight = w;
        }
    };

    Node* nodeArray = {};
    Edge* edgeArray = {};

    Graph() = default;

    Graph(Node* nodeArray, Edge* edgeArray, size_t numberOfNodes, size_t numberOfEdges) {
        this->numberOfNodes = numberOfNodes;
        this->numberOfEdges = numberOfEdges;
        this->nodeArray = nodeArray;
        this->edgeArray = edgeArray;
    }

    static Graph* readFromFile(const string graphFileName) {
        ifstream file;
        file.open(graphFileName);
        string buffer;

        size_t nOfNodes = 0, nOfEdges = 0;

        getline(file, buffer);
        nOfNodes = stoi(buffer);
        Node* nArray = new Node[nOfNodes];

        for (int i = 0; i < nOfNodes; ++i) {
            float x = 0, y = 0;
            getline(file, buffer);
            stringstream ss(buffer);
            for (int j = 0; j < 2; ++j) {
                string str;
                ss >> str;

                switch (j) {
                    case 0:
                        x = stof(str);
                        break;
                    case 1:
                        y = stof(str);
                        break;
                    default:
                        break;
                }
            }
           nArray[i].setNode(x,y);
        }

        getline(file, buffer);
        nOfEdges = stoi(buffer);
        Edge* eArray = new Edge[nOfEdges];

        for (int i = 0; i < nOfEdges; ++i) {
            size_t firstNode, secondNode;
            float weight;

            getline(file, buffer);
            stringstream ss(buffer);
            for (int j = 0; j < 3; ++j) {
                string str;
                ss >> str;

                switch (j) {
                    case 0:
                        firstNode = stoi(str);
                        break;
                    case 1:
                        secondNode = stoi(str);
                        break;
                    case 2:
                        weight = stof(str);
                        break;
                    default:
                        break;
                }
            }
            eArray[i].setEdge(firstNode,secondNode,weight);
        }
        struct Graph* graph = new Graph(nArray, eArray, nOfNodes, nOfEdges);
        return graph;
    }
};

class UnionFind {
    size_t* indexPointer = {};
    size_t* ranges = {};

    UnionFind(size_t numberOfNodes) {

    }

    static void unionStandard(size_t index1, size_t index2) {

    }

    static void unionByRank(size_t index1, size_t index2) {

    }


    static int findStandard() {

    }

    static int findPathCompression() {

    }

    static void sort(Graph* graph) {
        bool swapped;
        for (int i = 0; i < graph->numberOfEdges - 1; ++i) {
            swapped = false;
            for (int j = 0; j < graph->numberOfEdges - i - 1; ++j) {
                if (graph->edgeArray[j].weight > graph->edgeArray[j + 1].weight) {
                    swap(graph->edgeArray[j], graph->edgeArray[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) {break;}
        }
    }
};

class Kruskal{

};

int main() {
    struct Graph* graphs[3];

    string graphFiles[3] = {"g1.txt", "g2.txt", "g3.txt"};
    for (int i = 0; i < 3; ++i) {
        graphs[i] = Graph::readFromFile(graphFiles[i]);
    }

    return 0;
}
