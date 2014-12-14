// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "costs.h"
#include "trailblazer.h"
// TODO: include any other headers you need; remove this comment
using namespace std;

vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    graph.resetData();
    Stack<vector<Vertex*> > pathStack;
    vector<Vertex*> path;
    path.push_back(start);
    pathStack.push(path);
    Vertex* currentNode = start;
    while (!pathStack.isEmpty()){
        path = pathStack.pop();
        currentNode = path.back();
        currentNode->visited = true;
        currentNode->setColor(GREEN);
        if (currentNode == end){
            return path;
        }
        bool foundWay = false;
        for (Node* node : graph.getNeighbors(currentNode)){
            if (node->visited != true){
                vector<Vertex*> newPath = path;
                newPath.push_back(node);
                pathStack.push(newPath);
                foundWay = true;
            }
        }
        if (!foundWay){
            for (int i = path.size()-1;i >= 0;--i){
                bool hasNeighbours = false;
                Node* currentNode = path[i];
                for (Node* node : graph.getNeighbors(currentNode)){
                    if (!(node->visited))
                            hasNeighbours = true;
                }
                if (hasNeighbours){
                    break;
                }
                currentNode->setColor(GRAY);
            }
        }

    }
    return vector<Vertex*>();
    // MAYBE CRASHES SOMETIMES

    /*
    Vertex* currentNode = start;
    currentNode->visited = true;
    vector<Vertex*> path;
    if(currentNode == end){
        path.push_back(currentNode);
        graph.resetData();
        return path;
    }
    for(Node* node: graph.getNeighbors(currentNode)){
        if (!(node->visited)){
            path.push_back(currentNode);
            vector<Vertex*> newPath = depthFirstSearch(graph, node, end);
            if (newPath.size() == 0) path.pop_back();
            for(auto it = newPath.begin(); it != newPath.end(); ++it){
                path.push_back(*it);
            }
        }
    }
    return path;
         */
}

vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    vector<Vertex*> path;
    return path;
}

vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    vector<Vertex*> path;
    return path;
}

vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    vector<Vertex*> path;
    return path;
}
