/*
 TODO: Laczenie tabel
*/

#include "column.hpp"
#include "table.hpp"

#include <iostream>

int main()
{
	// Pierwsza kolumna
	Column<std::string> *firstNameColumn = new Column<std::string>("Imie", false, false, false);
	firstNameColumn->addValue("Krzysztof");
	firstNameColumn->addValue("Marek");
	firstNameColumn->addValue("Pawel");
	firstNameColumn->addValue("Andrzej");
    firstNameColumn->addValue("Maciej");

	
    // Druga kolumna
	Column<unsigned int> *ageColumn = new Column<unsigned int>("Wiek", true);
	ageColumn->addValue(32);
	ageColumn->addValue(54);
	ageColumn->addValue(18);
	ageColumn->addValue(28);
    ageColumn->addValue(21);
    
    // Add to table
    Table *firstTable = new Table({firstNameColumn, ageColumn});
    firstTable->printTable();
    
    std::cout << std::endl;
    
    // Trzecia kolumna
    Column<std::string> *eyesColorColumn = new Column<std::string>("Kolor oczu", false, true);
    eyesColorColumn->addValue("Zielone");
    eyesColorColumn->addValue("Niebieskie");
    eyesColorColumn->addValue("Brazowe");
    eyesColorColumn->addValue("Zielone");
    eyesColorColumn->addValue("Piwne");

    firstTable->addColumnToTable(eyesColorColumn);
    firstTable->printTable();
    
    delete firstNameColumn;
	delete ageColumn;	
    delete eyesColorColumn;
    delete firstTable;

	return 0;
}
