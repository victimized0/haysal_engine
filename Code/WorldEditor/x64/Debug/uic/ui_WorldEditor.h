/********************************************************************************
** Form generated from reading UI file 'WorldEditor.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORLDEDITOR_H
#define UI_WORLDEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WorldEditorClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WorldEditorClass)
    {
        if (WorldEditorClass->objectName().isEmpty())
            WorldEditorClass->setObjectName(QString::fromUtf8("WorldEditorClass"));
        WorldEditorClass->resize(600, 400);
        menuBar = new QMenuBar(WorldEditorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        WorldEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(WorldEditorClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        WorldEditorClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(WorldEditorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        WorldEditorClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(WorldEditorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        WorldEditorClass->setStatusBar(statusBar);

        retranslateUi(WorldEditorClass);

        QMetaObject::connectSlotsByName(WorldEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *WorldEditorClass)
    {
        WorldEditorClass->setWindowTitle(QCoreApplication::translate("WorldEditorClass", "WorldEditor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WorldEditorClass: public Ui_WorldEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORLDEDITOR_H
