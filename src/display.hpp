// Klasa Wyjscia odpowiadajaca za komunikacje z uzytkownikiem
#pragma once

#include "database.hpp"

#include <ncurses.h>
#include <iostream>
#include <vector>
#include <map>

namespace choices {
    typedef enum {
        MAIN, ADD
    } choices_t;

    static const std::vector<std::string> main_v = {
        "Wyswietl", "Dodaj", "Usun", "Zapisz",
        "Wczytaj", "Wyjdz"
    };
    static const std::vector<std::string> add_v = {
        "Tabele", "Kolumne", "Rekord"
    };

    static const std::vector<std::string> names = {
        "Menu glowne", "Dodaj"
    };

    static std::map<choices_t, std::vector<std::string>> choices = {
        {MAIN, main_v}, {ADD, add_v}
    };

    typedef enum {
        MAIN_WYSWIETL, MAIN_DODAJ, MAIN_USUN,
        MAIN_ZAPISZ, MAIN_WCZYTAJ, MAIN_WYJDZ
    } main_t;
    typedef enum {
        ADD_TABELE, ADD_KOLUMNE, ADD_REKORD
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

    // Wskaznik na okno informacyjne
    WINDOW *infoWindow;

    // Wskaznik na okno bledu
    WINDOW *errorWindow;

    // Wskaznik na okno wyboru
    WINDOW *choiceWindow;

    // Wypisuje menu
    // (Przyjmuje argument, ktore okno ma rysowac i odpowiednie podswietlenie)
    void printMenuChoices(choices::choices_t whichMenu, unsigned int highlight);
    // Przeciazona wersja rysowania menu (Przyjmuje pozycje menu, rozmiar tabel,
    // nazwe okna, odpowiednie podswietlenie)
    void printMenuChoices(const std::string *menuChoices, unsigned int size,
                          std::string windowName, unsigned int highlight);

    // Rysuje menu (Przyjmuje argument, ktore okno ma rysowac,
    // zwraca liczbe calkowita oznaczajaca wybor)
    int drawMenu(choices::choices_t whichMenu);
    // Przeciazona wersja rysowania menu
    // (Przyjmuje pozycje menu, rozmiar tabeli oraz nazwe okna)
    int drawMenu(const std::string menuChoices[], unsigned int size,
                          std::string windowName);

    // Wypisuje poziome menu (Przyjmuje odpowiednie okno pozycje menu,
    // rozmiar tabeli, pionowe polozenie oraz odpowiednie podswietlenie)
    void printHorizontalMenuChoices(WINDOW *window, const std::string *menuChoices,
                                    unsigned int size, unsigned int shift,
                                    unsigned int highlight);

    // Rysuje poziome menu (Przyjmuje odpowiednie okno, pozycje menu,
    // rozmiar tabeli oraz pionowe polozenie)
    int drawHorizontalMenu(WINDOW *window,const std::string *menuChoices,
                           unsigned int size, unsigned int shift);

    // Wypisuje menu z zaznaczeniem
    // (Przyjmuje pozycje menu, rozmiar tabeli, pionowe polozenie,
    // odpowiednie podswietlenie i tablice flag zaznaczenia)
    void printMenuSelectable(const std::string *menuChoices, unsigned int size,
                            unsigned int shift, unsigned int highlight,
                            bool *selected);

    // Rysuje menu z zaznaczeniem
    // (Przyjmuje pozycje menu, rozmiar tabeli, pionowe polozenie,
    // zwraca tablice flag zaznaczenia)
    bool *drawMenuSelectable(const std::string *menuChoices, unsigned int size,
                            unsigned int shift);

    // Rysuje podstawowe, puste okno z nazwami
    // (przyjmuje nazwe okna)
    void drawBasicWindow(std::string windowName);

    // Rysuje okno informacyjne
    void drawInfoWindow();

    // Rysuje okno bledu (Przyjmuje odpowiedni blad)
    void drawErrorWindow(std::string errorMessage);

    // Rysuje okno wyboru (Przyjmuje odpowiednia wiadomosc, zwraca wybor)
    bool drawChoiceWindow(std::string choiceQuestion);

    // Wypisuje informacje o operacji
    inline void printInfo(std::string info);

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

    // Dodaje tabele
    void addTable();

    // Dodaj kolumne (Zwraca flage, ktora okresla powrot do menu)
    bool addColumn();

    // Dodaje rekord (Zwraca flage, ktora okresla powrot do menu)
    bool addRecord();
};
