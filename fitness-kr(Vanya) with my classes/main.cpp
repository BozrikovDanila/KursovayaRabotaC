#include <QApplication>
#include "MainWindow.h"

/*
 * тема: фитнес клуб
 *
 * Human:
 *  фио, пол, возраст, вес
 *
 * HumanTeacher (тренер):
 *  ID, телефон, ЗП
 *
 * HumanManager (менеджер):
 *  ЗП, есть ли вышка
 *
 * HumanClient (Клиент):
 *  ID абонемента, срок окончания абонемента
 *
 */

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
