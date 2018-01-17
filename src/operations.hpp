// Klasa implementujaca poszczegolne operacje na bazie danych
#pragma once

#include <ncurses.h>

#include "database.hpp"

class Operations
{
public:
    // Rysuje cala baze danych
    static void drawDatabase(Database *database);

    // Rysuje menu dodawania
    static void drawAddMenu(Database *database);

    // Dodaje tabele (Zwraca flage, ktora okresla powrot do menu)
    static bool addTable(Database *database);

    // Dodaj kolumne (Zwraca flage, ktora okresla powrot do menu)
    static bool addColumn(Database *database);

    // Dodaje rekord (Zwraca flage, ktora okresla powrot do menu)
    static bool addRecord(Database *database);

    // Zaladuj baze danych
    static void loadDatabase(Database *database);

    // Zapisz baze danych
    static void saveDatabase(Database *database);
};
