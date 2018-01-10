// Kod glowny
#include "column.hpp"
#include "table.hpp"
#include "database.hpp"

#include <iostream>

#define DATABASE_FILENAME ".local_database"

Database *database;

// Przykladowe tabele
Table *create_person_table();
Table *create_address_table();

int main()
{
 	database = new Database("Baza_danych", DATABASE_FILENAME);

	Table *personTable = create_person_table();
	Table *addressTable = create_address_table();

    database->saveDatabase();
    database->loadDatabase();
    database->printDatabase();

    delete addressTable;
	delete personTable;
	delete database;
	return EXIT_SUCCESS;
}


Table *create_person_table()
{
	Column<int> *idColumn = new Column<int>("Id_osoby", true);
	idColumn->addValue(1);
	idColumn->addValue(2);
	idColumn->addValue(3);
	idColumn->addValue(4);
	idColumn->addValue(5);

	// Druga kolumna
	Column<std::string> *firstNameColumn = new Column<std::string>("Imie", false, false, false);
	firstNameColumn->addValue("Andrzej");
	firstNameColumn->addValue("Maciej");
	firstNameColumn->addValue("Krzysztof");
	firstNameColumn->addValue("Anna");
	firstNameColumn->addValue("Pawel");

	// Trzecia kolumna
	Column<std::string> *secondNameColumn = new Column<std::string>("Nazwisko", false, false, false);
	secondNameColumn->addValue("Kowalski");
	secondNameColumn->addValue("Krawczyk");
	secondNameColumn->addValue("Duda");
	secondNameColumn->addValue("Makowska");
	secondNameColumn->addValue("Szymanski");

	// Czwarta kolumna
	Column<int> *ageColumn = new Column<int>("Wiek", false, false, false);
	ageColumn->addValue(25);
	ageColumn->addValue(41);
	ageColumn->addValue(38);
	ageColumn->addValue(40);
	ageColumn->addValue(19);

    Column<bool> *employedColumn = new Column<bool>("Zatrudniony", false, false, false);
    employedColumn->addValue(true);
    employedColumn->addValue(true);
    employedColumn->addValue(false);
    employedColumn->addValue(true);
    employedColumn->addValue(false);

    Column<double> *earningsColumn = new Column<double>("Zarobki");
    earningsColumn->addValue(4137.12);
    earningsColumn->addValue(2400.00);
    earningsColumn->addValue(5999.99);
    earningsColumn->addValue(3411.05);
    earningsColumn->addValue(11030.00);

	// Piata kolumna
	Column<int> *addressIdColumn = new Column<int>("Id_adresu", false, true);
	addressIdColumn->addValue(1);
	addressIdColumn->addValue(3);
	addressIdColumn->addValue(2);
	addressIdColumn->addValue(2);
	addressIdColumn->addValue(5);

	// Stworz pusta tabele
	Table *personTable = new Table("Osoby");

	// Dodaj kolumny do tabeli
	personTable->attachColumnToTable(idColumn);
	personTable->attachColumnToTable(firstNameColumn);
	personTable->attachColumnToTable(secondNameColumn);
	personTable->attachColumnToTable(ageColumn);
    personTable->attachColumnToTable(employedColumn);
    personTable->attachColumnToTable(earningsColumn);
	personTable->attachColumnToTable(addressIdColumn);

	// Zwroc wskaznik na utworzona tabele
	return personTable;
}

Table *create_address_table()
{
	Column<int> *idColumn = new Column<int>("Id_adresu", true);
	idColumn->addValue(1);
	idColumn->addValue(2);
	idColumn->addValue(3);
	idColumn->addValue(4);
	idColumn->addValue(5);

	// Druga kolumna
	Column<std::string> *cityColumn = new Column<std::string>("Miasto", false, false, false);
	cityColumn->addValue("Kielce");
	cityColumn->addValue("Warszawa");
	cityColumn->addValue("Poznan");
	cityColumn->addValue("Krakow");
	cityColumn->addValue("Kielce");

	// Trzecia kolumna
	Column<std::string> *streetColumn = new Column<std::string>("Ulica", false, false, true);
	streetColumn->addValue("Swietokrzyska");
	streetColumn->addValue("Domanska");
	streetColumn->addValue("Krzywa");
	streetColumn->addValue("Rakowska");
	streetColumn->addValue("Sienkiewicza");

	// Czwarta kolumna
	Column<std::string> *numberColumn = new Column<std::string>("Nr_domu", false, false, false);
	numberColumn->addValue("4");
	numberColumn->addValue("52a");
	numberColumn->addValue("211/2");
	numberColumn->addValue("22");
	numberColumn->addValue("19");

	Table *addressTable = new Table("Adresy");

	addressTable->attachColumnToTable(idColumn);
	addressTable->attachColumnToTable(cityColumn);
	addressTable->attachColumnToTable(streetColumn);
	addressTable->attachColumnToTable(numberColumn);

	return addressTable;
}
