# Baza danych NoSQL

Projekt bazy danych NoSQL wykonany w C++ z użyciem biblioteki ncurses

## Wymagane biblioteki

Projekt został przetestowany na dystrybucji Debian systemu operacyjnego Linux oraz macOS. Dodatkowo do uruchomienia aplikacji potrzebna jest biblioteka ncurses

### Instalacja ncurses

Aby zainstalować bibliotekę ncurses należy uruchomić następujące polecenie

```
sudo apt-get install libncurses5-dev libncursesw5-dev
```

### Kompilacja programu

Aby skompilować program należy w katalogu głównym aplikacji uruchomić polecenie

```
make
```

Aby usunac wszystkie skompilowane pliki należy uruchomić

```
make clean
```

### Uruchamianie programu

Do aplikacji został napisany skrypt, który uruchamia program.
Aby uruchomić aplikację wystarczy wywołać polecenie

```
./runscript
```


## Autorzy

* **Tobiasz Nartowski** - [TobyNartowski](https://github.com/TobyNartowski)
* **Piotr Piasecki** - [Vattier56](https://github.com/Vattier56)

## Licencja

Program jest na licencji GNU General Public License v3.0 - aby zobaczy szczegóły zobacz plik [LICENSE.md](LICENSE.md)
