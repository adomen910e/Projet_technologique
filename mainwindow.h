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
#include <opencv2/opencv.hpp>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void afficherMat(cv::Mat mat,QImage::Format format);



public slots:
    void afficherMessage();
    void openFile();
    void quit();
    void separation();
    void floutage();
    void sobelSlot();
    void cannySlot();
    void resizeEvent(QResizeEvent * event);
    void sauverRectangle (QRect *rect, QString s);

public:
    QWidget *widge;

private:
    QMenu *aPropos;
    QMenu *fichier;
    QMenu *traitement;
    QLabel *label;
    QLabel *labeltmp;
    QAction *flouterImage;
    QAction *sobel;
    QAction *canny;
    QAction *aProposAction;
    QAction *ouvrir;
    QAction *quitter;
    QAction *separerImage;
    QImage image;
    QImage imageG;
    QImage imageD;
    QImage imagerect;
    QImage imagetmp;
    QPixmap map;
    QPixmap maprect;
    QPixmap qpix;
    QPixmap maptmp;
    cv::Mat imageCV;
    int xrec;
    int yrec;
    int h;
    int w;
    int nb_label;
};

#endif // MAINWINDOW_H
