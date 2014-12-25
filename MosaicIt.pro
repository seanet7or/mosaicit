
TARGET = MosaicIt
TEMPLATE = app

QT += core gui widgets sql

OTHER_FILES += doc/header.txt \
    lupdate_project.cmd \
    mosaicit_de.ts \
    lrelease_project.cmd \
    appiconwin.ico \
    appicon.rc \
    ico_src/256_32.png \
    gfx_work/mosaicit_logo2.png \
    gfx_work/logo.png \
    gfx_work/mosaicit_logo3.png \
    qt.conf \
    doc/end-user/docengl.qhcp \
    doc/end-user/doc_de.qhp \
    doc/end-user/doc_de.qhcp \
    doc/end-user/compile.cmd \
    doc/end-user/about_engl.txt \
    doc/end-user/docengl.qhp \
    doc/end-user/html/template.html \
    doc/end-user/html/engl/overview.html \
    doc/end-user/html/engl/chapter6.html \
    doc/end-user/html/engl/chapter5.html \
    doc/end-user/html/engl/chapter4.html \
    doc/end-user/html/engl/chapter3.html \
    doc/end-user/html/engl/chapter2.html \
    doc/end-user/html/engl/chapter1.html \
    doc/end-user/html/engl/style.css \
    doc/end-user/html/engl/pics/uninstall.png \
    doc/end-user/html/engl/pics/startmenu.png \
    doc/end-user/html/engl/pics/rendermoscompl.png \
    doc/end-user/html/engl/pics/rendermos.png \
    doc/end-user/html/engl/pics/mosaicit_logo.jpg \
    doc/end-user/html/engl/pics/mosaicdetails.png \
    doc/end-user/html/engl/pics/mainwin.png \
    doc/end-user/html/engl/pics/editdb.png \
    doc/end-user/html/engl/pics/dbincompl.png \
    doc/end-user/html/engl/pics/createmosaic.png \
    doc/end-user/html/engl/pics/builddbprog.png \
    doc/end-user/html/engl/pics/builddbcompl.png \
    doc/end-user/html/engl/pics/builddb.png \
    doc/end-user/html/engl/pics/browseresults.png \
    doc/end-user/html/ger/style.css \
    doc/end-user/html/ger/overview.html \
    doc/end-user/html/ger/chapter6.html \
    doc/end-user/html/ger/chapter5.html \
    doc/end-user/html/ger/chapter4.html \
    doc/end-user/html/ger/chapter3.html \
    doc/end-user/html/ger/chapter2.html \
    doc/end-user/html/ger/chapter1.html \
    doc/end-user/html/ger/pics/uninstall.png \
    doc/end-user/html/ger/pics/startmenu.png \
    doc/end-user/html/ger/pics/rendermoscompl.png \
    doc/end-user/html/ger/pics/rendermos.png \
    doc/end-user/html/ger/pics/mosaicit_logo.jpg \
    doc/end-user/html/ger/pics/mosaicdetails.png \
    doc/end-user/html/ger/pics/mainwin.png \
    doc/end-user/html/ger/pics/editdb.png \
    doc/end-user/html/ger/pics/dbincompl.png \
    doc/end-user/html/ger/pics/createmosaic.png \
    doc/end-user/html/ger/pics/builddbprog.png \
    doc/end-user/html/ger/pics/builddbcompl.png \
    doc/end-user/html/ger/pics/builddb.png \
    doc/end-user/html/ger/pics/browseresults.png
HEADERS += \
    rendermosaicthread.h \
    rendermosaicdlg.h \
    processimagesthread.h \
    pictureinfo.h \
    picturedatabase.h \
    newdatabasedlg.h \
    mosaicdetailsdlg.h \
    mainwindow.h \
    editdatabasedlg.h \
    debug.h \
    indexdirdlg.h \
    aboutdlg.h \
    appsettings.h \
    window.h \
    settings.h \
    database_workers/databaseworkmanager.h \
    database_workers/indexfilesthread.h \
    database_workers/pictureanalyzerthread.h
SOURCES += main.cpp \
    editdatabasedlg.cpp \
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
    appsettings.cpp \
    debug.cpp \
    window.cpp \
    settings.cpp \
    database_workers/databaseworkmanager.cpp \
    database_workers/indexfilesthread.cpp \
    database_workers/pictureanalyzerthread.cpp
FORMS += updatedatabasedlg.ui \
    rendermosaicdlg.ui \
    newdatabasedlg.ui \
    mosaicdetailsdlg.ui \
    mainwindow.ui \
    editdatabasedlg.ui \
    indexdirdlg.ui \
    aboutdlg.ui
TRANSLATIONS += mosaicit_de.ts
RC_FILE += appicon.rc
RESOURCES += \ 
    bundled_resources/resources.qrc
