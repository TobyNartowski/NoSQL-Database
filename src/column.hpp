// Klasa pojedynczej kolumny w tablicy
#pragma once

#include <iostream>
#include <vector>

// Klasa bazowa (dodatkowa)
// Jest zaimplementowana w celu ulatwienia przechowywania kolumn w tabelach
// (kolumny przechowywane sa w kontenerze Vector, ktory nie przechowuje szablonow klas)
class ColumnHandler
{
public:
    virtual void printColumn() = 0;
    virtual void addNullValue() = 0;
    virtual unsigned int getColumnSize() = 0;
    virtual void printValueAtPos(unsigned int index) = 0;
    virtual ~ColumnHandler() {};
};

// Klasa pochodna (glowna)
// Implementuje podstawowe operacje na kolumnie i jej rekordach
template <typename T> class Column : public ColumnHandler
{
private:
    // Atrybuty kolumn
    bool nullable;
    
	// Wartosci przechowywane w kolumnie
	std::vector<T> values;
	
	// Rozmiar kolumny
	unsigned int columnSize;
public:
	// Nazwa kolumny
	std::string nameOfColumn;

	// Konstruktor kolumny inicjalizujacy jej nazwe (przyjmuje nazwe nowej kolumny)
	Column(std::string nameOfColumn);
    
	// Dodaj wartosc (przyjmuje wartosc, ktora ma zostac dodana)
	void addValue(T value);
	
    // Dodaj puste pole
    void addNullValue();
    
	// Usun wartosc na pozycji (przyjmuje odpowiedni indeks)
	void deleteValueAtPos(unsigned int index);

	// Znajdz wartosc
	// (przyjmuje wartosc; zwraca wektor z indeksami, gdzie znaleziono wartosc)
	std::vector<unsigned int> findValue(T value);	

	// Wyswietl cala kolumne
	void printColumn(); // TODO: Curses

    // Wyswietla wartosc na pozycji (przyjmuje odpowiedni indeks)
    void printValueAtPos(unsigned int index);
    
	// Zwraca rozmiar kolumny
	unsigned int getColumnSize();
};
