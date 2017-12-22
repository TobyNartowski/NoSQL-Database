/*
 TODO: Laczenie tabel
 // Usuwanie dodawanie z tabeli, a nie kolumny
 // Przesuwanie w dodawaniu wartosci!!
*/

#include "column.hpp"
#include "table.hpp"

#include <iostream>

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
    eyesColorColumn->addValue("Piwne");
    eyesColorColumn->addValue("Niebieskie");

    // ERROR TODO WYPISYWANIE
    //firstNameColumn->deleteValue(2);
    firstNameColumn->addValue("Ktos", 1);
    
	// Add to table
	Table *firstTable = new Table({firstNameColumn, ageColumn, eyesColorColumn});
	firstTable->printTable();

    //    ERROR TODO!
    //    firstNameColumn->addValue("asdasdasd", 1);
    //    firstTable->printTable();
    // Tabele przyjmujace wskazniki na kolumny?
    
	delete firstNameColumn;
	delete ageColumn;	
    delete eyesColorColumn;
    delete firstTable;

	return 0;
}
