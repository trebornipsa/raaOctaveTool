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
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
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
    QGridLayout *gridLayout_4;
    QPushButton *screen_updtae_button;
    QCheckBox *screen_continual_update_check;
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
    QGroupBox *groupBox_8;
    QVBoxLayout *verticalLayout_9;
    QDoubleSpinBox *screen_far_spin;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_8;
    QDoubleSpinBox *screen_near_spin;
    QGroupBox *groupBox_9;
    QVBoxLayout *verticalLayout_11;
    QDoubleSpinBox *screen_image_rot_spin;
    QGroupBox *groupBox_19;
    QVBoxLayout *verticalLayout_16;
    QLineEdit *screen_add_edit;
    QPushButton *screen_add_button;
    QSpacerItem *verticalSpacer_3;
    QGroupBox *groupBox_10;
    QVBoxLayout *verticalLayout_10;
    QCheckBox *screen_x_flip_check;
    QCheckBox *screen_y_flip_check;
    QCheckBox *screen_z_flip_check;
    QWidget *page_5;
    QFormLayout *formLayout_2;
    QComboBox *window_combo;
    QPushButton *window_update_button;
    QCheckBox *window_continual_check;
    QGroupBox *groupBox_11;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *window_pos_x_spin;
    QSpinBox *window_pos_y_spin;
    QGroupBox *groupBox_12;
    QHBoxLayout *horizontalLayout_3;
    QSpinBox *window_width_spin;
    QSpinBox *window_height_spin;
    QWidget *page_6;
    QGridLayout *gridLayout_3;
    QPushButton *tracker_update_button;
    QCheckBox *tracker_update_check;
    QComboBox *tracker_combo;
    QGroupBox *groupBox_14;
    QVBoxLayout *verticalLayout_15;
    QGroupBox *groupBox_15;
    QHBoxLayout *horizontalLayout_4;
    QDoubleSpinBox *tracker_position_x_spin;
    QDoubleSpinBox *tracker_position_y_spin;
    QDoubleSpinBox *tracker_position_z_spin;
    QGroupBox *groupBox_16;
    QHBoxLayout *horizontalLayout_5;
    QDoubleSpinBox *tracker_direction_x_spin;
    QDoubleSpinBox *tracker_direction_y_spin;
    QDoubleSpinBox *tracker_direction_z_spin;
    QGroupBox *groupBox_17;
    QHBoxLayout *horizontalLayout_6;
    QDoubleSpinBox *tracker_up_x_spin;
    QDoubleSpinBox *tracker_up_y_spin;
    QDoubleSpinBox *tracker_up_z_spin;
    QGroupBox *groupBox_18;
    QHBoxLayout *horizontalLayout_7;
    QLabel *tracker_right_x_label;
    QLabel *tracker_right_y_label;
    QLabel *tracker_right_z_label;
    QGroupBox *groupBox_13;
    QVBoxLayout *verticalLayout_14;
    QSpinBox *tracker_sensor_spin;
    QCheckBox *trackr_sensor_show_check;
    QCheckBox *tracker_sensor_eye_tracker_check;
    QSpacerItem *verticalSpacer_2;
    QWidget *page_7;
    QFormLayout *formLayout;
    QGroupBox *groupBox_20;
    QVBoxLayout *verticalLayout_17;
    QLineEdit *config_name_edit;
    QPushButton *config_save_button;
    QListWidget *config_list_widget;
    QStackedWidget *stackedWidget;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_12;
    raaQTOSGWidget *gl_widget;
    QWidget *page_4;
    QVBoxLayout *verticalLayout_13;
    QGraphicsView *graphicsView;

    void setupUi(QMainWindow *raaOctaveToolInterfaceQt)
    {
        if (raaOctaveToolInterfaceQt->objectName().isEmpty())
            raaOctaveToolInterfaceQt->setObjectName(QStringLiteral("raaOctaveToolInterfaceQt"));
        raaOctaveToolInterfaceQt->resize(1473, 1422);
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
        page->setGeometry(QRect(0, 0, 282, 1187));
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
        page_2->setGeometry(QRect(0, 0, 327, 1157));
        gridLayout_4 = new QGridLayout(page_2);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        screen_updtae_button = new QPushButton(page_2);
        screen_updtae_button->setObjectName(QStringLiteral("screen_updtae_button"));

        gridLayout_4->addWidget(screen_updtae_button, 0, 0, 1, 2);

        screen_continual_update_check = new QCheckBox(page_2);
        screen_continual_update_check->setObjectName(QStringLiteral("screen_continual_update_check"));
        screen_continual_update_check->setChecked(true);

        gridLayout_4->addWidget(screen_continual_update_check, 0, 2, 1, 1);

        screen_combo = new QComboBox(page_2);
        screen_combo->setObjectName(QStringLiteral("screen_combo"));

        gridLayout_4->addWidget(screen_combo, 1, 0, 1, 1);

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


        gridLayout_4->addWidget(groupBox_5, 2, 0, 1, 2);

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


        gridLayout_4->addWidget(groupBox_6, 2, 2, 1, 1);

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


        gridLayout_4->addWidget(groupBox_3, 3, 0, 1, 2);

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


        gridLayout_4->addWidget(groupBox_4, 3, 2, 1, 1);

        groupBox_8 = new QGroupBox(page_2);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        verticalLayout_9 = new QVBoxLayout(groupBox_8);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        screen_far_spin = new QDoubleSpinBox(groupBox_8);
        screen_far_spin->setObjectName(QStringLiteral("screen_far_spin"));
        screen_far_spin->setAccelerated(true);
        screen_far_spin->setMaximum(10000);

        verticalLayout_9->addWidget(screen_far_spin);


        gridLayout_4->addWidget(groupBox_8, 4, 2, 1, 1);

        groupBox_7 = new QGroupBox(page_2);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        verticalLayout_8 = new QVBoxLayout(groupBox_7);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        screen_near_spin = new QDoubleSpinBox(groupBox_7);
        screen_near_spin->setObjectName(QStringLiteral("screen_near_spin"));
        screen_near_spin->setMaximum(1000);
        screen_near_spin->setSingleStep(0.1);

        verticalLayout_8->addWidget(screen_near_spin);


        gridLayout_4->addWidget(groupBox_7, 5, 2, 1, 1);

        groupBox_9 = new QGroupBox(page_2);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        verticalLayout_11 = new QVBoxLayout(groupBox_9);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        screen_image_rot_spin = new QDoubleSpinBox(groupBox_9);
        screen_image_rot_spin->setObjectName(QStringLiteral("screen_image_rot_spin"));
        screen_image_rot_spin->setMinimum(-180);
        screen_image_rot_spin->setMaximum(180);
        screen_image_rot_spin->setSingleStep(0.1);

        verticalLayout_11->addWidget(screen_image_rot_spin);


        gridLayout_4->addWidget(groupBox_9, 6, 2, 1, 1);

        groupBox_19 = new QGroupBox(page_2);
        groupBox_19->setObjectName(QStringLiteral("groupBox_19"));
        verticalLayout_16 = new QVBoxLayout(groupBox_19);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        screen_add_edit = new QLineEdit(groupBox_19);
        screen_add_edit->setObjectName(QStringLiteral("screen_add_edit"));

        verticalLayout_16->addWidget(screen_add_edit);

        screen_add_button = new QPushButton(groupBox_19);
        screen_add_button->setObjectName(QStringLiteral("screen_add_button"));

        verticalLayout_16->addWidget(screen_add_button);


        gridLayout_4->addWidget(groupBox_19, 7, 0, 1, 3);

        verticalSpacer_3 = new QSpacerItem(20, 265, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_3, 8, 1, 1, 1);

        groupBox_10 = new QGroupBox(page_2);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));
        verticalLayout_10 = new QVBoxLayout(groupBox_10);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        screen_x_flip_check = new QCheckBox(groupBox_10);
        screen_x_flip_check->setObjectName(QStringLiteral("screen_x_flip_check"));

        verticalLayout_10->addWidget(screen_x_flip_check);

        screen_y_flip_check = new QCheckBox(groupBox_10);
        screen_y_flip_check->setObjectName(QStringLiteral("screen_y_flip_check"));

        verticalLayout_10->addWidget(screen_y_flip_check);

        screen_z_flip_check = new QCheckBox(groupBox_10);
        screen_z_flip_check->setObjectName(QStringLiteral("screen_z_flip_check"));

        verticalLayout_10->addWidget(screen_z_flip_check);


        gridLayout_4->addWidget(groupBox_10, 4, 0, 3, 2);

        toolBox->addItem(page_2, QStringLiteral("Screen"));
        page_5 = new QWidget();
        page_5->setObjectName(QStringLiteral("page_5"));
        page_5->setGeometry(QRect(0, 0, 283, 1187));
        formLayout_2 = new QFormLayout(page_5);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        window_combo = new QComboBox(page_5);
        window_combo->setObjectName(QStringLiteral("window_combo"));

        formLayout_2->setWidget(0, QFormLayout::SpanningRole, window_combo);

        window_update_button = new QPushButton(page_5);
        window_update_button->setObjectName(QStringLiteral("window_update_button"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, window_update_button);

        window_continual_check = new QCheckBox(page_5);
        window_continual_check->setObjectName(QStringLiteral("window_continual_check"));
        window_continual_check->setChecked(true);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, window_continual_check);

        groupBox_11 = new QGroupBox(page_5);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        window_pos_x_spin = new QSpinBox(groupBox_11);
        window_pos_x_spin->setObjectName(QStringLiteral("window_pos_x_spin"));
        window_pos_x_spin->setMinimum(-5000);
        window_pos_x_spin->setMaximum(5000);

        horizontalLayout_2->addWidget(window_pos_x_spin);

        window_pos_y_spin = new QSpinBox(groupBox_11);
        window_pos_y_spin->setObjectName(QStringLiteral("window_pos_y_spin"));
        window_pos_y_spin->setMinimum(-5000);
        window_pos_y_spin->setMaximum(5000);

        horizontalLayout_2->addWidget(window_pos_y_spin);


        formLayout_2->setWidget(2, QFormLayout::SpanningRole, groupBox_11);

        groupBox_12 = new QGroupBox(page_5);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_12);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        window_width_spin = new QSpinBox(groupBox_12);
        window_width_spin->setObjectName(QStringLiteral("window_width_spin"));
        window_width_spin->setMaximum(10000);
        window_width_spin->setValue(200);

        horizontalLayout_3->addWidget(window_width_spin);

        window_height_spin = new QSpinBox(groupBox_12);
        window_height_spin->setObjectName(QStringLiteral("window_height_spin"));
        window_height_spin->setMaximum(10000);
        window_height_spin->setValue(200);

        horizontalLayout_3->addWidget(window_height_spin);


        formLayout_2->setWidget(3, QFormLayout::SpanningRole, groupBox_12);

        toolBox->addItem(page_5, QStringLiteral("Window"));
        page_6 = new QWidget();
        page_6->setObjectName(QStringLiteral("page_6"));
        page_6->setGeometry(QRect(0, 0, 417, 1157));
        gridLayout_3 = new QGridLayout(page_6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        tracker_update_button = new QPushButton(page_6);
        tracker_update_button->setObjectName(QStringLiteral("tracker_update_button"));

        gridLayout_3->addWidget(tracker_update_button, 0, 0, 1, 1);

        tracker_update_check = new QCheckBox(page_6);
        tracker_update_check->setObjectName(QStringLiteral("tracker_update_check"));
        tracker_update_check->setChecked(true);

        gridLayout_3->addWidget(tracker_update_check, 0, 1, 1, 1);

        tracker_combo = new QComboBox(page_6);
        tracker_combo->setObjectName(QStringLiteral("tracker_combo"));

        gridLayout_3->addWidget(tracker_combo, 1, 0, 1, 1);

        groupBox_14 = new QGroupBox(page_6);
        groupBox_14->setObjectName(QStringLiteral("groupBox_14"));
        verticalLayout_15 = new QVBoxLayout(groupBox_14);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        groupBox_15 = new QGroupBox(groupBox_14);
        groupBox_15->setObjectName(QStringLiteral("groupBox_15"));
        horizontalLayout_4 = new QHBoxLayout(groupBox_15);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        tracker_position_x_spin = new QDoubleSpinBox(groupBox_15);
        tracker_position_x_spin->setObjectName(QStringLiteral("tracker_position_x_spin"));
        tracker_position_x_spin->setAccelerated(true);
        tracker_position_x_spin->setMinimum(-1000);
        tracker_position_x_spin->setMaximum(1000);
        tracker_position_x_spin->setSingleStep(0.1);

        horizontalLayout_4->addWidget(tracker_position_x_spin);

        tracker_position_y_spin = new QDoubleSpinBox(groupBox_15);
        tracker_position_y_spin->setObjectName(QStringLiteral("tracker_position_y_spin"));
        tracker_position_y_spin->setAccelerated(true);
        tracker_position_y_spin->setMinimum(-1000);
        tracker_position_y_spin->setMaximum(1000);
        tracker_position_y_spin->setSingleStep(0.1);

        horizontalLayout_4->addWidget(tracker_position_y_spin);

        tracker_position_z_spin = new QDoubleSpinBox(groupBox_15);
        tracker_position_z_spin->setObjectName(QStringLiteral("tracker_position_z_spin"));
        tracker_position_z_spin->setAccelerated(true);
        tracker_position_z_spin->setMinimum(-1000);
        tracker_position_z_spin->setMaximum(1000);
        tracker_position_z_spin->setSingleStep(0.1);

        horizontalLayout_4->addWidget(tracker_position_z_spin);


        verticalLayout_15->addWidget(groupBox_15);

        groupBox_16 = new QGroupBox(groupBox_14);
        groupBox_16->setObjectName(QStringLiteral("groupBox_16"));
        horizontalLayout_5 = new QHBoxLayout(groupBox_16);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        tracker_direction_x_spin = new QDoubleSpinBox(groupBox_16);
        tracker_direction_x_spin->setObjectName(QStringLiteral("tracker_direction_x_spin"));
        tracker_direction_x_spin->setAccelerated(true);
        tracker_direction_x_spin->setMinimum(-1);
        tracker_direction_x_spin->setMaximum(1);
        tracker_direction_x_spin->setSingleStep(0.01);

        horizontalLayout_5->addWidget(tracker_direction_x_spin);

        tracker_direction_y_spin = new QDoubleSpinBox(groupBox_16);
        tracker_direction_y_spin->setObjectName(QStringLiteral("tracker_direction_y_spin"));
        tracker_direction_y_spin->setAccelerated(true);
        tracker_direction_y_spin->setMinimum(-1);
        tracker_direction_y_spin->setMaximum(1);
        tracker_direction_y_spin->setSingleStep(0.01);

        horizontalLayout_5->addWidget(tracker_direction_y_spin);

        tracker_direction_z_spin = new QDoubleSpinBox(groupBox_16);
        tracker_direction_z_spin->setObjectName(QStringLiteral("tracker_direction_z_spin"));
        tracker_direction_z_spin->setAccelerated(true);
        tracker_direction_z_spin->setMinimum(-1);
        tracker_direction_z_spin->setMaximum(1);
        tracker_direction_z_spin->setSingleStep(0.01);

        horizontalLayout_5->addWidget(tracker_direction_z_spin);


        verticalLayout_15->addWidget(groupBox_16);

        groupBox_17 = new QGroupBox(groupBox_14);
        groupBox_17->setObjectName(QStringLiteral("groupBox_17"));
        horizontalLayout_6 = new QHBoxLayout(groupBox_17);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        tracker_up_x_spin = new QDoubleSpinBox(groupBox_17);
        tracker_up_x_spin->setObjectName(QStringLiteral("tracker_up_x_spin"));
        tracker_up_x_spin->setAccelerated(true);
        tracker_up_x_spin->setMinimum(-1000);
        tracker_up_x_spin->setMaximum(1000);
        tracker_up_x_spin->setSingleStep(0.1);

        horizontalLayout_6->addWidget(tracker_up_x_spin);

        tracker_up_y_spin = new QDoubleSpinBox(groupBox_17);
        tracker_up_y_spin->setObjectName(QStringLiteral("tracker_up_y_spin"));
        tracker_up_y_spin->setAccelerated(true);
        tracker_up_y_spin->setMinimum(-1000);
        tracker_up_y_spin->setMaximum(1000);
        tracker_up_y_spin->setSingleStep(0.1);

        horizontalLayout_6->addWidget(tracker_up_y_spin);

        tracker_up_z_spin = new QDoubleSpinBox(groupBox_17);
        tracker_up_z_spin->setObjectName(QStringLiteral("tracker_up_z_spin"));
        tracker_up_z_spin->setAccelerated(true);
        tracker_up_z_spin->setMinimum(-1000);
        tracker_up_z_spin->setMaximum(1000);
        tracker_up_z_spin->setSingleStep(0.1);

        horizontalLayout_6->addWidget(tracker_up_z_spin);


        verticalLayout_15->addWidget(groupBox_17);

        groupBox_18 = new QGroupBox(groupBox_14);
        groupBox_18->setObjectName(QStringLiteral("groupBox_18"));
        horizontalLayout_7 = new QHBoxLayout(groupBox_18);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        tracker_right_x_label = new QLabel(groupBox_18);
        tracker_right_x_label->setObjectName(QStringLiteral("tracker_right_x_label"));

        horizontalLayout_7->addWidget(tracker_right_x_label);

        tracker_right_y_label = new QLabel(groupBox_18);
        tracker_right_y_label->setObjectName(QStringLiteral("tracker_right_y_label"));

        horizontalLayout_7->addWidget(tracker_right_y_label);

        tracker_right_z_label = new QLabel(groupBox_18);
        tracker_right_z_label->setObjectName(QStringLiteral("tracker_right_z_label"));

        horizontalLayout_7->addWidget(tracker_right_z_label);


        verticalLayout_15->addWidget(groupBox_18);


        gridLayout_3->addWidget(groupBox_14, 2, 0, 1, 2);

        groupBox_13 = new QGroupBox(page_6);
        groupBox_13->setObjectName(QStringLiteral("groupBox_13"));
        verticalLayout_14 = new QVBoxLayout(groupBox_13);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        tracker_sensor_spin = new QSpinBox(groupBox_13);
        tracker_sensor_spin->setObjectName(QStringLiteral("tracker_sensor_spin"));
        tracker_sensor_spin->setMaximum(31);

        verticalLayout_14->addWidget(tracker_sensor_spin);

        trackr_sensor_show_check = new QCheckBox(groupBox_13);
        trackr_sensor_show_check->setObjectName(QStringLiteral("trackr_sensor_show_check"));

        verticalLayout_14->addWidget(trackr_sensor_show_check);

        tracker_sensor_eye_tracker_check = new QCheckBox(groupBox_13);
        tracker_sensor_eye_tracker_check->setObjectName(QStringLiteral("tracker_sensor_eye_tracker_check"));

        verticalLayout_14->addWidget(tracker_sensor_eye_tracker_check);


        gridLayout_3->addWidget(groupBox_13, 3, 0, 1, 2);

        verticalSpacer_2 = new QSpacerItem(20, 193, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 4, 0, 1, 1);

        toolBox->addItem(page_6, QStringLiteral("Tracker"));
        page_7 = new QWidget();
        page_7->setObjectName(QStringLiteral("page_7"));
        formLayout = new QFormLayout(page_7);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        groupBox_20 = new QGroupBox(page_7);
        groupBox_20->setObjectName(QStringLiteral("groupBox_20"));
        verticalLayout_17 = new QVBoxLayout(groupBox_20);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        config_name_edit = new QLineEdit(groupBox_20);
        config_name_edit->setObjectName(QStringLiteral("config_name_edit"));

        verticalLayout_17->addWidget(config_name_edit);

        config_save_button = new QPushButton(groupBox_20);
        config_save_button->setObjectName(QStringLiteral("config_save_button"));

        verticalLayout_17->addWidget(config_save_button);


        formLayout->setWidget(1, QFormLayout::LabelRole, groupBox_20);

        config_list_widget = new QListWidget(page_7);
        config_list_widget->setObjectName(QStringLiteral("config_list_widget"));

        formLayout->setWidget(0, QFormLayout::LabelRole, config_list_widget);

        toolBox->addItem(page_7, QStringLiteral("Config"));
        splitter->addWidget(toolBox);
        stackedWidget = new QStackedWidget(splitter);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(2);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy3);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        verticalLayout_12 = new QVBoxLayout(page_3);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        gl_widget = new raaQTOSGWidget(page_3);
        gl_widget->setObjectName(QStringLiteral("gl_widget"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(gl_widget->sizePolicy().hasHeightForWidth());
        gl_widget->setSizePolicy(sizePolicy4);

        verticalLayout_12->addWidget(gl_widget);

        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        verticalLayout_13 = new QVBoxLayout(page_4);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        graphicsView = new QGraphicsView(page_4);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));

        verticalLayout_13->addWidget(graphicsView);

        stackedWidget->addWidget(page_4);
        splitter->addWidget(stackedWidget);

        horizontalLayout->addWidget(splitter);

        raaOctaveToolInterfaceQt->setCentralWidget(centralwidget);

        retranslateUi(raaOctaveToolInterfaceQt);

        toolBox->setCurrentIndex(0);
        stackedWidget->setCurrentIndex(0);


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
        screen_continual_update_check->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Continual", 0));
        groupBox_5->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Top Left", 0));
        groupBox_6->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Top Right", 0));
        groupBox_3->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Bottom Left", 0));
        groupBox_4->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Bottom Right", 0));
        groupBox_8->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Far", 0));
        groupBox_7->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Near", 0));
        groupBox_9->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Image Rotation", 0));
        groupBox_19->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Add Screen", 0));
        screen_add_button->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Add", 0));
        groupBox_10->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Image Flip", 0));
        screen_x_flip_check->setText(QApplication::translate("raaOctaveToolInterfaceQt", "X", 0));
        screen_y_flip_check->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Y", 0));
        screen_z_flip_check->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Z", 0));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("raaOctaveToolInterfaceQt", "Screen", 0));
        window_update_button->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Update", 0));
        window_continual_check->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Continual", 0));
        groupBox_11->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Position (x, y)", 0));
        groupBox_12->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Dim (width, height)", 0));
        toolBox->setItemText(toolBox->indexOf(page_5), QApplication::translate("raaOctaveToolInterfaceQt", "Window", 0));
        tracker_update_button->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Update", 0));
        tracker_update_check->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Continual", 0));
        groupBox_14->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Origin", 0));
        groupBox_15->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Position", 0));
        groupBox_16->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Direction", 0));
        groupBox_17->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Up", 0));
        groupBox_18->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Right", 0));
        tracker_right_x_label->setText(QApplication::translate("raaOctaveToolInterfaceQt", "0.00", 0));
        tracker_right_y_label->setText(QApplication::translate("raaOctaveToolInterfaceQt", "0.00", 0));
        tracker_right_z_label->setText(QApplication::translate("raaOctaveToolInterfaceQt", "0.00", 0));
        groupBox_13->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Sensors", 0));
        trackr_sensor_show_check->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Show", 0));
        tracker_sensor_eye_tracker_check->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Eye Tracker", 0));
        toolBox->setItemText(toolBox->indexOf(page_6), QApplication::translate("raaOctaveToolInterfaceQt", "Tracker", 0));
        groupBox_20->setTitle(QApplication::translate("raaOctaveToolInterfaceQt", "Save Current Config", 0));
        config_save_button->setText(QApplication::translate("raaOctaveToolInterfaceQt", "Save", 0));
        toolBox->setItemText(toolBox->indexOf(page_7), QApplication::translate("raaOctaveToolInterfaceQt", "Config", 0));
    } // retranslateUi

};

namespace Ui {
    class raaOctaveToolInterfaceQt: public Ui_raaOctaveToolInterfaceQt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // RAAOCTAVETOOLINTERFACEQT_H
