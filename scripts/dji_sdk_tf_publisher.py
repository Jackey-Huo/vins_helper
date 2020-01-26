#!/usr/bin/env python

## Publish a tf that put the origin of frame world at first local_position message

import rospy
import tf
import tf2_ros
import numpy 
import math
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import TransformStamped
from nav_msgs.msg import Odometry
from nav_msgs.msg import Path

drop_pose = 100
init_mav_pose = None
init_vio_pose = None

vio_started = False
translation = None

path_pub = None
path = Path()

def vio_callback(msg):
    global drop_pose, init_vio_pose, vio_started 
    vio_started = True 
    if (drop_pose > 0) or (init_vio_pose is not None):
        drop_pose -= 1
        rospy.loginfo(drop_pose)
    else:
        init_vio_pose = msg.pose

def mav_callback(msg):
    global vio_started, init_vio_pose, init_mav_pose, translation 
    if vio_started and not translation:
        translation = msg.pose.position

    if init_vio_pose:
        init_mav_pose = msg

    #    q = msg.pose.orientation
    #    euler2 = tf.transformations.euler_from_quaternion([q.x, q.y, q.z, q.w],'szyx')
    #    euler2 = numpy.array(euler2)*180/3.14
        
    #    q = init_vio_pose.pose.pose.orientation
    #    euler1 = tf.transformations.euler_from_quaternion([q.x, q.y, q.z, q.w],'szyx')
    #    euler1 = numpy.array(euler1)*180/3.14
    #    euler1[0] += 180
        
    #    rospy.loginfo([euler1[0],euler2[0],euler1[0]+euler2[0]])

def pose_callback(msg):
    global path, path_pub
    path.poses += (msg,)
    path_pub.publish(path)


if __name__ == '__main__':
    rospy.init_node('mavros_vins_tf_publisher')
    sub_mav = rospy.Subscriber('/dji_sdk/local_pose', PoseStamped, mav_callback)
    sub_vio = rospy.Subscriber('/vins_estimator/odometry', Odometry, vio_callback)
    path_pub = rospy.Publisher('/local_path', Path, queue_size=10)

    path.header.stamp=rospy.Time.now()
    path.header.frame_id="local"

    while not init_mav_pose:
        pass
    sub_mav.unregister()
    sub_vio.unregister()
    # sub_mav = rospy.Subscriber('/dji_sdk/local_pose', PoseStamped, pose_callback)
   

    q = init_vio_pose.pose.orientation
    euler = tf.transformations.euler_from_quaternion([q.x, q.y, q.z, q.w],'szyx')
    yaw1 = -euler[0]+3.1415926535 # mynteye camera is installed upside down

    q = init_mav_pose.pose.orientation
    euler = tf.transformations.euler_from_quaternion([q.x, q.y, q.z, q.w],'szyx')
    yaw2 = euler[0]
    
    rospy.loginfo([yaw1, yaw2, yaw1-yaw2])


    tf2 = TransformStamped()
    tf2.header.stamp = rospy.Time.now()
    tf2.header.frame_id = 'local'
    tf2.child_frame_id = 'world'
    tf2.transform.translation = translation 
    quat = tf.transformations.quaternion_from_euler(yaw2-yaw1+4.0/180*math.pi,0,0,'szyx')
    #quat = tf.transformations.quaternion_from_euler(0,0,0,'szyx')
    tf2.transform.rotation.x = quat[0] 
    tf2.transform.rotation.y = quat[1] 
    tf2.transform.rotation.z = quat[2] 
    tf2.transform.rotation.w = quat[3] 
    rospy.loginfo(tf2)

    
    broadcaster = tf2_ros.StaticTransformBroadcaster()
    while True:
        broadcaster.sendTransform(tf2)
        rospy.sleep(0.1)   

