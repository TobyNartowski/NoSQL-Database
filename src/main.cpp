
#include "column.hpp"
#include "table.hpp"
#include "database.hpp"

#include <iostream>

Database *database;

// DO TESTOWANIA
Table *create_table_with_columns()
{
	// Pierwsza kolumna
	Column<std::string> *firstNameColumn = new Column<std::string>("Imie");
	firstNameColumn->addValue("Krzysztof");
	firstNameColumn->addValue("Marek");
	firstNameColumn->addValue("Pawel");
	firstNameColumn->addValue("Andrzej");
	firstNameColumn->addValue("Maciej");

	// Druga kolumna
	Column<unsigned int> *ageColumn = new Column<unsigned int>("Wiek", true);
	ageColumn->addValue(32);
	ageColumn->addValue(33);
	ageColumn->addValue(18);
	ageColumn->addValue(28);
	ageColumn->addValue(21);
	ageColumn->addValue(11);

	// Trzecia kolumna
	Column<std::string> *eyesColorColumn = new Column<std::string>("Kolor oczu", false, true);
	eyesColorColumn->addValue("Zielone");
	eyesColorColumn->addValue("Niebieskie");
	eyesColorColumn->addValue("Brazowe");
	eyesColorColumn->addValue("Zielone");
	eyesColorColumn->addValue("Piwne");

	// Stworz pusta tabele
	Table *firstTable = new Table("Osoba");

	// Dodaj kolumny do tabeli
	firstTable->attachColumnToTable(firstNameColumn);
	firstTable->attachColumnToTable(ageColumn);
	firstTable->attachColumnToTable(eyesColorColumn);

	// Zwroc wskaznik na utworzona tabele
	return firstTable;
}

int main()
{
 	database = new Database("Baza danych");

	Table *table = create_table_with_columns();
	database->printDatabase();

	delete table;
	delete database;
	return 0;
}
