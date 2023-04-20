#include "database.h"
#include <QtSql>
#include <QString>
#include "./ui_database.h"



DataBase::DataBase(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DataBase)
{
    ui->setupUi(this);

    setUpDB();
    setUpView();
    makeConnects();
    addDummyValues();
}

//==================================== SET UP =====================================
void DataBase::makeConnects(){

    connect(ui->delButton, SIGNAL(released()), this, SLOT(delRow()));
    connect(ui->delButton_2 , SIGNAL(released()), this, SLOT(delRow()));
    connect(ui->delButton_3 , SIGNAL(released()), this, SLOT(delRow()));
    connect(ui->AddBookBtn , SIGNAL(released()), this, SLOT(AddBook()));
    connect(ui->AddUserBtn , SIGNAL(released()), this, SLOT(AddUser()));
    connect(ui->gimmeThatBookBtn , SIGNAL(released()), this, SLOT(borrow()));

    connect(ui->bookBtn,&QPushButton::released, this,[this]() {ui->pages->setCurrentIndex(1);});
    connect(ui->usrBtn,&QPushButton::released, this,[this]() {ui->pages->setCurrentIndex(2);});
    connect(ui->mixBtn,&QPushButton::released, this,[this]() {ui->pages->setCurrentIndex(3);});
    connect(ui->backToAddinBtn,&QPushButton::released, this,[this]() {ui->userPages->setCurrentIndex(0);});
    connect(ui->backToAddinBookBtn,&QPushButton::released, this,[this]() {ui->booksPages->setCurrentIndex(0);});

    connect(ui->updateUsrBtn,SIGNAL(released()), this, SLOT(makeUpdateUsr()));
    connect(ui->updateBookBtn,SIGNAL(released()), this, SLOT(makeUpdateBook()));

    connect(ui->tableBooks, &QTableView::clicked, this, &DataBase::onRowClicked);
    connect(ui->tableUsers, &QTableView::clicked, this, &DataBase::onRowClicked);
    connect(ui->tableBooks_2, &QTableView::clicked, this, &DataBase::onRowClicked);
    connect(ui->tableUsers_2, &QTableView::clicked, this, &DataBase::onRowClicked);


}
void DataBase::setUpView(){

    booksTableModel = new QSqlTableModel(this, db);
    booksTableModel->setTable("books");
    booksTableModel->select();

    ui->tableBooks->setModel(booksTableModel);
    ui->tableBooks->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableBooks->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableBooks->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableBooks->horizontalHeader()->setStretchLastSection(true);

    ui->tableBooks_2->setModel(booksTableModel);
    ui->tableBooks_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableBooks_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableBooks_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableBooks_2->horizontalHeader()->setStretchLastSection(true);

    usersTableModel = new QSqlTableModel(this, db);
    usersTableModel->setTable("users");
    usersTableModel->select();

    ui->tableUsers->setModel(usersTableModel);
    ui->tableUsers->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableUsers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableUsers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableUsers->horizontalHeader()->setStretchLastSection(true);

    ui->tableUsers_2->setModel(usersTableModel);
    ui->tableUsers_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableUsers_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableUsers_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableUsers_2->horizontalHeader()->setStretchLastSection(true);


    borrowsTableModel = new QSqlTableModel(this, db);
    borrowsTableModel->setTable("user_books");
    borrowsTableModel->select();

    ui->tableBookBorrows->setModel(borrowsTableModel);
    ui->tableBookBorrows->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableBookBorrows->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableBookBorrows->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableBookBorrows->horizontalHeader()->setStretchLastSection(true);
    ui->tableBookBorrows->setColumnHidden(1, true);
    ui->tableBookBorrows->setColumnHidden(2, true);
    ui->tableBookBorrows->setColumnHidden(0, true);

}


void DataBase::setUpDB(){

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./bookstore.db");
    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError().text();
    }
    if (!db.isValid()) {
        qDebug() << "Database not valid, driver not loaded";
        return;
    }

    QSqlQuery query(db);


    if (!query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, surname TEXT)")) {
          qDebug() << "Error creating table:" << query.lastError().text();

      }
    if (!query.exec("CREATE TABLE IF NOT EXISTS books (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, author TEXT)")) {
          qDebug() << "Error creating table:" << query.lastError().text();

      }
    if (!query.exec("CREATE TABLE IF NOT EXISTS user_books ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "user_id INTEGER, "
                    "book_id INTEGER, "
                    "book_title TEXT, "
                    "user_name TEXT, "
                    "FOREIGN KEY(user_id) REFERENCES users(id), "
                    "FOREIGN KEY(book_id) REFERENCES books(id))")) {
        qDebug() << "Error creating table:" << query.lastError().text();
    }

}



//==================================== DB =====================================
void DataBase::borrow(){
    addToDBBorrows(IndexBook.toInt(), IndexUser.toInt());
}
void DataBase::AddBook(){
    QString v1 = ui->inputBookTitle->toPlainText();
    QString v2 = ui->inputBookAuthor->toPlainText();
    addToDBBooks(v1, v2);

}
void DataBase::AddUser(){
    QString v1 = ui->inputUserName->toPlainText();
    QString v2 = ui->inputUserSurname->toPlainText();
    addToDBUsers(v1, v2);

}
void DataBase::makeUpdateUsr(){
    QSqlQuery query(db);
    QString primaryKey = ui->id_usr->text();
    //ui->userPages->setCurrentIndex(1);
    QString v1 = ui->inputUserName_2->toPlainText();
    QString v2 = ui->inputUserSurname_2->toPlainText();
    query.prepare("UPDATE users SET name = :value1, surname = :value2 WHERE id = :id");
    query.bindValue(":value1", v1);
    query.bindValue(":value2", v2);
    query.bindValue(":id", primaryKey);

    if (!query.exec()) {
        qDebug() << "Update Error:" << query.lastError().text();
    } else {
        usersTableModel->select();
    }
}
void DataBase::makeUpdateBook(){
    QSqlQuery query(db);
    QString primaryKey = ui->id_Book->text();
    //ui->userPages->setCurrentIndex(1);
    QString v1 = ui->inputBookTitle_2->toPlainText();
    QString v2 = ui->inputBookAuthor_2->toPlainText();
    query.prepare("UPDATE books SET title = :value1, author = :value2 WHERE id = :id");
    query.bindValue(":value1", v1);
    query.bindValue(":value2", v2);
    query.bindValue(":id", primaryKey);

    if (!query.exec()) {
        qDebug() << "Update Error:" << query.lastError().text();
    } else {
        booksTableModel->select();
    }
}

void DataBase::delRow(){
    QPushButton *button =(QPushButton*)sender();
    QString buttVal =button->objectName();
    //qDebug()<<buttVal;
    //qDebug()<<"deleting row "<< index;
    if (buttVal=="delButton") {
        index = ui->tableBooks->currentIndex();
        if (booksTableModel->removeRow(index.row())) {
            booksTableModel->submitAll();
        }
    }else if(buttVal=="delButton_3"){
        index = ui->tableBookBorrows->currentIndex();
        if (borrowsTableModel->removeRow(index.row())) {
            borrowsTableModel->submitAll();
        }
    }else {
        index = ui->tableUsers->currentIndex();
        if (usersTableModel->removeRow(index.row())) {
            usersTableModel->submitAll();
        }
    }
    booksTableModel->select();
    usersTableModel->select();
    borrowsTableModel->select();
}
void DataBase::addToDBUsers(QString v1, QString v2){
    QSqlQuery query(db);

    query.prepare("INSERT INTO users (name, surname) VALUES (:name, :surname)");
    query.bindValue(":name", v1);
    query.bindValue(":surname", v2);
    query.exec();
    usersTableModel->select();

}
void DataBase::addToDBBooks(QString v1, QString v2){
QSqlQuery query(db);

    query.prepare("INSERT INTO books (title, author) VALUES (:title, :author)");
    query.bindValue(":title", v1);
    query.bindValue(":author", v2);
    query.exec();
    booksTableModel->select();

}
void DataBase::addToDBBorrows(int book_id, int user_id){
    QSqlQuery query(db);

    if(!checkBookToBorrow(book_id)){

       QString book_title, user_name;
       query.prepare("SELECT title FROM books WHERE id = :book_id");
       query.bindValue(":book_id", book_id);

       if (query.exec() && query.next()) {
           book_title = query.value(0).toString();
       } else {
           return;
       }

       query.prepare("SELECT name, surname FROM users WHERE id = :user_id");
          query.bindValue(":user_id", user_id);
          if (query.exec() && query.next()) {
              QString name = query.value(0).toString();
              QString surname = query.value(1).toString();
              user_name = name + " " + surname;
          } else {
              return;
          }

       query.prepare("INSERT INTO user_books (user_id, book_id, book_title, user_name) VALUES (:user_id, :book_id, :book_title, :user_name)");
       query.bindValue(":user_id", user_id);
       query.bindValue(":book_id", book_id);
       query.bindValue(":book_title", book_title);
       query.bindValue(":user_name", user_name);

       if (!query.exec()) {
           qDebug() << "Update Error:" << query.lastError().text();
       }

    }else{
        showWarning("Książka jest już wypożyczona");
    }


    booksTableModel->select();
    usersTableModel->select();
    borrowsTableModel->select();
}


//==================================== HELPERS =====================================

void DataBase::showWarning(QString wrr){

    QMessageBox infoBox;
        infoBox.setWindowTitle(" ");
        infoBox.setText(wrr);
        infoBox.setIcon(QMessageBox::Information);
        infoBox.setStandardButtons(QMessageBox::Ok);

        infoBox.exec();
}

void DataBase::onRowClicked(const QModelIndex &index) {
    QSqlRecord record;
    if (index.isValid()) {
        QTableView *tab =(QTableView*)sender();
        QString tabVal =tab->objectName();
        //qDebug()<<tabVal;
        if(tabVal=="tableBooks_2"){record = booksTableModel->record(index.row());IndexBook = record.field(0).value().toInt();}
        //qDebug()<<index;
        //qDebug()<<IndexBook;
        if(tabVal=="tableUsers_2"){record = usersTableModel->record(index.row());IndexUser = record.field(0).value().toInt();}
        //qDebug()<<index;
        //qDebug()<<IndexUser;
        Index = index;
        if(tabVal=="tableUsers" ||tabVal=="tableBooks" ){

            if(tabVal=="tableUsers") record = usersTableModel->record(index.row());
            else record = booksTableModel->record(index.row());
            //qDebug() << "Row clicked:" << index.row();

                   // Get the values of the fields in the record
                   int id = record.field(0).value().toInt(); //record.value("id").toInt();
                   QString v1 = record.field(1).value().toString();
                   QString v2 = record.field(2).value().toString();

                  // qDebug() << " row values: id =" << id << ", v1 =" << v1 << ", v2 =" << v2;
                   if(tabVal=="tableBooks"){
                       ui->booksPages->setCurrentIndex(1);
                       ui->inputBookTitle_2->setText(v1);
                       ui->inputBookAuthor_2->setText(v2);
                       ui->id_Book->setText(QString::number(id));
                   }else{
                   ui->userPages->setCurrentIndex(1);
                   ui->inputUserName_2->setText(v1);
                   ui->inputUserSurname_2->setText(v2);
                   ui->id_usr->setText(QString::number(id));
                   }
            }
        }
}

bool DataBase::checkBookToBorrow(int id)
{
    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM user_books WHERE book_id = :id");
    query.bindValue(":id", id);
    //qDebug()<<"id to borrow: "<< id;
    if (query.exec() && query.next() && query.value(0).toInt() == 0) {
        return false;
    }
    return true;

}

void DataBase::addDummyValues(){

    addToDBUsers("Sofia","Karp-Rabin");
    addToDBUsers("Grzegorz","Lem");
    addToDBUsers("Stanisław","Bajo");
    addToDBUsers("Krzysztof","Jajo");

    addToDBBooks("Solaris","Stanisłam Lem");
    addToDBBooks("Stałe Natury","Jan Taczka");
    addToDBBooks("Kwiatostany","Grzegorz Bąk");
    addToDBBooks("Rynny i okiennice","Krzysztof Żuk");

    addToDBBorrows(2,3);

    booksTableModel->select();
    usersTableModel->select();
    borrowsTableModel->select();
}

DataBase::~DataBase()
{
    delete ui;
}

