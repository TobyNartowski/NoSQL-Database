// Klasa pojedynczej kolumny w tablicy
#pragma once

#include <iostream>
#include <vector>

#define ARG_NOT_PROVIDED -1

typedef enum e_ColumnType {
    COL_ERROR, COL_BOOL, COL_INT, COL_DOUBLE, COL_STRING
} ColumnType;

// Klasa bazowa (dodatkowa)
// Jest zaimplementowana w celu ulatwienia przechowywania kolumn w tabelach
// (kolumny przechowywane sa w kontenerze Vector, ktory nie przechowuje szablonow klas)
class ColumnHandler
{
public:
    virtual std::string getName() = 0;
    virtual void setTableName(std::string tableName) = 0;
    virtual std::string getTableName() = 0;
    virtual void addNullValue(unsigned int index = 0) = 0;
    virtual void deleteValue(unsigned int index) = 0;
    virtual unsigned int getColumnSize() = 0;
    virtual std::string streamPrint(unsigned int index, bool filePrint = false) = 0;
    virtual bool isNullable() = 0;
    virtual bool isPk() = 0;
    virtual bool isFk() = 0;
    virtual bool isUnique() = 0;
    virtual ColumnType whatType() = 0;
    virtual ~ColumnHandler() {};
};

// Klasa pochodna (glowna)
// Implementuje podstawowe operacje na kolumnie i jej polach
template <typename T> class Column : public ColumnHandler
{
private:
    // Nazwa kolumny
    std::string nameOfColumn;

    // Atrybuty kolumn
    bool nullable;
    bool unique;
    bool pk;
    bool fk;

	// Wartosci przechowywane w kolumnie
	std::vector<T*> values;

	// Rozmiar kolumny
	unsigned int columnSize;

    // Nazwa tabeli obejmujacej
    std::string tableName;
public:
	// Konstruktor kolumny inicjalizujacy jej parametry
    // (przyjmuje nazwe nowej kolumny oraz opcjonalne odpowiednie parametry)
    Column(std::string nameOfColumn, bool pk = false, bool fk = false, bool nullable = true, bool unique = false);

    // Opcjonalny konstruktor kolumny inicjalizujacy jej parametry
    // (przyjmuje nazwe nowej kolumny oraz opcjonalne odpowiednie parametry)
    Column(std::string nameOfColumn, bool *flags);

    // Zwraca nazwe kolumny
    std::string getName();

    // Ustala nazwe tabeli obejmujacej (przyjmuje nazwe tabeli)
    void setTableName(std::string tableName);

    // Zwraca nazwe tabeli obejmujacej
    std::string getTableName();

	// Dodaj wartosc (przyjmuje wartosc, ktora ma zostac dodana oraz odpowiedni indeks)
	void addValue(const T &value, int index = ARG_NOT_PROVIDED);

    // Dodaj puste pole (przyjmuje opdowiedni indeks)
    void addNullValue(unsigned int index = 0);

	// Usun wartosc (przyjmuje odpowiedni indeks)
	void deleteValue(unsigned int index);

    // Zwraca wartosc
    // (przyjmuje odpowiedni indeks oraz flage okreslajaca typ zwracania
    //  wartosci null do plikow)
    std::string streamPrint(unsigned int index, bool filePrint = false);

	// Zwraca rozmiar kolumny
	unsigned int getColumnSize();

    // Zwraca czy kolumna moze miec puste pola
    bool isNullable();

    // Zwraca czy kolumna jest kluczem glownym
    bool isPk();

    // Zwraca czy kolumna jest kluczem obcym
    bool isFk();

    // Zwraca czy pola kolumny musza byc unikalne
    bool isUnique();

    // Zwraca typ kolumny w postaci odpowiedniego znaku
    ColumnType whatType();
};
