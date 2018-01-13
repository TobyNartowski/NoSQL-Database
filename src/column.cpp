#include "column.hpp"

#include <sstream>
#include <type_traits>

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
    tableName = "NULL";
	columnSize = 0;
}

template <typename T>
Column<T>::Column(std::string nameOfColumn, bool *flags)
{
    this->pk = this->fk = false;
    this->nullable = true;
    this->unique = false;

    if(flags[2])
        this->nullable = false;
    if(flags[3])
        this->unique = true;
    if(flags[0]){
        this->unique = this->pk = true;
        this->nullable = false;
    }
    if(flags[1])
        this->fk = true;

    this->nameOfColumn = nameOfColumn;
    tableName = "NULL";
	columnSize = 0;
}

template <typename T>
std::string Column<T>::getName()
{
    return nameOfColumn;
}

template <typename T>
void Column<T>::setTableName(std::string tableName)
{
    this->tableName = tableName;
}

template <typename T>
std::string Column<T>::getTableName()
{
    return tableName;
}

template <typename T>
void Column<T>::addValue(const T &value, int index)
{
    T *buffer = new T;
    *buffer = const_cast<T&>(value);

    if(unique){
        for(unsigned int i = 0; i < columnSize; i++){
            if((*values[i]) == (*buffer)){
                std::cout << "Blad przy dodawaniu: kolumna musi miec unikalne pola" << std::endl;
                return;
            }
        }
    }

    if(index == ARG_NOT_PROVIDED){
        values.push_back(buffer);
    }
    else{
        while((unsigned)index >= columnSize){
            if(!nullable){
                std::cout << "Blad przy dodawaniu: kolumna nie moze miec pustych pol" << std::endl;
                return;
            }
            addNullValue(columnSize);
        }

        if((unsigned)index < columnSize)
            values.erase(values.begin()+index);
        values.insert(values.begin()+index, buffer);

        buffer = NULL;
        delete buffer;
        return;
    }

    buffer = NULL;
    delete buffer;
    columnSize++;
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
void Column<T>::deleteValue(unsigned int index)
{
    if(!nullable){
        std::cout << "Blad przy usuwaniu: kolumna nie moze miec pustych pol" << std::endl;
        return;
    }
	if(columnSize <= index){
		std::cerr << "Niepoprawny indeks" << std::endl;
		return;
	}

    values[index] = NULL;
}

template <typename T>
std::string Column<T>::streamPrint(unsigned int index, bool filePrint)
{
    if(values[index] == NULL || (index >= columnSize)){
        if(filePrint)
            return "NULL";
        else
            return "-";
    }

    std::stringstream buffer;
    buffer << *values[index];

    if(whatType() == COL_BOOL){
        std::string a = buffer.str();
        if(buffer.str().at(0) - '0')
            return "Tak";
        else
            return "Nie";
    }

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

template <typename T>
bool Column<T>::isUnique()
{
    return unique;
}

template <typename T>
ColumnType Column<T>::whatType()
{
    if(std::is_same<T, bool>::value)
        return COL_BOOL;
    if(std::is_same<T, int>::value)
        return COL_INT;
    if(std::is_same<T, double>::value)
        return COL_DOUBLE;
    if(std::is_same<T, std::string>::value)
        return COL_STRING;
    return COL_ERROR;
}

// Dozwolone typy przechowywane w kolumnach
template class Column<bool>;
template class Column<int>;
template class Column<double>;
template class Column<std::string>;
