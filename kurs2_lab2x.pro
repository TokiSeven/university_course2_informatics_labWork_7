#-------------------------------------------------
#
# Project created by QtCreator 2015-09-28T20:48:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kurs2_lab2x
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    book.cpp \
    journal.cpp \
    thread_worker.cpp \
    thread_saver.cpp \
    thread_add.cpp \
    thread_delete.cpp

HEADERS  += dialog.h \
    book.h \
    container.h \
    header.h \
    journal.h \
    thread_worker.h \
    thread_saver.h \
    thread_add.h \
    thread_delete.h

FORMS    += dialog.ui
