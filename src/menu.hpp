// Klasa zawierajaca implementacje menu
#pragma once

#include <ncurses.h>
#include <iostream>
#include <vector>
#include <map>

namespace Choices {
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

class Menu
{
public:
    // Atrybuty okien
    static const int mainMenuSizeFactor = 10;

    // Wypisuje menu
    // (Przyjmuje okno, argument, ktore okno ma wypisac,
    // odpowiednie podswietlenie oraz nazwe bazy danych)
    static void printMenuChoices(WINDOW *window, Choices::choices_t whichMenu,
                                 unsigned int highlight, std::string databaseName);
    // Przeciazona wersja rysowania menu (Przyjmuje okno, pozycje menu, rozmiar tabel,
    // nazwe okna, nazwe bazy danych oraz odpowiednie podswietlenie)
    static void printMenuChoices(WINDOW *window, const std::string *menuChoices, unsigned int size,
                          std::string windowName, std::string databaseName, unsigned int highlight);

    // Rysuje menu (Przyjmuje okno, argument, ktore okno ma rysowac oraz nazwe bazy danych,
    // zwraca liczbe calkowita oznaczajaca wybor)
    static int drawMenu(WINDOW *window, Choices::choices_t whichMenu, std::string databaseName);
    // Przeciazona wersja rysowania menu
    // (Przyjmuje pozycje menu, rozmiar tabeli, nazwe okna oraz nazwe bazy danych)
    static int drawMenu(WINDOW *window, const std::string menuChoices[],
                        unsigned int size, std::string windowName, std::string databaseName);

    // Wypisuje poziome menu (Przyjmuje odpowiednie okno pozycje menu,
    // rozmiar tabeli, pionowe polozenie oraz odpowiednie podswietlenie)
    static void printHorizontalMenuChoices(WINDOW *window, const std::string *menuChoices,
                                    unsigned int size, unsigned int shift,
                                    unsigned int highlight);

    // Rysuje poziome menu (Przyjmuje odpowiednie okno, pozycje menu,
    // rozmiar tabeli oraz pionowe polozenie)
    static int drawHorizontalMenu(WINDOW *window, const std::string *menuChoices,
                           unsigned int size, unsigned int shift);

    // Wypisuje menu z zaznaczeniem
    // (Przyjmuje okno, pozycje menu, rozmiar tabeli, pionowe polozenie,
    // odpowiednie podswietlenie i tablice flag zaznaczenia)
    static void printMenuSelectable(WINDOW *window, const std::string *menuChoices,
                                    unsigned int size, unsigned int shift,
                                    unsigned int highlight, bool *selected);

    // Rysuje menu z zaznaczeniem
    // (Przyjmuje pozycje menu, rozmiar tabeli, pionowe polozenie,
    // zwraca tablice flag zaznaczenia)
    static bool *drawMenuSelectable(WINDOW *window, const std::string *menuChoices,
                                    unsigned int size, unsigned int shift);
};
