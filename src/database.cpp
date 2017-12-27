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

void Database::printDatabase()
{
    std::cout << "==== " << nameOfDatabase << " ====" << std::endl;
    for(unsigned int i = 0; i < tables.size(); i++){
        tables[i]->printTable();
        std::cout << std::endl;
    }
}
