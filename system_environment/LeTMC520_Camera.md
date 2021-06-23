

## 0、网址

https://orbbec3d.com/develop/

## 1、A ROS driver for Orbbec 3D cameras.

https://github.com/orbbec/ros_astra_camera

### 1.1 astra_camera

A ROS driver for Orbbec 3D cameras.

### 1.2 Install

This package supports ROS Kinetic and Melodic.

1. Install [ROS](http://wiki.ros.org/ROS/Installation).

2. Install dependences

   ```bash
   sudo apt install ros-$ROS_DISTRO-rgbd-launch ros-$ROS_DISTRO-libuvc ros-$ROS_DISTRO-libuvc-camera ros-$ROS_DISTRO-libuvc-ros
   ```

3. Create a [ROS Workspace](http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment)(if you don't have one，这里直接放入ORB_SLAM3/Examples/ROS/ORB_SLAM3/src下)

   ```bash
   mkdir -p ~/catkin_ws/src
   cd ~/catkin_ws/
   catkin_make
   source devel/setup.bash
   ```

4. Pull the repository into your ROS workspace

   ```
   cd ~/catkin_ws/src
   git clone https://github.com/orbbec/ros_astra_camera
   ```

5. 修改ATTR{idProduct}和 ATTR{idVendor}项值

   ```bash
   sudo service udev reload
   sudo service udev restart
   ```

6. 移动到ROS工作目录，编译源代码

   ```bash
   catkin_make --pkg astra_camera
   ```

   





### 1.3 Examples

#### Use Astra

```
roslaunch astra_camera astra.launch
```

#### Important Topics

- ```
  */image_raw
  ```

  : depth/rgb/ir raw images

  - If showing IR image is required, it would be more visible to normalize it from 16bit to 8bit (0 to 255)

- `*/image_rect_raw`: images rectified by intrinsic/extrinsic parameters

- `*/camera_info`: camera intrinsic/extrinsic parameters

- `/camera/depth/points`: point cloud without color information

- `/camera/depth_registered/points`: xyzrgb point cloud (Currently, RGB-D regestration supports default resolution only)

