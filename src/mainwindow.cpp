#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Settings::setTheme(Settings::loadTheme());
    ui->actionDark_style->setChecked(Settings::loadTheme());
    timer = new QTimer(this);
    udpSocket = new QUdpSocket(this);
    ptr = new MyMACAddr();

    //initialize the model for store adapter names and MAC addresses
    mdl = new QStandardItemModel(0, 4, parent);
    mdl->setHeaderData(0, Qt::Horizontal, QObject::tr("Adapter"));
    mdl->setHeaderData(1, Qt::Horizontal, QObject::tr("MAC Address"));
    mdl->setHeaderData(2, Qt::Horizontal, QObject::tr("Changed"));

    fillData();
    restoreSelected(Settings::readSettings("Adapters", "selected").toList());
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshList()));
    timer->start(500);

    //hide the attack elements
    ui->pushButton_2->hide();
    ui->checkBox->hide();
    ui->doubleSpinBox->hide();
    ui->doubleSpinBox->setEnabled(false);

    if (Settings::readSettings("Startup", "Auto").toBool())
        on_pushButton_clicked();

    //if you add a -attack parameter then go to attack mode
    if (qApp->arguments().contains("-attack")) {
        ui->checkBox->show();
        ui->doubleSpinBox->show();
    }
}

MainWindow::~MainWindow()
{
    delete ptr;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    saveSelected();
    event->accept();
}

void MainWindow::on_actionDark_style_toggled(bool arg1)
{
    if (arg1)
        Settings::setTheme(Settings::darkFusion);
    else
        Settings::setTheme(Settings::lightFusion);
}

//fills the entire table from system adapters
void MainWindow::fillData()
{
    list.clear();
    list = ptr->getAdapters();

    mdl->removeRows(0, mdl->rowCount());
    for (auto &itm : list) {
        mdl->insertRow(0);
        mdl->setData(mdl->index(0, 0), QString::fromStdString(itm.first));
        mdl->setData(mdl->index(0, 1), QString::fromStdString(itm.second));
        mdl->setData(mdl->index(0, 2), "No");
        mdl->setData(mdl->index(0, 3), findOriginal(QString::fromStdString(itm.first)));
    }
    ui->treeView->setModel(mdl);
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
    ui->treeView->resizeColumnToContents(2);
    ui->treeView->hideColumn(3);
}

//save selected adapter list to file for call it during the next startup
void MainWindow::saveSelected()
{
    QVariantList indexesRW;

    for (auto &index : selList) {
        indexesRW.push_back(QString::fromStdString(index));
    }
    Settings::writeSettings("Adapters", "selected", indexesRW);
}

//restore selected adapters from selList
void MainWindow::restoreSelected()
{
    for (auto &index : selList) {
        for (auto data : mdl->findItems(QString::fromStdString(index))) {
            ui->treeView->selectionModel()->select(data->index(), QItemSelectionModel::Select);
            ui->treeView->selectionModel()->select(data->index().siblingAtColumn(1),
                                                   QItemSelectionModel::Select);
            ui->treeView->selectionModel()->select(data->index().siblingAtColumn(2),
                                                   QItemSelectionModel::Select);
        }
    }
}

//restore selected adapters from settings file
void MainWindow::restoreSelected(QList<QVariant> indexesR)
{
    for (auto &index : indexesR) {
        selList.push_back(index.toString().toStdString());
        for (auto data : mdl->findItems(index.toString())) {
            ui->treeView->selectionModel()->select(data->index(), QItemSelectionModel::Select);
            ui->treeView->selectionModel()->select(data->index().siblingAtColumn(1),
                                                   QItemSelectionModel::Select);
            ui->treeView->selectionModel()->select(data->index().siblingAtColumn(2),
                                                   QItemSelectionModel::Select);
        }
    }
}

//change MAC addresses for selected adapters
void MainWindow::changeMAC()
{
    ptr->assignRndMAC(&selList);
    //list = ptr->getAdapters();
}

//refresh the list for see new MAC addresses
void MainWindow::refreshList()
{
    list = ptr->getAdapters();
    for (auto &index : list) {
        for (auto data : mdl->findItems(QString::fromStdString(index.first))) {
            ui->treeView->model()->setData(data->index().siblingAtColumn(1),
                                           QString::fromStdString(index.second));
            if (mdl->data(data->index().siblingAtColumn(1)).toString()
                != mdl->data(data->index().siblingAtColumn(3)).toString())
                ui->treeView->model()->setData(data->index().siblingAtColumn(2), "Yes");
            else
                ui->treeView->model()->setData(data->index().siblingAtColumn(2), "No");
        }
    }
}

//receive manual MAC entry from edit dialog then set
void MainWindow::sendMAC()
{
    qDebug() << dialog->getDText().remove('-');
    ptr->assignOrigMAC(selname.toStdString(), dialog->getDText().remove('-').toStdString());
    list = ptr->getAdapters();
}

//send the broadcast packet from port 45454
void MainWindow::broadcastDatagram()
{
    QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo);
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 45454);
    //! [1]
    ++messageNo;
}

//change MAC address to random value and starts sending broadcast package
void MainWindow::startBroadcasting()
{
    thread = QThread::create([&] { changeMAC(); });
    thread->start();
    broadcastDatagram();
}

//find the original MAC address of given adapter name
QString MainWindow::findOriginal(QString name)
{
    QString result;
    for (uint8_t i = 0; i < 100; ++i) {
        if (i < 9) {
            QSettings findAdapter("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{"
                                  "4D36E972-E325-11CE-BFC1-08002bE10318}\\000"
                                      + QString::number(i),
                                  QSettings::NativeFormat);
            if (findAdapter.value("DriverDesc").toString() == name) {
                result = findAdapter.value("OriginalNetworkAddress").toString();
                break;
            }

        }

        else {
            QSettings findAdapter("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{"
                                  "4D36E972-E325-11CE-BFC1-08002bE10318}\\00"
                                      + QString::number(i),
                                  QSettings::NativeFormat);
            if (findAdapter.value("DriverDesc").toString() == name) {
                result = findAdapter.value("OriginalNetworkAddress").toString();
                break;
            }
        }
    }
    if (result.isNull())
        return "";
    else
        return result;
}

void MainWindow::on_pushButton_clicked()
{
    selList.clear();
    selectionModel = ui->treeView->selectionModel();
    QModelIndexList indexes = selectionModel->selectedIndexes();

    for (auto &index : indexes) {
        if (index.column() == 0)
            selList.push_back(selectionModel->model()->data(index).toString().toStdString());
    }

    thread = QThread::create([&] { changeMAC(); });
    thread->start();
    if (ui->checkBox->isChecked()) {
        attacktimer = new QTimer(this);
        connect(attacktimer, SIGNAL(timeout()), this, SLOT(startBroadcasting()));
        attacktimer->start(ui->doubleSpinBox->value() * 1000);
        ui->pushButton_2->show();
    }
}

//double click a adapter to manual entry or restore original MAC address
void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    dialog = new Dialog;
    connect(dialog, SIGNAL(accepted()), this, SLOT(sendMAC()));
    dialog->setDText(index.siblingAtColumn(0).data().toString(),
                     index.siblingAtColumn(1).data().toString());
    dialog->origMAC = findOriginal(index.siblingAtColumn(0).data().toString());
    selname = index.siblingAtColumn(0).data().toString();

    dialog->exec();
}

void MainWindow::on_actionAuto_change_at_startup_toggled(bool arg1)
{
    Settings::writeSettings("Startup", "Auto", arg1);
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    ui->doubleSpinBox->setEnabled(checked);
}

void MainWindow::on_pushButton_2_clicked()
{
    thread->exit();
    attacktimer->stop();
    ui->pushButton_2->hide();
}
