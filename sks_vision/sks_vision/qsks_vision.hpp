/**
 * @file /QServer_server/QServer.hpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef QServer_NODE_HPP_
#define QServer_NODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#include "../common/qnode.hpp"
#include <QObject>
#endif

/*****************************************************************************
** Class
*****************************************************************************/

Q_DECLARE_METATYPE(std::string);

class QSKS : public QNode {
 Q_OBJECT


public:
    QSKS(int argc, char** argv);
    virtual ~QSKS() {}
    void run();
    void ros_comms_init();

Q_SIGNALS:

private:
    ros::NodeHandle *nh;
    ros::Publisher pub;
};

#endif /* QServer_NODE_HPP_ */
