#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_flag_first_operand_isempty(true)
    , m_result("0")
{

    m_operator_functions = {
                            {"+", [this](double a, double b) { return add(a, b); }},
                            {"-", [this](double a, double b) { return subtract(a, b); }},
                            {"*", [this](double a, double b) { return multiply(a, b); }},
                            {"/", [this](double a, double b) { return divide(a, b); }},
                            };

    ui->setupUi(this);

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

std::optional<double> MainWindow::add(double a, double b)
{
    std::cout << "YES add" << std::endl;
    std::cout << a << " " << b << std::endl;
    return a + b;
}

std::optional<double> MainWindow::subtract(double a, double b)
{
    return a - b;
}

std::optional<double> MainWindow::multiply(double a, double b)
{
    return a * b;
}

std::optional<double> MainWindow::divide(double a, double b)
{
    if (b != 0)
        return a / b;
    else
        return std::nullopt;
}


void MainWindow::buttonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    if(button->text() == "c")
    {
        display->clear();
        m_first_operand = "";
        m_second_operand = "";
        m_result = "";
        m_flag_first_operand_isempty = true;
        return;
    }

    QRegExp operator_reg("([+\\-*/])");

    if(button->text().contains(operator_reg) && m_flag_first_operand_isempty)
    {
        m_first_operand = display->text();
        std::cout << m_first_operand.toStdString() << std::endl;
        m_flag_first_operand_isempty = false;
        display->clear();
        return;
    }
    else if(button->text().contains(operator_reg) && button->text().contains(operator_reg))
    {
        m_second_operand =display->text();
        std::cout << m_second_operand.toStdString() << std::endl;
        display->clear();

        QString operatorString = button->text();

        if(m_operator_functions.find(operatorString) != m_operator_functions.end())
        {
            std::cout << "YES map" << std::endl;

            auto operationFunction = m_operator_functions[operatorString];
            double firstOperand = m_first_operand.toDouble();
            double secondOperand = m_second_operand.toDouble();

            std::optional<double> result = operationFunction(firstOperand, secondOperand);

            if(result.has_value())
            {
                m_result = QString::number(result.value());
                 std::cout << "YES val" << std::endl;
            }
            else
            {
                std::cout << "YES err" << std::endl;
                display->setText("Error");
            }
        }

        return;
    }

    if(button->text() == "=")
    {
        std::cout << display->text().toStdString() << std::endl;;
        std::cout << "YES = " <<  m_result.toStdString()  <<std::endl;
        display->setText(QString::number(m_result.toDouble()));

        return;
    }

    display->setText(display->text() + button->text());


}

