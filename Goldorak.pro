#-------------------------------------------------
#
# Project created by QtCreator 2016-09-19T15:43:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Goldorak
TEMPLATE = app

#LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui
LIBS += -L~/projet_Goldorak/opencv2.4.13 -lopencv_core -lopencv_highgui #-lopencv_nonfree
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}
SOURCES += main.cpp\
        mainwindow.cpp \
    widget.cpp

HEADERS  += mainwindow.h \
    widget.h

#Besoin fonctionnels
#   suivre une personne
#       reconnaitre "un humain" -> initialisation
#       bouger
#       calculer une distance
#Besoin non-fonctionnels(quantifier)
#   robot doit etre suffisament rapide pour suivre une personne(moteurs)(6 à 8 km/h)
#   temps de calcul suffisament rapide pour //  //  //  //  //
#   robuste: au changement de luminosité + ombre
#            au changement de personne
#            au changement d'environement
#simulation
#   vidéo pré-enregistré
#   simulation env 3D -> images de synthèse
#   unity || povray
