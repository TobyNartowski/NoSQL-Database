// Klasa calej bazy danych zawierajacej tabele
#pragma once

#include "column.hpp"
#include "table.hpp"

#include <iostream>
#include <vector>

class Database
{
private:
    // Nazwa bazy danych
    std::string nameOfDatabase;

    // Zbior wszystkich tabel w bazie danych
    std::vector<Table*> tables;
public:
    // Konstruktor bazy danych (przyjmuje nazwe bazy danych)
    Database(std::string nameOfDatabase);

    // Zwraca nazwe bazy bazy danych
    std::string getName();

    // Dodaje tabele do bazy bazy danych
    // (zwraca flage, ktora mowi o tym czy udalo sie dolaczyc tabele)
    bool attachTableToDatabase(Table *table);

    // Wyswietla cala baze danych
    void printDatabase();
};
