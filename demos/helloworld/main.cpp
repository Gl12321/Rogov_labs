#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>

int main(int argc, char** argv) {
    QApplication a(argc, argv);
    QPushButton button("Hello, !", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
