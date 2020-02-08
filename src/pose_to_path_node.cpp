/* Subscribe /dji_sdk/local_pose topic and pack the history poses into a nav_msgs::Path message,
 * publish to /dji_path topic.
 * Need modified dji_sdk that publishes /dji_sdk/local_pose topic.
 */

#include "ros/ros.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"

ros::Publisher path_pub;
nav_msgs::Path path;

void pose_callback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    path.poses.push_back(*msg);
    path_pub.publish(path);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "pose_to_path_node");
    ros::NodeHandle n;

    path_pub = n.advertise<nav_msgs::Path>("/dji_path", 100);
    ros::Subscriber pose_sub = n.subscribe("/dji_sdk/local_pose", 100, pose_callback);

    path.header.stamp = ros::Time::now();
    path.header.frame_id = "local";

    ros::spin();
    return 0;
}


