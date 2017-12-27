#include "database.hpp"

//Implementacje metod
Database::Database(std::string nameOfDatabase)
{
    this->nameOfDatabase = nameOfDatabase;
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
