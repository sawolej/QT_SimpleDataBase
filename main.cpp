#include "database.h"
#include <QTextStream>
#include <QCoreApplication>
#include <QtSql>
#include <QApplication>
#include <QDebug>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataBase w;
    w.show();

    return a.exec();
}
