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

    // Nazwa pliku bazy danych
    std::string databaseFilename;
public:
    // Konstruktor bazy danych
    // (przyjmuje nazwe bazy danych, nazwe pliku lokalnego i wskaznik sprawdzajacy
    // czy poprawnie otwarto plik)
    Database(std::string nameOfDatabase, std::string databaseFilename);

    // Zwraca nazwe bazy danych
    std::string getName();

    // Ustawia nazwe bazy danych
    void setName(std::string databaseName);

    // Zwraca ilosc tabel w bazie danych
    unsigned int getDatabaseSize();

    // Wyczysc cala baze danych
    void clearDatabase();

    // Podlacz tabele do bazy bazy danych
    // (zwraca flage, ktora mowi o tym czy udalo sie dolaczyc tabele)
    bool attachTableToDatabase(Table *table);

    // Odlacz tabele z bazy danych
    void detachTableFromDatabase(unsigned int index);

    // Wyswietla cala baze danych
    void printDatabase();

    // Dodaje polaczenie klucza glownego (przyjmuje glowna kolumne)
    void setPk(ColumnHandler *pkColumn);

    // Dodaje polaczenie klucza obcego (przyjmuje obca kolumne)
    void setFk(ColumnHandler *fkColumn);

    // Usuwa polaczenie klucza glownego (przyjmuje glowna kolumne)
    void deletePk(ColumnHandler *pkColumn);

    // Usuwa polaczenie klucza obcego (przyjmuje obca kolumne)
    void deleteFk(ColumnHandler *fkColumn);

    // Zwraca wszystkie polaczenia miedzy tabelami
    std::string getKeysString();

    // Zwraca wskaznik na dana tabele
    // (przyjmuje nazwe tabeli lub odpowiedni indeks)
    Table *getTable(std::string nameOfTable);
    Table *getTable(unsigned int index);

    // Sprawdza czy tabela znajduje sie w bazie danych
    // (przyjmuje nazwe tabeli, zwraca prawde lub falsz)
    bool containsTable(std::string nameOfTable);

    // Sprawdza czy baza danych jest pusta (Zwraca prawde lub falsz)
    bool empty();

    // Sprawdza czy w bazie danych znajduja sie jakies polaczenia
    bool keysEmpty();

    // Zapisuje baze danych do pliku lokalnego
    void saveDatabase();

    // Odczytuje baze danych z pliku lokalnego
    // (zwraca flage czy powiodlo sie odczytywanie pliku)
    bool loadDatabase(std::string &time);
};
