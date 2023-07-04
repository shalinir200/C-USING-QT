#include "dialog.h"
#include "ui_dialog.h"
#include <QFormLayout>
#include<QPushButton>
#include<QVBoxLayout>
#include<QMessageBox>
#include <QFile>
#include<QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextStream>
#include<QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    setWindowTitle("Employee CRUD");

    nameLabel = new QLabel("Employee Name:");
    nameEdit = new QLineEdit();


    enrollNumberLabel = new QLabel("Employee EnrollNumber:");
    enrollNumberEdit = new QLineEdit();

    dobLabel = new QLabel("Date of Birth:");
    dobEdit = new QDateEdit();
    dobEdit->setDisplayFormat("dd-MM-yyyy");

    joinedDateLabel = new QLabel("Joined Date:");
    joinedDateEdit = new QDateEdit();
    joinedDateEdit->setDisplayFormat("dd-MM-yyyy");

    basicSalaryLabel = new QLabel("Basic Salary (INR):");
    basicSalaryEdit = new QLineEdit();

    lpaLabel = new QLabel("LPA:");
    lpaDisplay = new QLabel();

    exportButton = new QPushButton("Export to JSON");

    QFormLayout * formLayout = new QFormLayout();
    formLayout->addRow(nameLabel, nameEdit);
    formLayout->addRow(enrollNumberLabel, enrollNumberEdit);
    formLayout->addRow(dobLabel, dobEdit);
    formLayout->addRow(joinedDateLabel, joinedDateEdit);
    formLayout->addRow(basicSalaryLabel, basicSalaryEdit);
    formLayout->addRow(lpaLabel, lpaDisplay);
    formLayout->addRow(exportButton);
    setLayout(formLayout);


    connect(exportButton,SIGNAL(clicked()),this,SLOT(exportToJSON()));
}





Dialog::~Dialog()
{
    delete ui;
}

bool Dialog::validateForm()
{

    QString name = nameEdit->text();
    QString enrollNumber = enrollNumberEdit->text();
    QDate dob = dobEdit->date();
    QDate joinedDate = joinedDateEdit->date();
    QString basicSalaryString = basicSalaryEdit->text();

    if (name.isEmpty() || enrollNumber.isEmpty() || basicSalaryString.isEmpty()) {
        QMessageBox::critical(this,"Validation Error", "Please fill in all the required fields.");
        return false;
    }

    if (name.length() < 3 || name.length() > 15) {
        QMessageBox::critical(this, "Validation Error", "Employee name should have 3 to 15 letters.");
        return false;
    }

    if (enrollNumber.length() != 6) {
        QMessageBox::critical(this, "Validation Error", "EnrollNumber should have exactly 6 letters.");
        return false;
    }

    QDate currentDate = QDate::currentDate();
    if (dob.isNull() || dob >= currentDate) {
        QMessageBox::critical(this, "Validation Error", "Invalid date of birth.");
        return false;
    }

    int age = dob.daysTo(currentDate) / 365;
    if (age < 18) {
        QMessageBox::critical(this, "Validation Error", "Employee should be at least 18 years old.");
        return false;
    }

    if (joinedDate.isNull() || joinedDate >= currentDate) {
         QMessageBox::critical(this, "Validation Error", "Invalid joined date.");
        return false;
    }

    bool ok;
    double basicSalary = basicSalaryString.toDouble(&ok);
    if (!ok || basicSalary < 10000) {
         QMessageBox::critical(this, "Validation Error", "Minimum basic salary should be 10,000 INR.");
        return false;
    }

    return true;
}

void Dialog::clearForm()
{
    nameEdit->clear();
    enrollNumberEdit->clear();
    dobEdit->setDate(QDate::currentDate());
    joinedDateEdit->setDate(QDate::currentDate());
    basicSalaryEdit->clear();
    lpaDisplay->clear();
}

void Dialog::addEmployee()
{
    if (!validateForm())
        return;

    QString name = nameEdit->text();
    QString enrollNumber = enrollNumberEdit->text();
    QDate dob = dobEdit->date();
    QDate joinedDate = joinedDateEdit->date();
    double basicSalary = basicSalaryEdit->text().toDouble();
    double lpa = basicSalary * 12;

    Employee employee{ name, enrollNumber, dob, dob.daysTo(QDate::currentDate()) / 365,
                joinedDate, basicSalary, lpa };

    employeeList.append(employee);
    QMessageBox::information(this, "Success", "Employee added successfully!");

    clearForm();
}

void Dialog::exportToJSON()
{
  addEmployee();

    if (employeeList.isEmpty()) {
        QMessageBox::critical(this, "No Employees", "No employee details available to export.");
        return;
    }

    QString filePath = QDir::currentPath() + "/employee.json";
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this,"Error", "Failed to create/open the JSON file.");
        return;
    }

    QJsonArray jsonArray;

    for (const Employee &employee :employeeList) {
        QJsonObject jsonEmployee;
        jsonEmployee["Name"] = employee.name;
        jsonEmployee["EnrollNumber"] = employee.enrollNumber;
        jsonEmployee["DateOfBirth"] = employee.dob.toString("dd-MM-yyyy");
        jsonEmployee["Age"] = employee.age;
        jsonEmployee["JoinedDate"] = employee.joinedDate.toString("dd-MM-yyyy");
        jsonEmployee["BasicSalary"] = QString::number(employee.basicSalary);
        jsonEmployee["LPA"] = QString::number(employee.lpa);

        jsonArray.append(jsonEmployee);
    }

    QJsonDocument jsonDoc(jsonArray);
    QTextStream out(&file);
    out << jsonDoc.toJson();
    qDebug()<<" checking"<<out.readAll();

    file.close();

    QMessageBox::information(this, "Export Success", "Employee details exported to employee.json file.");
}
