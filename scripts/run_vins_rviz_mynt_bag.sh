#!/bin/bash

cd
tmux new-session -s "vins" -d -n "local"


#creat panes in local-window
tmux split-window -v
tmux split-window -h
tmux select-pane -U
tmux split-window -h

tmux split-window -v
tmux select-pane -L
tmux split-window -v
tmux select-pane -U
tmux split-window -h

#send all commands to each pane
tmux send -t 0 "roscore" Enter
sleep 2
rosparam set /use_sim_time true
tmux send -t 1 "roslaunch vins vins_rviz.launch" Enter
tmux send -t 5 "rosrun vins vins_node /home/mhc/catkin_ws/src/VINS-Fusion/config/mynteye-1030-1/mynt_stereo_imu_config.yaml use_sim_time:=true" Enter
tmux send -t 6 "rosrun loop_fusion loop_fusion_node /home/mhc/catkin_ws/src/VINS-Fusion/config/mynteye-1030-1/mynt_stereo_imu_config.yaml use_sim_time:=true" Enter
tmux send -t 3 "rosrun vins dji_sdk_tf_publisher.py" Enter
tmux send -t 2 "rosrun vins_helper pose_to_path_node" Enter
sleep 1
tmux send -t 4 "zsh" Enter
tmux send -t 4 "rosbag "

#here,we should set the inital point and then set up the following node
tmux select-window -t 0
tmux select-pane -t 4
tmux -2 attach-session -d
tmux a -t "vins"
