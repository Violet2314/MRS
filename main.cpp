
#include "mainwindow.h"
#include "loginrigsters.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QIcon>
#include "sql.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon((QIcon)":/images/music");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MRS_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    loginrigsters l2;
    sql SQL;
    l2.setSQL(SQL);
    w.setSQL(SQL);

    //连接信号
    QObject::connect(&l2, &loginrigsters::loginSuccessful, [&w](int userId,QString username) {//绑定信号
        w.setuid(userId,username);
        w.show();
    });

    //w.show();
    l2.show();
    return app.exec();
}
