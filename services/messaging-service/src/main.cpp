#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString langCode = "es_ES";

    QTranslator translator;

    // Chemin relatif vers le fichier .qm (supposé copié à côté de l'exécutable dans i18n/)
    QString qmFile = QCoreApplication::applicationDirPath() + "/i18n/" + langCode + ".qm";

    if (translator.load(qmFile)) {
        app.installTranslator(&translator);
        qDebug() << "Traduction chargée :" << qmFile;
    } else {
        qDebug() << "Erreur chargement traduction :" << qmFile;
    }

    std::cout << QObject::tr("K.Hello").toStdString() << std::endl;
    std::cout << QObject::tr("K.AGE").toStdString() << std::endl;
    std::cout << QObject::tr("K.NAME").toStdString() << std::endl;

    return app.exec();
}
