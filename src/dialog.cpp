#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getDText()
{
    return ui->lineEdit->text();
}

QString Dialog::getName()
{
    return name;
}

void Dialog::setDText(QString name_, QString text)
{
    name = name_;
    ui->label->setText(ui->label->text() + name_);
    ui->lineEdit->setText(text);
}

void Dialog::on_pushButton_clicked()
{
    ui->lineEdit->setText(origMAC);
}

void Dialog::on_pushButton_2_clicked()
{
    if (ui->lineEdit->text().size() < 17)
        QMessageBox::warning(this,
                             "Wrong entry!",
                             "Please enter correct MAC format (XX-XX-XX-XX-XX-XX)");
    else
        this->accept();
}

void Dialog::on_pushButton_3_clicked()
{
    this->reject();
}
