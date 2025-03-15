#include "main_window.h"
#include <QtWidgets/QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi();
    resize(800, 650);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    countSpinBox = new QSpinBox(this);
    countSpinBox->setRange(1, 101);
    countSpinBox->setValue(20);

    viewList = new QListWidget(this);
    viewList->setSelectionMode(QAbstractItemView::SingleSelection);
    viewList->setStyleSheet("QListWidget::item:selected { background: transparent; }");

    totalProgress = new QProgressBar(this);
    totalProgress->setRange(0, 100);
    totalProgress->setValue(0);
    totalProgress->setFormat("Общий прогресс: %p%");

    greenProgress = new QProgressBar(this);
    greenProgress->setRange(0, 100);
    greenProgress->setValue(0);
    greenProgress->setFormat("Изучено: %p%");

    questionView = new QWidget(this);
    QVBoxLayout *questionLayout = new QVBoxLayout(questionView);
    numberLabel = new QLabel("Номер: -", this);
    nameEdit = new QLineEdit("Название: -", this);
    nameEdit->setAlignment(Qt::AlignCenter);
    nameEdit->setStyleSheet("QLineEdit { color:#0000FF; }");
    statusCombo = new QComboBox(this);
    statusCombo->addItem("Не просмотрен");
    statusCombo->addItem("Просмотрен");
    statusCombo->addItem("Изучен");
    nextButton = new QPushButton("Следующий", this);
    prevButton = new QPushButton("Предыдущий", this);
    questionLayout->addWidget(numberLabel);
    questionLayout->addWidget(nameEdit);
    questionLayout->addWidget(statusCombo);
    questionLayout->addWidget(prevButton);
    questionLayout->addWidget(nextButton);

    layout->addWidget(countSpinBox);
    layout->addWidget(viewList);
    layout->addWidget(totalProgress);
    layout->addWidget(greenProgress);
    layout->addWidget(questionView);

    setCentralWidget(centralWidget);
    QObject::connect(countSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                     [this](int count) { updateTicketList(count); });
    QObject::connect(viewList, &QListWidget::itemClicked,
                     [this](QListWidgetItem *item) {
                         int index = viewList->row(item);
                         setupQuestionView(index);
                     });
    QObject::connect(viewList, &QListWidget::itemDoubleClicked,
                     [this](QListWidgetItem *item) {
                         int index = viewList->row(item);
                         QColor current = item->background().color();
                         if (current == Qt::gray) {
                             item->setBackground(Qt::yellow);
                             yellowState[index] = false;
                         } else if (current == Qt::yellow && !yellowState[index]) {
                             item->setBackground(Qt::green);
                             yellowState[index] = false;
                         } else if (current == Qt::green) {
                             item->setBackground(Qt::yellow);
                             yellowState[index] = true;
                         } else if (current == Qt::yellow && yellowState[index]) {
                             item->setBackground(Qt::gray);
                             yellowState[index] = false;
                         }
                         setupQuestionView(index);
                         updateProgressBars();
                     });
    QObject::connect(nameEdit, &QLineEdit::editingFinished,
                     [this]() {
                         int index = numberLabel->text().mid(7).toInt() - 1;
                         if (index >= 0 && index < viewList->count()) {
                             saveNameEdit(index);
                         }
                     });
    QObject::connect(nextButton, &QPushButton::clicked,
                     [this]() {
                         int index = numberLabel->text().mid(7).toInt();
                         if (index < viewList->count()) {
                             setupQuestionView(index);
                         }
                     });
    QObject::connect(prevButton, &QPushButton::clicked,
                     [this]() {
                         int index = numberLabel->text().mid(7).toInt() - 2;
                         if (index >= 0) {
                             setupQuestionView(index);
                         }
                     });
    QObject::connect(statusCombo, QOverload<int>::of(&QComboBox::activated),
                     [this](int statusIndex) {
                         int index = numberLabel->text().mid(7).toInt() - 1;
                         if (index >= 0 && index < viewList->count()) {
                             QListWidgetItem *item = viewList->item(index);
                             switch (statusIndex) {
                             case 0: 
                                 item->setBackground(Qt::gray);
                                 yellowState[index] = false;
                                 break;
                             case 1: 
                                 item->setBackground(Qt::yellow);
                                 yellowState[index] = false;
                                 break;
                             case 2: 
                                 item->setBackground(Qt::green);
                                 yellowState[index] = false;
                                 break;
                             }
                             updateProgressBars();
                         }
                     });

    updateTicketList(countSpinBox->value());
}

void MainWindow::updateTicketList(int count) {
    viewList->clear();
    yellowState.resize(count);
    yellowState.fill(false);
    for (int i = 0; i < count; ++i) {
        QListWidgetItem *item = new QListWidgetItem(QString("Билет %1").arg(i + 1));
        item->setBackground(Qt::gray);
        item->setTextAlignment(Qt::AlignCenter); 
        item->setForeground(QColor("##0000FF"));  
        viewList->addItem(item);
    }
    setupQuestionView(-1);
    updateProgressBars();
}

void MainWindow::setupQuestionView(int index) {
    if (index >= 0 && index < viewList->count()) {
        numberLabel->setText(QString("Номер: %1").arg(index + 1));
        nameEdit->setText(viewList->item(index)->text());
        QColor color = viewList->item(index)->background().color();
        if (color == Qt::gray) {
            statusCombo->setCurrentIndex(0);
        } else if (color == Qt::yellow) {
            statusCombo->setCurrentIndex(1);
        } else if (color == Qt::green) {
            statusCombo->setCurrentIndex(2);
        }
    } else {
        numberLabel->setText("Номер: -");
        nameEdit->setText("Название: -");
        statusCombo->setCurrentIndex(-1);
    }
}

void MainWindow::saveNameEdit(int index) {
    if (index >= 0 && index < viewList->count()) {
        QListWidgetItem *item = viewList->item(index);
        item->setText(nameEdit->text());
        item->setTextAlignment(Qt::AlignCenter); 
        item->setForeground(QColor("#00FF00"));  
    }
}

void MainWindow::updateProgressBars() {
    int totalCount = viewList->count();
    if (totalCount == 0) {
        totalProgress->setValue(0);
        greenProgress->setValue(0);
        return;
    }

    int viewedCount = 0;
    int greenCount = 0;
    for (int i = 0; i < totalCount; ++i) {
        QColor color = viewList->item(i)->background().color();
        if (color == Qt::yellow || color == Qt::green) {
            viewedCount++;
        }
        if (color == Qt::green) {
            greenCount++;
        }
    }

    totalProgress->setValue((viewedCount * 100) / totalCount);
    greenProgress->setValue((greenCount * 100) / totalCount);
}