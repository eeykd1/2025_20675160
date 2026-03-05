#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <vtkActor.h>

namespace Ui { class OptionDialog; }

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

    // Receives the actor and the current name from MainWindow
    void setActorData(vtkActor* actor, QString name);

private slots:
    // This runs when you click the "OK" button
    void on_buttonBox_accepted();

private:
    Ui::OptionDialog *ui;
    vtkActor* m_actor;
};

#endif
