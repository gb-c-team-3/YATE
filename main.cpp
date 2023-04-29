#include "TextEditor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextEditor textEditor;
    textEditor.show();
    return a.exec();
}
