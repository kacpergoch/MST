#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
using namespace std;


template <class T>
struct DynamicArray {
    int arraySize = {};
    int numberOfNodes = {};
    T* arrayPointer = nullptr;
    float resizeMultiplier = {};
    int firstAvailableIndex = {};

    DynamicArray() {
        arraySize = 1;
        arrayPointer = new T[1];
        numberOfNodes = 0;
        resizeMultiplier = 1.5;
        firstAvailableIndex = 0;
    }

    ~DynamicArray() {
        delete [] arrayPointer;
    }

    void resize() {
        int arrayNewSize = arraySize * resizeMultiplier;
        T* tempPointer = new T[arrayNewSize];

        for (int i = 0; i < arraySize; ++i) {
            tempPointer[i] = arrayPointer[i];
        }
        delete arrayPointer;
        arraySize = arrayNewSize;
        arrayPointer = tempPointer;
    }

    void resize(int arrayNewSize) {
        T* tempPointer = new T[arrayNewSize];

        for (int i = 0; i < arraySize; ++i) {
            tempPointer[i] = arrayPointer[i];
        }
        delete arrayPointer;
        arraySize = arrayNewSize;
        arrayPointer = tempPointer;
    }

    void resize(int arrayNewSize, T value) {
        T* tempPointer = new T[arrayNewSize];

        for (int i = 0; i < arraySize; ++i) {
            tempPointer[i] = value;
        }
        delete arrayPointer;
        arraySize = arrayNewSize;
        arrayPointer = tempPointer;
    }

    void addNode(T* newNode) {
        if(firstAvailableIndex >= arraySize) { resize(); }

        arrayPointer[firstAvailableIndex] = *newNode;
        firstAvailableIndex++;
        numberOfNodes++;
    }

    bool checkIndex(int index){
        return (index < 0 || index >= arraySize) ? true : false;
    }

    T v(int index) {
        return checkIndex(index) ? arrayPointer[index] : 0;
    }

    bool u(int index, T newValue) {
        if(checkIndex(index) == false) { return false; }
        arrayPointer[index] = newValue;
        return true;
    }

    void clear() {
        delete arrayPointer;
        arrayPointer = new T[arraySize];
        firstAvailableIndex = 0;
    }
};

template <class T>
struct MergeSort{
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
    int firstNode = {};
    int secondNode = {};
    float weight = {};

    Edge() = default;

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
        float x = {};
        float y = {};

        Node() = default;

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

        int nOfNodes = 0, nOfEdges = 0;

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
    int* size = {};

    DisjointSet(int numberOfNodes) {
        parent = new int[numberOfNodes];
        rank = new int[numberOfNodes];
        size = new int[numberOfNodes];

        for (int i = 0; i < numberOfNodes; i++) {
            parent[i] = i;
            rank[i] = 0;
            size[i] = 1;
        }
    }

    void unionSetStandard(int index1, int index2) {
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

    void unionSetByRank(int index1, int index2) {

    }


    int findSetStandard(int node) {
        if (parent[node] != node){
            parent[node]  = findSetStandard(parent[node]);
        }
        return parent[node];
    }

    static int findSetPathCompression() {

    }
};

void kruskal(Graph* graph, Edge* mst){
    DisjointSet ds(graph->numberOfNodes);
    MergeSort<Edge>::sort(graph->edgeArray, graph->numberOfEdges);
    float sumOfWeights = 0;
    int mstNumberOfEdges = 0, numberOfFindOperations = 0;

    for (int i = 0; i < graph->numberOfEdges; i++) {
        int node1 = graph->edgeArray[i].firstNode;
        int node2 = graph->edgeArray[i].secondNode;
        float weight = graph->edgeArray[i].weight;

        if(ds.findSetStandard(node1) != ds.findSetStandard(node2)){
            ds.unionSetStandard(node1, node2);
            sumOfWeights += weight;
            mstNumberOfEdges++;
        }
    }
    cout << "Sum of weights: " << sumOfWeights << ", Number of edges: " << mstNumberOfEdges << endl;
}

int main() {
    struct Graph* graphs[3];

    string graphFiles[3] = {"g1.txt", "g2.txt", "g3.txt"};
    for (int i = 0; i < 3; i++) {
        graphs[i] = Graph::readFromFile(graphFiles[i]);
        Edge* mst = nullptr;
        kruskal(graphs[i], mst);
    }

    return 0;
}
