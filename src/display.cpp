#include "display.hpp"

#include <string>
#include <unistd.h> // TODO: DEBUG DELETE

Display *Display::instance = 0;

// Implementacje metod
Display::Display(Database *database)
{
    this->database = database;

    initscr();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    cbreak();

    mainWindow = newwin(LINES-mainMenuSizeFactor, COLS-mainMenuSizeFactor,
        mainMenuSizeFactor/2, mainMenuSizeFactor/2);
    keypad(mainWindow, true);
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

void Display::printMenu(choices::choices_t whichMenu, int highlight)
{
    wclear(mainWindow);

    int x, y, xBuffer;
    getmaxyx(mainWindow, y, x);
    xBuffer = x;

    x = x/2 - 6;
    y = y/2 - ((choices::choices[whichMenu]).size()/2);

    box(mainWindow, 0, 0);
    mvwprintw(mainWindow, 0, 3, "%s", choices::names[choices::MAIN].c_str());
    mvwprintw(mainWindow, 0, xBuffer-3-strlen(database->getName().c_str()),
              "%s", database->getName().c_str());

    for(int i = 0; i < choices::choices[whichMenu].size(); i++){
        if(highlight == i){
            wattron(mainWindow, A_REVERSE);
            mvwprintw(mainWindow, y, x, "%s", choices::choices[whichMenu][i].c_str());
            wattroff(mainWindow, A_REVERSE);
        }
        else
            mvwprintw(mainWindow, y, x, "%s", choices::choices[whichMenu][i].c_str());
            y++;
    }
    wrefresh(mainWindow);
}

unsigned int Display::drawMenu(choices::choices_t whichMenu)
{
    int highlight = 0;
    int choice = -1;
    int buffer;

    box(mainWindow, 0, 0);

    refresh();
    printMenu(whichMenu, highlight);

    while(true){
        buffer = wgetch(mainWindow);
        switch(buffer){
            case KEY_UP:
                if(highlight == 0)
                    highlight = choices::choices[whichMenu].size()-1;
                else
                    highlight--;
                break;
            case KEY_DOWN:
                if(highlight == choices::choices[whichMenu].size()-1)
                    highlight = 0;
                else
                    highlight++;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                break;
        }
        printMenu(whichMenu, highlight);

        if(choice != -1)
            break;
    }
    return choice;
}

void Display::startMainMenu()
{
    while(true){
        switch(drawMenu(choices::MAIN)){
            case choices::MAIN_WYSWIETL:
                wclear(mainWindow);
                drawDatabase();
                break;
            case choices::MAIN_DODAJ:
                wclear(mainWindow);
                drawAddMenu();
                break;
            case choices::MAIN_USUN:
                wclear(mainWindow);
                getch();
                break;
            case choices::MAIN_ZAPISZ:
                wclear(mainWindow);
                getch();
                break;
            case choices::MAIN_WCZYTAJ:
                wclear(mainWindow);
                getch();
                break;
            case choices::MAIN_WYJDZ:
                return;
            default:
                break;
        }
    }
}

void Display::drawDatabase()
{
    int xBuffer = getmaxx(mainWindow);

    box(mainWindow, 0, 0);
    mvwprintw(mainWindow, 0, 3, "%s", choices::main_v[0].c_str());
    mvwprintw(mainWindow, 0, xBuffer-3-strlen(database->getName().c_str()),
              "%s", database->getName().c_str());

    while(true){
        if(!database->getDatabaseSize()) {
            mvwprintw(mainWindow, getmaxy(mainWindow)/2,
                      getmaxx(mainWindow)/2-strlen("Baza danych jest pusta!")/2, "Baza danych jest pusta!");
                      mvwprintw(mainWindow, getmaxy(mainWindow)-3,
                                getmaxx(mainWindow)/2-strlen("(Wcisnij RETURN, aby wyjsc)")/2, "(Wcisnij RETURN, aby wyjsc)");
            wrefresh(mainWindow);
            while(true){
                if(getch() == 10)
                    return;
            }
        }

        std::string readBuffer;
        std::string headerBuffer;
        unsigned int horizontalIndex;
        unsigned int verticalIndex = 0;
        bool printHeader = true;

        for(unsigned int i = 0; i < database->getDatabaseSize(); i++){
            std::string lineHeader = "";
            horizontalIndex = 0;
            for(unsigned int j = 0; j < database->getTable(i)->getTableSize(); j++){
                printHeader = true;
                for(unsigned int k = 0; k < database->getTable(i)->getColumn(j)->getColumnSize(); k++){
                    if(printHeader){
                        headerBuffer = database->getTable(i)->getColumn(j)->getName();
                        mvwprintw(mainWindow, verticalIndex+k+2, horizontalIndex+3, headerBuffer.c_str());
                        printHeader = false;

                        std::string tableName = database->getTable(i)->getName();
                        for(unsigned int l = 0; l < tableName.length(); l++)
                            tableName[l] = std::toupper(tableName[l]);

                        mvwprintw(mainWindow, verticalIndex+k+2,
                                  getmaxx(mainWindow)-6-tableName.length(), tableName.c_str());
                    }

                    readBuffer = database->getTable(i)->getColumn(j)->streamPrint(k);
                    mvwprintw(mainWindow, verticalIndex+k+4, horizontalIndex+3, readBuffer.c_str());
                }
                horizontalIndex += database->getTable(i)->getLength(j) + 3;
            }
            for(unsigned int l = 0; l < getmaxx(mainWindow)-6; l++)
                lineHeader += "-";
            mvwprintw(mainWindow, verticalIndex+3, 3, lineHeader.c_str());
            verticalIndex += database->getTable(i)->getHeight() + 3;
            wrefresh(mainWindow);
        }
        std::string enterText = "(ENTER, aby wyjsc)";
        mvwprintw(mainWindow, getmaxy(mainWindow)-2, getmaxx(mainWindow)/2-(enterText.length()/2), enterText.c_str());
        wrefresh(mainWindow);

        if(getch() == 10)
            break;
    }
}

void Display::drawAddMenu()
{

    box(mainWindow, 0, 0);
    mvwprintw(mainWindow, 0, 3, "%s", choices::names[choices::ADD].c_str());

    wrefresh(mainWindow);
    getch();

}
