#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ModelPart.h"
#include "ModelPartList.h"
#include <QFileDialog>
#include <QFileInfo>
#include "optiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- NEW: VTK 3D CYLINDER SETUP (Exercise 3) ---

    // 1. Create the Renderer (The Eyes)
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.1, 0.2, 0.4); // Dark blue background

    // 2. Create the Render Window (The Bridge)
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // 3. Link to the UI widget (Make sure objectName is 'openGLWidget' in Designer)
    ui->openGLWidget->setRenderWindow(renderWindow);

    // 4. Create the Cylinder Shape
    vtkSmartPointer<vtkCylinderSource> cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
    cylinderSource->SetRadius(5.0);
    cylinderSource->SetHeight(10.0);
    cylinderSource->SetResolution(100);

    // 5. Create Mapper and Actor
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(cylinderSource->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // 6. Add to scene and refresh
    renderer->AddActor(actor);
    renderer->ResetCamera();
    renderWindow->Render();

    // --- YOUR ORIGINAL TREE SETUP ---

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
        ModelPart* childItem = new ModelPart({ name, "false" });
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString subName = QString("Item %1,%2").arg(i).arg(j);
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

// --- YOUR ORIGINAL FUNCTIONS (UNCHANGED) ---

void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString partName = selectedPart->data(0).toString();
    ui->statusbar->showMessage("Clicked: " + partName, 2000);
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);;Text Files (*.txt);;All Files (*.*)")
        );

    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QString shortName = fileInfo.fileName();
        QModelIndex currentIndex = ui->treeView->currentIndex();
        this->partList->appendChild(currentIndex, { shortName, "false" });

        if (currentIndex.isValid()) {
            ui->treeView->expand(currentIndex);
        }
        ui->statusbar->showMessage("Added " + shortName + " to tree.", 3000);
    }
}

void MainWindow::on_actionItem_Options_triggered() {
    QModelIndex index = ui->treeView->currentIndex();

    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        OptionDialog dialog(this);

        QString currentName = selectedPart->data(0).toString();
        bool currentVisible = (selectedPart->data(1).toString() == "true");

        dialog.setValues(
            currentName,
            currentVisible,
            selectedPart->getColourR(),
            selectedPart->getColourG(),
            selectedPart->getColourB()
            );

        if (dialog.exec() == QDialog::Accepted) {
            QString newName = dialog.getName();
            bool isVisible = dialog.isVisibleChecked();
            QString visibleString = isVisible ? "true" : "false";

            this->partList->setData(index.siblingAtColumn(0), newName, Qt::EditRole);
            this->partList->setData(index.siblingAtColumn(1), visibleString, Qt::EditRole);

            selectedPart->setColour(dialog.getR(), dialog.getG(), dialog.getB());

            QModelIndex left = index.siblingAtColumn(0);
            QModelIndex right = index.siblingAtColumn(1);
            emit partList->dataChanged(left, right);

            ui->statusbar->showMessage("Updated: " + newName, 5000);
        }
    } else {
        ui->statusbar->showMessage("Please select an item first!", 2000);
    }
}

void MainWindow::handleButton1() { /* Placeholder */ }
void MainWindow::handleButton2() { on_actionItem_Options_triggered(); }
void MainWindow::handleButton3() { /* Placeholder */ }
