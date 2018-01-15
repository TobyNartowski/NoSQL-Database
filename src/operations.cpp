#include "operations.hpp"
#include "windows.hpp"
#include "menu.hpp"

#include <iostream>
#include <cstring>
#include <ncurses.h>
#include <unistd.h>

//Implementacje metod
void Operations::drawDatabase(Database *database)
{
    Windows::drawBasicWindow(database->getName(), "Wyswietl");

    while(true){
        if(!database->getDatabaseSize()) {
            mvwprintw(Windows::mainWindow, getmaxy(Windows::mainWindow)/2,
                      getmaxx(Windows::mainWindow)/2-strlen("Baza danych jest pusta!")/2, "Baza danych jest pusta!");
                      mvwprintw(Windows::mainWindow, getmaxy(Windows::mainWindow)-3,
                                getmaxx(Windows::mainWindow)/2-strlen("(Wcisnij RETURN, aby wyjsc)")/2, "(Wcisnij RETURN, aby wyjsc)");
            wrefresh(Windows::mainWindow);
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
                mvwprintw(Windows::mainWindow, verticalIndex+2,
                          getmaxx(Windows::mainWindow)-6-tableName.length(), tableName.c_str());
                wrefresh(Windows::mainWindow);
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

                        mvwprintw(Windows::mainWindow, verticalIndex+2, horizontalIndex+3, headerBuffer.c_str());
                        printHeader = false;

                        std::string tableName = database->getTable(i)->getName();
                        for(unsigned int l = 0; l < tableName.length(); l++)
                            tableName[l] = std::toupper(tableName[l]);

                        mvwprintw(Windows::mainWindow, verticalIndex+2,
                                  getmaxx(Windows::mainWindow)-6-tableName.length(), tableName.c_str());

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

                                mvwprintw(Windows::mainWindow, verticalIndex+k+2, horizontalIndex+3, headerBuffer.c_str());
                                printHeader = false;

                                std::string tableName = database->getTable(i)->getName();
                                for(unsigned int l = 0; l < tableName.length(); l++)
                                    tableName[l] = std::toupper(tableName[l]);

                                mvwprintw(Windows::mainWindow, verticalIndex+2,
                                          getmaxx(Windows::mainWindow)-6-tableName.length(), tableName.c_str());
                            }

                            readBuffer = database->getTable(i)->getColumn(j)->streamPrint(k);
                            mvwprintw(Windows::mainWindow, verticalIndex+k+4, horizontalIndex+3, readBuffer.c_str());
                        }

                        horizontalIndex += database->getTable(i)->getLength(j) + 2;                    }
                }
            }
            for(int l = 0; l < getmaxx(Windows::mainWindow)-6; l++)
                lineHeader += "-";
            mvwprintw(Windows::mainWindow, verticalIndex+3, 3, lineHeader.c_str());
            verticalIndex += database->getTable(i)->getHeight() + 3;
            wrefresh(Windows::mainWindow);
        }
        std::string enterText = "(ENTER, aby wyjsc)";
        mvwprintw(Windows::mainWindow, getmaxy(Windows::mainWindow)-2, getmaxx(Windows::mainWindow)/2-(enterText.length()/2), enterText.c_str());
        wrefresh(Windows::mainWindow);

        int getchBuffer = getch();
        if((getchBuffer == 10) || (getchBuffer == KEY_LEFT))
            break;
    }
}

void Operations::drawAddMenu(Database *database)
{
    while(true){
        Windows::drawBasicWindow(database->getName(), "Dodaj");

        switch(Menu::drawMenu(Windows::mainWindow, Choices::ADD, database->getName())){
            case Choices::ADD_TABELE:
                addTable(database);
                    return;
            case Choices::ADD_KOLUMNE:
                if(addColumn(database))
                    return;
                break;
            case Choices::ADD_REKORD:
                if(addRecord(database))
                    return;
                break;
            case -1:
            default:
                return;
        }
    }
}

void Operations::addTable(Database *database)
{
    while(true){
        Windows::drawBasicWindow(database->getName(), "Dodaj tabele");

        std::string message = "Podaj nazwe nowej tabeli";
        char inputBuffer[32];
        std::string convertBuffer;
        int maxx = getmaxx(Windows::mainWindow);
        int maxy = getmaxy(Windows::mainWindow);

        mvwprintw(Windows::mainWindow, maxy/2, (maxx-strlen(message.c_str()))/2+1, "%s", message.c_str());
        mvwprintw(Windows::mainWindow, maxy/2+1, (maxx/2+17), ":");
        mvwprintw(Windows::mainWindow, maxy/2+1, (maxx/2-17), "                                  ");
        mvwprintw(Windows::mainWindow, maxy/2+1, (maxx/2-16), ":");
        wrefresh(Windows::mainWindow);

        echo();
        curs_set(1);

        wgetnstr(Windows::mainWindow, inputBuffer, 32);

        noecho();
        curs_set(0);

        convertBuffer = inputBuffer;

        if(convertBuffer.empty()){
            Windows::drawErrorWindow("Nazwa tabeli nie moze byc pusta!");
            continue;
        }
        if(database->containsTable(convertBuffer)){
            Windows::drawErrorWindow("Tabela o podanej nazwie juz znajduje sie w bazie danych!");
            continue;
        }

        Table *table = new Table(convertBuffer);
        database->attachTableToDatabase(table);

        std::string infoBuffer = "Dodano tabele: ";
        infoBuffer += inputBuffer;
        Windows::printInfo(infoBuffer);

        sleep(1);
        break;
    }
}

bool Operations::addColumn(Database *database)
{
    if(database->empty()){
        Windows::drawErrorWindow("Baza danych jest pusta i nie posiada zadnych tabel!");
        return true;
    }

    int tableChoice, typeChoice;
    bool *attribChoices = new bool[4];

    std::string *names = new std::string[database->getDatabaseSize()+1];
    for(unsigned int i = 0; i < database->getDatabaseSize(); i++)
        names[i] = database->getTable(i)->getName();

    tableChoice = Menu::drawMenu(Windows::mainWindow, names, database->getDatabaseSize(),
    "Dodaj kolumne", database->getName());
    if(tableChoice == -1)
        return false;

    while(true){
        std::string headerName = "Dodaj kolumne do ";
        headerName += database->getTable(tableChoice)->getName();

        Windows::drawBasicWindow(database->getName(), headerName);

        const std::string message = "Podaj nazwe nowej kolumny";
        const std::string typeMessage = "Wybierz typ danych kolumny";
        const std::string attribMessage = "Zaznacz odpowiednie atrybuty (SPACJA, aby zaznaczyc)";
        const std::string types[4] = {"BOOL", "INT", "DOUBLE", "TEKST"};
        const std::string attribs[4] = {"PK", "FK", "NOT NULL", "UNIQUE"};

        char inputBuffer[32];
        std::string convertBuffer;
        int maxx = getmaxx(Windows::mainWindow);
        int maxy = getmaxy(Windows::mainWindow);

        mvwprintw(Windows::mainWindow, maxy/3-1, (maxx-strlen(message.c_str()))/2+1, "%s", message.c_str());
        mvwprintw(Windows::mainWindow, maxy/3, (maxx/2+17), ":");
        mvwprintw(Windows::mainWindow, maxy/3, (maxx/2-17), "                                  ");
        mvwprintw(Windows::mainWindow, maxy/3, (maxx/2-16), ":");
        wrefresh(Windows::mainWindow);

        echo();
        curs_set(1);

        wgetnstr(Windows::mainWindow, inputBuffer, 32);
        convertBuffer = inputBuffer;

        noecho();
        curs_set(0);

        if(convertBuffer.empty()){
            Windows::drawErrorWindow("Nazwa kolumny nie moze byc pusta!");
            continue;
        }
        if(database->getTable(tableChoice)->containsColumn(convertBuffer)){
            Windows::drawErrorWindow("Kolumna o podanej nazwie juz znajduje sie w tabeli!");
            continue;
        }

        mvwprintw(Windows::mainWindow, maxy/3+4, (maxx-strlen(typeMessage.c_str()))/2+1, "%s", typeMessage.c_str());
        typeChoice = Menu::drawHorizontalMenu(Windows::mainWindow, types, 4, maxy/3+5);

        mvwprintw(Windows::mainWindow, maxy/3+9, (maxx-strlen(attribMessage.c_str()))/2+1, "%s", attribMessage.c_str());
        attribChoices = Menu::drawMenuSelectable(Windows::mainWindow, attribs, 4, maxy/3+10);

        if(database->getTable(tableChoice)->isAlreadyPk() && attribChoices[0]){
            Windows::drawErrorWindow("Tabela ma juz jeden klucz glowny!");
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

        if(!Windows::drawChoiceWindow(questionBuffer))
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

        Windows::printInfo(infoBuffer);

        sleep(1);
        break;
    }
    return true;
}

bool Operations::addRecord(Database *database)
{
    return true;
}
