#include "detailsdialog.h"
#include "ui_detailsdialog.h"

DetailsDialog::DetailsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DetailsDialog)
{
    ui->setupUi(this);
    ui->closePushButton->setFocus();
}

DetailsDialog::~DetailsDialog()
{
    delete ui;
}

void DetailsDialog::setDetailsText(QString text)
{
    ui->detailsPlainTextEdit->appendPlainText(text);
}

void DetailsDialog::clearDetailsText()
{
    ui->detailsPlainTextEdit->clear();
}

void DetailsDialog::on_closePushButton_clicked()
{
    close();
}
