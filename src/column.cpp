#include "column.hpp"

#include <sstream>

// Implementacje metod
template <typename T>
Column<T>::Column(std::string nameOfColumn, bool pk, bool fk, bool nullable, bool unique)
{
    this->pk = this->fk = false;
    this->nullable = true;
    this->unique = false;
    
    if(!nullable)
        this->nullable = false;
    if(unique)
        this->unique = true;
    if(pk){
        this->unique = this->pk = true;
        this->nullable = false;
    }
    if(fk)
        this->fk = true;
    
    this->nameOfColumn = nameOfColumn;
	columnSize = 0;
}

template <typename T>
std::string Column<T>::getName()
{
    return nameOfColumn;
}

template <typename T>
void Column<T>::addValue(const T &value, unsigned int index)
{
    T *buffer = &(const_cast<T&>(value));

    if(unique){
        for(int i = 0; i < columnSize; i++){
            if((*values[i]) == (*buffer)){
                std::cout << "Blad przy dodawaniu: kolumna musi miec unikalne pola" << std::endl;
                return;
            }
        }
    }
    
    if(index == ARG_NOT_PROVIDED)
        values.push_back(buffer);
    else{
        while(index >= columnSize){
            if(!nullable){
                std::cout << "Blad przy dodawaniu: kolumna nie moze miec pustych pol" << std::endl;
                return;
            }
            addNullValue(columnSize);
        }
        
        if(index < columnSize)
            values.erase(values.begin()+index);
        values.insert(values.begin()+index, buffer);
        return;

    }
    columnSize++;
    //TODO: Curses
}

template <typename T>
void Column<T>::addNullValue(unsigned int index)
{
    if(!nullable)
        return;
    if(index <= columnSize)
        values.push_back(NULL);
    else
        values.insert(values.begin()+index, NULL);
    columnSize++;
}

template <typename T>
void Column<T>::printColumn()
{
    for(int i = 0; i < values.size(); i++){
            if(values[i] == NULL)
                std::cout << " " << std::endl;
            else
                std::cout << *values[i] << std::endl;
    }
    //TODO: Curses
}

template <typename T>
void Column<T>::deleteValue(unsigned int index)
{
    if(!nullable){
        std::cout << "Blad przy usuwaniu: kolumna nie moze miec pustych pol" << std::endl;
        return;
        //TODO: Curses
    }
	if(columnSize <= index){
		std::cerr << "Niepoprawny indeks" << std::endl; //TODO: Curses
		return;
	}
    
    values[index] = NULL;
}

template <typename T>
void Column<T>::printValue(unsigned int index)
{
    if(index >= columnSize){
        std::cout << " ";
        return;
    }
    
    if(values[index] == NULL){
        std::cout << " "; // TODO: Curses
        return;
    }
    std::cout << *values[index];
}

template <typename T>
std::string Column<T>::streamPrint(unsigned int index)
{
    if(values[index] == NULL)
        return "";
    
    std::stringstream buffer;
    buffer << *values[index];
    return buffer.str();
}

template <typename T>
unsigned int Column<T>::getColumnSize()
{
	return columnSize;
}

template <typename T>
bool Column<T>::isNullable()
{
    return nullable;
}

template <typename T>
bool Column<T>::isPk()
{
    return pk;
}

template <typename T>
bool Column<T>::isFk()
{
    return fk;
}

// Dozwolone typy przechowywane w kolumnach
template class Column<bool>;
template class Column<int>;
template class Column<unsigned int>;
template class Column<double>;
template class Column<std::string>;
