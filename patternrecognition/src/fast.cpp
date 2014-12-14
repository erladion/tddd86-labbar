/* Labb 7 - PatternRecognition
 *
 * Namn:    Johan Jansson
 * Id:      johja118
 *
 * Namn:    Filip Magnusson
 * Id:      filma035
 */

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Point.h"

using namespace std;

// constants
static const int SCENE_WIDTH = 512;
static const int SCENE_HEIGHT = 512;

/*
 * Ett funktionsobjekt som tar in en punkt p i konstruktorn
 * och sen jämför två punkter beroende på deras lutning mot punkten p
 */
class comparator{
public:
    explicit comparator(Point point) : p(point){}

    bool operator ()(Point q, Point r){
        return p.slopeTo(q) < p.slopeTo(r);
    }
private:
    Point p;
};

void render_points(QGraphicsScene* scene, const vector<Point>& points) {
    for(const auto& point : points) {
        point.draw(scene);
    }
}

void render_line(QGraphicsScene* scene, const Point& p1, const Point& p2) {
    p1.lineTo(scene, p2);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // open file
    string filename = "input12800.txt";
    ifstream input;
    input.open(filename);

    // the vector of points
    vector<Point> points;

    // read points from file
    int N;
    int x;
    int y;

    input >> N;

    for (int i = 0; i < N; ++i) {
        input >> x >> y;
        points.push_back(Point(x, y));
    }
    input.close();

    // setup graphical window
    QGraphicsView *view = new QGraphicsView();
    QGraphicsScene *scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    view->setScene(scene);
    // draw points to screen all at once
    render_points(scene, points);
    view->scale(1, -1); //screen y-axis is inverted
    view->resize(view->sizeHint());
    view->setWindowTitle("Brute Force Pattern Recognition");
    view->show();

    // sort points by natural order
    // makes finding endpoints of line segments easy
    sort(points.begin(), points.end());
    auto begin = chrono::high_resolution_clock::now();

    for(int i = 0; i < N-3;++i){
        Point p = points.at(i);
        // Skapar en ny lista som innehåller alla punkter som ligger efter p
        vector<Point> slopeList;
        for (int j = i+1; j < N;++j){
            slopeList.push_back(points.at(j));
        }
        // Sorterar den nya listan beorende på lutningen till p
        sort(slopeList.begin(),slopeList.end(),comparator(p));
        // De två senaste punkterna vi har kollat på
        Point sameSlope [2] = {
            slopeList.at(0),
            slopeList.at(1)
        };
        for(int j = 2;j < N-i-1;++j){
            // Kollar om båda punkterna i sameSlope och den nuvarande punkten har samma lutning mot p
            if(p.slopeTo(sameSlope[0]) == p.slopeTo(sameSlope[1]) && p.slopeTo(sameSlope[0]) == p.slopeTo(slopeList[j])){
                // Isåfall kollar vi vilken av de som har högst x-värde och ritar en linje från p till den
                Point largestX = sameSlope[0] > sameSlope[1] ? sameSlope[0] : sameSlope[1];
                largestX = largestX > slopeList.at(j) ? largestX : slopeList.at(j);
                render_line(scene,p,largestX);
                a.processEvents();
            }
            // Lägger in varannan punkt i slopeList[0] och varannan i slopeList[1]
            sameSlope[j%2] = slopeList[j];
        }
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    return a.exec(); // start Qt event loop
}
