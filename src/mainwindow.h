#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MyMACAddr.h"
#include "dialog.h"
#include "settings.h"
#include <QInputDialog>
#include <QListView>
#include <QMainWindow>
#include <QMessageBox>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QThread>
#include <QTimer>
#include <QUdpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QThread *thread;
    QStandardItemModel *mdl;
    QItemSelectionModel *selectionModel;
    QTimer *timer;
    QTimer *attacktimer;
    Dialog *dialog;
    MyMACAddr *ptr;
    unordered_map<string, string> list;
    unordered_map<string, string> listR;
    vector<string> selList;
    QString selname;
    QString selmac;
    int count;

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void on_actionDark_style_toggled(bool arg1);
    void fillData();
    void saveSelected();
    void restoreSelected();
    void restoreSelected(QList<QVariant> indexesR);
    void changeMAC();
    void refreshList();
    void sendMAC();
    void startBroadcasting();
    void broadcastDatagram();
    QString findOriginal(QString name);

private slots:
    void on_pushButton_clicked();

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_actionAuto_change_at_startup_toggled(bool arg1);

    void on_checkBox_toggled(bool checked);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket = nullptr;
    int messageNo = 1;
};

#endif // MAINWINDOW_H
