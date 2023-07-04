#ifndef DIALOG_H
#define DIALOG_H

#include <QDateEdit>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>

struct Employee {
    QString name;
    QString enrollNumber;
    QDate dob;
    int age;
    QDate joinedDate;
    double basicSalary;
    double lpa;
};


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();


private:
    Ui::Dialog *ui;
    QLabel *nameLabel;
    QLabel *enrollNumberLabel;
    QLabel *dobLabel;
    QLabel *joinedDateLabel;
    QLabel *basicSalaryLabel;
    QLabel *lpaLabel;
    QLineEdit *nameEdit;
    QLineEdit *enrollNumberEdit;
    QDateEdit *dobEdit;
    QDateEdit *joinedDateEdit;
    QLineEdit *basicSalaryEdit;
    QLabel *lpaDisplay;

    QPushButton *exportButton;
    QList<Employee> employeeList;

    bool validateForm();
    void clearForm();


public slots:

    void addEmployee();
    void exportToJSON();


};

#endif // DIALOG_H
