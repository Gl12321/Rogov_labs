#include "typing_test.h"

TypingTest::TypingTest(QWidget *parent) : QMainWindow(parent) {
    setupUi();
    resize(500, 300);
    testRunning = false;
    sampleText = QStringLiteral("Разработка программного обеспечения для космических исследований открывает новые горизонты в изучении Вселенной. Современные технологии позволяют создавать сложные системы управления спутниками, анализировать данные с телескопов и моделировать полёты на Марс. Программисты и инженеры вместе работают над тем, чтобы человечество могло прикоснуться к звёздам.");
    currentPosition = 0;
    errorCount = 0;
    bestCPM = 0;
    previousCPM = 0;
    currentCPM = 0;
    timer = new QTimer(this);
    elapsed = new QElapsedTimer();

    QObject::connect(startButton, &QPushButton::clicked, [this]() { startTest(); });
    QObject::connect(inputLine, &QLineEdit::textEdited, [this](const QString &text) { checkInput(text); });
    QObject::connect(timer, &QTimer::timeout, [this]() { updateTime(); });

    sampleLabel->setText(sampleText);
}

TypingTest::~TypingTest() {}

void TypingTest::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    sampleLabel = new QLabel(sampleText, this);
    sampleLabel->setAlignment(Qt::AlignCenter);
    sampleLabel->setWordWrap(true);
    sampleLabel->setMinimumHeight(50);

    inputLine = new QLineEdit(this);
    inputLine->setPlaceholderText("Начинай печатать после старта");
    inputLine->setMinimumWidth(300);

    startButton = new QPushButton("Начать тест", this);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(inputLine);
    inputLayout->addWidget(startButton);

    resultLabel = new QLabel("Скорость: - CPM", this);
    resultLabel->setAlignment(Qt::AlignCenter);

    errorLabel = new QLabel("Ошибки: 0", this);
    errorLabel->setAlignment(Qt::AlignCenter);

    statsLabel = new QLabel("Текущий: 0 | Предыдущий: 0 | Лучший: 0", this);
    statsLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(sampleLabel);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(resultLabel);
    mainLayout->addWidget(errorLabel);
    mainLayout->addWidget(statsLabel);
    mainLayout->addStretch();

    setCentralWidget(centralWidget);
}

void TypingTest::startTest() {
    if (!testRunning) {
        testRunning = true;
        inputLine->clear();
        inputLine->setFocus();
        inputLine->setStyleSheet("");
        resultLabel->setText("Печатай!");
        errorCount = 0;
        errorLabel->setText("Ошибки: 0");
        elapsed->start();
        timer->start(100);
        startButton->setText("Тест идет...");
        startButton->setEnabled(false);
        currentPosition = 0;
    }
}

void TypingTest::checkInput(const QString &text) {
    if (!testRunning) return;

    if (text.length() < currentPosition) {
        inputLine->setText(text.left(currentPosition));
        inputLine->setStyleSheet("");
        return;
    }

    if (text.length() > currentPosition) {
        QChar inputChar = text[currentPosition];
        QChar expectedChar = sampleText[currentPosition];

        if (inputChar == expectedChar) {
            currentPosition++;
            inputLine->setStyleSheet("");
            if (currentPosition == sampleText.length()) {
                timer->stop();
                testRunning = false;
                startButton->setText("Начать тест");
                startButton->setEnabled(true);
                double timeInMinutes = elapsed->elapsed() / 60000.0;
                currentCPM = static_cast<int>(sampleText.length() / timeInMinutes);
                resultLabel->setText(QString("Скорость: %1 CPM").arg(currentCPM));
                errorLabel->setText(QString("Ошибки: %1").arg(errorCount));
                updateStats();
            }
        } else {
            inputLine->setText(text.left(currentPosition));
            inputLine->setStyleSheet("background-color: pink;");
            errorCount++;
            errorLabel->setText(QString("Ошибки: %1").arg(errorCount));
        }
    }
}

void TypingTest::updateTime() {
    if (testRunning) {
        double timeInSeconds = elapsed->elapsed() / 1000.0;
        resultLabel->setText(QString("Время: %1 сек").arg(timeInSeconds, 0, 'f', 1));
    }
}

void TypingTest::updateStats() {
    if (currentCPM > bestCPM) {
        bestCPM = currentCPM;
    }
    statsLabel->setText(QString("Текущий: %1 | Предыдущий: %2 | Лучший: %3")
                        .arg(currentCPM).arg(previousCPM).arg(bestCPM));
    previousCPM = currentCPM;
}