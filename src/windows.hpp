// Klasa implementujaca obsluge okien ncurses
#pragma once

#include <ncurses.h>
#include <iostream>

class Windows
{
public:
    // Wskaznik na okno glowne
    static WINDOW *mainWindow;

    // Wskaznik na okno informacyjne
    static WINDOW *infoWindow;

    // Wskaznik na okno bledu
    static WINDOW *errorWindow;

    // Wskaznik na okno wyboru
    static WINDOW *choiceWindow;

    // Rysuje podstawowe, puste okno z nazwami
    // (przyjmuje nazwe bazy danych i nazwe okna)
    static void drawBasicWindow(std::string databaseName, std::string windowName);

    // Rysuje okno informacyjne
    static void drawInfoWindow();

    // Rysuje okno bledu (Przyjmuje odpowiedni blad)
    static void drawErrorWindow(std::string errorMessage);

    // Rysuje okno wyboru (Przyjmuje odpowiednia wiadomosc, zwraca wybor)
    static bool drawChoiceWindow(std::string choiceQuestion);

    // Wypisuje informacje o operacji
    static void printInfo(std::string info);

    // Pobiera zmienna od uzytkownika (Przyjmuje odpowiednie okno, nazwe pobieranego
    // parametru, przesuniecie oraz ewentualnie domyslna wartosc, zwraca pobrany bufor
    static std::string drawInputWindow(WINDOW *window, std::string name,
                                       unsigned int shift, std::string defaultText = "");
};
