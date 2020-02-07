#!/bin/bash

function show_help(){
    echo 'Usage: path_to_run.sh -c config_file_path [-b] [-n]'
    echo 'Args:'
    echo '    -b  rosbag mode, will not run mynteye sdk and dji sdk'
    echo '    -n  do not run visualization nodes'
}


#### Deal with parameters
bag_mode=false
run_rviz=true

while getopts "c:bn" arg
do
    case $arg in
    c)
	config_path=$OPTARG
            ;;
    b)
	bag_mode=true
            ;;
    n)
        run_rviz=false
            ;;
    ?) 
        show_help
        exit 1
        ;;
    esac
done

echo "config file path: $config_path"
echo "rosbag mode: $bag_mode"
echo "run rviz: $run_rviz"


#### Start tmux and spilt panes
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
tmux split-window -h


#### run nodes
tmux send -t 1 "roscore" Enter
sleep 2

if $bag_mode
then # rosbag mode
    rosparam set /use_sim_time true
    tmux send -t 3 "rosrun vins_helper dji_sdk_tf_publisher.py" Enter
    tmux send -t 4 "rosrun vins_helper pose_to_path_node" Enter
    tmux send -t 5 "rosrun vins vins_node ${config_path} use_sim_time:=true" Enter
    tmux send -t 6 "rosrun loop_fusion loop_fusion_node ${config_path} use_sim_time:=true" Enter
    #tmux send -t 0 "rosbag "
    tmux send -t 0 "rosbag play ~/Desktop/bags/2020-01-21-18-48-25_shanyang_04_circle_sunset.bag --clock -r 1.5 -u 126.49"
else # realtime mode
    tmux send -t 4 "roslaunch mynt_eye_ros_wrapper vins_fusion.launch" Enter
    tmux send -t 5 "rosrun vins vins_node ${config_path}" Enter
    tmux send -t 6 "rosrun loop_fusion loop_fusion_node ${config_path}" Enter
    # TODO: add dji_sdk and adis16448 support
    # tmux send -t 4 "rosrun adis16448_stm32_bridge adis16448_stm32_bridge_node" Enter 
fi

# TODO: add data record mode

if $run_rviz
then
    tmux send -t 2 "roslaunch vins_helper vins_rviz.launch" Enter
fi

tmux select-window -t 0
tmux select-pane -t 0
tmux -2 attach-session -d
tmux a -t "vins"

