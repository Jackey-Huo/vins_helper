some scripts and nodes to help run and display vins-fusion with wz's drone

## Hardware
dji-A3 flight controller with onboard sdk enabled
mynteye S1030 stereo camera
(Optional) ADIS16488 IMU with [wz's adapter and driver]:https://github.com/mhc994/ADIS16448Bridge 

## Usage
realtime: (not tested...)
```vins_helper/scripts/run.sh -c vins_helper/config/mynteye-1030-1/mynt_stereo_imu_config.yaml ```
with recorded rosbag:
```vins_helper/scripts/run.sh -c vins_helper/config/mynteye-1030-1/mynt_stereo_imu_config.yaml -b```

TODO: add data record mode
