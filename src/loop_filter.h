//
// A tf filter to make tf jump smoother
// Created by mhc on 2020/2/7.
//

#ifndef SRC_LOOP_FILTER_H
#define SRC_LOOP_FILTER_H

#include <mutex>
#include <eigen3/Eigen/Core>
#include <tf/transform_broadcaster.h>
#include <tf_conversions/tf_eigen.h>

class Loop_filter{
public:
    Loop_filter();
    void set_target_tf(const tf::StampedTransform &tf);
    void get_tf(tf::StampedTransform &tf);


private:
    tf::Quaternion  rotation_filtered_;
    tf::Quaternion  rotation_target_;
    tf::Vector3     translation_filtered_;
    tf::Vector3     translation_target_;

    double          last_update_time_;
};

#endif //SRC_LOOP_FILTER_H
