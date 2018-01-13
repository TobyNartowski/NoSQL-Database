#include "display.hpp"
#include "column.hpp"

#include <cstring>
#include <unistd.h>

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

    infoWindow = newwin(3, COLS-mainMenuSizeFactor, LINES - 5, 5);
    drawInfoWindow();

    errorWindow = newwin(11, COLS-48, LINES/2-6, 24);

    choiceWindow = newwin(11, COLS-48, LINES/2-6, 24);
    keypad(choiceWindow, true);
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

void Display::printMenuChoices(choices::choices_t whichMenu, unsigned int highlight)
{
    int x, y;
    getmaxyx(mainWindow, y, x);

    x = x/2 - 6;
    y = y/2 - ((choices::choices[whichMenu]).size()/2);

    drawBasicWindow(choices::names[whichMenu]);
    for(unsigned int i = 0; i < choices::choices[whichMenu].size(); i++){
        if(highlight == i){
            wattron(mainWindow, A_REVERSE);
            mvwprintw(mainWindow, y, x, "%s", choices::choices[whichMenu][i].c_str());
            wattroff(mainWindow, A_REVERSE);
        }
        else{
            mvwprintw(mainWindow, y, x, "%s", choices::choices[whichMenu][i].c_str());
        }
        y++;
    }
    wrefresh(mainWindow);
}

void Display::printMenuChoices(const std::string *menuChoices, unsigned int size,
                               std::string windowName, unsigned int highlight)
{
    int x, y;
    getmaxyx(mainWindow, y, x);

    x = x/2 - 6;
    y = y/2 - (size/2);

    drawBasicWindow(windowName);
    for(unsigned int i = 0; i < size; i++){
        if(highlight == i){
            wattron(mainWindow, A_REVERSE);
            mvwprintw(mainWindow, y, x, "%s", menuChoices[i].c_str());
            wattroff(mainWindow, A_REVERSE);
        }
        else{
            mvwprintw(mainWindow, y, x, "%s", menuChoices[i].c_str());
        }
        y++;
    }
    wrefresh(mainWindow);
}

int Display::drawMenu(choices::choices_t whichMenu)
{
    unsigned int highlight = 0;
    int choice = -1;
    int buffer;

    box(mainWindow, 0, 0);

    refresh();
    printMenuChoices(whichMenu, highlight);

    while(true){
        buffer = wgetch(mainWindow);
        switch(buffer){
            case KEY_LEFT:
                return -1;
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
            case KEY_RIGHT:
            case 10:
                choice = highlight;
                break;

            default:
                break;
        }
        printMenuChoices(whichMenu, highlight);

        if(choice != -1)
            break;
    }
    return choice;
}

int Display::drawMenu(const std::string menuChoices[], unsigned int size,
                               std::string windowName)
{

    unsigned int highlight = 0;
    int choice = -1;
    int buffer;

    box(mainWindow, 0, 0);

    refresh();
    printMenuChoices(menuChoices, size, windowName, highlight);

    while(true){
        buffer = wgetch(mainWindow);
        switch(buffer){
            case KEY_LEFT:
                return -1;
            case KEY_UP:
                if(highlight == 0)
                    highlight = size-1;
                else
                    highlight--;
                break;
            case KEY_DOWN:
                if(highlight == size-1)
                    highlight = 0;
                else
                    highlight++;
                break;
            case KEY_RIGHT:
            case 10:
                choice = highlight;
                break;
            default:
                break;
        }
        printMenuChoices(menuChoices, size, windowName, highlight);

        if(choice != -1)
            break;
    }
    return choice;
}

void Display::printHorizontalMenuChoices(WINDOW *window, const std::string *menuChoices,
                                         unsigned int size, unsigned int shift,
                                         unsigned int highlight)
{
    int totalLength = 0;
    int x = getmaxx(window);

    for(unsigned int i = 0; i < size; i++)
        totalLength += menuChoices[i].length();

    for(unsigned int i = 0, horizontal = 0; i < size; i++){
        if(highlight == i){
            wattron(window, A_REVERSE);
            mvwprintw(window, shift, (x/2-(totalLength/2)+horizontal), "%s", menuChoices[i].c_str());
            wattroff(window, A_REVERSE);
        }
        else{
            mvwprintw(window, shift, (x/2-(totalLength/2)+horizontal), "%s", menuChoices[i].c_str());
        }
        horizontal += menuChoices[i].length() + 1;

    }
    wrefresh(window);
}

int Display::drawHorizontalMenu(WINDOW *window, const std::string *menuChoices,
                                unsigned int size, unsigned int shift)
{
    unsigned int highlight = 0;
    int choice = -1;
    int buffer;

    printHorizontalMenuChoices(window, menuChoices, size, shift, highlight);

    while(true){
        buffer = wgetch(window);
        switch(buffer){
            case KEY_LEFT:
            case KEY_UP:
                if(highlight == 0)
                    highlight = size-1;
                else
                    highlight--;
                break;
            case KEY_RIGHT:
            case KEY_DOWN:
                if(highlight == size-1)
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
        printHorizontalMenuChoices(window, menuChoices, size, shift, highlight);

        if(choice != -1)
            break;
    }
    return choice;
}

void Display::printMenuSelectable(const std::string *menuChoices, unsigned int size,
                        unsigned int shift, unsigned int highlight,
                        bool *selected)
{
    int totalLength = 0;
    int x = getmaxx(mainWindow);

    for(unsigned int i = 0; i < size; i++)
        totalLength += menuChoices[i].length();

    for(unsigned int i = 0, horizontal = 0; i < size; i++){
        if(highlight == i){
            wattron(mainWindow, A_REVERSE);
            mvwprintw(mainWindow, shift, (x/2-(totalLength/2)+horizontal), "%s", menuChoices[i].c_str());
            wattroff(mainWindow, A_REVERSE);
        }
        else if(selected[i]){
            wattron(mainWindow, A_REVERSE);
            mvwprintw(mainWindow, shift, (x/2-(totalLength/2)+horizontal), "%s", menuChoices[i].c_str());
            wattroff(mainWindow, A_REVERSE);
        }
        else{
            mvwprintw(mainWindow, shift, (x/2-(totalLength/2)+horizontal), "%s", menuChoices[i].c_str());
        }
        horizontal += menuChoices[i].length() + 1;

    }
    wrefresh(mainWindow);
}

bool *Display::drawMenuSelectable(const std::string *menuChoices, unsigned int size,
                        unsigned int shift)
{
    unsigned int highlight = 0;
    int endSelecting = false;
    int buffer;
    bool *selected = new bool[size];

    for(unsigned int i = 0; i < size; i++)
        selected[i] = false;

    printMenuSelectable(menuChoices, size, shift, highlight, selected);

    while(true){
        buffer = wgetch(mainWindow);
        switch(buffer){
            case KEY_LEFT:
            case KEY_UP:
                if(highlight == 0)
                    highlight = size-1;
                else
                    highlight--;
                break;
            case KEY_RIGHT:
            case KEY_DOWN:
                if(highlight == size-1)
                    highlight = 0;
                else
                    highlight++;
                break;
            case ' ':
                selected[highlight] = !selected[highlight];
                break;
            case 10:
                endSelecting = true;
                break;
            default:
                break;
        }
        printMenuSelectable(menuChoices, size, shift, highlight, selected);

        if(endSelecting)
            break;
    }
    return selected;
}

void Display::drawBasicWindow(std::string windowName)
{
    wclear(mainWindow);
    box(mainWindow, 0, 0);
    mvwprintw(mainWindow, 0, 3, "%s", windowName.c_str());
    mvwprintw(mainWindow, 0, getmaxx(mainWindow)-3-strlen(database->getName().c_str()),
              "%s", database->getName().c_str());
}

void Display::drawInfoWindow()
{
    wclear(infoWindow);
    box(infoWindow, 0, 0);
    refresh();
    wrefresh(infoWindow);
}

void Display::drawErrorWindow(std::string errorMessage)
{
    std::string enterText = "(ENTER, aby pominac)";
    wclear(errorWindow);
    box(errorWindow, 0, 0);

    mvwprintw(errorWindow, getmaxy(errorWindow)/2,
            getmaxx(errorWindow)/2-(errorMessage.length()/2), errorMessage.c_str());
    mvwprintw(errorWindow, getmaxy(errorWindow)-2,
            getmaxx(errorWindow)/2-(enterText.length()/2), enterText.c_str());

    wrefresh(errorWindow);

    while(true){
        int getchBuffer = getch();
        if((getchBuffer == 10) || (getchBuffer == KEY_LEFT))
            break;
    }
    wclear(errorWindow);
}

bool Display::drawChoiceWindow(std::string choiceQuestion)
{
    const std::string choices[2] = {"TAK", "NIE"};
    int choice;
    wclear(choiceWindow);
    box(choiceWindow, 0, 0);

    if(choiceQuestion.length() >= (unsigned)getmaxx(choiceWindow)-2){
        std::string firstHalf, secondHalf;
        unsigned int cut = choiceQuestion.length()/2;
        while(choiceQuestion[cut] != ' ')
            cut++;

        firstHalf = choiceQuestion.substr(0, cut);
        secondHalf = choiceQuestion.substr(cut);
        mvwprintw(choiceWindow, getmaxy(choiceWindow)/3,
                     getmaxx(choiceWindow)/2-(firstHalf.length()/2), firstHalf.c_str());
        mvwprintw(choiceWindow, getmaxy(choiceWindow)/3+1,
                     getmaxx(choiceWindow)/2-(secondHalf.length()/2), secondHalf.c_str());
    }
    else{
        mvwprintw(choiceWindow, getmaxy(choiceWindow)/3+1,
                     getmaxx(choiceWindow)/2-(choiceQuestion.length()/2), choiceQuestion.c_str());
    }

    wrefresh(choiceWindow);
    choice = drawHorizontalMenu(choiceWindow, choices, 2, 2*getmaxy(choiceWindow)/3-1);

    wclear(choiceWindow);

    if(!choice)
        return true;
    else
        return false;
}

inline void Display::printInfo(std::string info)
{
    drawInfoWindow();
    mvwprintw(infoWindow, 1, 2, "%s", info.c_str());
    wrefresh(infoWindow);
}

void Display::startMainMenu()
{
    while(true){
        switch(drawMenu(choices::MAIN)){
            case choices::MAIN_WYSWIETL:
                drawDatabase();
                break;
            case choices::MAIN_DODAJ:
                drawAddMenu();
                break;
            case choices::MAIN_USUN:
                getch();
                break;
            case choices::MAIN_ZAPISZ:
                getch();
                break;
            case choices::MAIN_WCZYTAJ:
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
    drawBasicWindow("Wyswietl");

    while(true){
        if(!database->getDatabaseSize()) {
            mvwprintw(mainWindow, getmaxy(mainWindow)/2,
                      getmaxx(mainWindow)/2-strlen("Baza danych jest pusta!")/2, "Baza danych jest pusta!");
                      mvwprintw(mainWindow, getmaxy(mainWindow)-3,
                                getmaxx(mainWindow)/2-strlen("(Wcisnij RETURN, aby wyjsc)")/2, "(Wcisnij RETURN, aby wyjsc)");
            wrefresh(mainWindow);
            while(true){
                int getchBuffer = getch();
                if((getchBuffer == 10) || (getchBuffer == KEY_LEFT))
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

            if(database->getTable(i)->getTableSize() == 0){
                std::string tableName = database->getTable(i)->getName();
                for(unsigned int l = 0; l < tableName.length(); l++)
                    tableName[l] = std::toupper(tableName[l]);
                mvwprintw(mainWindow, verticalIndex+2,
                          getmaxx(mainWindow)-6-tableName.length(), tableName.c_str());
                wrefresh(mainWindow);
            }
            else{
                for(unsigned int j = 0; j < database->getTable(i)->getTableSize(); j++){
                    printHeader = true;
                    if(database->getTable(i)->getColumn(j)->getColumnSize() == 0){
                        headerBuffer = database->getTable(i)->getColumn(j)->getName();
                        if(database->getTable(i)->getColumn(j)->isPk())
                            headerBuffer += " (PK)";
                        if(database->getTable(i)->getColumn(j)->isFk())
                                headerBuffer += " (FK)";

                        mvwprintw(mainWindow, verticalIndex+2, horizontalIndex+3, headerBuffer.c_str());
                        printHeader = false;

                        std::string tableName = database->getTable(i)->getName();
                        for(unsigned int l = 0; l < tableName.length(); l++)
                            tableName[l] = std::toupper(tableName[l]);

                        mvwprintw(mainWindow, verticalIndex+2,
                                  getmaxx(mainWindow)-6-tableName.length(), tableName.c_str());

                        horizontalIndex += database->getTable(i)->getLength(j) + 2;
                    }
                    else{
                        for(unsigned int k = 0; k < database->getTable(i)->getColumn(j)->getColumnSize(); k++){
                            if(printHeader){
                                headerBuffer = database->getTable(i)->getColumn(j)->getName();
                                if(database->getTable(i)->getColumn(j)->isPk())
                                    headerBuffer += " (PK)";
                                if(database->getTable(i)->getColumn(j)->isFk())
                                        headerBuffer += " (FK)";

                                mvwprintw(mainWindow, verticalIndex+k+2, horizontalIndex+3, headerBuffer.c_str());
                                printHeader = false;

                                std::string tableName = database->getTable(i)->getName();
                                for(unsigned int l = 0; l < tableName.length(); l++)
                                    tableName[l] = std::toupper(tableName[l]);

                                mvwprintw(mainWindow, verticalIndex+2,
                                          getmaxx(mainWindow)-6-tableName.length(), tableName.c_str());
                            }

                            readBuffer = database->getTable(i)->getColumn(j)->streamPrint(k);
                            mvwprintw(mainWindow, verticalIndex+k+4, horizontalIndex+3, readBuffer.c_str());
                        }

                        horizontalIndex += database->getTable(i)->getLength(j) + 2;                    }
                }
            }
            for(int l = 0; l < getmaxx(mainWindow)-6; l++)
                lineHeader += "-";
            mvwprintw(mainWindow, verticalIndex+3, 3, lineHeader.c_str());
            verticalIndex += database->getTable(i)->getHeight() + 3;
            wrefresh(mainWindow);
        }
        std::string enterText = "(ENTER, aby wyjsc)";
        mvwprintw(mainWindow, getmaxy(mainWindow)-2, getmaxx(mainWindow)/2-(enterText.length()/2), enterText.c_str());
        wrefresh(mainWindow);

        int getchBuffer = getch();
        if((getchBuffer == 10) || (getchBuffer == KEY_LEFT))
            break;
    }
}

void Display::addTable()
{
    while(true){
        drawBasicWindow("Dodaj tabele");

        std::string message = "Podaj nazwe nowej tabeli";
        char inputBuffer[32];
        std::string convertBuffer;
        int maxx = getmaxx(mainWindow);
        int maxy = getmaxy(mainWindow);

        mvwprintw(mainWindow, maxy/2, (maxx-strlen(message.c_str()))/2+1, "%s", message.c_str());
        mvwprintw(mainWindow, maxy/2+1, (maxx/2+17), ":");
        mvwprintw(mainWindow, maxy/2+1, (maxx/2-17), "                                  ");
        mvwprintw(mainWindow, maxy/2+1, (maxx/2-16), ":");
        wrefresh(mainWindow);

        echo();
        curs_set(1);

        wgetnstr(mainWindow, inputBuffer, 32);

        noecho();
        curs_set(0);

        convertBuffer = inputBuffer;

        if(convertBuffer.empty()){
            drawErrorWindow("Nazwa tabeli nie moze byc pusta!");
            continue;
        }
        if(database->containsTable(convertBuffer)){
            drawErrorWindow("Tabela o podanej nazwie juz znajduje sie w bazie danych!");
            continue;
        }

        Table *table = new Table(convertBuffer);
        database->attachTableToDatabase(table);

        std::string infoBuffer = "Dodano tabele: ";
        infoBuffer += inputBuffer;
        printInfo(infoBuffer);

        sleep(1);
        break;
    }
}

bool Display::addColumn()
{
    if(database->empty()){
        drawErrorWindow("Baza danych jest pusta i nie posiada zadnych tabel!");
        return true;
    }

    int tableChoice, typeChoice;
    bool *attribChoices = new bool[4];

    std::string *names = new std::string[database->getDatabaseSize()+1];
    for(unsigned int i = 0; i < database->getDatabaseSize(); i++)
        names[i] = database->getTable(i)->getName();

    tableChoice = drawMenu(names, database->getDatabaseSize(), "Dodaj kolumne");
    if(tableChoice == -1)
        return false;

    while(true){
        std::string headerName = "Dodaj kolumne do ";
        headerName += database->getTable(tableChoice)->getName();

        drawBasicWindow(headerName);

        const std::string message = "Podaj nazwe nowej kolumny";
        const std::string typeMessage = "Wybierz typ danych kolumny";
        const std::string attribMessage = "Zaznacz odpowiednie atrybuty (SPACJA, aby zaznaczyc)";
        const std::string types[4] = {"BOOL", "INT", "DOUBLE", "TEKST"};
        const std::string attribs[4] = {"PK", "FK", "NOT NULL", "UNIQUE"};

        char inputBuffer[32];
        std::string convertBuffer;
        int maxx = getmaxx(mainWindow);
        int maxy = getmaxy(mainWindow);

        mvwprintw(mainWindow, maxy/3-1, (maxx-strlen(message.c_str()))/2+1, "%s", message.c_str());
        mvwprintw(mainWindow, maxy/3, (maxx/2+17), ":");
        mvwprintw(mainWindow, maxy/3, (maxx/2-17), "                                  ");
        mvwprintw(mainWindow, maxy/3, (maxx/2-16), ":");
        wrefresh(mainWindow);

        echo();
        curs_set(1);

        wgetnstr(mainWindow, inputBuffer, 32);
        convertBuffer = inputBuffer;

        noecho();
        curs_set(0);

        if(convertBuffer.empty()){
            drawErrorWindow("Nazwa kolumny nie moze byc pusta!");
            continue;
        }
        if(database->getTable(tableChoice)->containsColumn(convertBuffer)){
            drawErrorWindow("Kolumna o podanej nazwie juz znajduje sie w tabeli!");
            continue;
        }

        mvwprintw(mainWindow, maxy/3+4, (maxx-strlen(typeMessage.c_str()))/2+1, "%s", typeMessage.c_str());
        typeChoice = drawHorizontalMenu(mainWindow, types, 4, maxy/3+5);

        mvwprintw(mainWindow, maxy/3+9, (maxx-strlen(attribMessage.c_str()))/2+1, "%s", attribMessage.c_str());
        attribChoices = drawMenuSelectable(attribs, 4, maxy/3+10);

        if(database->getTable(tableChoice)->isAlreadyPk() && attribChoices[0]){
            drawErrorWindow("Tabela ma juz jeden klucz glowny!");
            continue;
        }

        std::string questionBuffer = "Czy na pewno chcesz dodac kolumne: ";
        questionBuffer += convertBuffer;
        questionBuffer += " <";
        questionBuffer += types[typeChoice];
        if((attribChoices[0] && attribChoices[1] && attribChoices[2] && attribChoices[3]))
            questionBuffer += "> -";
        else
            questionBuffer += ">";
        if(attribChoices[0])
                questionBuffer += " (PK)";
        if(attribChoices[1])
                questionBuffer += " (FK)";
        if(attribChoices[2])
                questionBuffer += " (NOT NULL)";
        if(attribChoices[3])
                questionBuffer += " (UNIQUE)";
        questionBuffer += "?";

        if(!drawChoiceWindow(questionBuffer))
            continue;

        switch(typeChoice){
            case 0:{
                Column<bool> *newColumnBool = new Column<bool>(convertBuffer, attribChoices);
                database->getTable(tableChoice)->attachColumnToTable(newColumnBool);
                break;
            }
            case 1:{
                Column<int> *newColumnInt = new Column<int>(convertBuffer, attribChoices);
                database->getTable(tableChoice)->attachColumnToTable(newColumnInt);
                break;
            }
            case 2:{
                Column<double> *newColumnDouble = new Column<double>(convertBuffer, attribChoices);
                database->getTable(tableChoice)->attachColumnToTable(newColumnDouble);
                break;
            }
            case 3:{
                Column<std::string> *newColumnString = new Column<std::string>(convertBuffer, attribChoices);
                database->getTable(tableChoice)->attachColumnToTable(newColumnString);
                break;
            }
            default:
                return true;
        }

        std::string infoBuffer = "Dodano kolumne: ";
        infoBuffer += inputBuffer;
        infoBuffer += " do tabeli ";
        infoBuffer += database->getTable(tableChoice)->getName();

        printInfo(infoBuffer);

        sleep(1);
        break;
    }
    return true;
}

bool Display::addRecord()
{
    return true;
}

void Display::drawAddMenu()
{
    while(true){
        drawBasicWindow("Dodaj");

        switch(drawMenu(choices::ADD)){
            case choices::ADD_TABELE:
                addTable();
                    return;
            case choices::ADD_KOLUMNE:
                if(addColumn())
                    return;
                break;
            case choices::ADD_REKORD:
                if(addRecord())
                    return;
                break;
            case -1:
            default:
                return;
        }
    }
}
