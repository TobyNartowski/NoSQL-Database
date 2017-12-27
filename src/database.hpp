// Klasa calej bazy danych zawierajacej tabele
#pragma once

#include "column.hpp"
#include "table.hpp"

#include <iostream>
#include <vector>
#include <utility>

class Database
{
private:
    // Nazwa bazy danych
    std::string nameOfDatabase;

    // Zbior wszystkich tabel w bazie danych
    std::vector<Table*> tables;

    // Zbior par kluczow glownych i obcych
    std::vector<std::pair<ColumnHandler*, ColumnHandler*>> keys;
public:
    // Konstruktor bazy danych (przyjmuje nazwe bazy danych)
    Database(std::string nameOfDatabase);

    // Zwraca nazwe bazy bazy danych
    std::string getName();

    // Podlacz tabele do bazy bazy danych
    // (zwraca flage, ktora mowi o tym czy udalo sie dolaczyc tabele)
    bool attachTableToDatabase(Table *table);

    // Odlacz tabele z bazy danych
    void detachTableFromDatabase(std::string nameOfTable);

    // Wyswietla cala baze danych
    void printDatabase();

    // Dodaje polaczenie klucza glownego (przyjmuje glowna kolumne)
    void setPk(ColumnHandler *pkColumn);

    // Dodaje polaczenie klucza obcego (przyjmuje obca kolumne)
    void setFk(ColumnHandler *fkColumn);

    // Wyswietla wszystkie polaczenia miedzy tabelami
    void printKeys();
};
