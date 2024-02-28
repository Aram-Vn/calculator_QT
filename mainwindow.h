#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <optional>
#include <functional>
#include <QTextCharFormat>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void keyPressEvent(QKeyEvent* e) override;

private slots:
    void buttonClicked();

private:
    void add(double a, double b);
    void subtract(double a, double b);
    void multiply(double a, double b);
    void divide(double a, double b);
    void clear_screan();
    void buttonClicked_handler(QString str);

private:
    Ui::MainWindow *ui;
    QLineEdit*     display;
    std::map<QString, std::function<void(double, double)>> m_operationMap;
    QString m_first_operand;
    bool m_flag_first_operand_isempty;
    QString m_operator;
    bool m_operator_is_set;
    QString m_second_operand;
    bool m_flag_second_operand_isempty;
};
#endif // MAINWINDOW_H
