#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PersonManage.h"
#include <QStandardItemModel>
#include "PersonSet.h"
#include "addPage.h"
#include <stack>

class PersonManage : public QMainWindow
{
    Q_OBJECT

public:
    PersonManage(QWidget* parent = nullptr);
    ~PersonManage();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void sortTable(int column);
    void openNewWindow();
    void openEditWindow();
    void openViewWindow();
    void undo();
    void redo();
    void showContextMenu(const QPoint& pos);

private:
    Ui::PersonManageClass ui;
    QStandardItemModel* model;
    PersonSet personSet;
    addPageClass* addpages;
    addPageClass* editpages;
    string selectedPerson = "";
    Qt::SortOrder sortOrder = Qt::AscendingOrder;
    QAction* m_pActionCopy;
    void populateTableView();
    void search();
    void onCellClicked(const QModelIndex& index);
    void deletePerson();
    void saveData();
    std::stack<std::tuple<QString, Person, Person>> undoStack; // 记录旧信息和新信息
    std::stack<std::tuple<QString, Person, Person>> redoStack;
    void pushUndoStack(const QString& action, const Person& oldPerson, const Person& newPerson);
};

