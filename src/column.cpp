#include "column.hpp"

// Implementacje metod
template <typename T>
Column<T>::Column(std::string nameOfColumn)
{
	this->nameOfColumn = nameOfColumn;
	columnSize = 0;
}

template <typename T>
void Column<T>::addValue(T value)
{
	values.push_back(value);
	columnSize++;
}

template <typename T>
void Column<T>::addNullValue()
{
    // TODO
    //values.push_back(reinterpret_cast<T>(NULL));
    columnSize++;
}

template <typename T>
void Column<T>::printColumn()
{
	for(int i = 0; i < values.size(); i++)
		std::cout << values[i] << std::endl;
}

template <typename T>
void Column<T>::deleteValueAtPos(unsigned int index)
{
	if(values.size() < index){
		std::cerr << "Niepoprawny indeks" << std::endl; //TODO: Curses
		return;
	}
	else
		values.erase(values.begin() + index - 1);
	columnSize--;
}

template <typename T>
std::vector<unsigned int> Column<T>::findValue(T value)
{
	std::vector <unsigned int>indexes;
	for(int i = 0; i < values.size(); i++)
		if(values[i] == value)
			indexes.push_back(i);
	std::cout << "Znaleziono " << indexes.size() << " razy" << std::endl; //TODO: Curses
	
	return indexes;
}

template <typename T>
void Column<T>::printValueAtPos(unsigned int index)
{
    if(index >= columnSize){
        std::cerr << "Niepoprawny indeks" << std::endl; //TODO: Curses
        return;
    }
    std::cout << values[index];
}

template <typename T>
unsigned int Column<T>::getColumnSize()
{
	return columnSize;
}


// Dozwolone typy przechowywane w kolumnach
template class Column<bool>;
template class Column<int>;
template class Column<unsigned int>;
template class Column<double>;
template class Column<std::string>;
