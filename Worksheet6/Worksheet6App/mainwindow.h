#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "ModelPart.h"
#include "ModelPartList.h"
#include "optiondialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void statusUpdateMessage(const QString &message, int timeout);

private slots:
    // --- File Menu Actions ---
    void on_actionOpen_File_triggered();

    // --- Exercise 10: Context Menu Action ---
    void on_actionItem_Options_triggered();

    // --- Tree and Button Handlers ---
    void handleTreeClicked(const QModelIndex &index);
    void handleButton1();
    void handleButton2();
    void handleButton3();

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;
};

#endif // MAINWINDOW_H
