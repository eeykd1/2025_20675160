#include "optiondialog.h"
#include "ui_optiondialog.h"
#include <vtkProperty.h>

OptionDialog::OptionDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::OptionDialog), m_actor(nullptr)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog() { delete ui; }

void OptionDialog::setActorData(vtkActor* actor, QString name) {
    m_actor = actor;
    if (m_actor) {
        ui->lineEdit_PartName->setText(name);
        double rgb[3];
        m_actor->GetProperty()->GetColor(rgb);
        ui->spinBox_Red->setValue(static_cast<int>(rgb[0] * 255));
        ui->spinBox_Green->setValue(static_cast<int>(rgb[1] * 255));
        ui->spinBox_Blue->setValue(static_cast<int>(rgb[2] * 255));
        ui->checkBox_Visibility->setChecked(m_actor->GetVisibility());
    }
}

// ADDED: Returns the text currently inside the name box
QString OptionDialog::getName() const {
    return ui->lineEdit_PartName->text();
}

void OptionDialog::on_buttonBox_accepted() {
    if (m_actor) {
        double r = ui->spinBox_Red->value() / 255.0;
        double g = ui->spinBox_Green->value() / 255.0;
        double b = ui->spinBox_Blue->value() / 255.0;
        m_actor->GetProperty()->SetColor(r, g, b);
        m_actor->SetVisibility(ui->checkBox_Visibility->isChecked());
    }
}
