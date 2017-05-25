#include "hexator.h"
#include <QtGui>
#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <QMessageBox>
#include <limits.h>
#include <math.h>

Hexator::Hexator(QWidget *parent) : QWidget(parent)
{
    // Creates calculator display and set contraints
    display = new QLineEdit("0");
    display->setAlignment(Qt::AlignRight);
    display->setReadOnly(true);
    display->setMaxLength(10);
    display->setFixedHeight(70);
    display->setFixedWidth(303);
    QFont font = display->font();
    font.setPointSize(48);
    display->setFont(font);

    QPalette palette = display->palette();
    display->setAutoFillBackground(true);
    palette.setColor(QPalette::Base, QColor(254, 204, 52));
    palette.setColor(QPalette::Text, QColor(255, 255 ,255));

    display->setPalette(palette);

    // Creates operation buttons
    addButton = createButton(tr("+"), SLOT(changeModeToAdd()), true);
    subButton = createButton(tr("-"), SLOT(changeModeToSub()), true);
    assignButton = createButton(tr("="), SLOT(calculate()), true);
    clearButton = createButton(tr("Clr"), SLOT(clear()), true);

    // Creates first 10 digit buttons
    for (int i = 0; i < 10; i++)
        digitButtons[i] = createButton(QString::number(i), SLOT( pressDigit() ));

    // Creates last 6 hexadecimal digit buttons
    char ascii = 0;
    for (int i = 10; i < 16; i++){
        ascii = i + 55;
        digitButtons[i] = createButton(QString(ascii), SLOT( pressDigit() ), false , true);
    }

    // Customizing layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *operationsLayout = new QHBoxLayout;
    QHBoxLayout *digitsRows[4];
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);


    // Adds the display of the calculator
    mainLayout->addWidget(display);

    // Adds the operations row and the operands
    mainLayout->addLayout(operationsLayout);
    operationsLayout->addWidget(addButton);
    operationsLayout->addWidget(subButton);
    operationsLayout->addWidget(assignButton);
    operationsLayout->addWidget(clearButton);
    mainLayout->setSpacing(1);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setMargin(0);


    // Adds the digit rows
    for (int i = 0; i < 4; i++){
        digitsRows[i] = new QHBoxLayout;
        for (int j = 0; j < 4; j++)
            digitsRows[i]->addWidget(digitButtons[(i*4)+j]);
    }

    for (int i = 0; i < 4; i++)
        mainLayout->addLayout(digitsRows[i]);


    setLayout(mainLayout);
    setWindowTitle(tr("Hexator"));
    setWindowIcon(QIcon(":/logo.png"));
    clear();
}

QPushButton *Hexator::createButton(const QString &text, const char *member, bool isOperand, bool isChar)
{
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), this, member);

    button->setFlat(true);
    button->setAutoFillBackground(true);
    button->setFixedWidth(75);
    button->setFixedHeight(75);

    QPalette palette = button->palette();
    if(isOperand){
        palette.setColor(QPalette::Button, QColor(150, 150, 150));
        palette.setColor(QPalette::ButtonText, QColor(75, 75, 75));
    }
    else if(isChar){
        palette.setColor(QPalette::Button, QColor(70, 33, 70));
        palette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    }
    else {
        palette.setColor(QPalette::Button, QColor(100, 43, 100));
        palette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    }

    button->setPalette(palette);
    QFont font = button->font();
    font.setPointSize(32);
    button->setFont(font);

    return button;
}

void Hexator::changeModeToAdd(){
    QString currentValue = display->text();
    bool ok;
    savedNum = currentValue.toUInt(&ok, 16);
    lastButtonWasMode = true;
    currentMode = Addition;
}

void Hexator::changeModeToSub(){
    savedNum = getNumberFromDisplay();
    lastButtonWasMode = true;
    currentMode = Subtraction;
}


void Hexator::calculate(){
    QString currentValue = display->text();
    bool ok;
    int secondNum = currentValue.toUInt(&ok, 16);
    int result = 0;
    if(currentMode == Addition && !lastButtonWasMode){
        result = savedNum + secondNum;
        setDisplayNumber(result);
        currentMode = NotSet;
        savedNum = result;
    }
    else if(currentMode == Subtraction && !lastButtonWasMode){
        result = savedNum - secondNum;
        setDisplayNumber(result);
        currentMode = NotSet;
        savedNum = result;
    }
    lastButtonWasMode = true;
}

void Hexator::clear(){
    display->setText(tr("0"));
    currentMode = NotSet;
    savedNum = 0;
    lastButtonWasMode = false;
}

void Hexator::pressDigit(){
    QPushButton *pressedButton = qobject_cast<QPushButton *>(sender());
    QString digitValue = pressedButton->text();
    QString currentValue = display->text();

    if(currentValue == "0" || lastButtonWasMode)
        currentValue = QString("");

    lastButtonWasMode = false;
    display->setText(currentValue + digitValue);
}

void setDisplayNumber(const int *number){
    display->setText(QString::number(number, 16).toUpper());
}

int getNumberFromDisplay(){
    QString currentValue = display->text();
    bool ok;
    return currentValue.toUInt(&ok, 16);
}
