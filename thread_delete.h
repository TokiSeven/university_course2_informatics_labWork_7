#ifndef THREAD_DELETE_H
#define THREAD_DELETE_H

#include "container.h"
#include <QMainWindow>
#include <QObject>
#include <QThread>
#include "dialog.h"

class Thread_Delete : public QObject
{
    Q_OBJECT
public:
    Thread_Delete(container<book> *cont2, Ui::Dialog *ui, int i) : ui(ui)
    {
        this->ID = ++id;
        this->cont = cont2;
        numberDelete = i;
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
    int numberDelete;
};

#endif // THREAD_DELETE_H
