#ifndef JOURNAL_H
#define JOURNAL_H

#include "book.h"

class journal : public book
{
public:
    //конструкторы и деструкторы
    journal();//по-умолчанию
    journal(const journal&);//копирования
    journal(const string, const dataHuman, const dateStr, const int, const string);//с параметрами
    ~journal();//деструкторы

    //селекторы
    int getNumber()const{return number;}//взять номер выпуска журнала
    const string getTheme()const{return theme;}//взять тему журнала

    //модификаторы
    void setNumber(const int);//задать номер выпуска
    void setTheme(const string);//задать тему журнала

    //методы
    void editBook(dataHuman, dateStr, string, string, int);//редактирование журнала
    void saveToFile(ofstream&);//сохранение в файл
    void loadFromFile(ifstream&);//загрузка из файла

    //операторы
    journal operator = (journal&);//присваивание
    virtual ifstream &operator << (ifstream &file);//аналог загрузке
    virtual ofstream &operator >> (ofstream &file);//аналог сохранению
    void operator () (dataHuman, dateStr, string, string, int);//оператор круглые скобки
private:
    int number;//номер выпуска журнала
    string theme;//тема журнала
};

#endif // JOURNAL_H
