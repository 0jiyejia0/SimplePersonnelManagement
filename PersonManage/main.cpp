#include "PersonManage.h"
#include <QtWidgets/QApplication>
#include <Date.h>
#include <Person.h>
#include <PersonSet.h>
using namespace std;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    PersonManage w;
    w.show();
    return a.exec();
}
