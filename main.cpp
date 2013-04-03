#include "conotas.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CoNotas w;
    w.show();
    
    return a.exec();
}
