#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

int countFindOperations = 0;

template <class T>
class MergeSort{
    static void merge(T* array, T* leftArray, T* rightArray, int leftArraySize, int rightArraySize){
        int arrayIndex = 0, leftArrayIndex = 0, rightArrayIndex = 0;

        while(leftArrayIndex < leftArraySize && rightArrayIndex < rightArraySize){
            if(leftArray[leftArrayIndex].weight < rightArray[rightArrayIndex].weight){
                array[arrayIndex++] = leftArray[leftArrayIndex++];
            }
            else{
                array[arrayIndex++] = rightArray[rightArrayIndex++];
            }
        }

        while(leftArrayIndex < leftArraySize){
            array[arrayIndex++] = leftArray[leftArrayIndex++];
        }

        while(rightArrayIndex < rightArraySize){
            array[arrayIndex++] = rightArray[rightArrayIndex++];
        }
    }

public:
    static void sort(T* array,int size){
        if (size < 2) {
            return;
        }
        int mid = size / 2;
        T* leftArray = new T[mid];
        T* rightArray = new T[size-mid];

        int k = 0;
        for (int i = 0; i < size; i++) {
            if(i < mid){
                leftArray[i] = array[i];
            }
            else{
                rightArray[k] = array[i];
                k++;
            }
        }

        sort(leftArray,mid);
        sort(rightArray,size - mid);

        merge(array, leftArray, rightArray, mid, size - mid);
    }
};

struct Edge {
    int firstNode;
    int secondNode;
    float weight;

    void setEdge(int fNode, int sNode, float w) {
        this->firstNode = fNode;
        this->secondNode = sNode;
        this->weight = w;
    }
};

struct Graph {
    int numberOfNodes = {};
    int numberOfEdges = {};

    struct Node {
        float x;
        float y;

        void setNode(float xx, float yy){
            this->x = xx;
            this->y = yy;
        }
    };

    Node* nodeArray = {};
    Edge* edgeArray = {};

    Graph() = default;

    Graph(Node* nodeArray, Edge* edgeArray, int numberOfNodes, int numberOfEdges) {
        this->numberOfNodes = numberOfNodes;
        this->numberOfEdges = numberOfEdges;
        this->nodeArray = nodeArray;
        this->edgeArray = edgeArray;
    }

    static Graph* readFromFile(const string graphFileName) {
        ifstream file;
        file.open(graphFileName);
        string buffer;

        int nOfNodes, nOfEdges;

        getline(file, buffer);
        nOfNodes = stoi(buffer);
        Node* nArray = new Node[nOfNodes];

        for (int i = 0; i < nOfNodes; i++) {
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

        for (int i = 0; i < nOfEdges; i++) {
            int firstNode, secondNode;
            float weight;

            getline(file, buffer);
            stringstream ss(buffer);
            for (int j = 0; j < 3; j++) {
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

struct DisjointSet {
    int* parent = {};
    int* rank = {};

    explicit DisjointSet(int numberOfNodes) {
        parent = new int[numberOfNodes];
        rank = new int[numberOfNodes];

        for (int i = 0; i < numberOfNodes; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    void unionSetStandard(int index1, int index2) {
        int root1 = findSetStandard(index1);
        int root2 = findSetStandard(index2);
        parent[root1] = root2;
    }

    void unionSetByRank(int index1, int index2) {
        int root1 = findSetStandard(index1);
        int root2 = findSetStandard(index2);

        if (root1 == root2){
            return;
        }

        if(rank[root1] < rank[root2]) {
            parent[root1] = root2;
        }
        else if(rank[root1] > rank[root2]) {
            parent[root2] = root1;
        }
        else {
            parent[root2] = root1;
            rank[root1]++;
        }
    }


    int findSetStandard(int node) {
        countFindOperations++;
        if (parent[node] != node){
            parent[node]  = findSetStandard(parent[node]);
        }
        return parent[node];
    }

    int findSetPathCompression(int node) {
        countFindOperations++;
        if (parent[node] != node) {
            int result = findSetPathCompression(parent[node]);
            parent[node] = result;
            return result;
        }
        return parent[node];
    }
};

Edge* kruskal(Graph* graph, bool optimizationFlag){
    clock_t timeBefore, timeAfter;
    double timeSort, timeLoop;

    DisjointSet ds(graph->numberOfNodes);

    timeBefore = clock();
    MergeSort<Edge>::sort(graph->edgeArray, graph->numberOfEdges);
    timeAfter = clock();
    timeSort = ((timeAfter - timeBefore) / (double)CLOCKS_PER_SEC);

    float sumOfWeights = 0;
    int mstNumberOfEdges = 0;

    if (!optimizationFlag) {
        timeBefore = clock();
        for (int i = 0; i < graph->numberOfEdges; i++) {
            int node1 = graph->edgeArray[i].firstNode;
            int node2 = graph->edgeArray[i].secondNode;
            float weight = graph->edgeArray[i].weight;

            if (ds.findSetStandard(node1) != ds.findSetStandard(node2)) {
                ds.unionSetStandard(node1, node2);
                sumOfWeights += weight;
                mstNumberOfEdges++;
            }
        }
        timeAfter = clock();
        timeLoop = ((timeAfter - timeBefore) / (double) CLOCKS_PER_SEC);
    } else {
        timeBefore = clock();
        for (int i = 0; i < graph->numberOfEdges; i++) {
            int node1 = graph->edgeArray[i].firstNode;
            int node2 = graph->edgeArray[i].secondNode;
            float weight = graph->edgeArray[i].weight;

            if(ds.findSetPathCompression(node1) != ds.findSetPathCompression(node2)){
                ds.unionSetByRank(node1, node2);
                sumOfWeights += weight;
                mstNumberOfEdges++;
            }
        }
        timeAfter = clock();
        timeLoop = ((timeAfter - timeBefore) / (double)CLOCKS_PER_SEC);
    }

    cout << "\t- Sum of weights:\t\t" << sumOfWeights << endl
         << "\t- Number of edges:\t\t" << mstNumberOfEdges << endl
         << "\t- Number of find() operations:\t" << countFindOperations << endl
         << "\t- Time of sort:\t\t\t" << timeSort << endl
         << "\t- Time of Main Loop:\t\t" << timeLoop << endl;
    countFindOperations = 0;
}

int main() {
    struct Graph* graphs[3];
    struct Edge* mstGraphs[3];
    string graphFiles[3] = {"g1.txt", "g2.txt", "g3.txt"};

    cout << "\t\tUnionFind Standard " << endl;
    for (int i = 0; i < 3; i++) {
        graphs[i] = Graph::readFromFile(graphFiles[i]);
        cout << "[ Graph " << i+1 << " ]" << endl;
        mstGraphs[i] = kruskal(graphs[i], false);
    }

    cout << endl << endl << "\t\tUnionFind Optimized" << endl;
    for (int i = 0; i < 3; i++) {
        graphs[i] = Graph::readFromFile(graphFiles[i]);
        cout << "[ Graph " << i+1 << " ]" << endl;
        mstGraphs[i] = kruskal(graphs[i], true);
    }
    return 0;
}
