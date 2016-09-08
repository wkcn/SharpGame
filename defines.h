#ifndef DEFINES_H
#define DEFINES_H

#include <QApplication>
#include <QMainWindow>
#include <QFileDialog>
#include <QMouseEvent>
#include <QTimer>
#include <QPainter>
#include <string>
#include <map>

using namespace std;

QDir DirectoryOf(const QString &subdir);
QString GetFileDir(QString file);
string GetStdFileDir(string file);

#endif // DEFINES_H
