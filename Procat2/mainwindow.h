#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>

#include <QWidget>
#include <QFileSystemModel>
#include <QDir>
#include "object.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    List<Object_of*> Data;

private slots:
    void edit();
    void del();
    void on_comboBox_type_currentIndexChanged(int index);
    void on_comboBox_case_2_activated(int index);
    void on_comboBox_case_1_activated(int index);
    void to_del();
};
#endif // MAINWINDOW_H
