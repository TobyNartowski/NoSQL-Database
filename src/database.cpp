#include "database.hpp"

#include <ctime>
#include <sstream>

//Implementacje metod
Database::Database(std::string nameOfDatabase, std::string databaseFilename)
{
    this->nameOfDatabase = nameOfDatabase;
    this->databaseFilename = databaseFilename;
}

std::string Database::getName()
{
    return nameOfDatabase;
}

unsigned int Database::getDatabaseSize()
{
    return tables.size();
}

void Database::clearDatabase()
{
    nameOfDatabase = "";
    tables.clear();
    keys.clear();
}

bool Database::attachTableToDatabase(Table *table)
{
    for(unsigned int i = 0; i < tables.size(); i++)
        if(tables[i]->getName() == table->getName())
            return false;
    tables.push_back(table);
    return true;
}

void Database::detachTableFromDatabase(std::string nameOfTable)
{
    for(unsigned int i = 0; i < tables.size(); i++)
        if(tables[i]->getName() == nameOfTable){
            tables.erase(tables.begin() + i);
            return;
        }
    std::cout << "Nie znaleziono tabeli o podanej nazwie" << std::endl;
    return;
}

void Database::printDatabase()
{
    if(tables.empty()){
        std::cout << "Baza danych jest pusta!" << std::endl;
        return;
    }

    std::cout << "==== " << nameOfDatabase << " ====" << std::endl;
    for(unsigned int i = 0; i < tables.size(); i++){
        tables[i]->printTable();
        std::cout << std::endl;
    }
}

void Database::setPk(ColumnHandler *pkColumn)
{
    for(unsigned int i = 0; i < keys.size(); i++){
        if(keys[i].second == NULL)
            continue;
        if(keys[i].second->getName() == pkColumn->getName()){
            keys[i].first = pkColumn;
            return;
        }
    }
    keys.push_back(std::make_pair(pkColumn, (ColumnHandler *)NULL));
}

void Database::setFk(ColumnHandler *fkColumn)
{
    for(unsigned int i = 0; i < keys.size(); i++){
        if(keys[i].first == NULL)
            continue;
        if(keys[i].first->getName() == fkColumn->getName()){
            keys[i].second = fkColumn;
            return;
        }
    }
    keys.push_back(std::make_pair((ColumnHandler *)NULL, fkColumn));
}

void Database::printKeys()
{
    std::cout << "Polaczenia:" << std::endl;
    for(unsigned int i = 0; i < keys.size(); i++){
        std::cout << "PK: ";
        if(keys[i].first == NULL)
            std::cout << "NULL";
        else
            std::cout << "(" << keys[i].first->getTableName() << ")" << keys[i].first->getName();
        std::cout << " --> Fk: ";
        if(keys[i].second == NULL)
            std::cout << "NULL";
        else
            std::cout << "(" << keys[i].second->getTableName() << ")" << keys[i].second->getName();
        std::cout << std::endl;
    }
}

Table *Database::getTable(std::string nameOfTable)
{
    for(unsigned int i = 0; i < tables.size(); i++)
        if(tables[i]->getName() == nameOfTable)
            return tables[i];
    std::cout << "Nie znaleziono tabeli o nazwie \"" << nameOfTable << "\"" << std::endl;
    return NULL;
}

Table *Database::getTable(unsigned int index)
{
    if(index > tables.size())
        return NULL;
    return tables[index];
}

bool Database::containsTable(std::string nameOfTable)
{
    std::string lowercaseName = nameOfTable;

    for(unsigned int i = 0; i < nameOfTable.length(); i++)
        lowercaseName[i] = std::tolower(nameOfTable[i]);

    for(unsigned int i = 0; i < tables.size(); i++){
        std::string lowercaseNameBuffer = tables[i]->getName();
        for(unsigned int j = 0; j < (tables[i]->getName()).length(); j++)
            lowercaseNameBuffer[j] = std::tolower(tables[i]->getName()[j]);
        if(lowercaseNameBuffer == lowercaseName)
            return true;
    }
    return false;
}

bool Database::empty()
{
    if(!tables.size())
        return true;
    return false;
}

void Database::saveDatabase()
{
    localFile.open(databaseFilename.c_str(), std::fstream::out | std::fstream::trunc);

    if(!localFile.good()){
        std::cerr << "Blad przy otwieraniu pliku lokalnego" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::time_t localTime = std::time(nullptr);
    localFile << "* " << std::asctime(std::localtime(&localTime)) << std::endl << std::endl;

    localFile << "!" << getName() << std::endl;
    for(unsigned int i = 0; i < tables.size(); i++){
    localFile << "  #" << tables[i]->getName() << std::endl;
        for(unsigned int j = 0; j < tables[i]->getTableSize(); j++){
            localFile << "\t$" << tables[i]->getColumn(j)->getName() << std::endl;

            localFile << "\t" << "@";
            switch(tables[i]->getColumn(j)->whatType()){
                case COL_BOOL:
                    localFile << "BOOL";
                    break;
                case COL_INT:
                    localFile << "INT";
                    break;
                case COL_DOUBLE:
                    localFile << "DOUBLE";
                    break;
                case COL_STRING:
                    localFile << "STRING";
                    break;
                default:
                    return;
            }

            localFile << std::endl << "\t^ ";
            if(tables[i]->getColumn(j)->isPk())
                localFile << "pk ";
            if(tables[i]->getColumn(j)->isFk())
                localFile << "fk ";
            if(tables[i]->getColumn(j)->isNullable())
                localFile << "nullable ";
            if(tables[i]->getColumn(j)->isUnique())
                localFile << "unique ";

            localFile << " ; " << std::endl << "\t\t";
            for(unsigned int k = 0; k < tables[i]->getColumn(j)->getColumnSize(); k++){
                localFile << tables[i]->getColumn(j)->streamPrint(k, true);
                localFile << " ";
            }
            localFile << ";" << std::endl;
        }
    }
    std::cout << "Zapisano baze danych!" << std::endl;
    localFile.close();
}

bool Database::loadDatabase()
{
    std::ifstream readFile;
    std::string buffer, columnNameBuffer, tableNameBuffer, typeBuffer;
    bool pkFlag, fkFlag, nullableFlag, uniqueFlag;
    bool databaseFileReaded = false;
    std::string date;

    Column<bool> *boolColumn;
    Column<int> *intColumn;
    Column<double> *doubleColumn;
    Column<std::string> *stringColumn;

    Table *tableBuffer;

    readFile.open(databaseFilename.c_str());
    if(!readFile.good())
        return false;

    clearDatabase();

    readFile >> buffer;
    if(buffer[0] == '*'){
        readFile >> buffer;
        while(buffer[0] != '!'){
            date += buffer + " ";
            readFile >> buffer;
        }
    }
    else
        return false;

    if(!databaseFileReaded)
        if(buffer[0] == '!'){
            nameOfDatabase = buffer.substr(1);
            databaseFileReaded = true;
        }
    readFile >> buffer;

    while(!readFile.eof()){
        tableBuffer = nullptr;

        if(buffer[0] == '#'){
            tableNameBuffer = buffer.substr(1);
            tableBuffer = new Table(tableNameBuffer);

            readFile >> buffer;
            while(buffer[0] == '$'){
                    pkFlag = fkFlag = nullableFlag = uniqueFlag = false;
                    boolColumn = nullptr;
                    intColumn = nullptr;
                    doubleColumn = nullptr;
                    stringColumn = nullptr;

                    columnNameBuffer = buffer.substr(1);
                    readFile >> buffer;
                    if(buffer[0] == '@'){
                        typeBuffer = buffer.substr(1);
                        readFile >> buffer;
                        if(buffer[0] == '^'){
                            readFile >> buffer;
                            while(buffer[0] != ';'){
                                if(buffer == "pk")
                                    pkFlag = true;
                                else if(buffer == "fk")
                                    fkFlag = true;
                                else if(buffer == "nullable")
                                    nullableFlag = true;
                                else if(buffer == "unique")
                                    uniqueFlag = true;
                                else
                                    return false;

                                readFile >> buffer;
                            }
                        }
                        else
                            return false;

                        if(typeBuffer == "BOOL")
                            boolColumn = new Column<bool>(columnNameBuffer,
                            pkFlag, fkFlag, nullableFlag, uniqueFlag);
                        else if(typeBuffer == "INT")
                            intColumn = new Column<int>(columnNameBuffer,
                            pkFlag, fkFlag, nullableFlag, uniqueFlag);
                        else if(typeBuffer == "DOUBLE")
                            doubleColumn = new Column<double>(columnNameBuffer,
                            pkFlag, fkFlag, nullableFlag, uniqueFlag);
                        else if(typeBuffer == "STRING")
                            stringColumn = new Column<std::string>(columnNameBuffer,
                            pkFlag, fkFlag, nullableFlag, uniqueFlag);
                        else
                            return false;
                    }
                    else
                        return false;
                    readFile >> buffer;

                    while(buffer[0] != ';'){
                        if(typeBuffer == "BOOL"){
                            if(buffer == "NULL")
                                boolColumn->addNullValue();
                            else if(buffer == "Tak")
                                boolColumn->addValue(1);
                            else if(buffer == "Nie")
                                boolColumn->addValue(0);
                        }
                        else if(typeBuffer == "INT"){
                            if(buffer == "NULL")
                                intColumn->addNullValue();
                            else
                                intColumn->addValue(std::stoi(buffer));
                        }
                        else if(typeBuffer == "DOUBLE"){
                            if(buffer == "NULL")
                                doubleColumn->addNullValue();
                            else
                                doubleColumn->addValue(std::stod(buffer));
                        }
                        else if(typeBuffer == "STRING"){
                            if(buffer == "NULL")
                                stringColumn->addNullValue();
                            else
                                stringColumn->addValue(buffer);
                        }
                        else
                            return false;
                        readFile >> buffer;
                    }

                    if(boolColumn)
                        tableBuffer->attachColumnToTable(boolColumn);
                    if(intColumn)
                        tableBuffer->attachColumnToTable(intColumn);
                    if(doubleColumn)
                        tableBuffer->attachColumnToTable(doubleColumn);
                    if(stringColumn)
                        tableBuffer->attachColumnToTable(stringColumn);

                    readFile >> buffer;
            }
        }
        else
            return false;
    }

    std::cout << "Odczytano baze danych z: " + date << std::endl;
    readFile.close();
    return true;
}
