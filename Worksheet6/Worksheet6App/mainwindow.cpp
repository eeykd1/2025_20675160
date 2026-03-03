#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ModelPart.h"
#include "ModelPartList.h"
#include <QFileDialog>
#include <QFileInfo>      // Needed to turn long file paths into short names
#include "optiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Make the columns in the tree view fit the text automatically
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Setup the data model and connect it to the TreeView widget
    this->partList = new ModelPartList("PartsList");
    ui->treeView->setModel(this->partList);

    // Connect clicking and buttons to their functions
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::handleButton2);

    // Add the "Options" action to the TreeView (for right-click support)
    ui->treeView->addAction(ui->actionItem_Options);

    // --- INITIALIZING THE TREE WITH "FALSE" ---
    ModelPart* rootItem = this->partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        // Create top-level items with "false" visibility string
        ModelPart* childItem = new ModelPart({ name, "false" });
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString subName = QString("Item %1,%2").arg(i).arg(j);
            // Create sub-items with "false" visibility string
            ModelPart* childChildItem = new ModelPart({ subName, "false" });
            childItem->appendChild(childChildItem);
        }
    }
}

MainWindow::~MainWindow()
{
    delete partList;
    delete ui;
}

void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    // Simple status bar message when you click an item
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString partName = selectedPart->data(0).toString();
    ui->statusbar->showMessage("Clicked: " + partName, 2000);
}

// --- OPEN FILE: ADDS NEW ITEMS TO THE TREE ---
void MainWindow::on_actionOpen_File_triggered()
{
    // 1. Open the file browser (showing all files)
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);;Text Files (*.txt);;All Files (*.*)")
        );

    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QString shortName = fileInfo.fileName();

        // 2. CHECK SELECTION: Find out which item is currently highlighted
        QModelIndex currentIndex = ui->treeView->currentIndex();

        // 3. ADD TO TREE:
        // If currentIndex is valid, the file becomes a child of the selection.
        // If not valid, it adds to the top level.
        this->partList->appendChild(currentIndex, { shortName, "false" });

        // 4. EXPAND: Automatically open the branch so you can see the new child
        if (currentIndex.isValid()) {
            ui->treeView->expand(currentIndex);
        }

        ui->statusbar->showMessage("Added " + shortName + " to tree.", 3000);
    }
}

// --- ITEM OPTIONS: UPDATES NAME, VISIBILITY, AND RGB ---
void MainWindow::on_actionItem_Options_triggered() {
    QModelIndex index = ui->treeView->currentIndex();

    if (index.isValid()) {
        // 1. Get the actual ModelPart object we clicked on
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        OptionDialog dialog(this);

        // 2. PRE-FILL: Pull existing data from the part into the dialog
        QString currentName = selectedPart->data(0).toString();
        bool currentVisible = (selectedPart->data(1).toString() == "true");

        dialog.setValues(
            currentName,
            currentVisible,
            selectedPart->getColourR(),
            selectedPart->getColourG(),
            selectedPart->getColourB()
            );

        // 3. SHOW DIALOG: Wait for user to click OK
        if (dialog.exec() == QDialog::Accepted) {
            QString newName = dialog.getName();
            bool isVisible = dialog.isVisibleChecked();
            QString visibleString = isVisible ? "true" : "false";

            // 4. SAVE DATA: Put the new name and true/false into the table columns
            this->partList->setData(index.siblingAtColumn(0), newName, Qt::EditRole);
            this->partList->setData(index.siblingAtColumn(1), visibleString, Qt::EditRole);

            // 5. SAVE RGB: Store the color values inside the ModelPart object memory
            selectedPart->setColour(dialog.getR(), dialog.getG(), dialog.getB());

            // 6. REFRESH: Tell the tree to redraw these specific cells immediately
            QModelIndex left = index.siblingAtColumn(0);
            QModelIndex right = index.siblingAtColumn(1);
            emit partList->dataChanged(left, right);

            ui->statusbar->showMessage("Updated: " + newName, 5000);
        }
    } else {
        ui->statusbar->showMessage("Please select an item first!", 2000);
    }
}

// Button Handlers
void MainWindow::handleButton1() { /* Placeholder */ }
void MainWindow::handleButton2() { on_actionItem_Options_triggered(); }
void MainWindow::handleButton3() { /* Placeholder */ }
