/* Labb 3 - TSP
 *
 * Namn:    Filip Magnusson
 * Liuid:   filma035
 *
 * Namn:    Johan Jansson
 * Liuid:   johja118
 */

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"
#include <limits>
#include <string>

Tour::Tour()
{
    firstNode = nullptr;
}

Tour::~Tour()
{
    if (firstNode == nullptr) return;
    Node* currentPtr = firstNode;

    while(currentPtr->next != nullptr){
        Node* nextPtr = currentPtr->next;
        delete currentPtr;
        currentPtr = nextPtr;
    }
    delete currentPtr;
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
    // Gå igenom alla noder
    do {
        // Beräkna avståndet mellan nuvarande nod och punkten p
        double distance = current->point.distanceTo(p);
        if (distance < shortestDistance){
            // Om avståndet är kortast hittills spara noden i bestNode
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
    // Gå igenom alla noder
    do {
        // Beräkna skillnaden mellan (nuvarande punkt -> p -> nästa punkt (s1 + s2)) och (nuvarande punkt -> nästa punkt (s3))
        /*
         *       p
         *      / \
         * s1  /   \  s2
         *    /     \
         *  nu———————nä
         *
         *      s3
         */
        double difference = (current->point.distanceTo(p)+
                             p.distanceTo(current->next->point))-
                current->point.distanceTo(current->next->point);
        if (difference < shortestDifference){
            // Om avståndet är kortast hittills spara noden i bestNode
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
    Node* p = current;
    Node* q = current->next;
    bool hasChanged = true;
    //En loop som kör tills det inte finns några korsningar mer.
    while(hasChanged) {
        hasChanged = false;
        // En dubbel loop som går igenom alla par av linjer
        do {
            p = current;
            q = current->next;
            Node* current2 = current->next;
            Node* s = current2->next;
            Node* t = current2->next->next;
            do {
                Node* prevS = current2;
                s = current2->next;
                t = current2->next->next;
                // Kollar om pq och st korsar varandra
                double side1 = (p->point.x-q->point.x)*(s->point.y-p->point.y) - (p->point.y-q->point.y)*(s->point.x-p->point.x);
                double side2 = (p->point.x-q->point.x)*(t->point.y-p->point.y) - (p->point.y-q->point.y)*(t->point.x-p->point.x);
                if ((side1 <= 0 && side2 > 0) || (side1 >= 0 && side2 < 0)){
                    double side3 = (s->point.x-t->point.x)*(p->point.y-s->point.y) - (s->point.y-t->point.y)*(p->point.x-s->point.x);
                    double side4 = (s->point.x-t->point.x)*(q->point.y-s->point.y) - (s->point.y-t->point.y)*(q->point.x-s->point.x);
                    if ((side3 <= 0 && side4 > 0) || (side3 >= 0 && side4 < 0)){
                        /*
                         * Fixar korsningen mellan pq och st på följande sätt
                         * Väg: p>q>s>t          Ny väg: p>s>q>t
                         *
                         * —p     s——\           —p——————s——\
                         *   \   /    \                      \
                         *    \ /      \                      \
                         *     X       |    =>                |
                         *    / \      /                      /
                         *   /   \    /                      /
                         * —t     q——/           —t——————q——/
                         *
                         */
                        hasChanged = true;
                        Node* currentPoint = q;
                        Node* nextPoint = q->next;
                        Node* yetAnotherPoint = nextPoint->next;
                        // Byt riktning på vägarna mellan s och q, så att det ej blir några stängda loopar
                        while (nextPoint != s){
                            nextPoint->next = currentPoint;
                            currentPoint = nextPoint;
                            nextPoint = yetAnotherPoint;
                            yetAnotherPoint = yetAnotherPoint->next;
                        }
                        p->next = s;
                        if (prevS == q)
                            s->next = q;
                        else{
                            s->next = prevS;
                        }
                        q->next = t;
                        break;
                    }
                }
                current2 = current2->next;
            } while (t != p);
            current = current->next;
        } while (q->next->next != firstNode);
    }
}
