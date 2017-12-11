// Klasa tabeli w bazie danych zawierajacej kolumny
#pragma once

#include "column.hpp"

#include <iostream>
#include <vector>
#include <initializer_list>

class Table
{
private:
	// Kolumny zawierajace sie w tabeli
	std::vector<ColumnHandler*> columns;

	// Rozmiar tabeli
	unsigned int tableSize;
public:
	// Nazwa tabeli
	std::string name;

	// Konstruktor tworzacy tabele (przyjmuje kolumny)
	Table(std::initializer_list<ColumnHandler*> columns);

	// Dodaj kolumne do tabeli (przyjmuje kolumne do dodania)
	void addColumnToTable(ColumnHandler* column);

	// Wyswietl cala tabele
	void printTable();

	// Wyrownaj kolumny, rozmiar kolumn zostaje zwiekszony do
	// rozmiaru najwiekszej kolumny w tabeli;
	// puste miejsca sa wypelniane znakiem '-'
	void alignColumns();
};
