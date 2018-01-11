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

    mainMenuWindow = newwin(LINES-mainMenuSizeFactor, COLS-mainMenuSizeFactor,
        mainMenuSizeFactor/2, mainMenuSizeFactor/2);
    keypad(mainMenuWindow, true);

    drawDatabaseWindow = newwin(LINES-mainMenuSizeFactor, COLS-mainMenuSizeFactor,
        mainMenuSizeFactor/2, mainMenuSizeFactor/2);
    keypad(drawDatabaseWindow, true);
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

void Display::drawMainMenu()
{
    int highlight = 1;
    int choice = 0;
    int buffer;

    box(mainMenuWindow, 0, 0);

    refresh();
    printMenu(mainMenuWindow, "Menu glowne", highlight);

    while(true){
        buffer = wgetch(mainMenuWindow);
        switch(buffer){
            case KEY_UP:
                if(highlight == 1)
                    highlight = MENU_WYJDZ;
                else
                    highlight--;
                break;
            case KEY_DOWN:
                if(highlight == MENU_WYJDZ)
                    highlight = MENU_WYSWIETL;
                else
                    highlight++;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                refresh();
                break;
        }
        printMenu(mainMenuWindow, "Menu glowne", highlight);

        if(choice != 0)
            break;
    }

    switch(choice){
        case MENU_WYSWIETL:
            drawDatabase();
            break;
        case MENU_DODAJ:
            getch();
            break;
        case MENU_USUN:
            getch();
            break;
        case MENU_ZAPISZ:
            getch();
            break;
        case MENU_WCZYTAJ:
            getch();
            break;
        case MENU_WYJDZ:
        default:
            break;
    }
    clrtoeol();
    refresh();
}

void Display::printMenu(WINDOW *menuWindow, std::string windowName, int highlight)
{
    int x, y, xBuffer;
    getmaxyx(menuWindow, y, x);
    xBuffer = x;

    x = x/2 - 6;
    y = y/2 - (choicesNumber/2);

    box(menuWindow, 0, 0);
    mvwprintw(menuWindow, 0, 3, "%s", windowName.c_str());
    mvwprintw(menuWindow, 0, xBuffer-3-strlen(database->getName().c_str()),
              "%s", database->getName().c_str());
    for(int i = 0; i < choicesNumber; i++){
        if(highlight == i + 1){
            wattron(menuWindow, A_REVERSE);
            mvwprintw(menuWindow, y, x, "%s", menuChoices[i].c_str());
            wattroff(menuWindow, A_REVERSE);
        }
        else
            mvwprintw(menuWindow, y, x, "%s", menuChoices[i].c_str());
            y++;
    }
    wrefresh(menuWindow);
}

void Display::drawDatabase()
{
    int xBuffer = getmaxx(drawDatabaseWindow);

    box(drawDatabaseWindow, 0, 0);
    mvwprintw(drawDatabaseWindow, 0, 3, "%s", menuChoices[0].c_str());
    mvwprintw(drawDatabaseWindow, 0, xBuffer-3-strlen(database->getName().c_str()),
              "%s", database->getName().c_str());

    while(true){
        if(!database->getDatabaseSize()) {
            mvwprintw(drawDatabaseWindow, getmaxy(drawDatabaseWindow)/2,
                      getmaxx(drawDatabaseWindow)/2-strlen("Baza danych jest pusta!")/2, "Baza danych jest pusta!");
                      mvwprintw(drawDatabaseWindow, getmaxy(drawDatabaseWindow)-3,
                                getmaxx(drawDatabaseWindow)/2-strlen("(Wcisnij RETURN, aby wyjsc)")/2, "(Wcisnij RETURN, aby wyjsc)");
            wrefresh(drawDatabaseWindow);
            while(true){
                if(getch() == 10)
                    return;
            }
        }

        std::string readBuffer;
        unsigned int horizontalIndex;
        unsigned int verticalIndex = 0;

        for(int i = 0; i < database->getDatabaseSize(); i++){
            horizontalIndex = 0;
            for(int j = 0; j < database->getTable(i)->getTableSize(); j++){
                for(int k = 0; k < database->getTable(i)->getColumn(j)->getColumnSize(); k++){
                    readBuffer.clear();
                    readBuffer = database->getTable(i)->getColumn(j)->streamPrint(k);
                    mvwprintw(drawDatabaseWindow, verticalIndex+k+2, horizontalIndex+3, readBuffer.c_str());

                    wrefresh(drawDatabaseWindow);
                }
                horizontalIndex += database->getTable(i)->getLength(j) + 3;
            }
            verticalIndex += database->getTable(i)->getHeight() + 3;
        }
        if(getch() == 10)
            break;
    }
}
