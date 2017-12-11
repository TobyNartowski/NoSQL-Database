/*
 TODO: Laczenie tabel
*/

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
    eyesColorColumn->addValue("Zielone");
    eyesColorColumn->addValue("Niebieskie");
    eyesColorColumn->addValue("Brazowe");
    eyesColorColumn->addValue("Zielone");
    
    //firstNameColumn->deleteValue(0);
    //ageColumn->deleteValue(1);
    //firstNameColumn->addValue("Piotr", 6);
    
	// Add to table
	Table *firstTable = new Table({firstNameColumn, ageColumn, eyesColorColumn});
	firstTable->printTable();
    //firstTable->printRow(3);
    //firstTable->printRowWhereStringIs("Kolor oczu", "Zielone");
    
	delete firstNameColumn;
	delete ageColumn;	
    delete eyesColorColumn;
    delete firstTable;

	return 0;
}
