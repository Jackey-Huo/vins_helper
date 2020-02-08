//
// A tf filter to make tf jump smoother
// Created by mhc on 2020/2/7.
//

#include "loop_filter.h"

Loop_filter::Loop_filter():rotation_filtered(0,0,0,1),
                           rotation_target(0,0,0,1),
                           translation_filtered(0,0,0),
                           translation_target(0,0,0){
    last_update_time = 0;
}

void Loop_filter::set_target_tf(const tf::StampedTransform &tf){
    rotation_target = tf.getRotation();
    translation_target = tf.getOrigin();
//    std::cout << "set_target_tf:" << translation_target.getX() << " " << translation_filtered.getX() << " "<<std::endl;
}

void Loop_filter::get_tf(tf::StampedTransform &tf)
{
    double now = ros::Time::now().toSec();

    // if this is the first pose request, let the filtered = target
    if(last_update_time == 0){
        rotation_filtered = rotation_target;
        translation_filtered = translation_target;
        last_update_time = now;
    }

    // calculate time since last update
    double dt = now - last_update_time;
    last_update_time = now;
//    std::cout << "dt:" << dt << std::endl;

    // perform a low-pass filter
    translation_filtered += (translation_target - translation_filtered) * dt / 5.0; // todo: a general filter form
    // todo: rotation fliter

    tf.setOrigin(translation_filtered);
    tf.setRotation(rotation_filtered);
    tf.child_frame_id_ = "loop_filtered";
    tf.stamp_ = ros::Time::now();// todo use original time in tf
    tf.frame_id_ = "world";
//    std::cout << "get_tf:" << translation_target.getX() << " " << translation_filtered.getX() << " "<<std::endl;
}
