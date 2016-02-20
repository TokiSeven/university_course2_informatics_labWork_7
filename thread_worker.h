#ifndef THREAD_WORKER_H
#define THREAD_WORKER_H

#include "container.h"
#include <QMainWindow>
#include <QObject>
#include <QThread>
#include "dialog.h"

class Thread_Worker : public QObject
{
    Q_OBJECT
public:
    Thread_Worker(container<book> *cont2, Ui::Dialog *ui, QString file_name) : ui(ui)
    {
        this->ID = ++id;
        this->cont = cont2;
        fileName = file_name;
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
    QString fileName;
};

#endif // THREAD_WORKER_H
