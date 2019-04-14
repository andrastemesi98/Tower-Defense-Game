#include "../Include/view.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    view::view w;
    w.show();
    return a.exec();
}
