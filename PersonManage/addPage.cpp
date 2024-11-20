#include "addPage.h"
#include "ui_addPage.h"
#include "Person.h"
#include "Date.h"
#include "PersonManage.h"
#include <QMessageBox>

addPageClass::addPageClass(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::addPageClass)
{
    ui->setupUi(this);

    ui->id->setText(QString::fromStdString(""));
    ui->name->setText(QString::fromStdString(""));
    ui->phoneNo->setText(QString::fromStdString(""));
    ui->address->setText(QString::fromStdString(""));
    ui->name_employeeNo->setText(QString::fromStdString(""));
    ui->department->setText(QString::fromStdString(""));
    ui->post->setText(QString::fromStdString(""));
    ui->salary->setValue(0.0);
    ui->birthday->setDate(QDate(2000, 1, 1));
    ui->comboBoxsex->setCurrentText(QString::fromStdString("男"));

    // Connect signals and slots
    //connect(ui->submit, &QPushButton::clicked, this, &addPageClass::on_submit_clicked);
    //connect(ui->cancel, &QPushButton::clicked, this, &addPageClass::on_cancel_clicked);
}

addPageClass::~addPageClass()
{
    delete ui;
}

void addPageClass::setPerson(const Person& person) {
    currentPerson=person;
    ui->id->setText(QString::fromStdString(person.GetId()));
    ui->name->setText(QString::fromStdString(person.GetName()));
    ui->phoneNo->setText(QString::fromStdString(person.GetPhoneNo()));
    ui->address->setText(QString::fromStdString(person.GetAddress()));
    ui->name_employeeNo->setText(QString::fromStdString(person.GetEmployeeNo()));
    ui->department->setText(QString::fromStdString(person.GetDepartment()));
    ui->post->setText(QString::fromStdString(person.GetPost()));
    ui->salary->setValue(person.GetSalary());
    ui->birthday->setDate(QDate(person.GetBirthday().GetYear(), person.GetBirthday().GetMonth(), person.GetBirthday().GetDay()));
    ui->comboBoxsex->setCurrentText(QString::fromStdString(person.GetSex()));
}

void addPageClass::setEditMode(const Person& person) {
    setPerson(person);
        //ui->submit->setText((editMode ? QString::fromUtf8("更新") : QString::fromUtf8("添加")));
    connect(ui->submit, &QPushButton::clicked, this, &addPageClass::setSendMode);
    connect(ui->cancel, &QPushButton::clicked, this, &addPageClass::close);
    ui->close->setVisible(false);
}

void addPageClass::setViewMode(const Person& person)
{
    setPerson(person);
    // 禁用所有输入控件
    ui->id->setReadOnly(true);
    ui->name->setReadOnly(true);
    ui->phoneNo->setReadOnly(true);
    ui->department->setReadOnly(true);
    ui->post->setReadOnly(true);
    ui->address->setReadOnly(true);
    ui->name_employeeNo->setReadOnly(true);
    ui->birthday->setDisabled(true);
    ui->salary->setDisabled(true);
    ui->comboBoxsex->setDisabled(true);
    ui->submit->setVisible(false);
    ui->cancel->setVisible(false);
    connect(ui->close, &QPushButton::clicked, this, &addPageClass::close);
}

void addPageClass::setAddMode() {
    connect(ui->submit, &QPushButton::clicked, this, &addPageClass::setSendMode);
    connect(ui->cancel, &QPushButton::clicked, this, &addPageClass::close);
    ui->close->setVisible(false);
}

void addPageClass::setSendMode() {
    QString name = ui->name->toPlainText();
    QString id = ui->id->toPlainText();
    QString phoneNo = ui->phoneNo->toPlainText();
    QString department = ui->department->toPlainText();
    QString post = ui->post->toPlainText();
    QString address = ui->address->toPlainText();
    QString employeeNo = ui->name_employeeNo->toPlainText();
    QDate birthday = ui->birthday->date();
    double salary = ui->salary->value();
    QString sex = ui->comboBoxsex->currentText();

    if (id.isEmpty()) {
        //QMessageBox::warning(this, tr("Invalid Input"), tr("ID cannot be empty."));
        return; // 阻止添加操作
    }
    if (id.toStdString()[0] == '0') {
        return;
    }

    currentPerson.edit(id.toStdString(), name.toStdString(), sex.toStdString(), phoneNo.toStdString(),
        address.toStdString(), employeeNo.toStdString(), department.toStdString(), post.toStdString(),
        salary, Date(birthday.year(), birthday.month(), birthday.day()));

    emit sendPerson(currentPerson);
    this->close();
}