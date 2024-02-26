#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_flag_first_operand_isempty(true)
    , m_flag_second_operand_isempty(true)
    , m_operator_is_set(false)
{
    ui->setupUi(this);

    m_operationMap["+"] = [this](double a, double b) { add(a, b); };
    m_operationMap["-"] = [this](double a, double b) { subtract(a, b); };
    m_operationMap["*"] = [this](double a, double b) { multiply(a, b); };
    m_operationMap["/"] = [this](double a, double b) { divide(a, b); };

    QVBoxLayout* vl = new QVBoxLayout(ui->centralwidget);
    display = new QLineEdit;
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMinimumHeight(50);

    vl->addWidget(display);

    QGridLayout * grid = new QGridLayout();

    QStringList buttons {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "c", "0", "=", "+"
    };

    int pos = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            QPushButton *butt = new QPushButton(buttons[pos++]);
            grid->addWidget(butt, i, j);
            connect(butt, &QPushButton::clicked, this, &MainWindow::buttonClicked);
        }
    }

    vl->addLayout(grid);
    setFixedSize(300, 400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add(double a, double b)
{
    display->setText( QString::number(a + b));
    m_flag_first_operand_isempty = true;
    m_flag_second_operand_isempty = true;
}

void MainWindow::subtract(double a, double b)
{
    display->setText( QString::number(a - b));
    m_flag_first_operand_isempty = true;
    m_flag_second_operand_isempty = true;
}

void MainWindow::multiply(double a, double b)
{
    display->setText( QString::number(a * b));
    m_flag_first_operand_isempty = true;
    m_flag_second_operand_isempty = true;
}

void MainWindow::divide(double a, double b)
{
    if(b != 0)
    {
        display->setText( QString::number(a / b));
        m_flag_first_operand_isempty = true;
        m_flag_second_operand_isempty = true;
    }
    else
    {
        display->setText("Error");
    }
}

void MainWindow::clear_screan()
{
    display->clear();
    m_flag_first_operand_isempty = true;
    m_flag_second_operand_isempty = true;
    m_operator_is_set = false;
}


void MainWindow::buttonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    if(display->text() == "Error")
    {
        clear_screan();
    }

    if(button->text() == "c")
    {
        clear_screan();
        return;
    }
    QRegExp operator_reg("([+\\-*/])");

    if(button->text().contains(operator_reg))
    {
        if(m_flag_first_operand_isempty)
        {
            m_first_operand = display->text();
            m_flag_first_operand_isempty = false;

            m_operator = button->text();
            m_operator_is_set = true;
            display->clear();
            return;
        }
        else if(m_flag_second_operand_isempty)
        {
            m_second_operand = display->text();
            m_flag_second_operand_isempty = false;

            m_operator = button->text();
            m_operator_is_set = true;
            display->clear();
            return;
        }
        else
        {
            m_operator = button->text();
            m_operator_is_set = true;
            display->clear();
            return;
        }
    }

    if(button->text() == "=")
    {
        if(!m_flag_first_operand_isempty && !m_flag_second_operand_isempty)
        {
            auto operationFunction = m_operationMap[m_operator];
            double firstOperand = m_first_operand.toDouble();
            double secondOperand = m_second_operand.toDouble();
            operationFunction(firstOperand, secondOperand);
            return;
        }
        else if(m_flag_second_operand_isempty)
        {
            m_second_operand = display->text();
            m_flag_second_operand_isempty = false;

            display->clear();
        }

        auto operationFunction = m_operationMap[m_operator];
        double firstOperand = m_first_operand.toDouble();
        double secondOperand = m_second_operand.toDouble();
        operationFunction(firstOperand, secondOperand);
        return;
    }

    display->setText(display->text() + button->text());

}

