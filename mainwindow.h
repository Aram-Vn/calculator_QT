#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <optional>
#include <functional>

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

private slots:
    void buttonClicked();

private:
    std::optional<double> add(double a, double b);
    std::optional<double> subtract(double a, double b);
    std::optional<double> multiply(double a, double b);
    std::optional<double> divide(double a, double b);
    void handleOperatorClicked(const QString& op);
private:
    Ui::MainWindow *ui;
    QLineEdit*     display;
    std::map<QString, std::function<std::optional<double>(double, double)>> m_operator_functions;
    QString        m_first_operand;
    QString        m_second_operand;
    QString        m_result;
    bool           m_flag_first_operand_isempty;
};
#endif // MAINWINDOW_H
