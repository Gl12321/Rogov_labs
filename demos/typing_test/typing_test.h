#ifndef TYPING_TEST_H
#define TYPING_TEST_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QTimer>
#include <QElapsedTimer>

class TypingTest : public QMainWindow {
public:
    TypingTest(QWidget *parent = nullptr);
    ~TypingTest();

private:
    QLabel *sampleLabel;
    QLineEdit *inputLine;
    QPushButton *startButton;
    QLabel *resultLabel;
    QLabel *errorLabel;
    QLabel *statsLabel; 
    QTimer *timer;
    QElapsedTimer *elapsed;
    QString sampleText;
    bool testRunning;
    int currentPosition;
    int errorCount;
    int bestCPM;    
    int previousCPM; 
    int currentCPM;  

    void setupUi();
    void startTest();
    void checkInput(const QString &text);
    void updateTime();
    void updateStats(); 
};

#endif