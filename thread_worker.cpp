#include "thread_worker.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>

int Thread_Worker::id = 0;

void Thread_Worker::process()
{
    QString message = "WORKER ID:" + QString::number(ID);
    qDebug() << message + " STARTED";

    cont->load(fileName.toStdString());

    qDebug() << message + " FINISHED";
    emit finished();
}
