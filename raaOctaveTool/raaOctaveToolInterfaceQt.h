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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
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
    QFormLayout *formLayout;
    QPushButton *screen_updtae_button;
    QComboBox *screen_combo;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_6;
    QDoubleSpinBox *screen_tl_x_spin;
    QDoubleSpinBox *screen_tl_y_spin;
    QDoubleSpinBox *screen_tl_z_spin;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_7;
    QDoubleSpinBox *screen_tr_x_spin;
    QDoubleSpinBox *screen_tr_y_spin;
    QDoubleSpinBox *screen_tr_z_spin;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_4;
    QDoubleSpinBox *screen_bl_x_spin;
    QDoubleSpinBox *screen_bl_y_spin;
    QDoubleSpinBox *screen_bl_z_spin;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_5;
    QDoubleSpinBox *screen_br_x_spin;
    QDoubleSpinBox *screen_br_y_spin;
    QDoubleSpinBox *screen_br_z_spin;
    QSpacerItem *verticalSpacer_2;
    QCheckBox *screen_continual_update_check;
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
        page->setGeometry(QRect(0, 0, 282, 1009));
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
        page_2->setGeometry(QRect(0, 0, 302, 1009));
        formLayout = new QFormLayout(page_2);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        screen_updtae_button = new QPushButton(page_2);
        screen_updtae_button->setObjectName(QStringLiteral("screen_updtae_button"));

        formLayout->setWidget(0, QFormLayout::SpanningRole, screen_updtae_button);

        screen_combo = new QComboBox(page_2);
        screen_combo->setObjectName(QStringLiteral("screen_combo"));

        formLayout->setWidget(2, QFormLayout::SpanningRole, screen_combo);

        groupBox_5 = new QGroupBox(page_2);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        verticalLayout_6 = new QVBoxLayout(groupBox_5);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        screen_tl_x_spin = new QDoubleSpinBox(groupBox_5);
        screen_tl_x_spin->setObjectName(QStringLiteral("screen_tl_x_spin"));
        screen_tl_x_spin->setAccelerated(true);
        screen_tl_x_spin->setMinimum(-100);
        screen_tl_x_spin->setMaximum(100);
        screen_tl_x_spin->setSingleStep(0.01);

        verticalLayout_6->addWidget(screen_tl_x_spin);

        screen_tl_y_spin = new QDoubleSpinBox(groupBox_5);
        screen_tl_y_spin->setObjectName(QStringLiteral("screen_tl_y_spin"));
        screen_tl_y_spin->setAccelerated(true);
        screen_tl_y_spin->setMinimum(-100);
        screen_tl_y_spin->setMaximum(100);
        screen_tl_y_spin->setSingleStep(0.01);

        verticalLayout_6->addWidget(screen_tl_y_spin);

        screen_tl_z_spin = new QDoubleSpinBox(groupBox_5);
        screen_tl_z_spin->setObjectName(QStringLiteral("screen_tl_z_spin"));
        screen_tl_z_spin->setAccelerated(true);
        screen_tl_z_spin->setMinimum(-100);
        screen_tl_z_spin->setMaximum(100);
        screen_tl_z_spin->setSingleStep(0.01);

        verticalLayout_6->addWidget(screen_tl_z_spin);


        formLayout->setWidget(4, QFormLayout::LabelRole, groupBox_5);

        groupBox_6 = new QGroupBox(page_2);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        verticalLayout_7 = new QVBoxLayout(groupBox_6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        screen_tr_x_spin = new QDoubleSpinBox(groupBox_6);
        screen_tr_x_spin->setObjectName(QStringLiteral("screen_tr_x_spin"));
        screen_tr_x_spin->setAccelerated(true);
        screen_tr_x_spin->setMinimum(-100);
        screen_tr_x_spin->setMaximum(100);
        screen_tr_x_spin->setSingleStep(0.01);

        verticalLayout_7->addWidget(screen_tr_x_spin);

        screen_tr_y_spin = new QDoubleSpinBox(groupBox_6);
        screen_tr_y_spin->setObjectName(QStringLiteral("screen_tr_y_spin"));
        screen_tr_y_spin->setAccelerated(true);
        screen_tr_y_spin->setMinimum(-100);
        screen_tr_y_spin->setMaximum(100);
        screen_tr_y_spin->setSingleStep(0.01);

        verticalLayout_7->addWidget(screen_tr_y_spin);

        screen_tr_z_spin = new QDoubleSpinBox(groupBox_6);
        screen_tr_z_spin->setObjectName(QStringLiteral("screen_tr_z_spin"));
        screen_tr_z_spin->setAccelerated(true);
        screen_tr_z_spin->setMinimum(-100);
        screen_tr_z_spin->setMaximum(100);
        screen_tr_z_spin->setSingleStep(0.01);

        verticalLayout_7->addWidget(screen_tr_z_spin);


        formLayout->setWidget(4, QFormLayout::FieldRole, groupBox_6);

        groupBox_3 = new QGroupBox(page_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_4 = new QVBoxLayout(groupBox_3);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        screen_bl_x_spin = new QDoubleSpinBox(groupBox_3);
        screen_bl_x_spin->setObjectName(QStringLiteral("screen_bl_x_spin"));
        screen_bl_x_spin->setAccelerated(true);
        screen_bl_x_spin->setMinimum(-100);
        screen_bl_x_spin->setMaximum(100);
        screen_bl_x_spin->setSingleStep(0.01);

        verticalLayout_4->addWidget(screen_bl_x_spin);

        screen_bl_y_spin = new QDoubleSpinBox(groupBox_3);
        screen_bl_y_spin->setObjectName(QStringLiteral("screen_bl_y_spin"));
        screen_bl_y_spin->setAccelerated(true);
        screen_bl_y_spin->setMinimum(-100);
        screen_bl_y_spin->setMaximum(100);
        screen_bl_y_spin->setSingleStep(0.01);

        verticalLayout_4->addWidget(screen_bl_y_spin);

        screen_bl_z_spin = new QDoubleSpinBox(groupBox_3);
        screen_bl_z_spin->setObjectName(QStringLiteral("screen_bl_z_spin"));
        screen_bl_z_spin->setAccelerated(true);
        screen_bl_z_spin->setMinimum(-100);
        screen_bl_z_spin->setMaximum(100);
        screen_bl_z_spin->setSingleStep(0.01);

        verticalLayout_4->addWidget(screen_bl_z_spin);


        formLayout->setWidget(6, QFormLayout::LabelRole, groupBox_3);

        groupBox_4 = new QGroupBox(page_2);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        verticalLayout_5 = new QVBoxLayout(groupBox_4);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        screen_br_x_spin = new QDoubleSpinBox(groupBox_4);
        screen_br_x_spin->setObjectName(QStringLiteral("screen_br_x_spin"));
        screen_br_x_spin->setAccelerated(true);
        screen_br_x_spin->setMinimum(-100);
        screen_br_x_spin->setMaximum(100);
        screen_br_x_spin->setSingleStep(0.01);

        verticalLayout_5->addWidget(screen_br_x_spin);

        screen_br_y_spin = new QDoubleSpinBox(groupBox_4);
        screen_br_y_spin->setObjectName(QStringLiteral("screen_br_y_spin"));
        screen_br_y_spin->setAccelerated(true);
        screen_br_y_spin->setMinimum(-100);
        screen_br_y_spin->setMaximum(100);
        screen_br_y_spin->setSingleStep(0.01);

        verticalLayout_5->addWidget(screen_br_y_spin);

        screen_br_z_spin = new QDoubleSpinBox(groupBox_4);
        screen_br_z_spin->setObjectName(QStringLiteral("screen_br_z_spin"));
        screen_br_z_spin->setAccelerated(true);
        screen_br_z_spin->setMinimum(-100);
        screen_br_z_spin->setMaximum(100);
        screen_br_z_spin->setSingleStep(0.01);

        verticalLayout_5->addWidget(screen_br_z_spin);


        formLayout->setWidget(6, QFormLayout::FieldRole, groupBox_4);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(8, QFormLayout::LabelRole, verticalSpacer_2);

        screen_continual_update_check = new QCheckBox(page_2);
        screen_continual_update_check->setObjectName(QStringLiteral("screen_continual_update_check"));
        screen_continual_update_check->setChecked(true);

        formLayout->setWidget(1, QFormLayout::LabelRole, screen_continual_update_check);

        toolBox->addItem(page_2, QStringLiteral("Screen"));
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
        screen_updtae_button->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Update", 0));
        groupBox_5->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Top Left", 0));
        groupBox_6->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Top Right", 0));
        groupBox_3->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Bottom Left", 0));
        groupBox_4->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Bottom Right", 0));
        screen_continual_update_check->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Continual", 0));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("raaOctaveToolInterfaceQt", "Screen", 0));
    } // retranslateUi

};

namespace Ui {
    class raaOctaveToolInterfaceQt: public Ui_raaOctaveToolInterfaceQt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // RAAOCTAVETOOLINTERFACEQT_H
