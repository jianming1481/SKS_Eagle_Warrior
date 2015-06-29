/**
 * @file common/main_window.hpp
 *
 * @brief Qt based gui for eros_qtalker.
 *
 * @date November 2010
 **/
#ifndef QTUTORIALS_MAIN_WINDOW_H
#define QTUTORIALS_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>
#ifndef Q_MOC_RUN
#include "sks_ui/ui_main_window.h"
//#include "../common/qnode.hpp"
#include "qClient.hpp"
#endif

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QClient *node, QWidget *parent = 0);
	~MainWindow();

	void ReadSettings(); // Load up qt program settings at startup
	void WriteSettings(); // Save qt program settings when closing

	void closeEvent(QCloseEvent *event); // Overloaded function
	void showNoMasterMessage();

public Q_SLOTS:
	// Put automatically triggered slots here (because of connectSlotsByName())
	// void on_button_enable_clicked(bool check); // example only
    void on_actionAbout_triggered();
	void on_button_connect_clicked(bool check );
	void on_checkbox_use_environment_stateChanged(int state);

    /******************************************
    ** Manual connections
    *******************************************/
//    void updateLoggingView(); // no idea why this can't connect automatically

private Q_SLOTS:

    void on_forward_btn_pressed();

    void on_forward_btn_released();

    void on_left_btn_pressed();

    void on_left_btn_released();

    void on_back_btn_pressed();

    void on_back_btn_released();

    void on_right_btn_pressed();

    void on_right_btn_released();

    void on_L_Front_btn_pressed();

    void on_L_Front_btn_released();

    void on_R_Front_btn_pressed();

    void on_R_Front_btn_released();

    void on_L_Rear_btn_pressed();

    void on_L_Rear_btn_released();

    void on_R_Rear_btn_pressed();

    void on_R_Rear_btn_released();

    void on_vel_Slider_actionTriggered(int action);

    void on_L_Rot_btn_pressed();

    void on_R_Rot_btn_pressed();

    void on_L_Rot_btn_released();

    void on_R_Rot_btn_released();

    void on_right_btn_clicked();

    void on_forward_btn_clicked();

private:
    Ui::MainWindowDesign ui;
    //QNode *qnode;
    QClient *mClient;

    geometry_msgs::Twist vel;
};

#endif // QTUTORIALS_MAIN_WINDOW_H
