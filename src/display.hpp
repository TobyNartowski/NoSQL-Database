// Klasa Wyjscia odpowiadajaca za komunikacje z uzytkownikiem
#pragma once

#include "database.hpp"

#include <ncurses.h>
#include <iostream>

#define MENU_CHOICES_SIZE 6

// Pozycje w menu
const std::string menuChoices[MENU_CHOICES_SIZE] = {
    "Wyswietl",
    "Dodaj",
    "Usun",
    "Zapisz",
    "Wczytaj",
    "Wyjdz",
};

typedef enum e_menuChoices {
    MENU_WYSWIETL = 1,
    MENU_DODAJ,
    MENU_USUN,
    MENU_ZAPISZ,
    MENU_WCZYTAJ,
    MENU_WYJDZ
} MenuChoices;

class Display
{
private:
    // Wskaznik na baze danych
    Database *database;

    // Wskaznik na instancje Klasy
    static Display *instance;

    // Prywatny konstruktor
    Display(Database *database);

    // Wskaznik na okno z menu glownym
    WINDOW *mainMenuWindow;

    // Wskaznik na okno rysujace baze danych
    WINDOW *drawDatabaseWindow;

    // Rysuje menu
    // (Przyjmuje wskaznik na menu, nazwe okna i odpowiednie podswietlenie)
    void printMenu(WINDOW *menuWindow, std::string windowName, int highlight);
public:
    // Prywatny destruktor
    ~Display();

    // Liczba elementow w menu
    const int choicesNumber = MENU_CHOICES_SIZE;
public:
    // Atrybuty okien
    static const int mainMenuSizeFactor = 10;

    // Zainicjuj klase wyjscia (Zwraca pojedynczy obiekt)
    static Display *initDisplay(Database *database);

    // Wywoluje destruktor obiektu - konczy prace klasy wyjscia
    void destroyDisplay();

    // Rysuje menu glowne
    void startMainMenu();

    // Rysuje cala baze danych
    void drawDatabase();
};
