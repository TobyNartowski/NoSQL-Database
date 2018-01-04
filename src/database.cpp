#include "database.hpp"

#include <ctime>

//Implementacje metod
Database::Database(std::string nameOfDatabase, std::string databaseFilename)
{
    this->nameOfDatabase = nameOfDatabase;
    localFile.open(databaseFilename.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);

    if(!localFile.good()){
        std::cerr << "Blad przy otwieraniu pliku lokalnego" << std::endl;
        exit(-1);
    }
}

Database::~Database()
{
    localFile.close();
}

std::string Database::getName()
{
    return nameOfDatabase;
}

bool Database::attachTableToDatabase(Table *table)
{
    for(unsigned int i = 0; i < tables.size(); i++)
        if(tables[i]->getName() == table->getName()){
            std::cout << "Table o podanej nazwie juz znajduje sie w bazie danych" << std::endl;
            return true;
        }
    tables.push_back(table);
    return false;
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

void Database::saveDatabase()
{
    std::time_t localTime = std::time(nullptr);
    localFile << "* " << std::asctime(std::localtime(&localTime)) << std::endl << std::endl;

    localFile << "!" << getName() << std::endl;
    for(unsigned int i = 0; i < tables.size(); i++){
    localFile << "  #" << tables[i]->getName() << std::endl;
        for(unsigned int j = 0; j < tables[i]->getTableSize(); j++){
            localFile << "\t$" << tables[i]->getColumn(j)->getName() << std::endl;

            localFile << "\t" << "@";
            switch(tables[i]->getColumn(j)->whatType()){
                case 'B':
                    localFile << "BOOL";
                    break;
                case 'I':
                    localFile << "INT";
                    break;
                case 'D':
                    localFile << "DOUBLE";
                    break;
                case 'S':
                    localFile << "STRING";
                    break;
                default:
                    return;
            }

            localFile << std::endl << "\t^";
            if(tables[i]->getColumn(j)->isPk())
                localFile << "pk ";
            if(tables[i]->getColumn(j)->isFk())
                localFile << "fk ";
            if(tables[i]->getColumn(j)->isNullable())
                localFile << "nullable ";
            if(tables[i]->getColumn(j)->isUnique())
                localFile << "unique ";

            localFile << std::endl << "\t\t";
            for(unsigned int k = 0; k < tables[i]->getColumn(j)->getColumnSize(); k++){
                localFile << tables[i]->getColumn(j)->streamPrint(k);
                localFile << " ";
            }
            localFile << std::endl;
        }
    }
    std::cout << "Zapisano baze danych!" << std::endl;
}
