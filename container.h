#ifndef CONTAINER
#define CONTAINER

#include "header.h"
#include <QQueue>
#include "book.h"
#include "journal.h"
#include <algorithm>
#include <QDebug>
#include <fstream>
#include <QMutex>

//------------------------------------
//--------->класс контейнера<---------
//------------------------------------
template <class T>
class container
{
public:
    //                   <<!<<!<< --- конструкторы и деструктор --- >>!>>!>>
    container();                                              //конструктор
    container(container&);                               //конструктор копирования
    ~container();                                             //деструктор

    //                   <<!<<!<< --- методы --- >>!>>!>>
    template <typename T2>
    T* addBook(T2*);                                         //добавим книгу в храниище
    T* deleteBook(int);                                     //удаление книги
    bool isEmpty();                                           //пустой ли список?
    bool isFull(){return !isEmpty();}                         //полон ли список?
    int getSizeOfBook()const{return books.size();}            //взятие кол-ва книг
    void sortName(int);                                       //сортировка каким-либо методом (от int зависит)
    void save(const string);                                  //сохранение всех данных из контйнера
    void load(const string);                                  //загрузка всех данных в контейнер
    book* bookFactory(TypeOfThis type);                       //завод книг

    //                   <<!<<!<< --- перегруз операторов --- >>!>>!>>
    T* operator [] (int i){return books[i];}                  //индексация в массиве
    template <typename T2>
    container operator + (const T2&);                                     //добавление книги в очередь
    container operator - (int);                                    //удаление книги
    container operator * (container&);                             //берем из второго контейнера данные и пихаем в наш
    container operator / (container&);                             //исключаем из нашего то, что есть в другом
    container operator = (container&);
    T* operator () (int i){return (*this)[i];}                //аналог []
    T* operator ++ ();                                        //добавление пустой книги в очередь
    T* operator ++ (int){return ++(*this);}                   //аналог ++
    T* operator -- ();                                      //удаляет последнюю книгу из очереди
    T* operator -- (int){return --(*this);}                        //аналог --
    friend ifstream& operator >> (ifstream &file, T *&bk)  //дружественная функция для загрузки из файла
    {
        bk->loadFromFile(file);
        return file;
    }
    friend ofstream& operator << (ofstream &file, T *bk)   //дружественная функция для сохранения в файл
    {
        bk->saveToFile(file);
        return file;
    }
    template <typename T2>
    T* sum(T*, T2*);
    QMutex locker;

private:
    //                   <<!<<!<< --- поля --- >>!>>!>>
    QQueue<T*> books;                                         //наша очередь книг
};

//--------------------------------------------------------------------------------------------------
//                       <<!<<!<< --- Сами функции, методы, операторы --- >>!>>!>>
//--------------------------------------------------------------------------------------------------

template <class T>
//конструкторы и деструктор
container<T>::container()//конструктор по-умолчанию
{
}

template <class T>
container<T>::container(container &cont)
{
    for (int i = 0; i < cont.getSizeOfBook(); i++)
        this->addBook(cont[i]);
}

template <class T>
container<T>::~container()//деструктор
{
}

//методы
template <class T>
template <typename T2>
T* container<T>::addBook(T2 *newBook)//скопируем книжку
{
    locker.lock();
    books.append(newBook);
    locker.unlock();
    return newBook;
}

template <class T>
T* container<T>::deleteBook(int i)//удаление книги
{
    //i - её номер с начала
    locker.lock();
    book *res = books[i];
    if (i >= 0 && i < books.size())
        books.removeAt(i);
    locker.unlock();
    return res;
}

template <class T>
bool container<T>::isEmpty()//пустой ли список?
{
    return books.empty();
}

template <class T>
//сортировка книг, в зависимости от метода
void container<T>::sortName(int k)
{
    locker.lock();
    for (int i = 0; i < books.size() - 1; i++)
        for (int j = i + 1; j < books.size(); j++)
        {
            qDebug() << i;
            switch(k)
            {
            case 1:
                if (((*books[i]) > (*books[j])))
                    swap(books[i], books[j]);
                break;
            case 2:
                if (((*books[i]) >= (*books[j])))
                    swap(books[i], books[j]);
                break;
            case 3:
                if (((*books[i]) < (*books[j])))
                    swap(books[i], books[j]);
                break;
            case 4:
                if (((*books[i]) <= (*books[j])))
                    swap(books[i], books[j]);
                break;
            }
        }
    locker.unlock();
}

template <class T>
//сохранение в файл
void container<T>::save(const string nameFile)
{
    locker.lock();
    //открытия файла
    ofstream fout(nameFile.c_str(), ios_base::trunc);
    if (!fout.is_open())
        return;

    //сохранение каждой книги (сама знает, как сохранться)
    int tmp;
    for (int i = 0; i < books.size(); i++)
    {
        tmp = books[i]->getType();

        fout << "->NEW<-" << endl;
        fout.write((char*)&tmp, sizeof(int));
        fout << endl;
        books[i]->saveToFile(fout);
    }

    fout.close();
    locker.unlock();
}

template <class T>
//загрузка из файла
void container<T>::load(const string nameFile)
{
    books.clear();
    ifstream fin(nameFile.c_str());
    if (!fin.is_open())
        return;

    //немного переменных для считывания данных
    string currLine;
    string buff, buff2;

    //пока не конец файла
    while(fin)
    {
        //если текущая строка - новая книга
        std::getline(fin, currLine);
        if (currLine == "->NEW<-")
        {
            T *newb;
            TypeOfThis type;
            fin.read((char*)&type, sizeof(TypeOfThis));
            fin.get();

            newb = bookFactory(type);
            newb->loadFromFile(fin);

            this->addBook(newb);
        }
    }
    fin.close();
}

template <class T>
//фабрика книг     !!!ВАЖНО!!!
book* container<T>::bookFactory(TypeOfThis type)
{
    //СПИСОК ВСЕХ ВОЗМОЖНЫХ ТИПОВ КНИГ
    if (type == typeBook) return new book;
    if (type == typeJournal) return new journal;
    return NULL;
}

//добавление в контейнер книги
template <class T>
template <typename T2>
container<T> container<T>::operator +(const T2 &curr)
{
    container<T> cont(*this);
    cont.addBook(curr);
    return cont;
}

template <class T>
//удаление из контейнера книги
container<T> container<T>::operator - (int i)
{
    container cont(*this);
    cont.deleteBook(i);
    return cont;
}

template <class T>
//добавление в контейнер элементы другого контейнера
container<T> container<T>::operator * (container &cont)
{
    container res(*this);
    for (int i = 0; i < cont.getSizeOfBook(); i++)
        res->addBook(cont[i]);
    return res;
}

template <class T>
//удаление из контейнера элементы другого контейнера
container<T> container<T>::operator / (container &cont)
{
    container res(*this);
    for (int i = 0; i < res.getSizeOfBook();)
    {
        bool flag = false;
        for (int j = 0; j < cont.getSizeOfBook(); j++)
            if (res[i] == cont[j])
            {
                res.deleteBook(i);
                flag = true;
                break;
            }
        if (!flag)
            i++;
    }
    return res;
}

template <class T>
//добавление пустой книги в контейнер
T* container<T>::operator ++()
{
    T *curr = new T;
    this->addBook(curr);
    return curr;
}

template <class T>
//удаление последней книги из контейнера
T* container<T>::operator -- ()
{
    T *res = (*this)[this->getSizeOfBook() - 1];
    if (this->getSizeOfBook() > 0)
        this->deleteBook(this->getSizeOfBook() - 1);
    return res;
}

template <class T>
container<T> container<T>::operator = (container<T> &cont)
{
    this->books.clear();
    for (int i = 0; i < cont.getSizeOfBook(); i++)
        this->addBook(cont[i]);
}

template <class T>
template <typename T2>
T* container<T>::sum(T *one, T2 *two)
{
    T *curr = new T((*one + *two));
    return curr;
}

#endif // CONTAINER
