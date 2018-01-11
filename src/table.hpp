// Klasa tabeli w bazie danych zawierajacej kolumny
#pragma once

#include "column.hpp"

#include <iostream>
#include <vector>

class Table
{
private:
	// Nazwa tabeli
	std::string nameOfTable;

	// Kolumny zawierajace sie w tabeli
	std::vector<ColumnHandler*> columns;

	// Maksymalna dlugosc kolumny
	std::vector<unsigned int> columnLengths;

	// Rozmiar tabeli
	unsigned int tableSize;

    // indeks klucza glownego
    int pkIndex;

    // indeksy kluczow obcych
    std::vector<int> fkIndexes;

    // sprawdza czy w wektorze znajduje sie podany indeks
    bool vectorContains(std::vector<int> vector, int index);
public:
	// Konstruktor tworzacy pusta tabele
	// (przyjmuje baze danych, do ktorej ma zostac dolaczona oraz nazwe tabeli)
	Table(std::string nameOfTable);

	// Nazwa tabeli
	std::string name;

	// Zwraca nazwe tabeli
	std::string getName();

	// Zwraca rozmiar tabeli
	unsigned int getTableSize();

	// Zwraca wysokosc tabeli
	unsigned int getHeight();

	// Zwraca dlugosc danej kolumny (przyjmuje odpowiedni indeks)
	unsigned int getLength(unsigned int index);

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

	// Oblicza maksymalna dlugosc kolumny i zapisuje do wektora
	void calculateLength(ColumnHandler* column);

    // Podlacz kolumne do tabeli (przyjmuje kolumne do dodania)
    void attachColumnToTable(ColumnHandler* column);

	// Odlacz kolumne z tabeli (przyjmuje nazwe kolumny do usuniecia)
	void detachColumnFromTable(std::string nameOfColumn);

	// Zwraca wskaznik na dana kolumne
	// (przyjmuje nazwe kolumny lub odpowiedni indeks)
	ColumnHandler *getColumn(std::string nameOfColumn);
	ColumnHandler *getColumn(unsigned int index);
};
