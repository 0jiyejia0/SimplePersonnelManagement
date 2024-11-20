#include "PersonManage.h"
#include "Person.h"
#include "addPage.h"
#include <stack>
#include <QTableWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QDebug>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCloseEvent>
#include <QMessageBox>
#include <QLabel>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QStatusBar>
#include <QMenu>

PersonManage::PersonManage(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    if (!ui.statusBar) {
        ui.statusBar = new QStatusBar(this);
        setStatusBar(ui.statusBar);
    }

    // 添加永久小部件和时间标签
    ui.statusBar->addPermanentWidget(new QLabel("Capital Normal University"));
    QLabel* timeLabel = new QLabel;
    ui.statusBar->addWidget(timeLabel);

    // 设置计时器更新时间
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [timeLabel]() {
        QDateTime dateTime = QDateTime::currentDateTime();
        timeLabel->setText(dateTime.toString("yyyy-MM-dd hh:mm:ss"));
        });
    timer->start(1000);

    // 初始化模型
    model = new QStandardItemModel(this);

    // 设置模型到tableView
    ui.tableView->setModel(model);

    // 启用排序功能
    ui.tableView->setSortingEnabled(true);
    ui.tableView->horizontalHeader()->setSortIndicatorShown(true);
    ui.tableView->horizontalHeader()->setSectionsClickable(true);

    // 自动调整列宽
    ui.tableView->resizeColumnsToContents();

    // 拉伸最后一列以填满剩余空间
    ui.tableView->horizontalHeader()->setStretchLastSection(true);

    // 设置所有列为可拉伸模式
    for (int i = 0; i < model->columnCount(); ++i) {
        ui.tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }

    // 添加表头
    model->setHorizontalHeaderLabels({ "ID", "Name", "Sex", "Salary", "Birthday" });

    // 填充一些Person对象到personSet中（测试数据）
    personSet.loadFromFile("persons.json");

    ui.addpersonnel->setIcon(QIcon("./icon/add.png"));
    ui.deletePersonnel->setIcon(QIcon("./icon/delete.png"));
    ui.editPersonnel->setIcon(QIcon("./icon/modify.png"));
    ui.Undo->setIcon(QIcon("./icon/undo.png"));
    ui.Redo->setIcon(QIcon("./icon/redo.png"));
    ui.search->setIcon(QIcon("./icon/search.ico"));
    ui.view->setIcon(QIcon("./icon/view.png"));

    ui.Undo->setEnabled(false);
    ui.Redo->setEnabled(false);

    populateTableView();

    connect(ui.Undo, &QAction::triggered, this, &PersonManage::undo);
    connect(ui.Redo, &QAction::triggered, this, &PersonManage::redo);

    connect(ui.addpersonnel, &QAction::triggered, this, &PersonManage::openNewWindow);

    connect(ui.tableView, &QAbstractItemView::clicked, this, &PersonManage::onCellClicked);

    connect(ui.deletePersonnel, &QAction::triggered, this, &PersonManage::deletePerson);

    connect(ui.editPersonnel, &QAction::triggered, this, &PersonManage::openEditWindow);

    connect(ui.view, &QAction::triggered, this, &PersonManage::openViewWindow);

    connect(ui.search, &QPushButton::clicked, this, &PersonManage::search);

    connect(ui.tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &PersonManage::sortTable);

    ui.tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tableView, &QTableView::customContextMenuRequested, this, &PersonManage::showContextMenu);
}

PersonManage::~PersonManage()
{
    delete model;
}

void PersonManage::sortTable(int column)
{
    // 切换排序顺序
    sortOrder = (sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    ui.tableView->sortByColumn(column, sortOrder);
}

void PersonManage::showContextMenu(const QPoint& pos)
{
    QModelIndex index = ui.tableView->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    QMenu contextMenu(QString::fromLocal8Bit("菜单"), this);

    QAction* editAction = new QAction(QString::fromLocal8Bit("编辑"), this);
    selectedPerson = index.sibling(index.row(), 0).data().toString().toStdString();
    connect(editAction, &QAction::triggered, [this, index]() {
        openEditWindow();
    });
    contextMenu.addAction(editAction);

    QAction* viewAction = new QAction(QString::fromLocal8Bit("查看"), this);
    connect(viewAction, &QAction::triggered, [this, index]() {
        openViewWindow();
    });
    contextMenu.addAction(viewAction);

    contextMenu.exec(ui.tableView->viewport()->mapToGlobal(pos));
}

void PersonManage::populateTableView()
{
    model->removeRows(0, model->rowCount());
    for (const Person& person : personSet.getAllPersons())
    {
        QList<QStandardItem*> items;

        //QStandardItem* idItem = new QStandardItem();
        //idItem->setData(QString::fromStdString(person.GetId()).toInt(), Qt::DisplayRole);
        //items.append(idItem);
        items.append(new QStandardItem(QString::fromStdString(person.GetId())));
        items.append(new QStandardItem(QString::fromStdString(person.GetName())));
        items.append(new QStandardItem(QString::fromStdString(person.GetSex())));

        QStandardItem* salaryItem = new QStandardItem();
        salaryItem->setData(QString::number(person.GetSalary(), 'f', 2), Qt::DisplayRole);
        //salaryItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter); // 对齐方式
        items.append(salaryItem);

        items.append(new QStandardItem(QString::fromStdString(person.GetBirthday().Format())));
        model->appendRow(items);
    }

    ui.tableView->setModel(model);
    ui.tableView->setSortingEnabled(true);  // 启用排序功能
    ui.tableView->horizontalHeader()->setSortIndicatorShown(true);
    ui.tableView->horizontalHeader()->setSectionsClickable(true);
}

void PersonManage::search()
{
    QCheckBox* isSearch = findChild<QCheckBox*>("isSearch");
    QLineEdit* searchCondition = findChild<QLineEdit*>("searchCondition");

    if (isSearch->isChecked()) {
        QString condition = searchCondition->text();
        model->removeRows(0, model->rowCount());
        for (const Person& person : personSet.getAllPersons()) {
            if (QString::fromStdString(person.GetId()) == condition|| QString::fromStdString(person.GetName())==condition
                || QString::fromStdString(person.GetPhoneNo())==condition|| QString::fromStdString(person.GetPost())==condition
                || QString::fromStdString(person.GetSex())==condition|| QString::fromStdString(person.GetAddress())==condition
                || QString::fromStdString(person.GetDepartment())== condition|| QString::fromStdString(person.GetEmployeeNo()) == condition
                || QString::number(person.GetSalary())==condition|| QString::fromStdString(person.GetBirthday().Format())== condition) {
                QList<QStandardItem*> items;
                items.append(new QStandardItem(QString::fromStdString(person.GetId())));
                items.append(new QStandardItem(QString::fromStdString(person.GetName())));
                items.append(new QStandardItem(QString::fromStdString(person.GetSex())));
                items.append(new QStandardItem(QString::number(person.GetSalary())));
                items.append(new QStandardItem(QString::fromStdString(person.GetBirthday().Format())));
                model->appendRow(items);
            }
        }
    }
    else {
        populateTableView();
    }
}

void PersonManage::openNewWindow()
{
    addPageClass* addpages = new addPageClass();
    addpages->setAddMode();
    connect(addpages, &addPageClass::sendPerson, this, [this](const Person& person) {
        personSet.push_back(person);
        pushUndoStack("add", Person(), person);
        populateTableView();
        });
    addpages->show();
}

void PersonManage::openEditWindow()
{
    if (selectedPerson == "") {
        qDebug()<<"No person selected";
        return; // 未选中任何人员
    }
    addPageClass* editPages = new addPageClass();
    auto it = std::find_if(personSet.getAllPersons().begin(), personSet.getAllPersons().end(), [&](Person& p) {
        return p.GetId() == selectedPerson;
        });
    editPages->setEditMode(*it);
    connect(editPages, &addPageClass::sendPerson, this, [this, it](const Person& person) {
        Person oldPerson = *it;
        *it = person;
        pushUndoStack("edit", oldPerson, person);
        populateTableView();
        });
    editPages->show();
}

void PersonManage::openViewWindow()
{
    auto it = std::find_if(personSet.getAllPersons().begin(), personSet.getAllPersons().end(), [&](Person& p) {
        return p.GetId() == selectedPerson;
        });
    addPageClass* viewPages = new addPageClass();
    viewPages->setViewMode(*it);
    viewPages->show();
}

void PersonManage::onCellClicked(const QModelIndex& index)
{
    selectedPerson = index.sibling(index.row(), 0).data().toString().toStdString();
}

void PersonManage::deletePerson()
{
    qDebug() << selectedPerson;
    if (selectedPerson != "")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, QString().fromLocal8Bit("删除人员"), QString().fromLocal8Bit("确定是否删除?"),
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
        Person person = personSet.getPersonById(selectedPerson);
        personSet.deletePersonById(selectedPerson);
        pushUndoStack("delete", person, Person()); // 记录旧信息和空的新信息
        populateTableView();
        selectedPerson = "";
    }
}

void PersonManage::closeEvent(QCloseEvent* event)
{
    saveData();
    QMainWindow::closeEvent(event);
}

void PersonManage::saveData()
{
    personSet.saveToFile("persons.json");
}

void PersonManage::pushUndoStack(const QString& action, const Person& oldPerson, const Person& newPerson = Person())
{
    ui.Undo->setEnabled(true);
    if (undoStack.empty()) ui.Redo->setEnabled(false);
    undoStack.push({ action, oldPerson, newPerson });
    while (!redoStack.empty()) redoStack.pop(); // 清空 redo 栈
}

void PersonManage::undo()
{
    if (undoStack.empty()) return;

    auto [action, oldPerson, newPerson] = undoStack.top();
    undoStack.pop();
    if (undoStack.empty()) ui.Undo->setEnabled(false);

    if (action == "add")
    {
        ui.Redo->setEnabled(true);
        personSet.deletePersonById(newPerson.GetId());
        redoStack.push({ "delete", newPerson, Person() });
    }
    else if (action == "delete")
    {
        ui.Redo->setEnabled(true);
        personSet.push_back(oldPerson);
        redoStack.push({ "add", oldPerson, Person() });
    }
    else if (action == "edit")
    {
        ui.Redo->setEnabled(true);
        Person tempPerson;
        tempPerson=newPerson;
        personSet.editPerson(newPerson, oldPerson);
        redoStack.push({ "edit", tempPerson, oldPerson});
    }
    populateTableView();
}

void PersonManage::redo()
{
    if (redoStack.empty()) return;
    auto [action, oldPerson, newPerson] = redoStack.top();
    redoStack.pop();
    if (redoStack.empty()) ui.Redo->setEnabled(false);

    if (action == "add")
    {
        ui.Undo->setEnabled(true);
        personSet.deletePersonById(oldPerson.GetId());
        undoStack.push({ "delete", oldPerson, Person() });
    }
    else if (action == "delete")
    {
        ui.Undo->setEnabled(true);
        personSet.push_back(oldPerson);
        undoStack.push({ "add", Person(), oldPerson });
    }
    else if (action == "edit")
    {
        ui.Undo->setEnabled(true);
        Person tempPerson;
        tempPerson = newPerson;
        personSet.editPerson(newPerson, oldPerson);
        undoStack.push({ "edit", tempPerson,oldPerson});
    }
    populateTableView();
}
