#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cmath>

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

//    ros::Rate loop_rate(10000);

//    while (ros::ok()) {
//        int readCount = 0;
//		while (1) {
//		    usleep(100);
//		    long r = read(fd, ((unsigned char *) &usb_frame) + readCount, sizeof(usb_frame) - readCount);
//		    if (r > 0) {
//			    readCount += r;
//		    }
//		    if (readCount > sizeof(usb_frame)) {
//			    ROS_ERROR("overflow.\n");
//			    return(666);
//		    }
//		    if (readCount == sizeof(usb_frame)) {
//			    if (usb_frame.head == 0x1234 && usb_frame.end == 0x5678) {
//				    break;
//			    } else {
//				    ROS_WARN("read invalid frame");
//				    syncFrame(fd);
//				    readCount = 0;
//			    }
//		    }
//	    }

//    ros::spinOnce();
//    loop_rate.sleep();

//    }

//}

