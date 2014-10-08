// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"
#include <limits>

Tour::Tour()
{
    firstNode = nullptr;
}

Tour::Tour(Point& a,Point& b,Point& c,Point& d){
    Node* nodeD = new Node(d,nullptr);
    Node* nodeC = new Node(c,nodeD);
    Node* nodeB = new Node(b,nodeC);
    Node* nodeA = new Node(a,nodeB);


    nodeD->next = nodeA;
    firstNode = nodeA;
}

Tour::~Tour()
{

}

void Tour::show()
{
    if (firstNode == nullptr) return;
    Node* current = firstNode;
    do {
        cout << current->point.toString() << endl;
        current = current->next;
    } while (current != firstNode);
}

void Tour::draw(QGraphicsScene *scene)
{
    scene->clear();
    if (firstNode == nullptr) return;
    Node* current = firstNode;
    do {
        current->point.drawTo(current->next->point,scene);
        current = current->next;
    } while (current != firstNode);}

int Tour::size()
{
    if ( firstNode == nullptr) return 0;
    int res = 0;
    Node* current = firstNode;
    do {
        res++;
        current = current->next;
    } while (current != firstNode);
    return res;
}

double Tour::distance()
{    
    if (firstNode == nullptr) return 0.0;
    double res = 0;
    Node* current = firstNode;
    do {
        res += current->point.distanceTo(current->next->point);
        current = current->next;
    } while (current != firstNode);
    return res;
}

void Tour::insertNearest(Point p)
{
    if (firstNode == nullptr) {
        firstNode = new Node(p,nullptr);
        firstNode->next = firstNode;
        return;
    }

    double shortestDistance = std::numeric_limits<double>::infinity();
    Node* bestNode;
    Node* current = firstNode;
    do {
        double distance = current->point.distanceTo(p);
        if (distance < shortestDistance){
            shortestDistance = distance;
            bestNode = current;
        }
        current = current->next;
    } while (current != firstNode);
    Node* nodeP = new Node(p,bestNode->next);
    bestNode->next = nodeP;
}

void Tour::insertSmallest(Point p)
{
    if (firstNode == nullptr) {
        firstNode = new Node(p,nullptr);
        firstNode->next = firstNode;
        return;
    }

    double shortestDifference = std::numeric_limits<double>::infinity();
    Node* bestNode;
    Node* current = firstNode;
    do {
        double difference = (current->point.distanceTo(p)+
                            p.distanceTo(current->next->point))-
                            current->point.distanceTo(current->next->point);
        if (difference < shortestDifference){
            shortestDifference = difference;
            bestNode = current;
        }
        current = current->next;
    } while (current != firstNode);
    Node* nodeP = new Node(p,bestNode->next);
    bestNode->next = nodeP;
}

void Tour::untangle(){
    Node* current = firstNode;
    do {
        Point p = current->point;
        Point q = current->next->point;
        Node* current2 = current->next->next;
        do {
            Point s = current2->point;
            Point t = current2->next->point;

            double side1 = (p.x-q.x)*(s.y-p.y) - (p.y-q.y)*(s.x-p.x);
            double side2 = (p.x-q.x)*(t.y-p.y) - (p.y-q.y)*(t.x-p.x);
            if ((side1 < 0 && side2 > 0) || side1 > 0 && side2 < 0 ){
                //TODO: Sätta ihop punkterna på ett nytt sätt
            }



            current2 = current2->next;
        } while (current2 != firstNode);


        current = current->next;
    } while (current != firstNode);
}
