#include "thread_saver.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>

int thread_saver::id = 0;

void thread_saver::process()
{
    QString message = "SAVER ID:" + QString::number(ID);
    qDebug() << message + " STARTED";

    cont->save(fileName.toStdString());

    qDebug() << message + " FINISHED";
    emit finished();
}
