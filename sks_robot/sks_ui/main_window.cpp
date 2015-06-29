/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date November 2010
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "main_window.hpp"

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(QClient *node, QWidget *parent) :
    QMainWindow(parent),
    mClient(node)
{
	ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt())); // qApp is a global variable for the application

    ReadSettings();
    setWindowIcon(QIcon(":/images/icon.png"));
    ui.tab_manager->setCurrentIndex(0); // ensure the first tab is showing - qt-designer should have this already hardwired, but often loses it (settings?).

    setWindowTitle(QApplication::translate("MainWindowDesign", mClient->nodeName().c_str(), 0, QApplication::UnicodeUTF8));

    /*********************
    ** Logging
    **********************/
    QObject::connect(mClient, SIGNAL(rosShutdown()), this, SLOT(close()));

    /*********************
    ** Auto Start
    **********************/
    if ( ui.checkbox_remember_settings->isChecked() ) {
        on_button_connect_clicked(true);
    }


}

MainWindow::~MainWindow() {}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

void MainWindow::showNoMasterMessage() {
	QMessageBox msgBox;
	msgBox.setText("Couldn't find the ros master.");
	msgBox.exec();
    close();
}

/*
 * These triggers whenever the button is clicked, regardless of whether it
 * is already checked or not.
 */

void MainWindow::on_button_connect_clicked(bool check ) {
	if ( ui.checkbox_use_environment->isChecked() ) {
        if ( !mClient->on_init() ) {
			showNoMasterMessage();
		} else {
			ui.button_connect->setEnabled(false);
		}
	} else {
        if ( ! mClient->on_init(
					ui.line_edit_master->text().toStdString(),
					ui.line_edit_host->text().toStdString() )
				) {
			showNoMasterMessage();
		} else {
			ui.button_connect->setEnabled(false);
			ui.line_edit_master->setReadOnly(true);
			ui.line_edit_host->setReadOnly(true);
		}
    }
    ui.vel_Slider->setRange(0,100);
    ui.vel_Slider->setPageStep(1);
}

void MainWindow::on_checkbox_use_environment_stateChanged(int state) {
	bool enabled;
	if ( state == 0 ) {
		enabled = true;
	} else {
		enabled = false;
	}
	ui.line_edit_master->setEnabled(enabled);
	ui.line_edit_host->setEnabled(enabled);
}

/*****************************************************************************
** Implemenation [Slots][manually connected]
*****************************************************************************/

/**
 * This function is signalled by the underlying model. When the model changes,
 * this will drop the cursor down to the last line in the QListview to ensure
 * the user can always see the latest log message.
 */

/*****************************************************************************
** Implementation [Menu]
*****************************************************************************/

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("About ..."),tr("<h2>PACKAGE_NAME Test Program 0.10</h2><p>Copyright Yujin Robot</p><p>This package needs an about description.</p>"));
}

/*****************************************************************************
** Implementation [Configuration]
*****************************************************************************/

void MainWindow::ReadSettings() {
    QSettings settings("Qt-Ros Package", mClient->nodeName().c_str());
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    QString master_url = settings.value("master_url",QString("http://192.168.1.2:11311/")).toString();
    QString host_url = settings.value("host_url", QString("192.168.1.3")).toString();
    QString topic_name = settings.value("topic_name", QString("/chatter")).toString();
    ui.line_edit_master->setText(master_url);
    ui.line_edit_host->setText(host_url);
    bool remember = settings.value("remember_settings", false).toBool();
    ui.checkbox_remember_settings->setChecked(remember);
    bool checked = settings.value("use_environment_variables", false).toBool();
    ui.checkbox_use_environment->setChecked(checked);
    if ( checked ) {
    	ui.line_edit_master->setEnabled(false);
    	ui.line_edit_host->setEnabled(false);
    }
}

void MainWindow::WriteSettings() {
    QSettings settings("Qt-Ros Package", mClient->nodeName().c_str());
    settings.setValue("geometry", geometry());
    settings.setValue("master_url",ui.line_edit_master->text());
    settings.setValue("host_url",ui.line_edit_host->text());
   	settings.setValue("use_environment_variables",QVariant(ui.checkbox_use_environment->isChecked()));
    settings.setValue("windowState", saveState());
    settings.setValue("remember_settings",QVariant(ui.checkbox_remember_settings->isChecked()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mClient->shutdown();
	WriteSettings();
	QMainWindow::closeEvent(event);
}

void MainWindow::on_forward_btn_pressed()
{
    double vel = ui.vel_Slider->value();
    mClient->set_robot_speed(vel,vel,0);
}

void MainWindow::on_left_btn_pressed()
{
    double vel = ui.vel_Slider->value();
    mClient->set_robot_speed(0,vel,0);
}

void MainWindow::on_back_btn_pressed()
{
    double vel = ui.vel_Slider->value();
    mClient->set_robot_speed(-vel,-vel,0);
}

void MainWindow::on_right_btn_pressed()
{
    double vel = ui.vel_Slider->value();
    mClient->set_robot_speed(vel,0,0);
}

void MainWindow::on_L_Front_btn_pressed()
{
    double vel = ui.vel_Slider->value()/sqrt(2);
    mClient->set_robot_speed(vel,vel,0);
}

void MainWindow::on_R_Front_btn_pressed()
{
    double vel = ui.vel_Slider->value()/sqrt(2);
    mClient->set_robot_speed(vel,-vel,0);
}

void MainWindow::on_L_Rear_btn_pressed()
{
    double vel = ui.vel_Slider->value()/sqrt(2);
    mClient->set_robot_speed(-vel,vel,0);
}


void MainWindow::on_R_Rear_btn_pressed()
{
    double vel = ui.vel_Slider->value()/sqrt(2);
    mClient->set_robot_speed(-vel,-vel,0);
}

void MainWindow::on_L_Rot_btn_pressed()
{
    double vel = ui.vel_Slider->value();
    mClient->set_robot_speed(0,0,vel);
}

void MainWindow::on_R_Rot_btn_pressed()
{
    double vel = ui.vel_Slider->value();
    mClient->set_robot_speed(0,0,-vel);
}

void MainWindow::on_forward_btn_released()
{
    mClient->set_robot_speed(0,0,0);
}

void MainWindow::on_left_btn_released()
{
    mClient->set_robot_speed(0,0,0);

}

void MainWindow::on_back_btn_released()
{
    mClient->set_robot_speed(0,0,0);

}

void MainWindow::on_right_btn_released()
{
    mClient->set_robot_speed(0,0,0);

}

void MainWindow::on_L_Front_btn_released()
{
    mClient->set_robot_speed(0,0,0);

}


void MainWindow::on_R_Front_btn_released()
{
    mClient->set_robot_speed(0,0,0);

}

void MainWindow::on_L_Rear_btn_released()
{
    mClient->set_robot_speed(0,0,0);

}

void MainWindow::on_R_Rear_btn_released()
{
    mClient->set_robot_speed(0,0,0);
}

void MainWindow::on_L_Rot_btn_released()
{
    mClient->set_robot_speed(0,0,0);
}

void MainWindow::on_R_Rot_btn_released()
{
    mClient->set_robot_speed(0,0,0);
}

void MainWindow::on_vel_Slider_actionTriggered(int action)
{
    int value = ui.vel_Slider->value();
    QString val = QString::number(value);
    ui.label_4->setText(val);
}

void MainWindow::on_right_btn_clicked()
{

}

void MainWindow::on_forward_btn_clicked()
{

}
