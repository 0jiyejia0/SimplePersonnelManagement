#ifndef ADDPAGECLASS_H
#define ADDPAGECLASS_H

#include <QMainWindow>
#include <qdialog.h>
#include "Person.h"

QT_BEGIN_NAMESPACE
namespace Ui { class addPageClass; }
QT_END_NAMESPACE

class addPageClass : public QMainWindow
{
    Q_OBJECT

public:
    explicit addPageClass(QWidget* parent = nullptr);
    ~addPageClass();
    void setPerson(const Person& person);
    void setEditMode(const Person& person);
    void setViewMode(const Person& person);
    void setAddMode();
    void setSendMode();

signals:
    void sendPerson(const Person& person);

private:
    Ui::addPageClass* ui;
    bool isEditMode;
    Person currentPerson;
};

#endif // ADDPAGECLASS_H
