#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optiondialog.h"
#include <QFileDialog>
#include <QMenu>
#include <QFileInfo>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);
    renderWindow->AddRenderer(renderer);
    renderer->SetBackground(0.2, 0.5, 0.9);

    treeModel = new QStandardItemModel(this);
    ui->treeView->setModel(treeModel);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::showContextMenu);

    ui->pushButton_Open->setText("Open STL");
    ui->pushButton_Edit->setText("Edit Properties");
    ui->pushButton_Remove->setText("Remove Item");

    renderWindow->Render();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_Open_clicked() {
    on_actionOpen_triggered();
}

void MainWindow::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open STL"), "", tr("STL Files (*.stl)"));
    if (fileName.isEmpty()) return;

    // VTK Loading Logic
    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(fileName.toUtf8().constData());
    reader->Update();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
    renderer->ResetCamera();
    renderWindow->Render();

    // UI List Logic
    QFileInfo info(fileName);
    QStandardItem* newItem = new QStandardItem(info.fileName());

    // Store actor pointer
    QVariant v = QVariant::fromValue((void*)actor.GetPointer());
    newItem->setData(v, Qt::UserRole);

    // --- PARENT-CHILD LOGIC ---
    // Check if there is already an item in the list
    if (treeModel->rowCount() > 0) {
        // Get the very first item we ever added
        QStandardItem* firstItem = treeModel->item(0);
        // Add this new file as a CHILD of the first one
        firstItem->appendRow(newItem);
        // Expand the list so you can see the child
        ui->treeView->expandAll();
    } else {
        // If the list is empty, this is the first item (the Parent)
        treeModel->appendRow(newItem);
    }
}

void MainWindow::on_pushButton_Edit_clicked() {
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    QStandardItem* item = treeModel->itemFromIndex(index);
    vtkActor* actor = (vtkActor*)item->data(Qt::UserRole).value<void*>();

    if (actor) {
        OptionDialog dialog(this);
        dialog.setActorData(actor, item->text());
        if (dialog.exec() == QDialog::Accepted) {
            renderWindow->Render();
        }
    }
}

void MainWindow::on_pushButton_Remove_clicked() {
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    QStandardItem* item = treeModel->itemFromIndex(index);
    vtkActor* actor = (vtkActor*)item->data(Qt::UserRole).value<void*>();

    if (actor) {
        renderer->RemoveActor(actor);
        // Use the model to remove the row correctly even if it's a child
        treeModel->removeRow(index.row(), index.parent());
        renderWindow->Render();
    }
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid()) return;

    QMenu menu(this);
    QAction* editAction = menu.addAction("Edit Properties");
    QAction* removeAction = menu.addAction("Remove Item");

    QAction* selectedAction = menu.exec(ui->treeView->mapToGlobal(pos));

    if (selectedAction == editAction) {
        on_pushButton_Edit_clicked();
    } else if (selectedAction == removeAction) {
        on_pushButton_Remove_clicked();
    }
}
