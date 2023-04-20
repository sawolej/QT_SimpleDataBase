#ifndef DATABASE_H
#define DATABASE_H
#include <QString>
#include <QSqlTableModel>
#include <QTableView>
#include <QMainWindow>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class DataBase; }
QT_END_NAMESPACE

class DataBase : public QMainWindow
{
    Q_OBJECT

public:
    DataBase(QWidget *parent = nullptr);
    ~DataBase();

private slots:
    void borrow();
    void delRow();
    void AddBook();
    void AddUser();
    void onRowClicked(const QModelIndex &index);
    void makeUpdateUsr();
    void makeUpdateBook();
private:

    Ui::DataBase *ui;
    QModelIndex index;
    QModelIndex Index;
    QVariant IndexBook;
    QVariant IndexUser;
    QSqlDatabase db;
    QSqlTableModel *booksTableModel;
    QSqlTableModel *usersTableModel;
    QSqlTableModel *borrowsTableModel;
    QSqlTableModel *tableBooks_2model;


    void setUpDB();
    void addToDBUsers(QString v1, QString v2);
    void addToDBBooks(QString v1, QString v2);
    void addToDBBorrows(int book_id, int user_id);
    void setUpView();
    void makeConnects();
    void addDummyValues();
    bool checkBookToBorrow(int id);
    void showWarning(QString wrr);

};
#endif // DATABASE_H
