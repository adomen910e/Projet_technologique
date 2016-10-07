#include "mainwindow.h"
#include <QtGui>
#include <QFileDialog>
#include <iostream>
#include <QGraphicsRectItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    labeltmp = new QLabel(this);
    label = new QLabel(this);
    label->move(0,30);
    labelG = new QLabel(this);
    labelD = new QLabel(this);
    aProposAction=new QAction(tr("&A Propos"),this);
    ouvrir=new QAction(tr("&Ouvrir"),this);
    quitter=new QAction(tr("&Quitter"),this);
    separerImage=new QAction(tr("&Separer l'image en 2"),this);
    flouterImage=new QAction(tr("&Flouter l'image"),this);
    sobel=new QAction(tr("&Effet Sobel"),this);
    aPropos = menuBar()->addMenu(tr("&A Propos"));
    fichier = menuBar()->addMenu(tr("&Fichier"));
    aPropos->addAction(aProposAction);
    fichier->addAction(ouvrir);
    fichier->addAction(quitter);
    QObject::connect(aProposAction,SIGNAL(triggered()),this,SLOT(afficherMessage()));
    QObject::connect(ouvrir,SIGNAL(triggered()),this,SLOT(openFile()));
    QObject::connect(quitter,SIGNAL(triggered()),this,SLOT(quit()));
}

MainWindow::~MainWindow()
{
    aPropos=NULL;
    fichier=NULL;
    label=NULL;
    labelG=NULL;
    labelD=NULL;
    aProposAction=NULL;
    ouvrir=NULL;
    quitter=NULL;
    xrec = 0;
    yrec = 0;
}

void MainWindow::afficherMessage()
{
    QMessageBox msgBox;
    msgBox.setText("Application TP1");
    msgBox.exec();
}

void MainWindow::openFile()
{
    labelG->clear();
    labelD->clear();
    QString fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir une image"),"/",tr("Image Files (*.png *.jpg *.bmp)"));
    QImageReader *reader = new QImageReader();
    reader->setFileName(fileName);
    image =reader->read();
    map=QPixmap::fromImage(image);
    label->setPixmap(map);
    fichier->addAction(separerImage);
    QObject::connect(separerImage,SIGNAL(triggered()),this,SLOT(separation()));
    traitement = menuBar()->addMenu(tr("&Traitement de l'image"));
    traitement->addAction(flouterImage);
    traitement->addAction(sobel);
    QObject::connect(flouterImage,SIGNAL(triggered()),this,SLOT(floutage()));
    QObject::connect(sobel,SIGNAL(triggered()),this,SLOT(sobelSlot()));
    label->resize(image.size());
    this->resize(image.width(),image.height()+30);
    label->show();
}

void MainWindow::afficherMat(cv::Mat mat){
    imagetmp = QImage(mat.data, mat.cols, mat.rows, mat.step, image.format());
    maptmp = QPixmap::fromImage(imagetmp);
    labeltmp->resize(image.size());
    labeltmp->move(10+label->width(),30);
    labeltmp->show();
    labeltmp->setPixmap(maptmp);
    this->resize(2*label->width()+10,label->height()+30);
}

void MainWindow::sobelSlot(){
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
    cv::imshow("image sobel",imageCV);
    afficherMat(imageCV);

}

void MainWindow::floutage(){
    cv::Mat tmp(image.height(),image.width(),CV_8UC4,(uchar*)image.bits(),image.bytesPerLine());
    imageCV = tmp;
    cv::blur(imageCV,imageCV,cv::Size(5,5));
    afficherMat(imageCV);
}

void MainWindow::separation(){
    label->clear();
    this->resize(image.width(),image.height()+30);
    QImage image1(w/2,h,QImage::Format_RGB32);
    QImage image2(w/2,h,QImage::Format_RGB32);
    for(int y = 0;y<h;y++){
        for(int x = 0;x<w/2;x++){
            image1.setPixel(x,y,image.pixel(x,y));
            image2.setPixel(x,y,image.pixel(x+w/2,y));
        }
    }
    imageG = image1;
    imageD = image2;
    map=QPixmap::fromImage(imageG);
    labelG->move(0,30);
    labelG->setPixmap(map);
    labelG->show();
    map=QPixmap::fromImage(imageD);
    labelD->move(w/2+10,30);
    labelD->setPixmap(map);
    labelD->show();
    this->resize(image.width()+10,image.height());
}

void MainWindow::quit()
{
    close();
}
void MainWindow::mousePressEvent(QMouseEvent * event){
    xrec = event->x();
    yrec = event->y();
}
void MainWindow::mouseReleaseEvent(QMouseEvent * event){
    int xmin = 0;
    int xmax = 0;
    int ymin = 0;
    int ymax = 0;
    if(xrec<event->x()){
       xmin=xrec;
       xmax=event->x();
    }
    else {
        xmin=event->x();
        xmax=xrec;
    }
    if(yrec<event->y()){
       ymin=yrec;
       ymax=event->y();
    }
    else {
        ymin=event->y();
        ymax=yrec;
    }
    QImage imagerect(xmax-xmin,ymax-ymin,QImage::Format_RGB32);
    for(int y = ymin;y<ymax;y++){
        for(int x = xmin;x<xmax;x++){
            imagerect.setPixel(x-xmin,y-ymin,image.pixel(x,y));
        }
    }
    maprect=QPixmap::fromImage(imagerect);
    label->setPixmap(maprect);
    label->show();
}

void MainWindow::resizeEvent(QResizeEvent * event){
    QSize tailleActuelle = event->size();
    w = tailleActuelle.width();
    h = tailleActuelle.height();
    //label->setPixmap(map.scaled(w,h,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    label->resize(w,h);
    labeltmp->resize(w,h);
    labelG->resize(w/2,h);
    labelD->resize(w/2,h);
}
