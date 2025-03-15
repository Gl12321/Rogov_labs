#include <QtWidgets/QApplication>
#include "typing_test.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    TypingTest window;
    window.show();
    return app.exec();
}
