/* Labb 3 - TSP
 *
 * Namn:    Filip Magnusson
 * Liuid:   filma035
 *
 * Namn:    Johan Jansson
 * Liuid:   johja118
 */

#ifndef TOUR_H
#define TOUR_H

#include "Node.h"
#include "Point.h"

class Tour {
public:
    // Skapar en ny tom tour
    Tour();    
    // Tar bort touren
    ~Tour();
    // Skriver ut alla noders koordinater
    void show();
    // Ritar touren
    void draw(QGraphicsScene* scene);
    // Antalet noder i touren
    int size();
    // Räknar ut av avståndet av hela touren
    double distance();
    // Sätter in en ny nod med insertNearest heuristiken
    void insertNearest(Point p);
    // Sätter in en ny nod med insertSmallest heuristiken
    void insertSmallest(Point p);
    // Tar bort alla korsningar om sådana finns
    void untangle();

private:
    Node* firstNode;
};

#endif // TOUR_H
