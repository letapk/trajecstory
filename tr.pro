#/*
# * tr.pro
# *
# * Copyright (c) Kartik Patel
# * E-mail: letapk@gmail.com
# * Download from: https://letapk.wordpress.com
# *
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation; either version 2 of the License, or
# * (at your option) any later version.
# *
# */

#//Last modified 25 June 2017

QT           += opengl

HEADERS       = tr.h \
               tr_externs.h \
               tr_vars.h

SOURCES       = tr_gl.cpp \
                tr_main.cpp \
                tr_window.cpp \
                tr_traj.cpp

RESOURCES += tr.qrc

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lgsl -lgslcblas -lGLU

INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

DISTFILES += \
    COPYING \
    AUTHORS \
    ISSUES \
    README

