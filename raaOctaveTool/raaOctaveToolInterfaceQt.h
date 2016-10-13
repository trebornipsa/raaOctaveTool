/********************************************************************************
** Form generated from reading UI file 'raaOctaveToolInterfaceQt.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef RAAOCTAVETOOLINTERFACEQT_H
#define RAAOCTAVETOOLINTERFACEQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <raaQTOSGWidget/raaQTOSGWidget.h>

QT_BEGIN_NAMESPACE

class Ui_raaOctaveToolInterfaceQt
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QToolBox *toolBox;
    QWidget *page;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *physical_translation_radio;
    QRadioButton *physical_rotation_radio;
    QFrame *frame;
    QGridLayout *gridLayout;
    QPushButton *physical_zup_button;
    QPushButton *physical_yup_button;
    QPushButton *physical_xdown_button;
    QPushButton *physical_xup_button;
    QPushButton *physical_zdown_button;
    QPushButton *physical_ydown_button;
    QCheckBox *phy_lock_camera_check;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *virtual_translation_radio;
    QRadioButton *virtual_rotation_radio;
    QFrame *frame_2;
    QGridLayout *gridLayout_2;
    QPushButton *virtual_zup_button;
    QPushButton *virtual_yup_button;
    QPushButton *virtual_xdown_button;
    QPushButton *virtual_xup_button;
    QPushButton *virtual_zdown_button;
    QPushButton *virtual_ydown_button;
    QSpacerItem *verticalSpacer;
    QWidget *page_2;
    raaQTOSGWidget *gl_widget;

    void setupUi(QMainWindow *raaOctaveToolInterfaceQt)
    {
        if (raaOctaveToolInterfaceQt->objectName().isEmpty())
            raaOctaveToolInterfaceQt->setObjectName(QStringLiteral("raaOctaveToolInterfaceQt"));
        raaOctaveToolInterfaceQt->resize(1473, 1121);
        centralwidget = new QWidget(raaOctaveToolInterfaceQt);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        toolBox = new QToolBox(splitter);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(toolBox->sizePolicy().hasHeightForWidth());
        toolBox->setSizePolicy(sizePolicy);
        toolBox->setMinimumSize(QSize(0, 0));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 436, 1023));
        verticalLayout = new QVBoxLayout(page);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(page);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        physical_translation_radio = new QRadioButton(groupBox);
        physical_translation_radio->setObjectName(QStringLiteral("physical_translation_radio"));
        physical_translation_radio->setCheckable(true);
        physical_translation_radio->setChecked(true);

        verticalLayout_3->addWidget(physical_translation_radio);

        physical_rotation_radio = new QRadioButton(groupBox);
        physical_rotation_radio->setObjectName(QStringLiteral("physical_rotation_radio"));

        verticalLayout_3->addWidget(physical_rotation_radio);

        frame = new QFrame(groupBox);
        frame->setObjectName(QStringLiteral("frame"));
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        physical_zup_button = new QPushButton(frame);
        physical_zup_button->setObjectName(QStringLiteral("physical_zup_button"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(physical_zup_button->sizePolicy().hasHeightForWidth());
        physical_zup_button->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(physical_zup_button, 0, 0, 1, 1);

        physical_yup_button = new QPushButton(frame);
        physical_yup_button->setObjectName(QStringLiteral("physical_yup_button"));
        sizePolicy2.setHeightForWidth(physical_yup_button->sizePolicy().hasHeightForWidth());
        physical_yup_button->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(physical_yup_button, 0, 1, 1, 1);

        physical_xdown_button = new QPushButton(frame);
        physical_xdown_button->setObjectName(QStringLiteral("physical_xdown_button"));
        sizePolicy2.setHeightForWidth(physical_xdown_button->sizePolicy().hasHeightForWidth());
        physical_xdown_button->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(physical_xdown_button, 1, 0, 1, 1);

        physical_xup_button = new QPushButton(frame);
        physical_xup_button->setObjectName(QStringLiteral("physical_xup_button"));
        sizePolicy2.setHeightForWidth(physical_xup_button->sizePolicy().hasHeightForWidth());
        physical_xup_button->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(physical_xup_button, 1, 2, 1, 1);

        physical_zdown_button = new QPushButton(frame);
        physical_zdown_button->setObjectName(QStringLiteral("physical_zdown_button"));
        sizePolicy2.setHeightForWidth(physical_zdown_button->sizePolicy().hasHeightForWidth());
        physical_zdown_button->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(physical_zdown_button, 2, 0, 1, 1);

        physical_ydown_button = new QPushButton(frame);
        physical_ydown_button->setObjectName(QStringLiteral("physical_ydown_button"));
        sizePolicy2.setHeightForWidth(physical_ydown_button->sizePolicy().hasHeightForWidth());
        physical_ydown_button->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(physical_ydown_button, 2, 1, 1, 1);


        verticalLayout_3->addWidget(frame);

        phy_lock_camera_check = new QCheckBox(groupBox);
        phy_lock_camera_check->setObjectName(QStringLiteral("phy_lock_camera_check"));

        verticalLayout_3->addWidget(phy_lock_camera_check);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(page);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        virtual_translation_radio = new QRadioButton(groupBox_2);
        virtual_translation_radio->setObjectName(QStringLiteral("virtual_translation_radio"));
        virtual_translation_radio->setChecked(true);

        verticalLayout_2->addWidget(virtual_translation_radio);

        virtual_rotation_radio = new QRadioButton(groupBox_2);
        virtual_rotation_radio->setObjectName(QStringLiteral("virtual_rotation_radio"));

        verticalLayout_2->addWidget(virtual_rotation_radio);

        frame_2 = new QFrame(groupBox_2);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        sizePolicy1.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy1);
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        virtual_zup_button = new QPushButton(frame_2);
        virtual_zup_button->setObjectName(QStringLiteral("virtual_zup_button"));
        sizePolicy2.setHeightForWidth(virtual_zup_button->sizePolicy().hasHeightForWidth());
        virtual_zup_button->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(virtual_zup_button, 0, 0, 1, 1);

        virtual_yup_button = new QPushButton(frame_2);
        virtual_yup_button->setObjectName(QStringLiteral("virtual_yup_button"));
        sizePolicy2.setHeightForWidth(virtual_yup_button->sizePolicy().hasHeightForWidth());
        virtual_yup_button->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(virtual_yup_button, 0, 1, 1, 1);

        virtual_xdown_button = new QPushButton(frame_2);
        virtual_xdown_button->setObjectName(QStringLiteral("virtual_xdown_button"));
        sizePolicy2.setHeightForWidth(virtual_xdown_button->sizePolicy().hasHeightForWidth());
        virtual_xdown_button->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(virtual_xdown_button, 1, 0, 1, 1);

        virtual_xup_button = new QPushButton(frame_2);
        virtual_xup_button->setObjectName(QStringLiteral("virtual_xup_button"));
        sizePolicy2.setHeightForWidth(virtual_xup_button->sizePolicy().hasHeightForWidth());
        virtual_xup_button->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(virtual_xup_button, 1, 2, 1, 1);

        virtual_zdown_button = new QPushButton(frame_2);
        virtual_zdown_button->setObjectName(QStringLiteral("virtual_zdown_button"));
        sizePolicy2.setHeightForWidth(virtual_zdown_button->sizePolicy().hasHeightForWidth());
        virtual_zdown_button->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(virtual_zdown_button, 2, 0, 1, 1);

        virtual_ydown_button = new QPushButton(frame_2);
        virtual_ydown_button->setObjectName(QStringLiteral("virtual_ydown_button"));
        sizePolicy2.setHeightForWidth(virtual_ydown_button->sizePolicy().hasHeightForWidth());
        virtual_ydown_button->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(virtual_ydown_button, 2, 1, 1, 1);


        verticalLayout_2->addWidget(frame_2);


        verticalLayout->addWidget(groupBox_2);

        verticalSpacer = new QSpacerItem(20, 323, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        toolBox->addItem(page, QStringLiteral("ViewPoint"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 436, 1023));
        toolBox->addItem(page_2, QStringLiteral("Page 2"));
        splitter->addWidget(toolBox);
        gl_widget = new raaQTOSGWidget(splitter);
        gl_widget->setObjectName(QStringLiteral("gl_widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(2);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(gl_widget->sizePolicy().hasHeightForWidth());
        gl_widget->setSizePolicy(sizePolicy3);
        splitter->addWidget(gl_widget);

        horizontalLayout->addWidget(splitter);

        raaOctaveToolInterfaceQt->setCentralWidget(centralwidget);

        retranslateUi(raaOctaveToolInterfaceQt);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(raaOctaveToolInterfaceQt);
    } // setupUi

    void retranslateUi(QMainWindow *raaOctaveToolInterfaceQt)
    {
        raaOctaveToolInterfaceQt->setWindowTitle(QApplication::translate("raaOctaveToolInterfaceQt", "MainWindow", 0));
        groupBox->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Physical", 0));
        physical_translation_radio->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Translation", 0));
        physical_rotation_radio->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Rotation", 0));
        physical_zup_button->setText(QString());
        physical_yup_button->setText(QString());
        physical_xdown_button->setText(QString());
        physical_xup_button->setText(QString());
        physical_zdown_button->setText(QString());
        physical_ydown_button->setText(QString());
        phy_lock_camera_check->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Lock Camera View", 0));
        groupBox_2->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Virtual", 0));
        virtual_translation_radio->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Translation", 0));
        virtual_rotation_radio->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Rotation", 0));
        virtual_zup_button->setText(QString());
        virtual_yup_button->setText(QString());
        virtual_xdown_button->setText(QString());
        virtual_xup_button->setText(QString());
        virtual_zdown_button->setText(QString());
        virtual_ydown_button->setText(QString());
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("raaOctaveToolInterfaceQt", "ViewPoint", 0));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("raaOctaveToolInterfaceQt", "Page 2", 0));
    } // retranslateUi

};

namespace Ui {
    class raaOctaveToolInterfaceQt: public Ui_raaOctaveToolInterfaceQt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // RAAOCTAVETOOLINTERFACEQT_H
