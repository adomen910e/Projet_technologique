#include "mainwindow.h"
#include "widget.h"

#include <QtGui>
#include <QFileDialog>
#include <iostream>
#include <QGraphicsRectItem>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/nonfree/nonfree.hpp">


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    nb_label = 0;
    traitement = NULL;
    label = new QLabel(this);
    labeltmp = new QLabel(this);
    label->move(0,30);
    aProposAction=new QAction(tr("&A Propos"),this);
    ouvrir=new QAction(tr("&Ouvrir"),this);
    quitter=new QAction(tr("&Quitter"),this);
    separerImage=new QAction(tr("&Separer l'image en 2"),this);
    aPropos = menuBar()->addMenu(tr("&A Propos"));
    fichier = menuBar()->addMenu(tr("&Fichier"));
    flouterImage=new QAction(tr("&Flouter l'image"),this);
    sobel=new QAction(tr("&Effet Sobel"),this);
    canny=new QAction(tr("&Effet Canny"),this);
    carteProfondeurAction=new QAction(tr("&Carte de Profondeur"),this);
    aPropos->addAction(aProposAction);
    fichier->addAction(ouvrir);
    fichier->addAction(quitter);
    widge = new Widget(this);
    this->setCentralWidget(widge);
    QObject::connect(aProposAction,SIGNAL(triggered()),this,SLOT(afficherMessage()));
    QObject::connect(ouvrir,SIGNAL(triggered()),this,SLOT(openFile()));
    QObject::connect(quitter,SIGNAL(triggered()),this,SLOT(quit()));

}

MainWindow::~MainWindow()
{
    aPropos=NULL;
    fichier=NULL;
    label=NULL;
    aProposAction=NULL;
    canny = NULL;
    ouvrir=NULL;
    quitter=NULL;
    xrec = 0;
    yrec = 0;
    nb_label = 0;
    traitement = NULL;
}

void MainWindow::carteProfondeur(){

}

cv::Mat MainWindow::extractionFeatures(cv::Mat mat){
    int minHessian = 400;
    //cv::SurfFeatureDetector detector( minHessian );
}


void MainWindow::sauverRectangle (QRect *rect, QString s)
{
     QPixmap qp = *label->pixmap();

     QPixmap tmp = qp.copy(*rect);
     tmp.save(s);
 }



void MainWindow::afficherMessage()
{
    QMessageBox msgBox;
    msgBox.setText("Application TP1");
    msgBox.exec();
}

void MainWindow::openFile()
{
    if(traitement!=NULL){
        traitement->removeAction(carteProfondeurAction);
    }
    nb_label = 1;
    labeltmp->clear();
    QString fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir une image"),"/",tr("Image Files (*.png *.jpg *.bmp)"));
    QImageReader *reader = new QImageReader();
    reader->setFileName(fileName);
    image =reader->read();
    map=QPixmap::fromImage(image);
    label->setPixmap(map);
    fichier->addAction(separerImage);
    QObject::connect(separerImage,SIGNAL(triggered()),this,SLOT(separation()));
    if(init){
        traitement = menuBar()->addMenu(tr("&Traitement de l'image"));
        traitement->addAction(flouterImage);
        traitement->addAction(sobel);
        traitement->addAction(canny);
        QObject::connect(flouterImage,SIGNAL(triggered()),this,SLOT(floutage()));
        QObject::connect(sobel,SIGNAL(triggered()),this,SLOT(sobelSlot()));
        QObject::connect(canny,SIGNAL(triggered()),this,SLOT(cannySlot()));
        init = false;
    }
    label->resize(image.size());
    this->resize(image.width(),image.height()+30);
    labeltmp->move(10+label->width(),30);
    label->show();
}

void MainWindow::afficherMat(cv::Mat mat,QImage::Format format){
    imagetmp = QImage(mat.data, mat.cols, mat.rows, mat.step,format);
    maptmp = QPixmap::fromImage(imagetmp);
    labeltmp->resize(image.size());
    labeltmp->show();
    labeltmp->setPixmap(maptmp);
    this->resize(2*label->width(),label->height());
}

void MainWindow::sobelSlot(){
    nb_label = 2;
    cv::Mat tmp(image.height(),image.width(),CV_8UC4,(uchar*)image.bits(),image.bytesPerLine());
    imageCV = tmp;
    cv::Mat src_gray;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::cvtColor( imageCV, src_gray, cv::COLOR_BGR2GRAY );
    cv::Sobel(src_gray,grad_x,CV_16S,1,0,3,1,0,cv::BORDER_DEFAULT);
    cv::Sobel(src_gray,grad_y,CV_16S,0,1,3,1,0,cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_x,abs_grad_x);
    cv::convertScaleAbs(grad_y,abs_grad_y);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, imageCV );
    afficherMat(imageCV,image.Format_Indexed8);
}
void MainWindow::cannySlot(){
    nb_label = 2;
    cv::Mat tmp(image.height(),image.width(),CV_8UC4,(uchar*)image.bits(),image.bytesPerLine());
    cv::Mat imgGrayscale;
    cv::Mat imgBlurred;
    cv::Mat imgCanny;
    imageCV = tmp;
    cv::cvtColor(imageCV, imgGrayscale, CV_BGR2GRAY);
    cv::GaussianBlur(imgGrayscale,imgBlurred,cv::Size(5, 5),1.5);
    cv::Canny(imgBlurred,imgCanny,100,200);
    afficherMat(imgCanny,image.Format_Indexed8);
}

void MainWindow::floutage(){
    nb_label = 2;
    cv::Mat tmp(image.height(),image.width(),CV_8UC4,(uchar*)image.bits(),image.bytesPerLine());
    cv::Mat imgBlurred;
    imageCV = tmp;
    cv::blur(imageCV,imgBlurred,cv::Size(5,5));
    afficherMat(imgBlurred,image.Format_ARGB32);
}

void MainWindow::separation(){
    traitement->addAction(carteProfondeurAction);
    QObject::connect(carteProfondeurAction,SIGNAL(triggered()),this,SLOT(carteProfondeur()));
    nb_label = 2;
    this->resize(image.width(),image.height()+30);
    QImage image1(w/2,h,QImage::Format_RGB32);
    QImage image2(w/2,h,QImage::Format_RGB32);
    for(int y = 0;y<h-30;y++){
        for(int x = 0;x<w/2;x++){
            image1.setPixel(x,y,image.pixel(x,y));
            image2.setPixel(x,y,image.pixel(x+w/2,y));
        }
    }
    imageG = image1;
    imageD = image2;
    map=QPixmap::fromImage(imageG);
    label->setPixmap(map);
    label->show();
    map=QPixmap::fromImage(imageD);
    labeltmp->move(w/2+10,30);
    labeltmp->setPixmap(map);
    labeltmp->show();
    this->resize(image.width(),image.height());
}

void MainWindow::quit()
{
    close();
}

void MainWindow::resizeEvent(QResizeEvent * event){
    QSize tailleActuelle = event->size();
    w = tailleActuelle.width();
    h = tailleActuelle.height();
    switch (nb_label) {
        case 0:
            break;
        case 1:
            label->resize(w,h);
            label->show();
            break;
        case 2:
            label->resize(w/2,h);
            labeltmp->resize(w/2,h);
            label->show();
            labeltmp->show();
            break;
        default:
            exit(1);
            break;
    }
}

