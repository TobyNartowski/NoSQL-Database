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
    virtual std::string getName() = 0;
    virtual void printColumn() = 0;
    virtual void addNullValue(unsigned int index) = 0;
    virtual unsigned int getColumnSize() = 0;
    virtual void printValue(unsigned int index) = 0;
    virtual std::string streamPrint(unsigned int index) = 0;
    virtual bool isNullable() = 0;
    virtual ~ColumnHandler() {};
};

// Klasa pochodna (glowna)
// Implementuje podstawowe operacje na kolumnie i jej rekordach
template <typename T> class Column : public ColumnHandler
{
private:
    // Nazwa kolumny
    std::string nameOfColumn;
    
    // Atrybuty kolumn
    bool nullable;
    
	// Wartosci przechowywane w kolumnie
	std::vector<T*> values;
	
	// Rozmiar kolumny
	unsigned int columnSize;
public:
	// Konstruktor kolumny inicjalizujacy jej nazwe (przyjmuje nazwe nowej kolumny)
    Column(std::string nameOfColumn, bool nullable = true);
    
    // Zwraca nazwe kolumny
    std::string getName();
    
	// Dodaj wartosc (przyjmuje wartosc, ktora ma zostac dodana oraz odpowiedni indeks)
	void addValue(const T &value, unsigned int index = 0);
	
    // Dodaj puste pole (przyjmuje opdowiedni indeks)
    void addNullValue(unsigned int index);
    
	// Usun wartosc (przyjmuje odpowiedni indeks)
	void deleteValue(unsigned int index);

	// Wyswietl cala kolumne
	void printColumn(); // TODO: Curses

    // Wyswietla wartosc (przyjmuje odpowiedni indeks)
    void printValue(unsigned int index);
    
    // TESTOWANIE
    std::string streamPrint(unsigned int index);
    
	// Zwraca rozmiar kolumny
	unsigned int getColumnSize();
    
    // Zwraca czy kolumna moze miec puste pola
    bool isNullable();
};
