#include "display.hpp"
#include "column.hpp"
#include "menu.hpp"
#include "windows.hpp"
#include "operations.hpp"

#include <cstring>
#include <unistd.h>

Display *Display::instance = nullptr;

// Implementacje metod
Display::Display(Database *database)
{
    this->database = database;

    initscr();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    cbreak();

    Windows::mainWindow = newwin(LINES-Menu::mainMenuSizeFactor, COLS-Menu::mainMenuSizeFactor,
        Menu::mainMenuSizeFactor/2, Menu::mainMenuSizeFactor/2);
    keypad(Windows::mainWindow, true);

    Windows::infoWindow = newwin(3, COLS-Menu::mainMenuSizeFactor, LINES - 5, 5);
    Windows::drawInfoWindow();

    Windows::errorWindow = newwin(11, COLS-48, LINES/2-6, 24);

    Windows::choiceWindow = newwin(11, COLS-48, LINES/2-6, 24);
    keypad(Windows::choiceWindow, true);
}

Display *Display::initDisplay(Database *database)
{
    if(instance == 0)
        instance = new Display(database);
    return instance;
}

Display::~Display()
{
    endwin();
}

void Display::destroyDisplay()
{
    delete instance;
}

void Display::startMainMenu()
{
    while(true){
        switch(Menu::drawMenu(Windows::mainWindow, Choices::MAIN, database->getName())){
            case Choices::MAIN_WYSWIETL:
                Operations::drawDatabase(database);
                break;
            case Choices::MAIN_DODAJ:
                Operations::drawAddMenu(database);
                break;
            case Choices::MAIN_USUN:
                getch();
                break;
            case Choices::MAIN_ZAPISZ:
                Operations::saveDatabase(database);
                break;
            case Choices::MAIN_WCZYTAJ:
                Operations::loadDatabase(database);
                break;
            case Choices::MAIN_WYJDZ:
                return;
            default:
                break;
        }
    }
}
