// Klasa calej bazy danych zawierajacej tabele
#pragma once

#include "column.hpp"
#include "table.hpp"

#include <iostream>
#include <vector>
#include <utility>
#include <fstream>

class Database
{
private:
    // Nazwa bazy danych
    std::string nameOfDatabase;

    // Zbior wszystkich tabel w bazie danych
    std::vector<Table*> tables;

    // Zbior par kluczow glownych i obcych
    std::vector<std::pair<ColumnHandler*, ColumnHandler*>> keys;

    // Plik lokalny bazy danych
    std::fstream localFile;
public:
    // Konstruktor bazy danych
    // (przyjmuje nazwe bazy danych, nazwe pliku lokalnego i wskaznik sprawdzajacy
    // czy poprawnie otwarto plik)
    Database(std::string nameOfDatabase, std::string databaseFilename);

    // Destruktor bazy danych
    ~Database();

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

    // Zwraca wskaznik na dana tabele
    // (przyjmuje nazwe tabeli lub odpowiedni indeks)
    Table *getTable(std::string nameOfTable);
    Table *getTable(unsigned int index);

    // Zapisuje baze danych do pliku lokalnego
    void saveDatabase();
};
