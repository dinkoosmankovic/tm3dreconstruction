/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Dec 18 22:38:52 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QRadioButton *radioHSV;
    QRadioButton *radioJET;
    QRadioButton *radioGRAYSCALE;
    QPushButton *btn_RefreshColormap;
    QRadioButton *radioAUTUMN;
    QPushButton *btnUP;
    QPushButton *btnRIGHT;
    QPushButton *btnDOWN;
    QPushButton *btnLEFT;
    QPushButton *pushButton;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(671, 518);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 491, 451));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(520, 110, 141, 211));
        groupBox->setStyleSheet(QString::fromUtf8(""));
        radioHSV = new QRadioButton(groupBox);
        radioHSV->setObjectName(QString::fromUtf8("radioHSV"));
        radioHSV->setGeometry(QRect(20, 30, 91, 22));
        radioHSV->setChecked(true);
        radioJET = new QRadioButton(groupBox);
        radioJET->setObjectName(QString::fromUtf8("radioJET"));
        radioJET->setGeometry(QRect(20, 60, 91, 22));
        radioGRAYSCALE = new QRadioButton(groupBox);
        radioGRAYSCALE->setObjectName(QString::fromUtf8("radioGRAYSCALE"));
        radioGRAYSCALE->setGeometry(QRect(20, 90, 101, 22));
        btn_RefreshColormap = new QPushButton(groupBox);
        btn_RefreshColormap->setObjectName(QString::fromUtf8("btn_RefreshColormap"));
        btn_RefreshColormap->setGeometry(QRect(10, 160, 121, 41));
        radioAUTUMN = new QRadioButton(groupBox);
        radioAUTUMN->setObjectName(QString::fromUtf8("radioAUTUMN"));
        radioAUTUMN->setGeometry(QRect(20, 120, 101, 22));
        btnUP = new QPushButton(centralWidget);
        btnUP->setObjectName(QString::fromUtf8("btnUP"));
        btnUP->setGeometry(QRect(570, 350, 31, 27));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        btnUP->setFont(font);
        btnRIGHT = new QPushButton(centralWidget);
        btnRIGHT->setObjectName(QString::fromUtf8("btnRIGHT"));
        btnRIGHT->setGeometry(QRect(610, 390, 31, 27));
        btnRIGHT->setFont(font);
        btnDOWN = new QPushButton(centralWidget);
        btnDOWN->setObjectName(QString::fromUtf8("btnDOWN"));
        btnDOWN->setGeometry(QRect(570, 430, 31, 27));
        btnDOWN->setFont(font);
        btnLEFT = new QPushButton(centralWidget);
        btnLEFT->setObjectName(QString::fromUtf8("btnLEFT"));
        btnLEFT->setGeometry(QRect(530, 390, 31, 27));
        btnLEFT->setFont(font);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(530, 10, 121, 41));
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ThermalMapperInspector", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Select Colormap", 0, QApplication::UnicodeUTF8));
        radioHSV->setText(QApplication::translate("MainWindow", "HSV", 0, QApplication::UnicodeUTF8));
        radioJET->setText(QApplication::translate("MainWindow", "JET", 0, QApplication::UnicodeUTF8));
        radioGRAYSCALE->setText(QApplication::translate("MainWindow", "Grayscale", 0, QApplication::UnicodeUTF8));
        btn_RefreshColormap->setText(QApplication::translate("MainWindow", "Refresh\n"
"Colormap", 0, QApplication::UnicodeUTF8));
        radioAUTUMN->setText(QApplication::translate("MainWindow", "AUTUMN", 0, QApplication::UnicodeUTF8));
        btnUP->setText(QApplication::translate("MainWindow", "\342\206\221", 0, QApplication::UnicodeUTF8));
        btnRIGHT->setText(QApplication::translate("MainWindow", "\342\206\222", 0, QApplication::UnicodeUTF8));
        btnDOWN->setText(QApplication::translate("MainWindow", "\342\206\223", 0, QApplication::UnicodeUTF8));
        btnLEFT->setText(QApplication::translate("MainWindow", "\342\206\220", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Open File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
