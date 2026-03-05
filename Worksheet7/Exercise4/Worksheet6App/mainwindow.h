#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkActor.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void showContextMenu(const QPoint &pos);

    // NEW BUTTON SLOTS
    void on_pushButton_Open_clicked();   // Far Left
    void on_pushButton_Edit_clicked();   // Middle
    void on_pushButton_Remove_clicked(); // Far Right

private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    QStandardItemModel *treeModel;
};

#endif
