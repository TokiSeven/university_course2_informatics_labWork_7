#ifndef BOOK
#define BOOK

#include <header.h>

//структура для дат
struct dateStr
{
    int year;//год
    int month;//месяц(1-12)
    int day;//день(1-31)
};

//структура для описания людей
struct dataHuman
{
    string name;//имя человека
    string surname;//его фамилия
};

//перечисляемый тип для типов книг
enum TypeOfThis {typeBook, typeJournal};

//-------------------------------
//--------->класс книги<---------
//-------------------------------
class book
{
public:
    //конструкторы и деструктор
    book();//конструктор по умолчанию
    book(const book&);//конструктор копирования книги
    book(const string, const dataHuman, const dateStr);//конструктор
    ~book();//деструктор

    //селекторы
    const string getName()const;//взятие имени
    const dataHuman getAuthor()const;//взятие автора
    const dateStr getDateCreation()const;//взятие даты создания
    TypeOfThis getType()const{return type;}//взять тип книги

    //модификаторы
    bool setAuthor(dataHuman);//установка автора
    bool setDateCreation(dateStr);//установка даты создания
    bool setName(string);//установка имени книги

    //методы
    void editBook(dataHuman, dateStr, string);//редактирование книги
    virtual void saveToFile(ofstream&);//сохранение в файл
    virtual void loadFromFile(ifstream&);//загрузка из файла
    void messageToDebug()const;

    //перегруз операторов
    virtual book operator = (book&);//присваивание (копирование всех полей)
    bool operator == (book&);//сравнение книг
    bool operator != (book &book1) {return !(*this == book1) ? true : false;}//логический оператор
    bool operator > (book&);//логический оператор
    bool operator >= (book &book1) {return (*this > book1 || *this == book1) ? true : false;}//логический оператор
    bool operator < (book &book1) {return !(*this >= book1) ? true : false;}//логический оператор
    bool operator <= (book &book1) {return !(*this > book1) ? true : false;}//логический оператор
    virtual ifstream &operator << (ifstream &file);//аналог загрузки из файла
    virtual ofstream &operator >> (ofstream &file);//аналог сохранению в файл
    void operator () (dataHuman, dateStr, string);//оператор скобки
    book operator + (book&);
protected:
    //поля
    dataHuman author;//инфо об авторе
    dateStr dateCreation;//дата создания книги
    string name;//имя книги
    TypeOfThis type;
    mutable bool isGetDone;
};

#endif // BOOK

