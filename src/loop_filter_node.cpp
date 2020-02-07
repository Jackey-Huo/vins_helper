#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cmath>

#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>

#include <tf2_msgs/TFMessage.h>
//#include <tf2_ros>
#include <tf/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include "loop_filter.h"

//void pose_callback(const geometry_msgs::PoseStamped::ConstPtr& msg)
//{
//    path.poses.push_back(*msg);
//    path_pub.publish(path);
//}

Loop_filter filter;
ros::Publisher pose_pub;

//void tf_callback(const tf2_msgs::TFMessage::ConstPtr& tf_msg)
//{
//    if(!tf_msg->transforms[0].child_frame_id.compare("loop")){
//
//        fliter.update_tf(tf_msg->transforms[0].transform);
//    }
//
//
//}

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
//    tf.child_frame_id_ = "loop_filtered";
    br.sendTransform(tf);
    std::cout<<"tf:"<<tf.getOrigin().getX()<<std::endl;

    geometry_msgs::Pose pose;
    geometry_msgs::TransformStamped tf_msg;
    tf::transformStampedTFToMsg(tf,tf_msg);

    tf2::doTransform(msg->pose.pose,pose, tf_msg);
    geometry_msgs::PoseStamped poseStamped;
    poseStamped.pose = pose;
//    poseStamped.header.frame_id = "world";
//    poseStamped.header.seq = msg->header.seq;
//    poseStamped.header.stamp = msg->header.stamp;
    poseStamped.header = msg->header;

    pose_pub.publish(poseStamped);


//    tf2::doTransform(msg->pose.pose,pose.pose,tf);
//    tf2::doTransform(msg->pose,pose,tf1);

}


int main(int argc, char **argv) {
    ros::init(argc, argv, "loop_filter_node");
    ros::NodeHandle n;

    ros::Subscriber tf_sub = n.subscribe("/vins_estimator/odometry", 100, odometry_callback);
    pose_pub = n.advertise<geometry_msgs::PoseStamped>("/pose_loop_filtered", 100);

    ros::spin();

//    while(n.ok()){
//        ros::Time now = ros::Time::now();
//        tf::StampedTransform tf;
//        try {
//            tf_listener.lookupTransform("/world", "/loop", ros::Time(0), tf);
//        }catch (tf::TransformException ex) {
//            ROS_ERROR(ex.what());
//            usleep(100000);
//        }
//
//
//        filter.update_tf(tf);
//
//        static tf::TransformBroadcaster br;
//        tf.child_frame_id_ = "loop_filtered";
//        br.sendTransform(tf);
//
//    }
//
//    while(n.ok()){
//        ros::Time now = ros::Time::now();
//        tf::StampedTransform tf;
//        if(tf_listener.waitForTransform("/world","/loop",now, ros::Duration(1))){
//            tf_listener.lookupTransform("/world","/loop",now, tf);
//            filter.update_tf(tf);
//
//            static tf::TransformBroadcaster br;
//            tf.child_frame_id_ = "loop_filtered";
//            br.sendTransform(tf);
//        }else{
//            ROS_WARN("No tf from world to loop received in 1s");
//        }
//
//    }

//    ros::Subscriber tf_sub = n.subscribe("/tf", 100, tf_callback);



//    path_pub = n.advertise<nav_msgs::Path>("/dji_path", 100);
//    ros::Subscriber pose_sub = n.subscribe("/dji_sdk/local_pose", 100, pose_callback);

//    path.header.stamp = ros::Time::now();
//    path.header.frame_id = "local";

//    ros::spin();
    return 0;
}





