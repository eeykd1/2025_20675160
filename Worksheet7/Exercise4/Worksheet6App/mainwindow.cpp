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

    // 1. Setup VTK with Blue Background
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);
    renderWindow->AddRenderer(renderer);
    renderer->SetBackground(0.2, 0.5, 0.9);

    // 2. Setup TreeView
    treeModel = new QStandardItemModel(this);
    ui->treeView->setModel(treeModel);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::showContextMenu);

    // 3. SET BUTTON TEXT VIA CODE
    // This ensures your buttons don't just say "PushButton"
    ui->pushButton_Open->setText("Open STL");
    ui->pushButton_Edit->setText("Edit Properties");
    ui->pushButton_Remove->setText("Remove Item");

    renderWindow->Render();
}

MainWindow::~MainWindow() { delete ui; }

// --- FAR LEFT BUTTON: OPEN FILE ---
void MainWindow::on_pushButton_Open_clicked() {
    on_actionOpen_triggered();
}

void MainWindow::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open STL"), "", tr("STL Files (*.stl)"));
    if (fileName.isEmpty()) return;

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

    QFileInfo info(fileName);
    QStandardItem* item = new QStandardItem(info.fileName());

    QVariant v = QVariant::fromValue((void*)actor.GetPointer());
    item->setData(v, Qt::UserRole);
    treeModel->appendRow(item);
}

// --- MIDDLE BUTTON: EDIT PROPERTIES ---
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

// --- FAR RIGHT BUTTON: REMOVE ITEM ---
void MainWindow::on_pushButton_Remove_clicked() {
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    QStandardItem* item = treeModel->itemFromIndex(index);
    vtkActor* actor = (vtkActor*)item->data(Qt::UserRole).value<void*>();

    if (actor) {
        renderer->RemoveActor(actor);
        treeModel->removeRow(index.row());
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
