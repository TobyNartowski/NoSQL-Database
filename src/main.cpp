#include "column.hpp"
#include "table.hpp"

#include <iostream>
#include <vector>

int main()
{

	// Pierwsza kolumna
	Column<std::string> *firstNameColumn = new Column<std::string>("Imie");	
	firstNameColumn->addValue("Krzysztof");
	firstNameColumn->addValue("Marek");
	firstNameColumn->addValue("Pawel");
	firstNameColumn->addValue("Andrzej");

	// Druga kolumna
	Column<unsigned int> *ageColumn = new Column<unsigned int>("Wiek");
	ageColumn->addValue(32);
	ageColumn->addValue(54);
	ageColumn->addValue(18);
	ageColumn->addValue(28);

    // Trzecia kolumna
    Column<std::string> *eyesColorColumn = new Column<std::string>("Kolor oczu");
    eyesColorColumn->addValue("Zielony");
    eyesColorColumn->addValue("Niebieski");
    eyesColorColumn->addValue("Brazowe");
    eyesColorColumn->addValue("Niebieskie");
    eyesColorColumn->addValue("Niebieskie");
    eyesColorColumn->addValue("Niebieskie");
    eyesColorColumn->addValue("Zielony");
    eyesColorColumn->addValue("Brazowe");
    eyesColorColumn->addValue("Piwne");
    
	// Add to table
	Table *firstTable = new Table({firstNameColumn, ageColumn, eyesColorColumn});
	firstTable->printTable();
    
	delete firstNameColumn;
	delete ageColumn;	
    delete eyesColorColumn;
    delete firstTable;
	return 0;
}
