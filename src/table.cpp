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
    
	// TODO: Curses
	for(int i = 0; i < tableSize; i++){
        std::cout << "| ";
		for(int j = 0; j < columns.size(); j++){
            columns[j]->printValueAtPos(i);
            std::cout << " | ";
        }
		std::cout << std::endl;
	}
}

void Table::alignColumns()
{
	for(int i = 0; i < columns.size(); i++){
        while(tableSize > columns[i]->getColumnSize()){
            columns[i]->addNullValue();
        }
	}
}
