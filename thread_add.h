#ifndef THREAD_ADD_H
#define THREAD_ADD_H

#include "container.h"
#include <QMainWindow>
#include <QObject>
#include <QThread>
#include "dialog.h"

class Thread_Add : public QObject
{
    Q_OBJECT
public:
    Thread_Add(container<book> *cont2, Ui::Dialog *ui, book *curr) : ui(ui)
    {
        this->ID = ++id;
        this->cont = cont2;
        this->bookAdd = curr;
    }

signals:
    void finished();

public slots:
    void process();

private:
    int ID;
    static int id;
    container<book> *cont;
    Ui::Dialog *ui;
    book *bookAdd;
};

#endif // THREAD_ADD_H
