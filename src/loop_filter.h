//
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
//    ~Loop_filter();
    void update_tf(const tf::StampedTransform &tf);
    void get_tf(tf::StampedTransform &tf);

private:

    tf::Quaternion rotation_filtered, rotation_target;
    tf::Vector3 translation_filtered, translation_target;
    double last_update_time;
};

#endif //SRC_LOOP_FILTER_H
