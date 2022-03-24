#include "fightinggame.h"
#include "model.h"
#include "controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model m;
    Controller c(&m);
    FightingGame v(&c);
    c.setView(&v);

    v.show();
    return a.exec();  
}
