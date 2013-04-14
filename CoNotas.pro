#-------------------------------------------------
#
# Project created by QtCreator 2013-03-15T19:37:37
#
#-------------------------------------------------

QT       += core gui sql xml printsupport widgets

TARGET = CoNotas
TEMPLATE = app


SOURCES += main.cpp\
				conotas.cpp \
		csv_lector.cpp \
		rut.cpp \
		db.cpp \
		qtrpt.cpp \
    dblocal.cpp \
    dbcolegio.cpp

HEADERS  += conotas.h \
		csv_lector.h \
		rut.h \
		db.h \
		qtrpt.h \
    dblocal.h \
    dbcolegio.h

FORMS    += conotas.ui

OTHER_FILES += \
		inf_parcial.xml

#Descomentar para compilar estaticamente
#CONFIG += static

#static {
#        QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -s -static
#        LFLAGS += -static-libgcc -static-libstdc++ -s -static
#        LDFLAGS += -static-libgcc -static-libstdc++
#        CONFIG += staticlib # Solo para las librerias
#        DEFINES += STATIC
#}
