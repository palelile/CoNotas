#-------------------------------------------------
#
# Project created by QtCreator 2013-03-15T19:37:37
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CoNotas
TEMPLATE = app


SOURCES += main.cpp\
				conotas.cpp \
		csv_lector.cpp \
		rut.cpp \
		db.cpp \
		qtrpt.cpp

HEADERS  += conotas.h \
		csv_lector.h \
		rut.h \
		db.h \
		qtrpt.h

FORMS    += conotas.ui

OTHER_FILES += \
		inf_parcial.xml

CONFIG += static

QMAKE_LFLAGS += -static-libgcc -static-libstdc++

LFLAGS += -static-libgcc -static-libstdc++ -s -static
LDFLAGS += -static-libgcc -static-libstdc++

static { # everything below takes effect with CONFIG += static

		CONFIG += static
		CONFIG += staticlib # this is needed if you create a static library, not a static executable
		DEFINES += STATIC
		message("~~~ static build ~~~") # this is for information, that the static build is done
		win32: TARGET = $$join(TARGET,,,s) #this adds an s in the end, so you can seperate static build from non static build
}
