#include <QApplication>
#include <QDebug>
#include <iostream>

#include "../../common/include/TranslationManager.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TranslationManager translationManager(&app);

    translationManager.initialize();

    cout << QObject::tr("K.Hello").toStdString() << endl;

    return app.exec();
}
