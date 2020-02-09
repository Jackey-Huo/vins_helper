//
// A tf filter to make tf jump smoother
// Created by mhc on 2020/2/7.
//

#include "loop_filter.h"

Loop_filter::Loop_filter():rotation_filtered_(0,0,0,1),
                           rotation_target_(0,0,0,1),
                           translation_filtered_(0,0,0),
                           translation_target_(0,0,0){
    last_update_time_ = 0;
}

void Loop_filter::set_target_tf(const tf::StampedTransform &tf){
    rotation_target_ = tf.getRotation();
    translation_target_ = tf.getOrigin();
//    std::cout << "set_target__tf:" << translation_target_.getX() << " " << translation_filtered_.getX() << " "<<std::endl;
}

void Loop_filter::get_tf(tf::StampedTransform &tf)
{
    double now = ros::Time::now().toSec();

    // if this is the first pose request, let the filtered_ = target_
    if(last_update_time_ == 0){
        rotation_filtered_ = rotation_target_;
        translation_filtered_ = translation_target_;
        last_update_time_ = now;
    }

    // calculate time since last update
    double dt = now - last_update_time_;
    last_update_time_ = now;
//    std::cout << "dt:" << dt << std::endl;

    // perform a low-pass filter
    translation_filtered_ += (translation_target_ - translation_filtered_) * dt / 5.0; // todo: a general filter form
    // todo: rotation fliter

    tf.setOrigin(translation_filtered_);
    tf.setRotation(rotation_filtered_);
    tf.child_frame_id_ = "loop_filtered_";
    tf.stamp_ = ros::Time::now();// todo use original time in tf
    tf.frame_id_ = "world";
//    std::cout << "get_tf:" << translation_target_.getX() << " " << translation_filtered_.getX() << " "<<std::endl;
}
