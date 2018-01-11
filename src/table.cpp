#include "table.hpp"

#include "database.hpp"

extern Database *database;

//Implementacje metod
Table::Table(std::string nameOfTable)
{
    this->nameOfTable = nameOfTable;
    pkIndex = -1;
    fkIndexes.clear();
    tableSize = 0;

    if(!database->attachTableToDatabase(this))
        return;
}

std::string Table::getName()
{
    return nameOfTable;
}

unsigned int Table::getTableSize()
{
    return columns.size();
}

unsigned int Table::getHeight()
{
    return tableSize;
}

unsigned int Table::getLength(unsigned int index)
{
    if(index > columnLengths.size())
        return 0;
    else
        return columnLengths[index];
}

bool Table::vectorContains(std::vector<int> vector, int index)
{
    if(vector.empty())
        return false;
    for(unsigned int i = 0; i < vector.size(); i++)
        if(vector[i] == index)
            return true;
    return false;
}

void Table::printTable()
{
    std::cout << "---- " << nameOfTable << " ----" << std::endl;
    alignColumns();

	// TODO: Curses
	if(columns.size() == 0){
		std::cout << "Tabela jest pusta" << std::endl;
		return;
	}

    for(unsigned int i = 0; i < columns.size(); i++)
        if(tableSize != columns[i]->getColumnSize()){
            //TODO: Curses
            std::cout << "Tabela nie jest spojna" << std::endl;
            return;
        }

    std::cout << "| ";
    for(unsigned int i = 0; i < columns.size(); i++){
        if(i == (unsigned)pkIndex)
            std::cout << columns[i]->getName() << " (PK) | ";
        else if(vectorContains(fkIndexes, i))
            std::cout << columns[i]->getName() << " (FK) | ";
        else
            std::cout << columns[i]->getName() << " | ";
    }
    std::cout << std::endl;

	// TODO: Curses
	for(unsigned int i = 0; i < tableSize; i++){
        std::cout << "| ";
		for(unsigned int j = 0; j < columns.size(); j++){
            std::cout << columns[j]->streamPrint(i) << " | ";
        }
		std::cout << std::endl;
	}
}

void Table::printRow(unsigned int index, bool printHeader)
{
    //TODO: Curses
    if(index >= tableSize){
        std::cout << "Niepoprawny indeks" << std::endl;
        return;
    }

    if(printHeader){
        std::cout << "| ";
        for(unsigned int i = 0; i < columns.size(); i++){
            if(i == (unsigned)pkIndex)
                std::cout << columns[i]->getName() << " (PK) | ";
            else if(vectorContains(fkIndexes, i))
                std::cout << columns[i]->getName() << " (FK) | ";
            else
                std::cout << columns[i]->getName() << " | ";
        }
        std::cout << std::endl;
    }

    std::cout << "| ";
    for(unsigned int i = 0; i < columns.size(); i++)
        std::cout << columns[i]->streamPrint(index) << " | ";
    std::cout << std::endl;
}

void Table::printRowWhereStringIs(std::string name, std::string value)
{
    // TODO: Curses
    int whichColumn = -1;
    std::string buffer;
    bool found = false;

    for(unsigned int i = 0; i < columns.size(); i++)
        if(columns[i]->getName() == name)
            whichColumn = i;

    if(whichColumn == -1){
        std::cout << "Nie znaleziono kolumny" << std::endl;
        return;
    }

    for(unsigned int i = 0; i < columns[whichColumn]->getColumnSize(); i++){
        buffer = columns[whichColumn]->streamPrint(i);
        if(buffer == value){
            if(!found){
                std::cout << "| ";
                for(unsigned int i = 0; i < columns.size(); i++){
                    if(i == (unsigned)pkIndex)
                        std::cout << columns[i]->getName() << " (PK) | ";
                    else if(vectorContains(fkIndexes, i))
                        std::cout << columns[i]->getName() << " (FK) | ";
                    else
                        std::cout << columns[i]->getName() << " | ";
                }
                std::cout << std::endl;
                found = true;
            }
            printRow(i, false);
        }
    }

    if(!found)
        std::cout << "Nie znaleziono wynikow w kolumnie" << std::endl;
}

void Table::alignColumns()
{
    for(unsigned int i = 0; i < columns.size(); i++)
        if((!columns[i]->isNullable()) && (columns[i]->getColumnSize() < tableSize)){
            std::cout << "Brak mozliwosci wyrownania kolumn, kolumna: \"" << columns[i]->getName() << "\" nie miec pustych pol" << std::endl;
            // TODO: Curses
            return;
        }

    for(unsigned int i = 0; i < columns.size(); i++){
        if(tableSize < columns[i]->getColumnSize())
            tableSize = columns[i]->getColumnSize();
        while(tableSize > columns[i]->getColumnSize())
                columns[i]->addNullValue(columns[i]->getColumnSize());
    }
}

void Table::calculateLength(ColumnHandler* column)
{
    unsigned int max = (column->streamPrint(0)).length();
    for(unsigned int i = 1; i < column->getColumnSize(); i++)
        if((column->streamPrint(i)).length() > max)
            max = (column->streamPrint(i)).length();
    columnLengths.push_back(max);
}

void Table::attachColumnToTable(ColumnHandler* column)
{
    for(unsigned int i = 0; i < columns.size(); i++){
        if(columns[i]->getName() == column->getName()){
            std::cout << "Kolumna o podanej nazwie juz istnieje" << std::endl;
            return;
        }
    }

    if(column->isPk()){
        if(pkIndex != -1){
            std::cout << "Wiecej niz jeden klucz glowny, kolumna \"" << column->getName() << "\" nie zostala dodana" << std::endl;
            return;
        }
        pkIndex = columns.size();
        database->setPk(column);
    }

    if(column->isFk()){
        fkIndexes.push_back(columns.size());
        database->setFk(column);
    }

    if(tableSize < column->getColumnSize())
        tableSize = column->getColumnSize();

    column->setTableName(getName());
    columns.push_back(column);
    calculateLength(column);
}

void Table::detachColumnFromTable(std::string nameOfColumn)
{
    for(unsigned int i = 0; i < columns.size(); i++)
        if(columns[i]->getName() == nameOfColumn){
            columns[i]->setTableName("NULL");
            columns.erase(columns.begin() + i);
            columnLengths.erase(columnLengths.begin() + i);
            std::cout << "Odlaczono kolumne \"" << nameOfColumn << "\" z tabeli" << std::endl;
            return;
        }
    std::cout << "Nie znaleziono kolumny: \"" << nameOfColumn << "\" w tabeli" << std::endl;
}

ColumnHandler *Table::getColumn(std::string nameOfColumn)
{
    for(unsigned int i = 0; i < columns.size(); i++)
        if(columns[i]->getName() == nameOfColumn)
            return columns[i];
    std::cout << "Nie znaleziono kolumny o nazwie \"" << nameOfColumn << "\"" << std::endl;
    return NULL;
}

ColumnHandler *Table::getColumn(unsigned int index)
{
    if(index > columns.size())
        return NULL;
    return columns[index];
}
