#include <cstring>
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include "loop_filter.h"


Loop_filter filter;
ros::Publisher pose_pub;


void odometry_callback(const nav_msgs::Odometry::ConstPtr& msg)
{
    static tf::TransformListener tf_listener;
    tf::StampedTransform tf;
    try {
        tf_listener.lookupTransform("/world", "/loop", ros::Time(0), tf);
        filter.update_tf(tf);
    }catch (tf::TransformException &ex) {
        ROS_WARN(ex.what());
        usleep(100000);
    }

    static tf::TransformBroadcaster br;
    filter.get_tf(tf);
    br.sendTransform(tf);
    std::cout<<"tf:"<<tf.getOrigin().getX()<<std::endl;

    geometry_msgs::Pose pose;
    geometry_msgs::TransformStamped tf_msg;
    tf::transformStampedTFToMsg(tf,tf_msg);

    tf2::doTransform(msg->pose.pose,pose, tf_msg);
    geometry_msgs::PoseStamped poseStamped;
    poseStamped.pose = pose;
    poseStamped.header = msg->header;

    pose_pub.publish(poseStamped);
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "loop_filter_node");
    ros::NodeHandle n;

    ros::Subscriber tf_sub = n.subscribe("/vins_estimator/odometry", 100, odometry_callback);
    pose_pub = n.advertise<geometry_msgs::PoseStamped>("/pose_loop_filtered", 100);

    ros::spin();

    return 0;
}





