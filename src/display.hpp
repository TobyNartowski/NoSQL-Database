// Klasa Wyjscia odpowiadajaca za komunikacje z uzytkownikiem
#pragma once

#include "database.hpp"

#include <ncurses.h>

class Display
{
private:
    // Wskaznik na instancje Klasy
    static Display *instance;

    // Prywatny konstruktor
    Display();

    // Wskaznik na baze danych
    Database *database;

    // Prywatny destruktor
    ~Display();
public:
    // Zainicjuj klase wyjscia (Zwraca pojedynczy obiekt)
    static Display *initDisplay();

    // Laczy baze danych
    void connectDatabase(Database *database);

    // Wywoluje destruktor obiektu - konczy prace klasy wyjscia
    void destroyDisplay();

    // Metoda pobierajaca nazwe bazy danych
    std::string getDatabaseName();

    // Uruchamia menu glowne
    void startMainMenu();
};
