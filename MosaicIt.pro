OTHER_FILES += doc/header.txt \
    lupdate_project.cmd \
    mosaicit_de.ts \
    lrelease_project.cmd \
    appiconwin.ico \
    appicon.rc \
    ico_src/256_32.png
HEADERS += updatedatabasedlg.h \
    rendermosaicthread.h \
    rendermosaicdlg.h \
    processimagesthread.h \
    pictureinfo.h \
    picturedatabase.h \
    newdatabasedlg.h \
    mosaicdetailsdlg.h \
    mainwindow.h \
    indexfilesthread.h \
    editdatabasedlg.h \
    debug.h \
    createmosaicdlg.h \
    builddatabasedlg.h \
    indexdirdlg.h \
    aboutdlg.h \
    appsettings.h
SOURCES += main.cpp \
    indexfilesthread.cpp \
    editdatabasedlg.cpp \
    createmosaicdlg.cpp \
    builddatabasedlg.cpp \
    updatedatabasedlg.cpp \
    rendermosaicthread.cpp \
    rendermosaicdlg.cpp \
    processimagesthread.cpp \
    pictureinfo.cpp \
    picturedatabase.cpp \
    newdatabasedlg.cpp \
    mosaicdetailsdlg.cpp \
    mainwindow.cpp \
    indexdirdlg.cpp \
    aboutdlg.cpp \
    appsettings.cpp
FORMS += updatedatabasedlg.ui \
    rendermosaicdlg.ui \
    newdatabasedlg.ui \
    mosaicdetailsdlg.ui \
    mainwindow.ui \
    editdatabasedlg.ui \
    createmosaicdlg.ui \
    builddatabasedlg.ui \
    indexdirdlg.ui \
    aboutdlg.ui
TRANSLATIONS += mosaicit_de.ts
RC_FILE += appicon.rc
RESOURCES += 
