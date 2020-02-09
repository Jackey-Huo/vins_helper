//
// Subscribe VIO published poses and transform them into a filtered loop-closure frame.
// Then publish to /pose_loop_filtered.
//
// Created by mhc on 2020/2/7.
//

#include "loop_filter.h"

#include <cstring>
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <boost/bind.hpp>


ros::Publisher pose_pub;

void odometry_callback(const nav_msgs::Odometry::ConstPtr& msg, Loop_filter& filter,
                       tf::TransformListener& tf_listener, tf::TransformBroadcaster& br)
{

    // try to lookup tf between /world and /loop. Update filter if success.
    tf::StampedTransform transform;
    try {
        tf_listener.lookupTransform("/world", "/loop", ros::Time(0), transform);
        filter.set_target_tf(transform);
    }catch (tf::TransformException &ex) {
        ROS_WARN("Failed to lookupTransform from /world to /loop");
    }

    // publish filtered transform
    filter.get_tf(transform); // get filtered transform
    br.sendTransform(transform);
//    std::cout << "transform:" << transform.getOrigin().getX() << std::endl;

    // transform and publish filtered pose
    geometry_msgs::Pose filtered_pose;
    geometry_msgs::TransformStamped tf_msg;
    tf::transformStampedTFToMsg(transform, tf_msg);
    tf2::doTransform(msg->pose.pose,filtered_pose, tf_msg);
    geometry_msgs::PoseStamped poseStamped;
    poseStamped.pose = filtered_pose;
    poseStamped.header = msg->header;
    pose_pub.publish(poseStamped);
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "loop_filter_node");
    ros::NodeHandle n;

    // advertise filtered pose /pose_loop_filtered
    pose_pub = n.advertise<geometry_msgs::PoseStamped>("/pose_loop_filtered", 100);

    // create context for odometry_callback
    Loop_filter filter;
    tf::TransformListener tf_listener;
    tf::TransformBroadcaster br;

    // subscribe VIO published pose
    ros::Subscriber tf_sub = n.subscribe<nav_msgs::Odometry>
          ("/vins_estimator/odometry", 100, boost::bind(&odometry_callback, _1,
                                                        boost::ref(filter),
                                                        boost::ref(tf_listener),
                                                        boost::ref(br)));

    ros::spin();

    return 0;
}





