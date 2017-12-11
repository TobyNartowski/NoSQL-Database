#include "column.hpp"

#include <sstream>

// Implementacje metod
template <typename T>
Column<T>::Column(std::string nameOfColumn, bool nullable)
{
    if(nullable)
        this->nullable = true;
    
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
    if(index == 0)
        values.push_back(buffer);
    else{
        while(index > columnSize){
            if(!nullable){
                std::cout << "Blad przy dodawaniu: kolumna nie moze miec pustych pol" << std::endl;
                return;
            }
            addNullValue(columnSize);
        }
            //TODO: Curses
        values.insert(values.begin()+index, buffer);
    }
	columnSize++;
}

template <typename T>
void Column<T>::addNullValue(unsigned int index)
{
    if(!nullable)
        return;
    
    values.insert(values.begin()+index, NULL);
    columnSize++;
}

template <typename T>
void Column<T>::printColumn()
{
	for(int i = 0; i < values.size(); i++)
		std::cout << values[i] << std::endl;
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
	if(values.size() < index){
		std::cerr << "Niepoprawny indeks" << std::endl; //TODO: Curses
		return;
	}
    
    addNullValue(index);
	columnSize--;
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


// Dozwolone typy przechowywane w kolumnach
template class Column<bool>;
template class Column<int>;
template class Column<unsigned int>;
template class Column<double>;
template class Column<std::string>;
