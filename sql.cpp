#include "sql.h"

//execBatch()是批量执行，就是执行一个list，vector这样的
//exec()是执行，就是执行一次sql指令

sql::sql() {
    // 开始连接
    db = QSqlDatabase::addDatabase("QMYSQL");//addDatabase() 是一个静态方法，用于创建一个新的数据库连接，并将其添加到已存在的数据库连接列表中。"QMYSQL"表示的是连接mysql
    //db.setDatabaseName("path/to/your/database.db");
    db.setHostName("localhost"); // 设置数据库主机名
    db.setDatabaseName("music"); // 设置数据库名称
    db.setUserName("root"); // 设置数据库用户名
    db.setPassword("123456"); // 设置数据库密码

    if (db.open()) {
        qDebug() << "SQL connect YES" << "\n";
    }
    else {
        // 打开数据库失败，输出错误信息
        qDebug() << "Database error: " << db.lastError().text();
    }

    // 关闭数据库连接
    db.close();
}

int sql::loginsql(QString& username,QString &password){
    db.open();
    QSqlQuery query;
    query.prepare("SELECT id FROM user WHERE name = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Query execution error:" << query.lastError().text();
        db.close();
        return -1;
    }

    if (query.next()) {
        int id = query.value("id").toInt();
        db.close();
        return id;
    }

    db.close();
    return -1;
}

std::vector<int> sql::getloveid(int &uid){
    db.open();
    std::vector<int> musicids;
    QSqlQuery query;
    query.prepare("SELECT musicid FROM userlove WHERE userid = :id");
    query.bindValue(":id",uid);
    if (!query.exec()) {
        qDebug() << "Query execution error:" << query.lastError().text();
        db.close();
        return musicids; // 返回空的向量表示查询出错
    }
    while (query.next()){
        int musicId = query.value("musicid").toInt();
        //qDebug() << musicId;
        musicids.push_back(musicId);
    }
    db.close();
    return musicids;
}

std::vector<QString> sql::getlovename(std::vector<int> musicids){
    db.open();
    std::vector<QString> musicnames;
    QSqlQuery query2;
    query2.prepare("SELECT musicname FROM musicname WHERE id = :id");
    for(auto musicId : musicids) {
        query2.bindValue(":id",musicId);
        if (query2.exec() && query2.next()) {
            QString now = query2.value("musicname").toString();
            //qDebug() << now;
            musicnames.push_back(now);
        } else {
            qDebug() << "Query execution error or no record found for id:" << musicId << query2.lastError().text();
        }
    }
    db.close();
    return musicnames;
}

bool sql::registersql(QString& username,QString& password){
    db.open();
    QSqlQuery query;
    query.prepare("INSERT INTO user (name, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (!query.exec()) {
        qDebug() << "Query execution error:" << query.lastError().text();
        db.close();
        return false;
    } else {
        db.close();
        return true;
    }
}

void sql::updatemusic(QStringList* list){
    db.open();
    QSqlQuery query;
    query.prepare("INSERT INTO musicname (musicname) VALUES (:musicname)");

    QSqlQuery checkQuery; // 用于检测是否存在相同名称的歌曲
    checkQuery.prepare("SELECT COUNT(*) FROM musicname WHERE musicname = :musicname");

    QVariantList musicNames;
    for (auto it : *list) {
        checkQuery.bindValue(":musicname", it);
        checkQuery.exec();
        if (checkQuery.next() && checkQuery.value(0).toInt() == 0) {
            // 如果不存在相同名称的歌曲，则加入列表
            musicNames << it;
        } else {
            qDebug() << "Skipped duplicate: " << it << "\n";
        }
    }

    if (!musicNames.isEmpty()) {
        query.bindValue(":musicname", musicNames);
        if (!query.execBatch()) {
            qDebug() << "Batch insert failed: " << query.lastError();
        } else {
            qDebug() << "Batch insert successful";
        }
    } else {
        qDebug() << "No new songs to insert";
    }
    db.close();

    /*query.prepare("INSERT INTO musicname (musicname) VALUES (:musicname)");
    for(auto it : *list){
        query.bindValue(":musicname", it);
        query.exec(); // 执行插入操作
    }*/
}

int sql::getmusicid(QString musicname){
    db.open();
    QSqlQuery query;
    query.prepare("SELECT id FROM musicname WHERE musicname = :musicname");
    query.bindValue(":musicname",musicname);
    if(!query.exec()){
        qDebug() << "Query execution error:" << query.lastError().text();
        db.close();
        return -1;
    }
    if(query.next()){
        int id = query.value("id").toInt();
        qDebug() << musicname << "  " << id << "\n";
        db.close();
        return id;
    }
    db.close();
    return -1;
}

void sql::uploadlove(int& uid,int &musicid){
    db.open();
    QSqlQuery query;
    query.prepare("INSERT INTO userlove (userid,musicid) VALUES (:userid,:musicid)");
    query.bindValue(":userid",uid);
    query.bindValue(":musicid",musicid);
    if (!query.exec()) {
        qDebug() << "Query execution error:" << query.lastError().text();
        db.close();
    } else {
        db.close();
    }
}

void sql::deletelove(int& uid,int musicid){
    db.open();
    QSqlQuery query;
    query.prepare("DELETE FROM userlove WHERE userid = :userid AND musicid = :musicid");
    query.bindValue(":userid",uid);
    query.bindValue(":musicid",musicid);
    if (!query.exec()) {
        qDebug() << "Query execution error:" << query.lastError().text();
    } else {
        qDebug() << "Record deleted successfully.";
    }
    db.close();
}
