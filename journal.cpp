#include "journal.h"
#include <fstream>

//конструкторы и деструкторы
journal::journal() : book()
{
    this->setNumber(0);
    this->setTheme("Empty theme of Journal");
    this->type = typeJournal;
}

journal::journal(const journal &jour) : book((book)jour)//book(jour.getName(), jour.getAuthor(), jour.getDateCreation())
{
    this->setNumber(jour.getNumber());
    this->setTheme(jour.getTheme());
    this->type = typeJournal;
}

journal::journal(const string name, const dataHuman author, const dateStr date, const int num, const string theme) : book(name, author, date)
{
    this->setNumber(num);
    this->setTheme(theme);
    this->type = typeJournal;
}

journal::~journal()
{
    //delete this;
}

//модификаторы
void journal::setNumber(const int num)
{
    if (num >= 0)
        this->number = num;
    else
        this->number = 0;
}

void journal::setTheme(const string theme)
{
    if (!theme.empty())
        this->theme = theme;
    else
        this->theme = "Empty theme";
}

//редактирование журнала
void journal::editBook(dataHuman author, dateStr dateCreation, string name, string theme, int number)
{
    book::editBook(author, dateCreation, name);
    this->setTheme(theme);
    this->setNumber(number);
}

//сохранение журнала в файл
void journal::saveToFile(ofstream &fout)
{
    //сохраняем сначало как книгу, а потом дописываем поля журнала
    book::saveToFile(fout);
    fout << this->getTheme() << endl;
    fout << this->getNumber() << endl;
}

//загрузка из файла
void journal::loadFromFile(ifstream &fin)
{
    book::loadFromFile(fin);

    string theme;
    int number;

    std::getline(fin, theme);
    fin >> number;
    fin.get();

    this->editBook(this->getAuthor(), this->getDateCreation(), this->getName(), theme, number);
}

//опреаторы
journal journal::operator = (journal &jr)
{
    this->editBook(jr.getAuthor(), jr.getDateCreation(), jr.getName(), jr.getTheme(), jr.getNumber());
    return jr;
}

//потоковое считывание из файла
ifstream& journal::operator << (ifstream &file)
{
    this->loadFromFile(file);
}

//потоковое сохранение в файл
ofstream& journal::operator >> (ofstream &file)
{
    this->saveToFile(file);
}

void journal::operator ()(dataHuman auth, dateStr dc, string name, string theme, int num)
{
    this->editBook(auth, dc, name, theme, num);
}
