#include "thread_delete.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>

int Thread_Delete::id = 0;

void Thread_Delete::process()
{
    QString message = "DELETER ID:" + QString::number(ID);
    qDebug() << message + " STARTED";

    book *cr = cont->deleteBook(numberDelete);
    delete cr;

    qDebug() << message + " FINISHED";
    emit finished();
}
