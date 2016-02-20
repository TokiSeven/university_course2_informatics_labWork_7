//-----------------------------------
//----------->класс книги<-----------
//-----------------------------------
#include "book.h"
#include <fstream>
#include <QDebug>

//конструкторы и деструктор
book::book()//конструктор по-умлочанию
{
    //инициализация
    dataHuman author;
    dateStr dateCreation;
    string name;

    //присваивания начальные
    author.name = "Empty Name Author";
    author.surname = "Empty Surname Author";
    dateCreation.day = 1;
    dateCreation.month = 1;
    dateCreation.year = 2000;
    name = "Empty Name of book";

    //функции присваивания
    setAuthor(author);
    setDateCreation(dateCreation);
    setName(name);

    this->isGetDone = false;
    this->type = typeBook;
}
book::book(const book &bookCopy)//конструктор копированияяяя
{
    //функции присваивания
    this->setAuthor(bookCopy.author);
    this->setDateCreation(bookCopy.dateCreation);
    this->setName(bookCopy.name);

    this->isGetDone = false;
    this->type = typeBook;
}
book::book(const string name, const dataHuman author, const dateStr dateCreation)//конструктор с параметрами
{
    //функции присваивания
    this->setAuthor(author);
    this->setDateCreation(dateCreation);
    this->setName(name);

    this->isGetDone = false;
    this->type = typeBook;
}
book::~book()//деструктор
{
    //delete this;
}
//селекторы
const string book::getName()const//взятие имени
{
    //this->messageToDebug();
    return this->name;
}
const dataHuman book::getAuthor()const//взятие структуры автора
{
    //this->messageToDebug();
    return this->author;
}
const dateStr book::getDateCreation()const//взятие структуры даты создания книги
{
    //this->messageToDebug();
    return this->dateCreation;
}
//модификаторы
bool book::setAuthor(dataHuman author)//установка автора
{
    if (!author.name.empty() && !author.surname.empty())
    {
        this->author = author;
        return true;
    }
    else
    {
        this->author.name = "Empty name of author";
        this->author.surname = "Empty surname of author";
        return false;
    }
}
bool book::setDateCreation(dateStr dateCreation)//установка даты создания
{
    if (dateCreation.day >= 0 && dateCreation.day <= 31 && dateCreation.month >= 0 && dateCreation.month <= 12 && dateCreation.year >= 0 && dateCreation.year <=2015)
    {
        this->dateCreation = dateCreation;
        return true;
    }
    else
    {
        this->dateCreation.day = 1;
        this->dateCreation.month = 1;
        this->dateCreation.year = 2000;
        return false;
    }
}
bool book::setName(string name)//установка имени книги
{
    if (!name.empty())
    {
        this->name = name;
        return true;
    }
    else
    {
        this->name = "Empty name of book";
        return false;
    }
}
void book::editBook(dataHuman author, dateStr dateCreation, string name)
{
    this->setAuthor(author);
    this->setDateCreation(dateCreation);
    this->setName(name);
}

//---------------ОПЕРАТОРЫ----------
book book::operator = (book &bk)
{
    this->editBook(bk.getAuthor(), bk.getDateCreation(), bk.getName());
    return bk;
}

//перегруз операторов
bool book::operator == (book &book1) //оператор равенства
{
    return (this->getName() == book1.getName() &&
            this->getAuthor().name == book1.getAuthor().name &&
            this->getAuthor().surname == book1.getAuthor().surname &&
            this->getDateCreation().day == book1.getDateCreation().day &&
            this->getDateCreation().month == book1.getDateCreation().month &&
            this->getDateCreation().year == book1.getDateCreation().year)
            ? true : false;
}

bool book::operator > (book &book1) //оператор больше
{
    if (this->getName() > book1.getName())
        return true;
    if (this->getAuthor().name > book1.getAuthor().name)
        return true;
    if (this->getAuthor().surname > book1.getAuthor().surname)
        return true;
    if (this->getDateCreation().year > book1.getDateCreation().year)
        return true;
    if (this->getDateCreation().month > book1.getDateCreation().month)
        return true;
    if (this->getDateCreation().day > book1.getDateCreation().day)
        return true;
    return false;
}

//сохранение книги в файл
void book::saveToFile(ofstream &fout)
{
    fout << this->getName() << endl;
    fout << this->getAuthor().name << endl;
    fout << this->getAuthor().surname << endl;
    fout << this->getDateCreation().day << endl;
    fout << this->getDateCreation().month << endl;
    fout << this->getDateCreation().year << endl;
}

//чтение книги из файла
void book::loadFromFile(ifstream &fin)
{
    //инициализация
    string nameb;
    dataHuman author;
    dateStr dc;

    //считываю свой формат
    std::getline(fin, nameb);
    std::getline(fin, author.name);
    std::getline(fin, author.surname);
    fin >> dc.day;
    fin >> dc.month;
    fin >> dc.year;
    fin.get();

    this->editBook(author, dc, nameb);
}

//потоковое считование из файла
ifstream& book::operator << (ifstream &file)
{
    this->loadFromFile(file);
    return file;
}

//потоковое сохранение в файл
ofstream& book::operator >> (ofstream &file)
{
    this->saveToFile(file);
    return file;
}

void book::operator ()(dataHuman auth, dateStr dc, string name)
{
    this->editBook(auth, dc, name);
}

book book::operator +(book &bk)
{
    book bf(*this);
    dataHuman au = bf.getAuthor();
    au.name.append(", ");
    au.name.append(bk.getAuthor().name);
    au.surname.append(", ");
    au.surname.append(bk.getAuthor().surname);
    bf.editBook(au, bf.getDateCreation(), bf.getName());
    return bf;
}

void book::messageToDebug()const
{
    if (this->isGetDone == true)
        qDebug() << QString::fromStdString("Was done already from name ") + QString::fromStdString(this->name);
    else
    {
        qDebug() << QString::fromStdString("First getting information from ") + QString::fromStdString(this->name);
        this->isGetDone = true;
    }
}
