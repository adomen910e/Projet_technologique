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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    label = new QLabel(this);

    label->move(0,30);

    labelG = new QLabel(this);
    labelD = new QLabel(this);

    aProposAction=new QAction(tr("&A Propos"),this);
    ouvrir=new QAction(tr("&Ouvrir"),this);
    quitter=new QAction(tr("&Quitter"),this);
    separerImage=new QAction(tr("&Separer l'image en 2"),this);
    aPropos = menuBar()->addMenu(tr("&A Propos"));
    fichier = menuBar()->addMenu(tr("&Fichier"));

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

    label->resize(image.size());
    this->resize(image.size());

    label->show();
}

void MainWindow::separation(){
    label->clear();
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

    labelD->move(w/2+15,30);
    labelD->setPixmap(map);
    labelD->show();

    this->resize(image.width()+10,image.height());
}

void MainWindow::quit()
{
    close();
}

void MainWindow::resizeEvent(QResizeEvent * event){

    QSize tailleActuelle = event->size();
    w = tailleActuelle.width();
    h = tailleActuelle.height();

    label->resize(w,h);
    labelG->resize(w/2,h);
    labelD->resize(w/2,h);

    label->show();
}

