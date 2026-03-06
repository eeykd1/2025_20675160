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

    void setActorData(vtkActor* actor, QString name);

    // ADDED: This lets MainWindow "get" the new name from the dialog
    QString getName() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::OptionDialog *ui;
    vtkActor* m_actor;
};

#endif
