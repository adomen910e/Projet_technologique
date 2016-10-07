#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <opencv2/opencv.hpp>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void afficherMat(cv::Mat mat);


public slots:
    void afficherMessage();
    void openFile();
    void quit();
    void separation();
    void floutage();
    void sobelSlot();
    void resizeEvent(QResizeEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    QGridLayout *grid;
    QMenu *aPropos;
    QMenu *fichier;
    QMenu *traitement;
    QLabel *label;
    QLabel *labeltmp;
    QLabel *labelG;
    QLabel *labelD;
    QAction *aProposAction;
    QAction *ouvrir;
    QAction *quitter;
    QAction *separerImage;
    QAction *flouterImage;
    QAction *sobel;
    QImage image;
    QImage imageG;
    QImage imageD;
    QImage imagerect;
    QImage imagetmp;
    QPixmap map;
    QPixmap maprect;
    QPixmap maptmp;
    cv::Mat imageCV;
    int xrec;
    int yrec;
    int h;
    int w;
};

#endif // MAINWINDOW_H
