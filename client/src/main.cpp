#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QString>

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString langCode = "fr_FR";
    QTranslator translator;

    QString qmFile = QCoreApplication::applicationDirPath() + "/i18n/" + langCode + ".qm";

    if (translator.load(qmFile)) {
        app.installTranslator(&translator);
        qDebug() << "Traduction chargée :" << qmFile;
    } else {
        qDebug() << "Erreur chargement traduction :" << qmFile;
    }
    cout << QObject::tr("K.Hello").toStdString() << endl;
    return app.exec();
}
