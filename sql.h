#ifndef SQL_H
#define SQL_H

// 导入需要用到的头文件
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QMessageBox>
class sql
{
public:
    sql();
    int loginsql(QString& username,QString& password);
    bool registersql(QString& username,QString& password);
    std::vector<QString> getlovename(std::vector<int> musicids);
    std::vector<int> getloveid(int &uid);
    void updatemusic(QStringList* list);
    void uploadlove(int& uid,int &musicid);
    void deletelove(int& uid,int musicid);
    int getmusicid(QString musicname);
private:
    QSqlDatabase db;
};

#endif // SQL_H
