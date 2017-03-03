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
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/contrib/contrib.hpp"
#include <opencv2/opencv.hpp>

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
    carteProfondeurAction=new QAction(tr("&Carte de Disparité"),this);
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
    qDebug("%d",imageD.width());
    cv::Mat matG(imageD.height(),imageD.width() ,CV_8UC4,(uchar*)imageG.bits(),imageG.bytesPerLine());
    cv::Mat matD(imageD.height(),imageD.width(),CV_8UC4,(uchar*)imageD.bits(),imageD.bytesPerLine());
    MainWindow::extractionFeatures(matG,matD);
    //MainWindow::blockMatching(matG,matD);
}

void MainWindow::extractionFeatures( cv::Mat imgD, cv::Mat imgG){
    cv::OrbFeatureDetector detector(1500);
    cv::vector<cv::KeyPoint> keypoints1, keypoints2;
    detector.detect(imgD, keypoints1);
    detector.detect(imgG, keypoints2);

    // computing descriptors
    cv::OrbDescriptorExtractor extractor;
    cv::Mat descriptors1, descriptors2;
    extractor.compute(imgD, keypoints1, descriptors1);
    extractor.compute(imgG, keypoints2, descriptors2);

    // matching descriptors
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    cv::vector<cv::DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    // drawing the results
    cv::namedWindow("matches", CV_WINDOW_AUTOSIZE);
    cv::Mat img_matches;
    cv::drawMatches(imgD, keypoints1, imgG, keypoints2, matches, img_matches);
    cv::drawKeypoints();
    cv::imshow("matches", img_matches);
<<<<<<< HEAD

=======
>>>>>>> ff42b75457b438413b3c2ed9cb608ed9236ff03e
    //code par astronaut 2016-07-13T01:00:29-07:00 voir https://www.imagemagick.org/discourse-server/viewtopic.php?t=30064
}

void MainWindow::blockMatching(cv::Mat img1,cv::Mat img2){
    //ajouter référence
    cv::Mat g1,g2,disp,dispo,disp8,disp8o;
    cvtColor(img1, g1, CV_BGR2GRAY);
    cvtColor(img2, g2, CV_BGR2GRAY);
    cv::StereoBM sbm;
    sbm.state->SADWindowSize = 9;
    sbm.state->numberOfDisparities = 160;
    sbm.state->preFilterSize = 5;
    sbm.state->preFilterCap = 61;
    sbm.state->minDisparity = -39;
    sbm.state->textureThreshold = 507;
    sbm.state->uniquenessRatio = 0;
    sbm.state->speckleWindowSize = 0;
    sbm.state->speckleRange = 8;
    sbm.state->disp12MaxDiff = 1;
<<<<<<< HEAD
    /*
    sbm.state->SADWindowSize = 9;
    sbm.state->numberOfDisparities = 112;
    sbm.state->preFilterSize = 5;
    sbm.state->preFilterCap = 1;
    sbm.state->minDisparity = 0;
    sbm.state->textureThreshold = 5;
    sbm.state->uniquenessRatio = 5;
    sbm.state->speckleWindowSize = 0;
    sbm.state->speckleRange = 20;
    sbm.state->disp12MaxDiff = 64;*/
=======
    sbm(g1, g2, dispo);
    normalize(dispo, disp8o, 0, 255, CV_MINMAX, CV_8U);
    imshow("dispo", disp8o);
    cv::StereoSGBM sgbm;
    sgbm.SADWindowSize = 5;
    sgbm.numberOfDisparities = 192;
    sgbm.preFilterCap = 4;
    sgbm.minDisparity = -64;
    sgbm.uniquenessRatio = 1;
    sgbm.speckleWindowSize = 150;
    sgbm.speckleRange = 2;
    sgbm.disp12MaxDiff = 10;
    sgbm.fullDP = false;
    sgbm.P1 = 600;
    sgbm.P2 = 2400;
    sgbm(g1, g2, disp);
    normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
    imshow("disp", disp8);

    /*cv::StereoBM sbm;
    sbm.state->SADWindowSize = 9;
    sbm.state->numberOfDisparities = 112;
    sbm.state->preFilterSize = 5;
    sbm.state->preFilterCap = 61;
    sbm.state->minDisparity = -39;
    sbm.state->textureThreshold = 507;
    sbm.state->uniquenessRatio = 0;
    sbm.state->speckleWindowSize = 0;
    sbm.state->speckleRange = 8;
    sbm.state->disp12MaxDiff = 1;
>>>>>>> ff42b75457b438413b3c2ed9cb608ed9236ff03e
    sbm(g1, g2, disp);
    normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
    imshow("disp", disp8);
    /*cv::StereoSGBM sgbm;
    sgbm.preFilterCap = 61;//25
    sgbm.SADWindowSize = 7;//14
    sgbm.numberOfDisparities = 64; //64
    sgbm.minDisparity = 15;
    sgbm.uniquenessRatio = 10;
    sgbm.speckleWindowSize = 100;
    sgbm.speckleRange = 2;
    sgbm.disp12MaxDiff = 1;//10
    sgbm.fullDP = false;
    //sgbm.P1 = 600;
    //sgbm.P2 = 600;
    sgbm(g1, g2, disp);
    normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
    imshow("disp 2", disp8);*/
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
    qDebug("%d",imageD.width());
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

