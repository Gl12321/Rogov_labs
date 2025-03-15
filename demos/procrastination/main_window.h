#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QProgressBar> 
#include <QtCore/QVector>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QSpinBox *countSpinBox;
    QListWidget *viewList;
    QWidget *questionView;
    QLabel *numberLabel;
    QLineEdit *nameEdit;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QComboBox *statusCombo;
    QProgressBar *totalProgress; 
    QProgressBar *greenProgress; 
    QVector<bool> yellowState;

    void setupUi();
    void updateTicketList(int count);
    void setupQuestionView(int index);
    void saveNameEdit(int index);
    void updateProgressBars();   
};


#endif 