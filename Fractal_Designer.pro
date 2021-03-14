QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Complex.cpp \
    build_thread.cpp \
    create_image_info.cpp \
    create_image_task.cpp \
    create_images_range.cpp \
    frd_4_help.cpp \
    help.cpp \
    info_save.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    new_features.cpp \
    new_file.cpp \
    open_file.cpp \
    route_info.cpp \
    route_tool.cpp \
    search_result.cpp \
    set_colour.cpp \
    version.cpp

HEADERS += \
    Complex.h \
    build_thread.h \
    create_image_info.h \
    create_image_task.h \
    create_images_range.h \
    frd_4_help.h \
    help.h \
    info_save.h \
    login.h \
    mainwindow.h \
    new_features.h \
    new_file.h \
    open_file.h \
    route_info.h \
    route_tool.h \
    search_result.h \
    set_colour.h \
    version.h

FORMS += \
    create_image_info.ui \
    create_images_range.ui \
    frd_4_help.ui \
    help.ui \
    login.ui \
    mainwindow.ui \
    new_features.ui \
    new_file.ui \
    open_file.ui \
    route_tool.ui \
    search_result.ui \
    set_colour.ui \
    version.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    EXE_Icons.qrc \
    Icon.qrc \
    Icon_resources.qrc \
    Other_Images.qrc \
    StyleSheet.qrc

DISTFILES += \
    C:/Users/Teddy van Jerry/Desktop/1.jpg \
    EXE Icons/FRD_icon.png \
    EXE Icons/FRD_icon.png \
    Menu Icon/Preview.png \
    Menu Icon/Property_16x.svg \
    Menu Icon/Refresh.svg \
    Templates/Template_1.bmp \
    icon.rc

RC_ICONS += \
    icon.rc

RC_FILE += \
    icon.rc

VERSION = 5.0

QMAKE_TARGET_PRODUCT = Fractal Designer
QMAKE_TARGET_COMPANY = TVJ
QMAKE_TARGET_DESCRIPTION = This is a software to make fractals built by Qt.
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2021 Teddy van Jerry
