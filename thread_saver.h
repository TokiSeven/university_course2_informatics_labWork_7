#ifndef THREAD_SAVER_H
#define THREAD_SAVER_H

#include "container.h"
#include <QMainWindow>
#include <QObject>
#include <QThread>
#include "dialog.h"

class thread_saver : public QObject
{
    Q_OBJECT
public:
    thread_saver(container<book> *cont2, Ui::Dialog *ui, QString file_name) : ui(ui)
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

#endif // THREAD_SAVER_H
