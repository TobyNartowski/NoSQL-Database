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

    // Wyswietla caly wiersz (przyjmuje indeks, ktory rzad ma wyswietlic oraz flage czy maja zostac wyswietlone nazwy kolumn)
    void printRow(unsigned int index, bool printHeader = true);
    
    // Wyswietla wiersz, gdzie w danej kolumnie jest spelniony odpowiedni warunek dla typu tekstowego
    // (przyjmuje nazwe kolumny i oczekiwana wartosc)
    void printRowWhereStringIs(std::string name, std::string value);
    
	// Wyrownaj kolumny, rozmiar kolumn zostaje zwiekszony do
	// rozmiaru najwiekszej kolumny w tabeli;
	// puste miejsca sa wypelniane znakiem '-'
	void alignColumns();
};
