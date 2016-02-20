#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

//threads
#include "thread_worker.h"
#include "thread_saver.h"
#include "thread_add.h"
#include "thread_delete.h"

typedef book* bookUk;
typedef journal* jourUk;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //autoload from file
    loadThread("books.txt");

    //some default options
    ui->listWidget_3->setMaximumWidth(170);

    //signals - slots
    connect(ui->addAuthorN, SIGNAL(textChanged(QString)), this, SLOT(changeAddMenu()));
    connect(ui->addAuthorS, SIGNAL(textChanged(QString)), this, SLOT(changeAddMenu()));
    connect(ui->addName, SIGNAL(textChanged(QString)), this, SLOT(changeAddMenu()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::showAllBooks()
{
    QListWidget *widget1 = ui->listWidget_1;
    QListWidget *widget3 = ui->listWidget_3;

    //clear my widgetlists
    widget1->clear();
    widget3->clear();

    //show books in widgetlists
    for (int i = 0; i < containerMy.getSizeOfBook(); i++)
    {
        widget1->addItem("Книга номер: " + QString::number(i+1));
        showCurrentBook(*containerMy[i], widget1);

        widget3->addItem(QString::fromStdString(containerMy[i]->getName()));
    }

    if (!containerMy.getSizeOfBook())
        widget1->addItem("Нет книг.");
}

void Dialog::showCurrentBook(book &buff, QListWidget *widget)
{
    widget->addItem("  Название книги: " + QString::fromStdString(buff.getName()));
    widget->addItem("  Имя автора книги: " + QString::fromStdString(buff.getAuthor().name));
    widget->addItem("  Фамилия автора книги: " + QString::fromStdString(buff.getAuthor().surname));
    widget->addItem("  Дата создания книги: " + QString::number(buff.getDateCreation().day) + "." + QString::number(buff.getDateCreation().month) + "." + QString::number(buff.getDateCreation().year));
    widget->addItem("  Тип книги: " + QString::fromUtf8((buff.getType() == typeBook) ? "Книга" : "Журнал"));
    if (buff.getType() == typeJournal)
    {
        journal *buff2 = (journal*)&buff;
        widget->addItem("  Тема журнала: " + QString::fromStdString(buff2->getTheme()));
        widget->addItem("  Номер выпуска: " + QString::number(buff2->getNumber()));
    }
}

//if book are selected will start this slot
void Dialog::on_listWidget_3_itemActivated(QListWidgetItem *item)
{
    //initialization
    int num = ui->listWidget_3->row(item);
    book &buff = *containerMy[num];
    int y = buff.getDateCreation().year;
    int m = buff.getDateCreation().month;
    int d = buff.getDateCreation().day;

    //also inicialization
    QDate date(y, m, d);
    QString strId = QString::number(num);
    QString strName = QString::fromStdString(buff.getName());
    QString strAuthorN = QString::fromStdString(buff.getAuthor().name);
    QString strAuthorS = QString::fromStdString(buff.getAuthor().surname);

    //for tab_3 (edit tab)
    ui->nameEdit->setText(strName);
    ui->authorEdit1->setText(strAuthorN);
    ui->authorEdit2->setText(strAuthorS);
    ui->dataEdit->setDate(date);
    ui->tab_3->setEnabled(true);
    if (buff.getType() == typeJournal)
    {
        journal *buff2 = (journal*)&buff;
        ui->themeEdit->setText(QString::fromStdString(buff2->getTheme()));
        ui->numberEdit->setValue(buff2->getNumber());
        ui->themeEdit->show();
        ui->numberEdit->show();
    }
    else
    {
        ui->themeEdit->hide();
        ui->numberEdit->hide();
    }

    //for tab_4 (delete tab)
    ui->deleteName->setText(strName);
    ui->deleteAuthorN->setText(strAuthorN);
    ui->deleteAuthorS->setText(strAuthorS);
    ui->deleteDate->setText(date.toString("dd.MM.yyyy"));
    ui->tab_4->setEnabled(true);
    if (buff.getType() == typeJournal)
    {
        journal *buff2 = (journal*)&buff;
        ui->deleteTheme->setText(QString::fromStdString(buff2->getTheme()));
        ui->deleteNumber->setText(QString::number(buff2->getNumber()));
        ui->deleteTheme->show();
        ui->deleteNumber->show();
    }
    else
    {
        ui->deleteTheme->hide();
        ui->deleteNumber->hide();
    }
}

void Dialog::on_pushButton_clicked()
{
    int num = ui->listWidget_3->row(ui->listWidget_3->currentItem());
    book &buff = *containerMy[num];
    dataHuman author;
    dateStr date;
    string name;

    author.name = ui->authorEdit1->text().toStdString();
    author.surname = ui->authorEdit2->text().toStdString();
    date.day = ui->dataEdit->date().day();
    date.month = ui->dataEdit->date().month();
    date.year = ui->dataEdit->date().year();
    name = ui->nameEdit->text().toStdString();
    if (buff.getType() == typeJournal)
    {
        journal *buff2 = (journal*)&buff;
        buff2->editBook(author, date, name, ui->themeEdit->text().toStdString(), ui->numberEdit->value());
    }
    else
        buff.editBook(author, date, name);
    update();
}

//update запускаеть надо всегда, когда что-то меняется в контейнере
void Dialog::update()
{
    saveThread("books.txt");
    showAllBooks();

    ui->tab_3->setEnabled(false);
    ui->tab_4->setEnabled(false);

    ui->addAuthorN->setText("");
    ui->addAuthorS->setText("");
    ui->addName->setText("");
    ui->addTheme->setText("");
    ui->addNumber->setValue(0);
    if (ui->addSelectType->currentText() == "Книга")
    {
        ui->addTheme->hide();
        ui->addNumber->hide();
    }
    else
    {
        ui->addTheme->show();
        ui->addNumber->show();
    }

    ui->nameEdit->setText("");
    ui->authorEdit1->setText("");
    ui->authorEdit2->setText("");
    ui->themeEdit->setText("");
    ui->numberEdit->setValue(0);

    ui->deleteAuthorN->setText("Имя автора книги");
    ui->deleteAuthorS->setText("Фамилия автора книги");
    ui->deleteName->setText("Название книги");
    ui->deleteTheme->setText("Тема журнала");
    ui->deleteNumber->setText("Номер журнала");

    if (!ui->listWidget_3->item(0))
        ui->buttonSort->setEnabled(false);
    else
        ui->buttonSort->setEnabled(true);
}

void Dialog::on_deleteButton_clicked()
{
    int num = ui->listWidget_3->row(ui->listWidget_3->currentItem());
    int result = QMessageBox::information(this, "Удаление книги", "Вы уверенны, что хотите удалить эту книгу?", QMessageBox::Yes | QMessageBox::No);
    switch(result)
    {
    case QMessageBox::Yes:
        //containerMy.deleteBook(num);
        deleteThread(num);
        break;
    }
    update();
}

void Dialog::on_addButton_clicked()
{
    dataHuman author;
    dateStr date;
    string name;

    name = ui->addName->text().toStdString();
    author.name = ui->addAuthorN->text().toStdString();
    author.surname = ui->addAuthorS->text().toStdString();
    date.day = ui->addDate->date().day();
    date.month = ui->addDate->date().month();
    date.year = ui->addDate->date().year();

    if (ui->addSelectType->currentText() == "Книга")
    {
        book *bookNew = new book(name, author, date);
        //containerMy.addBook(bookNew);
        addThread(bookNew);
    }
    else
    {
        string theme;
        int num;

        theme = ui->addTheme->text().toStdString();
        num = ui->addNumber->value();

        journal *jourNew = new journal(name, author, date, num, theme);
        //containerMy.addBook(jourNew);
        addThread(jourNew);
    }

    update();
}

void Dialog::changeAddMenu()
{
    if (!ui->addName->text().isEmpty() && !ui->addAuthorN->text().isEmpty() && !ui->addAuthorS->text().isEmpty())
        ui->addButton->setEnabled(true);
    else
        ui->addButton->setEnabled(false);
}

void Dialog::on_buttonSort_clicked()
{
    containerMy.sortName(1);
    update();
}

void Dialog::on_addSelectType_activated(const QString &arg1)
{
    ui->addTheme->setText("");
    ui->addNumber->setValue(0);
    if (arg1 == "Книга")
    {
        ui->addTheme->hide();
        ui->addNumber->hide();
    }
    else
    {
        ui->addTheme->show();
        ui->addNumber->show();
    }
}

void Dialog::on_buttonSave_clicked()
{
    QString file_name = QFileDialog::getSaveFileName(this, "title", "books", "Text Files(*.txt)");
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly))
        return;
    file.close();
    saveThread(file_name);
}

void Dialog::on_buttonLoad_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), tr(""), tr("Text Files (*.txt)"));
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
        return;
    file.close();
    loadThread(file_name);
}

void Dialog::loadThread(QString file_name)
{
    // Создание потока
    QThread* thread = new QThread;
    Thread_Worker* worker = new Thread_Worker(&containerMy, this->ui, file_name);

    // Передаем права владения "рабочим" классом, классу QThread.
    worker->moveToThread(thread);

    // Соединяем сигнал started потока, со слотом process "рабочего" класса, т.е. начинается выполнение нужной работы.
    connect(thread, SIGNAL(started()), worker, SLOT(process()));

    // По завершению выходим из потока, и удаляем рабочий класс
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    connect(worker, SIGNAL(finished()), this, SLOT(updateSLOT()));
    // Удаляем поток, после выполнения операции
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void Dialog::saveThread(QString file_name)
{
    // Создание потока
    QThread* thread = new QThread;
    thread_saver* worker = new thread_saver(&containerMy, this->ui, file_name);

    // Передаем права владения "рабочим" классом, классу QThread.
    worker->moveToThread(thread);

    // Соединяем сигнал started потока, со слотом process "рабочего" класса, т.е. начинается выполнение нужной работы.
    connect(thread, SIGNAL(started()), worker, SLOT(process()));

    // По завершению выходим из потока, и удаляем рабочий класс
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    //connect(worker, SIGNAL(finished()), this, SLOT(updateSLOT()));
    // Удаляем поток, после выполнения операции
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void Dialog::addThread(book *bk)
{
    // Создание потока
    QThread* thread = new QThread;
    Thread_Add* worker = new Thread_Add(&containerMy, this->ui, bk);

    // Передаем права владения "рабочим" классом, классу QThread.
    worker->moveToThread(thread);

    // Соединяем сигнал started потока, со слотом process "рабочего" класса, т.е. начинается выполнение нужной работы.
    connect(thread, SIGNAL(started()), worker, SLOT(process()));

    // По завершению выходим из потока, и удаляем рабочий класс
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    //connect(worker, SIGNAL(finished()), this, SLOT(updateSLOT()));
    // Удаляем поток, после выполнения операции
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void Dialog::deleteThread(int i)
{
    // Создание потока
    QThread* thread = new QThread;
    Thread_Delete* worker = new Thread_Delete(&containerMy, this->ui, i);

    // Передаем права владения "рабочим" классом, классу QThread.
    worker->moveToThread(thread);

    // Соединяем сигнал started потока, со слотом process "рабочего" класса, т.е. начинается выполнение нужной работы.
    connect(thread, SIGNAL(started()), worker, SLOT(process()));

    // По завершению выходим из потока, и удаляем рабочий класс
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    //connect(worker, SIGNAL(finished()), this, SLOT(updateSLOT()));
    // Удаляем поток, после выполнения операции
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
