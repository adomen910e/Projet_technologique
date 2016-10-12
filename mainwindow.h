#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QPainter>
#include <QRect>
#include <QString>
#include "widget.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();




public slots:
    void afficherMessage();
    void openFile();
    void quit();
    void separation();
    void resizeEvent(QResizeEvent * event);
    void sauverRectangle (QRect *rect, QString s);

public:

    QWidget *widge;

private:

    QGridLayout *grid;
    QMenu *aPropos;
    QMenu *fichier;

    QLabel *label;
    QLabel *labelG;
    QLabel *labelD;

    QAction *aProposAction;
    QAction *ouvrir;
    QAction *quitter;
    QAction *separerImage;
    QImage image;
    QImage imageG;
    QImage imageD;
    QImage imagerect;
    QPixmap map;
    QPixmap maprect;
    QPixmap qpix;
    int xrec;
    int yrec;
    int h;
    int w;

};

#endif // MAINWINDOW_H
