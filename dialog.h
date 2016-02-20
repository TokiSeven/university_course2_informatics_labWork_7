#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "header.h"
#include "book.h"
#include "journal.h"
#include "container.h"
#include "ui_dialog.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void showAllBooks();//вывод всех книг
    void showCurrentBook(book&, QListWidget*);//вывод текущей книги
    void update();//апдейт - вызывается после любого действия пользователя (которые влияют на данные)
    void loadThread(QString);  //поток загрузки контейнера из файла
    void saveThread(QString);  //поток сохранения контейнера в файл
    void addThread(book*);     //поток добавление книги в контейнер
    void deleteThread(int);    //поток удаления книги из контейнера

private slots:
    void on_listWidget_3_itemActivated(QListWidgetItem *item);//выбран элемент из маленького списка
    void on_pushButton_clicked();//редактирование книги тыкнута
    void on_deleteButton_clicked();//удаление книги
    void on_addButton_clicked();//добавление книги в контейнер
    void changeAddMenu();//срабатывает при изменении имени в поле для добавления книги в контейнер
    void on_buttonSort_clicked();//кнопка сортировки
    void on_addSelectType_activated(const QString &arg1);//при обавлении элемента - выпадающий список - тип книги
    void on_buttonSave_clicked();//сохранение в файл
    void on_buttonLoad_clicked();//загрузка из файла
    void updateSLOT(){update();}

private:
    Ui::Dialog *ui;
    container<book> containerMy;
};

#endif // DIALOG_H
