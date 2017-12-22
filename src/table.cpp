#include "table.hpp"

//Implementacje metod
Table::Table(std::initializer_list<ColumnHandler*> columns)
{	
	// TODO: Curses
	if(columns.size() == 0){
		std::cout << "Utworzono pusta tabele" << std::endl;
		tableSize = 0;
		return;
	}
	else
		std::cout << "Utworzono tabele z " << columns.size() << " kolumnami" << std::endl;

	unsigned int columnMaxSize = (*columns.begin())->getColumnSize();

	for(auto it = columns.begin(); it != columns.end(); it++){
		this->columns.push_back(*it);
		if(columnMaxSize < (*it)->getColumnSize())
			columnMaxSize = (*it)->getColumnSize();
	}

	tableSize = columnMaxSize;
}

void Table::addColumnToTable(ColumnHandler* column)
{
	if(tableSize < column->getColumnSize())
		tableSize = column->getColumnSize();
	columns.push_back(column);
}

void Table::printTable()
{
    alignColumns();
    
    for(int i = 0; i < columns.size(); i++)
        if(tableSize != columns[i]->getColumnSize()){
            //TODO: Curses
            std::cout << "Tabela nie jest spojna" << std::endl;
            return;
        }
    
	// TODO: Curses
	if(columns.size() == 0){
		std::cout << "Tabela jest pusta" << std::endl;
		return;
	}
    
    std::cout << "| ";
    for(int i = 0; i < columns.size(); i++)
        std::cout << columns[i]->getName() << " | ";
    std::cout << std::endl;
    
	// TODO: Curses
	for(int i = 0; i < tableSize; i++){
        std::cout << "| ";
		for(int j = 0; j < columns.size(); j++){
            columns[j]->printValue(i);
            std::cout << " | ";
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
        for(int i = 0; i < columns.size(); i++)
            std::cout << columns[i]->getName() << " | ";
        std::cout << std::endl;
    }
    
    std::cout << "| ";
    for(int i = 0; i < columns.size(); i++){
        columns[i]->printValue(index);
        std::cout << " | ";
    }
    std::cout << std::endl;
}

void Table::printRowWhereStringIs(std::string name, std::string value)
{
    // TODO: Curses
    int whichColumn = -1;
    std::string buffer;
    bool found = false;
    
    for(int i = 0; i < columns.size(); i++)
        if(columns[i]->getName() == name)
            whichColumn = i;
   
    if(whichColumn == -1){
        std::cout << "Nie znaleziono kolumny" << std::endl;
        return;
    }

    for(int i = 0; i < columns[whichColumn]->getColumnSize(); i++){
        buffer = columns[whichColumn]->streamPrint(i);
        if(buffer == value){
            if(!found){
                std::cout << "| ";
                for(int j = 0; j < columns.size(); j++)
                    std::cout << columns[j]->getName() << " | ";
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
    for(int i = 0; i < columns.size(); i++)
        if(!columns[i]->isNullable()){
            std::cout << "Brak mozliwosci wyrownania kolumn, kolumna: \"" << columns[i]->getName() << "\" nie miec pustych pol" << std::endl;
            // TODO: Curses
            return;
        }
    
    for(int i = 0; i < columns.size(); i++)
        if(tableSize < columns[i]->getColumnSize())
            tableSize = columns[i]->getColumnSize();

	for(int i = 0; i < columns.size(); i++){
        while(tableSize > columns[i]->getColumnSize()){
            columns[i]->addNullValue(columns[i]->getColumnSize());
        }
	}
}
