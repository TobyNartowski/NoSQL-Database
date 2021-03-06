#include "operations.hpp"
#include "windows.hpp"
#include "menu.hpp"
#include "column.hpp"

#include <algorithm>
#include <cstring>
#include <sstream>

//Implementacje metod
void Operations::drawDatabase(Database *database)
{
    while(true){
        Windows::drawBasicWindow(database->getName(), "Wyswietl baze danych");

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

        for(unsigned int i = 0; i < database->getDatabaseSize(); i++)
            database->getTable(i)->alignColumns();

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
        std::string enterText = "(ENTER aby wyjsc; > aby wyswietlic klucze)";
        mvwprintw(Windows::mainWindow, getmaxy(Windows::mainWindow)-2, getmaxx(Windows::mainWindow)/2-(enterText.length()/2), enterText.c_str());
        wrefresh(Windows::mainWindow);

        int getchBuffer = getch();
        if(getchBuffer == KEY_RIGHT){
            if(drawDatabaseKeys(database)){
                wclear(Windows::mainWindow);
                continue;
            }
            else
                break;
        }
        if((getchBuffer == 10) || (getchBuffer == KEY_LEFT))
            break;
    }
}

bool Operations::drawDatabaseKeys(Database *database)
{
    Windows::drawBasicWindow(database->getName(), "Wyswietl klucze");

    while(true){
        std::string enterText = "(ENTER aby wyjsc; < aby wyswietlic baze danych)";
        mvwprintw(Windows::mainWindow, getmaxy(Windows::mainWindow)-2, getmaxx(Windows::mainWindow)/2-(enterText.length()/2), enterText.c_str());

        if(database->keysEmpty()){
            std::string noConnectionsText = "Brak polaczen w bazie danych";
            Windows::drawErrorWindow(noConnectionsText);
            return true;
        }

        std::stringstream streamBuffer(database->getKeysString().c_str());
        std::string textBuffer;
        int i = 0;

        while(std::getline(streamBuffer, textBuffer, '\n')){
            i++;
            mvwprintw(Windows::mainWindow, getmaxy(Windows::mainWindow)/3+i, getmaxx(Windows::mainWindow)/2-(textBuffer.length()/2), textBuffer.c_str());
        }

        wrefresh(Windows::mainWindow);

        int getchBuffer = getch();
        if(getchBuffer == KEY_LEFT)
            return true;
        if(getchBuffer == 10)
            return false;
    }
    return true;
}

void Operations::drawAddMenu(Database *database)
{
    while(true){
        Windows::drawBasicWindow(database->getName(), "Dodaj");

        switch(Menu::drawMenu(Windows::mainWindow, Choices::ADD, database->getName())){
            case Choices::ADD_TABELE:
                if(addTable(database))
                    return;
                break;
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

bool Operations::addTable(Database *database)
{
    while(true){
        Windows::drawBasicWindow(database->getName(), "Dodaj tabele");

        std::string message = "Podaj nazwe nowej tabeli";

        std::string stringBuffer = Windows::drawInputWindow(Windows::mainWindow, message, getmaxy(Windows::mainWindow)/2);

        if(stringBuffer.empty()){
            Windows::drawErrorWindow("Nazwa tabeli nie moze byc pusta!");
            continue;
        }
        if(database->containsTable(stringBuffer)){
            Windows::drawErrorWindow("Tabela o podanej nazwie juz znajduje sie w bazie danych!");
            continue;
        }

        std::string questionBuffer = "Czy na pewno chcesz dodac tabele: ";
        questionBuffer += stringBuffer;
        questionBuffer += "?";
        if(!Windows::drawChoiceWindow(questionBuffer))
            return false;

        Table *table = new Table(stringBuffer);
        database->attachTableToDatabase(table);

        std::string infoBuffer = "Dodano tabele: ";
        infoBuffer += stringBuffer;
        Windows::printInfo(infoBuffer);

        return true;
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
        const int maxx = getmaxx(Windows::mainWindow);
        const int maxy = getmaxy(Windows::mainWindow);

        std::string stringBuffer = Windows::drawInputWindow(Windows::mainWindow, message, getmaxy(Windows::mainWindow)/3-1);

        if(stringBuffer.empty()){
            Windows::drawErrorWindow("Nazwa kolumny nie moze byc pusta!");
            continue;
        }
        if(database->getTable(tableChoice)->containsColumn(stringBuffer)){
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

        if(attribChoices[0] && (typeChoice == 0)){
            Windows::drawErrorWindow("Typ BOOL nie moze byc kluczem glownym");
            continue;
        }

        if(attribChoices[0] && (typeChoice == 3)){
            Windows::drawErrorWindow("Typ STRING nie moze byc kluczem glownym");
            continue;
        }

        if(attribChoices[2] && database->getTable(tableChoice)->getHeight()){
            Windows::drawErrorWindow("Kolumna nie moze byc pusta, brak mozliwosci wyrownania tabel!");
            continue;
        }

        if(attribChoices[0] && database->getTable(tableChoice)->getHeight()){
            Windows::drawErrorWindow("Brak mozliwosci dodania klucza glownego!");
            continue;
        }

        std::string questionBuffer = "Czy na pewno chcesz dodac kolumne: ";
        questionBuffer += stringBuffer;
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
            return false;

        switch(typeChoice){
            case 0:{
                Column<bool> *newColumnBool = new Column<bool>(stringBuffer, attribChoices);
                database->getTable(tableChoice)->attachColumnToTable(newColumnBool);
                break;
            }
            case 1:{
                Column<int> *newColumnInt = new Column<int>(stringBuffer, attribChoices);
                database->getTable(tableChoice)->attachColumnToTable(newColumnInt);
                break;
            }
            case 2:{
                Column<double> *newColumnDouble = new Column<double>(stringBuffer, attribChoices);
                database->getTable(tableChoice)->attachColumnToTable(newColumnDouble);
                break;
            }
            case 3:{
                Column<std::string> *newColumnString = new Column<std::string>(stringBuffer, attribChoices);
                database->getTable(tableChoice)->attachColumnToTable(newColumnString);
                break;
            }
            default:
                return true;
        }

        std::string infoBuffer = "Dodano kolumne: ";
        infoBuffer += stringBuffer;
        infoBuffer += " do tabeli ";
        infoBuffer += database->getTable(tableChoice)->getName();

        Windows::printInfo(infoBuffer);

        break;
    }
    return true;
}

bool Operations::addRecord(Database *database)
{
    if(database->empty()){
        Windows::drawErrorWindow("Baza danych jest pusta!");
        return false;
    }

    ColumnHandler **columnHandlers;
    Column<bool> *boolColumn;
    Column<int> *intColumn;
    Column<double> *doubleColumn;
    Column<std::string> *stringColumn;

    int tableChoice;
    std::string columnNameBuffer;
    std::string *choiceBuffers = nullptr;
    ColumnType *columnTypes = nullptr;

    std::string *tableNames = new std::string[database->getDatabaseSize()+1];
    for(unsigned int i = 0; i < database->getDatabaseSize(); i++)
        tableNames[i] = database->getTable(i)->getName();

    tableChoice = Menu::drawMenu(Windows::mainWindow, tableNames, database->getDatabaseSize(),
                                     "Dodaj rekord", database->getName());
    if(tableChoice == -1)
        return false;

    if(database->getTable(tableChoice)->getTableSize() == 0){
        Windows::drawErrorWindow("Tabela nie ma zadnych kolumn!");
        return true;
    }

    std::string *columnNames = new std::string[database->getTable(tableChoice)->getTableSize()+1];
    for(unsigned int i = 0; i < database->getTable(tableChoice)->getTableSize(); i++){
        columnNames[i] = database->getTable(tableChoice)->getColumn(i)->getName();
    }

    std::string headerName = "Dodaj rekord do ";
    headerName += database->getTable(tableChoice)->getName();

    columnNames = new std::string[database->getTable(tableChoice)->getTableSize()+1];
    columnTypes = new ColumnType[database->getTable(tableChoice)->getTableSize()+1];
    columnHandlers = new ColumnHandler*[database->getTable(tableChoice)->getTableSize()+1];
    choiceBuffers = new std::string[database->getTable(tableChoice)->getTableSize()+1];

    Windows::drawBasicWindow(database->getName(), headerName);
    for(unsigned int i = 0; i < database->getTable(tableChoice)->getTableSize(); i++){
        bool pkFlag, nullableFlag;
        pkFlag = false;
        nullableFlag = true;

        columnNameBuffer = database->getTable(tableChoice)->getColumn(i)->getName();
        columnTypes[i] = database->getTable(tableChoice)->getColumn(i)->whatType();

        pkFlag = database->getTable(tableChoice)->getColumn(i)->isPk();
        nullableFlag = database->getTable(tableChoice)->getColumn(i)->isNullable();

        columnHandlers[i] = database->getTable(tableChoice)->getColumn(i);

        if(pkFlag){
            choiceBuffers[i] = Windows::drawInputWindow(Windows::mainWindow, columnNameBuffer, i*3+2,
                                                    std::to_string(database->getTable(tableChoice)->getColumn(i)->getColumnSize()+1));
        }
        else
            choiceBuffers[i] = Windows::drawInputWindow(Windows::mainWindow, columnNameBuffer, i*3+2);

        if(choiceBuffers[i].empty() && !nullableFlag){
            Windows::drawErrorWindow("Pole nie moze byc puste!");
            return false;
        }

        if(!choiceBuffers[i].empty() && nullableFlag){
            if(columnTypes[i] == COL_INT){
                try{
                    std::stoi(choiceBuffers[i]);
                }
                catch(...){
                    Windows::drawErrorWindow("Niepoprawny typ danych!");
                    return false;
                }
            }

            if(columnTypes[i] == COL_DOUBLE){
                try{
                    std::stod(choiceBuffers[i]);
                }
                catch(...){
                    Windows::drawErrorWindow("Niepoprawny typ danych!");
                    return false;
                }
            }
        }
    }

    if(database->getTable(tableChoice)->alignColumns()){
            if(!Windows::drawChoiceWindow("Czy na pewno chcesz dodac nowy rekord?"))
                return false;
            std::string infoBuffer = "Dodano rekord do tabeli ";
            infoBuffer += database->getTable(tableChoice)->getName();
            Windows::printInfo(infoBuffer);
    }
    else{
        Windows::drawErrorWindow("Blad przy dodawaniu rekordu!");
        return true;
    }

    for(unsigned int i = 0; i < database->getTable(tableChoice)->getTableSize(); i++){
        boolColumn = nullptr;
        intColumn = nullptr;
        doubleColumn = nullptr;
        stringColumn = nullptr;

        switch(columnTypes[i]){
            case COL_BOOL:
                boolColumn = dynamic_cast<Column<bool> *>(columnHandlers[i]);
                std::transform(choiceBuffers[i].begin(), choiceBuffers[i].end(), choiceBuffers[i].begin(), ::tolower);
                if(choiceBuffers[i] == "")
                    boolColumn->addNullValue();
                else{
                    if(choiceBuffers[i] == "tak" || choiceBuffers[i] == "1")
                        boolColumn->addValue(true);
                        else
                        boolColumn->addValue(false);
                    }
                break;
            case COL_INT:
                intColumn = dynamic_cast<Column<int> *>(columnHandlers[i]);
                if(choiceBuffers[i] == "")
                    intColumn->addNullValue();
                else
                    intColumn->addValue(std::stoi(choiceBuffers[i]));
                break;
            case COL_DOUBLE:
                doubleColumn = dynamic_cast<Column<double> *>(columnHandlers[i]);
                if(choiceBuffers[i] == "")
                    doubleColumn->addNullValue();
                else
                    doubleColumn->addValue(std::stod(choiceBuffers[i]));
                break;
            case COL_STRING:
                stringColumn = dynamic_cast<Column<std::string> *>(columnHandlers[i]);
                if(choiceBuffers[i] == "")
                    stringColumn->addNullValue();
                else
                    stringColumn->addValue(choiceBuffers[i]);
                break;
            case COL_ERROR:
            default:
                return true;
        }
    }

    database->getTable(tableChoice)->alignColumns();
    return true;
}

void Operations::drawDeleteMenu(Database *database)
{
    while(true){
        Windows::drawBasicWindow(database->getName(), "Usun");

        switch(Menu::drawMenu(Windows::mainWindow, Choices::DELETE, database->getName())){
            case Choices::DELETE_TABELE:
                if(deleteTable(database))
                    return;
                break;
            case Choices::DELETE_KOLUMNE:
                if(deleteColumn(database))
                    return;
                break;
            case Choices::CLEAR_DATABASE:
                if(clearDatabase(database))
                    return;
                break;
            case -1:
            default:
                return;
        }
    }
}

bool Operations::deleteTable(Database *database)
{
    if(database->empty()){
        Windows::drawErrorWindow("Baza danych jest pusta!");
        return false;
    }

    int tableChoice;

    std::string *tableNames = new std::string[database->getDatabaseSize()+1];
    for(unsigned int i = 0; i < database->getDatabaseSize(); i++)
        tableNames[i] = database->getTable(i)->getName();

    tableChoice = Menu::drawMenu(Windows::mainWindow, tableNames, database->getDatabaseSize(),
                                 "Usun tabele", database->getName());
    if(tableChoice == -1)
        return false;

    std::string choiceString = "Czy na pewno chcesz usunac tabele ";
    choiceString += database->getTable(tableChoice)->getName();
    choiceString += "?";

    if(Windows::drawChoiceWindow(choiceString)){
        database->detachTableFromDatabase(tableChoice);
        Windows::drawErrorWindow("Tabela zostala usunieta");

        std::string infoString = "Usunieto tabele: ";
        infoString += database->getTable(tableChoice)->getName();
        Windows::printInfo(infoString);
        return true;
    }
    return false;
}

bool Operations::deleteColumn(Database *database)
{
    if(database->empty()){
        Windows::drawErrorWindow("Baza danych jest pusta!");
        return false;
    }

    int tableChoice, columnChoice;

    std::string *tableNames = new std::string[database->getDatabaseSize()+1];
    for(unsigned int i = 0; i < database->getDatabaseSize(); i++)
        tableNames[i] = database->getTable(i)->getName();
    tableChoice = Menu::drawMenu(Windows::mainWindow, tableNames, database->getDatabaseSize(),
                                 "Usun kolumne (Wybierz tabele)", database->getName());
    if(tableChoice == -1)
        return false;

    if(!database->getTable(tableChoice)->getTableSize()){
        Windows::drawErrorWindow("Tabela jest pusta!");
        return false;
    }

    std::string *columnNames = new std::string[database->getTable(tableChoice)->getTableSize()+1];
    for(unsigned int i = 0; i < database->getTable(tableChoice)->getTableSize(); i++)
        columnNames[i] = database->getTable(tableChoice)->getColumn(i)->getName();

    columnChoice = Menu::drawMenu(Windows::mainWindow, columnNames, database->getTable(tableChoice)->getTableSize(),
                                 "Usun kolumne (Wybierz kolumne)", database->getName());
    if(columnChoice == -1)
        return false;

    std::string choiceString = "Czy na pewno chcesz usunac kolumne ";
    choiceString += database->getTable(tableChoice)->getColumn(columnChoice)->getName();
    choiceString += "?";

    if(Windows::drawChoiceWindow(choiceString)){
        database->getTable(tableChoice)->detachColumnFromTable(columnChoice);
        Windows::drawErrorWindow("Kolumna zostala usunieta");

        std::string infoString = "Usunieto kolumne: ";
        infoString += database->getTable(tableChoice)->getColumn(columnChoice)->getName();
        Windows::printInfo(infoString);
        return true;
    }
    return false;
}

bool Operations::clearDatabase(Database *database)
{
    if(database->empty()){
        Windows::drawErrorWindow("Baza danych aktualnie jest juz pusta!");
        return false;
    }

    if(Windows::drawChoiceWindow("Czy na pewno chcesz wyczyscic baze danych?")){
        std::string nameBuffer = database->getName();
        database->clearDatabase();
        database->setName(nameBuffer);
        Windows::drawErrorWindow("Baza danych zostala wyczyszczona!");
        Windows::printInfo("Wyczyszczono baze danych");
        return true;
    }
    return false;
}

void Operations::loadDatabase(Database *database)
{
    std::string time;
    if(!database->empty())
        if(!Windows::drawChoiceWindow("Wczytanie bazy danych spowoduje nadpisanie aktualnej, Czy na pewno chcesz to zrobic?"))
            return;

    if(database->loadDatabase(time)){
        Windows::drawErrorWindow("Wczytano baze danych z: " + time);
        Windows::printInfo("Wczytano baze danych");
    }
    else
        Windows::drawErrorWindow("Nie udalo sie wczytac bazy danych!");
}
void Operations::saveDatabase(Database *database)
{
    if(database->empty()){
        Windows::drawErrorWindow("Baza danych jest pusta!");
        return;
    }

    if(!Windows::drawChoiceWindow("Czy na pewno chcesz zapisac baze danych?"))
        return;

    database->saveDatabase();
    Windows::drawErrorWindow("Zapisano baze danych do pliku lokalnego");
    Windows::printInfo("Zapisano baze danych");
}
