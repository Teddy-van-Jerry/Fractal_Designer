QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

VERSION = 6.0.4.1

TARGET = Fractal_Designer

DESTDIR = ../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FRD_Editor.cpp \
    FRD_Json.cpp \
    FRD_TitleBar.cpp \
    Info.cpp \
    Interpreter.cpp \
    String_Evaluate.cpp \
    create_image_info.cpp \
    create_image_task.cpp \
    create_images_range.cpp \
    info_save.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    new_features.cpp \
    new_file.cpp \
    preview_setting.cpp \
    route_info.cpp \
    route_tool.cpp \
    search_result.cpp \
    template_2_settings.cpp \
    template_4_settings.cpp \
    version.cpp

HEADERS += \
    FRD_Editor.h \
    FRD_Json.h \
    FRD_TitleBar.h \
    Info.h \
    Interpreter.h \
    String_Evaluate.h \
    create_image_info.h \
    create_image_task.h \
    create_images_range.h \
    info_save.h \
    login.h \
    mainwindow.h \
    mainwindow_global.h \
    new_features.h \
    new_file.h \
    preview_setting.h \
    route_info.h \
    route_tool.h \
    search_result.h \
    template_2_settings.h \
    template_4_settings.h \
    version.h

FORMS += \
    create_image_info.ui \
    create_images_range.ui \
    login.ui \
    mainwindow.ui \
    new_features.ui \
    new_file.ui \
    preview_setting.ui \
    route_tool.ui \
    search_result.ui \
    template_2_settings.ui \
    template_4_settings.ui \
    version.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    EXE_Icons.qrc \
    Editor_Keywords.qrc \
    Icon.qrc \
    Icon_resources.qrc \
    Languages.qrc \
    StyleSheet.qrc

DISTFILES += \
    EXE Icons/FRD_icon.png \
    EXE Icons/FRD_icon.png \
    Menu Icon/Preview.png \
    Menu Icon/Property_16x.svg \
    Menu Icon/Refresh.svg

TRANSLATIONS += \
    Languages/FRD_zh_CN.ts \
    Languages/FRD_en_UK.ts

# Software Information
win32 {
    QMAKE_TARGET_PRODUCT     = Fractal Designer
    QMAKE_TARGET_COMPANY     = Teddy van Jerry
    QMAKE_TARGET_DESCRIPTION = Fractal Designer
    QMAKE_TARGET_COPYRIGHT   = "Copyright (C) 2021 Teddy van Jerry"

    RC_ICONS += \
        "EXE Icons/FRD.ico" \
        "EXE Icons/FRD_File.ico"
}

# QScintilla lib (specially compiled for FRD language)
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/ -lqscintilla2_qt6
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/ -lqscintilla2_qt6d
else:unix: LIBS += -L$$PWD/../lib/ -lqscintilla2_qt6

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
