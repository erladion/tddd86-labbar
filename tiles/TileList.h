/* Labb 3 - Tiles
 *
 * Namn:    Filip Magnusson
 * Liuid:   filma035
 *
 * Namn:    Johan Jansson
 * Liuid:   johja118
 */
#ifndef TILELIST_H
#define TILELIST_H

#include <QGraphicsScene>
#include "Tile.h"

/*
 * Klass för lista av tiles
 */
class TileList {
public:

    TileList();
    ~TileList();
    /*
     * Lägger till en ny tile i listan
     */
    void addTile(Tile tile);
    /*
     * Ritar ut alla tiles i listan
     */
    void drawAll(const QGraphicsScene* scene) const;
    /*
     * Returnerar index för den tile som ligger överst på koordinaterna x,y
     */
    int indexOfTopTile(int x, int y) const;
    /*
     * Flyttar ner den översta tilen på koordinaterna x,y till botten av listan
     */
    void lower(int x, int y);
    /*
     * Flyttar upp den översta tilen på koordinaterna x,y till toppen av listan
     */
    void raise(int x, int y);
    /*
     * Tar bort den översta tilen på koordinaterna x,y och flyttar om listan så att det ej finns tomrum
     */
    void remove(int x, int y);
    /*
     * Tar bort alla tiles på koordinaterna x,y och flyttar om alla element så att det ej finns tomrum
     */
    void removeAll(int x, int y);

private:
    // Antal element i listan
    int length = 0;
    // Totala kapaciteten på listan (10 som standard och utökas vid behov)
    int capacity = 10;
    // Listan där alla tiles sparas
    Tile* tileArray = new Tile[capacity];
    // Funktion för att ta bort tile på ett visst index i listan
    void removeIndex(int index);
};

#endif // TILELIST_H
