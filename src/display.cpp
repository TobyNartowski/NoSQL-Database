#include "display.hpp"
#include "column.hpp"
#include "menu.hpp"
#include "windows.hpp"
#include "operations.hpp"

#include <cstring>
#include <unistd.h>

Display *Display::instance = nullptr;

// Implementacje metod
Display::Display()
{
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

Display *Display::initDisplay()
{
    if(instance == 0)
        instance = new Display();
    return instance;
}

void Display::connectDatabase(Database *database)
{
    this->database = database;
}

Display::~Display()
{
    endwin();
}

void Display::destroyDisplay()
{
    delete instance;
}

std::string Display::getDatabaseName()
{
    while(true){
        bool spaceFlag = false;
        Windows::drawBasicWindow("", "Nowa baza danych");

        std::string message = "Podaj Nazwe bazy danych";
        std::string stringBuffer = Windows::drawInputWindow(Windows::mainWindow, message, getmaxy(Windows::mainWindow)/2);

        if(stringBuffer.empty()){
            Windows::drawErrorWindow("Nazwa bazy danych nie moze byc pusta!");
            continue;
        }

        for(unsigned int i = 0; i < stringBuffer.length(); i++)
            if(isspace(stringBuffer[i]))
                spaceFlag = true;

        if(spaceFlag){
            Windows::drawErrorWindow("Nazwa bazy danych nie moze zawierac spacji!");
            continue;
        }

        return stringBuffer;
    }
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
                Operations::drawDeleteMenu(database);
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
