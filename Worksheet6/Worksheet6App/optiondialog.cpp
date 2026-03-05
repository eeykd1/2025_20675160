#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent) : QDialog(parent), ui(new Ui::OptionDialog) {
    ui->setupUi(this);
}

OptionDialog::~OptionDialog() { delete ui; }

void OptionDialog::setValues(QString name, bool visible, int r, int g, int b) {
    ui->nameEdit->setText(name);
    ui->visibleCheck->setChecked(visible);
    ui->rSpin->setValue(r);
    ui->gSpin->setValue(g);
    ui->bSpin->setValue(b);
}

QString OptionDialog::getName() const { return ui->nameEdit->text(); }
bool OptionDialog::isVisibleChecked() const { return ui->visibleCheck->isChecked(); }
int OptionDialog::getR() const { return ui->rSpin->value(); }
int OptionDialog::getG() const { return ui->gSpin->value(); }
int OptionDialog::getB() const { return ui->bSpin->value(); }
