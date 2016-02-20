#include "thread_add.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>

int Thread_Add::id = 0;

void Thread_Add::process()
{
    QString message = "ADDER ID:" + QString::number(ID);
    qDebug() << message + " STARTED";

    cont->addBook(this->bookAdd);

    qDebug() << message + " FINISHED";
    emit finished();
}
