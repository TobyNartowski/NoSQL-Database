// Klasa Wyjscia odpowiadajaca za komunikacje z uzytkownikiem
#pragma once

#include "database.hpp"

#include <ncurses.h>
#include <iostream>
#include <vector>
#include <map>

namespace choices {
    typedef enum {
        MAIN = 0,
        ADD
    } choices_t;

    static const std::vector<std::string> main_v = {
        "Wyswietl", "Dodaj", "Usun", "Zapisz",
        "Wczytaj", "Wyjdz"
    };
    static const std::vector<std::string> add_v = {
        "Tabele", "Kolumne", "Rekord"
    };

    static const std::vector<std::string> names = {
        "Menu glowne", "Wyswietl", "Dodaj"
    };

    static std::map<choices_t, std::vector<std::string>> choices = {
        {MAIN, main_v}, {ADD, add_v}
    };

    typedef enum {
        MAIN_WYSWIETL = 0, MAIN_DODAJ, MAIN_USUN,
        MAIN_ZAPISZ, MAIN_WCZYTAJ, MAIN_WYJDZ
    } main_t;
    typedef enum {
        ADD_TABELE = 0, ADD_KOLUMNE, ADD_REKORD
    } add_t;
}

class Display
{
private:
    // Wskaznik na baze danych
    Database *database;

    // Wskaznik na instancje Klasy
    static Display *instance;

    // Prywatny konstruktor
    Display(Database *database);

    // Wskaznik na okno glowne
    WINDOW *mainWindow;

    // Rysuje menu
    // (Przyjmuje argument, ktore okno ma rysowac i odpowiednie podswietlenie)
    void printMenu(choices::choices_t whichMenu, int highlight);

    // Rysuje menu (Przyjmuje argument, ktore okno ma rysowac,
    // zwraca liczbe calkowita oznaczajaca wybor)
    unsigned int drawMenu(choices::choices_t whichMenu);

    // Prywatny destruktor
    ~Display();
public:
    // Atrybuty okien
    static const int mainMenuSizeFactor = 10;

    // Zainicjuj klase wyjscia (Zwraca pojedynczy obiekt)
    static Display *initDisplay(Database *database);

    // Wywoluje destruktor obiektu - konczy prace klasy wyjscia
    void destroyDisplay();

    // Uruchamia menu glowne
    void startMainMenu();


    // Rysuje cala baze danych
    void drawDatabase();

    // Rysuje menu dodawania
    void drawAddMenu();
};
