#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_flag_first_operand_isempty(true)
    , m_flag_second_operand_isempty(true)
    , m_operator_is_set(false)
    , display{new QLineEdit}
{
    ui->setupUi(this);

    m_operationMap["+"] = [this](double a, double b) { add(a, b); };
    m_operationMap["-"] = [this](double a, double b) { subtract(a, b); };
    m_operationMap["*"] = [this](double a, double b) { multiply(a, b); };
    m_operationMap["/"] = [this](double a, double b) { divide(a, b); };

    QVBoxLayout* vl = new QVBoxLayout(ui->centralwidget);

    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMinimumHeight(50);

    vl->addWidget(display);

    QGridLayout* grid = new QGridLayout(this);

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
            QPushButton* butt = new QPushButton(buttons[pos++], this);
            grid->addWidget(butt, i, j);
            connect(butt, &QPushButton::clicked, this, &MainWindow::buttonClicked);

            if(i < 3 && j < 3 || (i == 3 && j == 1))
            {
                butt->setStyleSheet("QPushButton {"
                                    "   background-color: #4CAF50;"
                                    "   border: 1px solid #4CAF50;"
                                    "   color: white;"
                                    "   padding: 8px 16px;"
                                    "   text-align: center;"
                                    "   text-decoration: none;"
                                    "   display: inline-block;"
                                    "   font-size: 16px;"
                                    "   margin: 4px 2px;"
                                    "   transition-duration: 0.4s;"
                                    "   cursor: pointer;"
                                    "   border-radius: 5px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "   background-color: white;"
                                    "   color: black;"
                                    "}");
            }
            else
            {
                butt->setStyleSheet("QPushButton {"
                                    "   background-color: #FFA500;"
                                    "   border: 1px solid #4CAF50;"
                                    "   color: white;"
                                    "   padding: 8px 16px;"
                                    "   text-align: center;"
                                    "   text-decoration: none;"
                                    "   display: inline-block;"
                                    "   font-size: 16px;"
                                    "   margin: 4px 2px;"
                                    "   transition-duration: 0.4s;"
                                    "   cursor: pointer;"
                                    "   border-radius: 5px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "   background-color: white;"
                                    "   color: black;"
                                    "}");
            }
        }
    }

    vl->addLayout(grid);
    setFixedSize(300, 325);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    QString keyText = e->text();

    if (keyText.contains(QRegExp("[0-9+\\-*/=c]")))
    {
        buttonClicked_handler(keyText);
    }
    else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return )
    {
        buttonClicked_handler("=");
    }
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
        QPalette palette = display->palette();
        palette.setColor(QPalette::Text, Qt::red);
        display->setPalette(palette);
        display->setText("Error");
    }
}

void MainWindow::clear_screan()
{
    display->clear();
    m_flag_first_operand_isempty = true;
    m_flag_second_operand_isempty = true;
    m_operator_is_set = false;

    QPalette palette = display->palette();
    palette.setColor(QPalette::Text, Qt::white);
    display->setPalette(palette);

    display->setPlaceholderText("0");
}

void MainWindow::buttonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    buttonClicked_handler(button->text());
}

void MainWindow::buttonClicked_handler(QString str)
{
    if(display->text() == "Error")
    {
        clear_screan();
    }

    if(str == "c")
    {
        clear_screan();
        return;
    }
    QRegExp operator_reg("([+\\-*/])");

    if(m_operator_is_set && str.contains(operator_reg) && !m_flag_first_operand_isempty && m_flag_second_operand_isempty)
    {
        return;
    }

    if(str.contains(operator_reg))
    {
        if(m_flag_first_operand_isempty)
        {
            m_first_operand = display->text();
            m_flag_first_operand_isempty = false;

            m_operator = str;
            m_operator_is_set = true;
            display->clear();
            display->setPlaceholderText(m_first_operand);
            return;
        }
        else if(m_flag_second_operand_isempty)
        {
            m_second_operand = display->text();
            m_flag_second_operand_isempty = false;

            m_operator = str;
            m_operator_is_set = true;
            display->clear();
            return;
        }
    }

    if(str == "=")
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

    display->setText(display->text() + str);
}



