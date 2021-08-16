## 一、问题描述：

### 1、环境描述

- 操作系统：ubuntu18.04.5 LTS （AMD64）
- 相关软件：ORB-SLAM2、ROS-melodic（OpenCV：3.2.0 installed）、OpenCV：3.4.15（手动编译，编译时CMake添加参数：-DWITH_GTK_2_X=ON）
- ROS使用包（与错误相关）：opencv_bridge、image_geometry
- 编译指令：catkin_make
- 注：实际这里的orbslam2是经过修改且加了很多其他功能的

### 2、命令行错误信息

```bash
/usr/bin/ld: warning: libopencv_calib3d.so.3.2, needed by /home/silencht/slam-nav-add/src/semantic_slam_nav_ros/lib/libslam_semantic_nav_ros.so, may conflict with libopencv_calib3d.so.3.4
/usr/bin/ld: warning: libopencv_features2d.so.3.2, needed by /home/silencht/slam-nav-add/src/semantic_slam_nav_ros/lib/libslam_semantic_nav_ros.so, may conflict with libopencv_features2d.so.3.4
/usr/bin/ld: warning: libopencv_highgui.so.3.2, needed by /home/silencht/slam-nav-add/src/semantic_slam_nav_ros/lib/libslam_semantic_nav_ros.so, may conflict with libopencv_highgui.so.3.4
/usr/bin/ld: warning: libopencv_core.so.3.2, needed by /home/silencht/slam-nav-add/src/semantic_slam_nav_ros/lib/libslam_semantic_nav_ros.so, may conflict with libopencv_core.so.3.4
/usr/bin/ld: warning: libopencv_ml.so.3.4, needed by /home/silencht/slam-nav-add/src/semantic_slam_nav_ros/lib/libslam_semantic_nav_ros.so, may conflict with libopencv_ml.so.3.2
/usr/bin/ld: warning: libopencv_imgcodecs.so.3.2, needed by /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.3.2.0, may conflict with libopencv_imgcodecs.so.3.4
```

```bash
(Display:1072): Gtk-ERROR **: 18:03:31.545: GTK+ 2.x symbols detected. Using GTK+ 2.x and GTK+ 3 in the same process is not supported
```

### 3、排查过程

该问题陆续困扰我很久，简要说一下正确解决路线：

#### 3.1、[stackoverflow相关问题](https://stackoverflow.com/questions/42534129/)

通过回答可得知，程序中有多个库用了不同版本的GTK库，因此造成了冲突。因此，首先要排查到底是哪些库的使用与GTK相关。

回答介绍了**一个查看动态库依赖的命令**：`ldd` （list dynamic dependencies）

**作用**：列出程序或动态库的依赖关系。

[**例如**](https://linuxtools-rst.readthedocs.io/zh_CN/latest/tool/ldd.html) 查看 test 程序运行所依赖的库:

```bash
/opt/app/todeav1/test$ldd test
libstdc++.so.6 => /usr/lib64/libstdc++.so.6 (0x00000039a7e00000)
libm.so.6 => /lib64/libm.so.6 (0x0000003996400000)
libgcc_s.so.1 => /lib64/libgcc_s.so.1 (0x00000039a5600000)
libc.so.6 => not found
#第一列：程序需要依赖什么库
#第二列: 程序所需库在系统中的路径位置
#第三列：即括号中的数，为库加载的开始地址
#如果需要某个库，但程序在系统找不到对应的库，会出现第5行的not found信息
```

#### 3.2、利用ldd命令排查

##### 1、GTK库

通过查阅[GTK官网](https://www.gtk.org/)得知，GTK 是一款跨平台的为创造图形化用户界面而设计的工具库。

> Offering a complete set of UI elements, GTK is suitable for projects ranging from small one-off tools to complete application suites.

##### 2、使用ldd命令排查库依赖

orbslam本身用的库：

orbslam2/lib/liborbslam2.so

orbslam2/Thirdparty/DBoW2/lib/libDBoW2.so

orbslam2/Thirdparty/g2o/lib/libg2o.so

opencv库

pangolin、eigen3等orbslam所需的库

ros功能包的相关库等。

**首先** 通过使用`ldd liborbslam2.so | grep gtk`命令发现 liborbslam2.so 确实链接了gtk2和gtk3两个版本的库，输出类似如下：

```bash
libgtk-x11-2.0.so.0 => /usr/lib/x86_64-linux-gnu/libgtk-x11-2.0.so.0（0x00*）
libgtk-3.so.0 => /usr/lib/x86_64-linux-gnu/libgtk-3.so.0（0x00*）
```

**缩小排查范围，分析**：因为GTK是UI工具库，因此较大可能是因为涉及到绘制界面、图形等相关的库依赖出现问题。因此初步推断是opencv、pangolin等库依赖出现问题。

简单排查pangolin库发现没有出现问题。

通过查看orbslam源码中system.h、viewer.h、mapdrawer.h等文件，发现引用了opencv_highgui.h等绘制ui的相关头文件。

定位liborbslam2.so依赖的opencv库，使用命令`ldd liborbslam2.so | grep opencv`，输出如下：

```bash
silencht@silencht:~/slam_ws/src/orbslam2/lib$ ldd libslam_semantic_nav_ros.so | grep libopencv
	libopencv_calib3d.so.3.2 => /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.3.2 (0x00007fce94e1c000)
	libopencv_core.so.3.2 => /usr/lib/x86_64-linux-gnu/libopencv_core.so.3.2 (0x00007fce949e1000)
	libopencv_features2d.so.3.2 => /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.3.2 (0x00007fce9472c000)
	libopencv_highgui.so.3.2 => /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.3.2 (0x00007fce94520000)
	libopencv_imgproc.so.3.2 => /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.3.2 (0x00007fce93fcf000)
	libopencv_video.so.3.2 => /usr/lib/x86_64-linux-gnu/libopencv_video.so.3.2 (0x00007fce93d6c000)
	libopencv_text.so.3.2 => /usr/lib/x86_64-linux-gnu/libopencv_text.so.3.2 (0x00007fce93a4d000)
	libopencv_core.so.3.4 => /usr/local/lib/libopencv_core.so.3.4 (0x00007fce8fef8000)
	libopencv_imgcodecs.so.3.4 => /usr/local/lib/libopencv_imgcodecs.so.3.4 (0x00007fce8fa9f000)
	libopencv_ml.so.3.4 => /usr/local/lib/libopencv_ml.so.3.4 (0x00007fce8f3e6000)
	libopencv_flann.so.3.4 => /usr/local/lib/libopencv_flann.so.3.4 (0x00007fce8d785000)
	libopencv_flann.so.3.2 => /usr/lib/x86_64-linux-gnu/libopencv_flann.so.3.2 (0x00007fce8c287000)
	libopencv_imgcodecs.so.3.2 => /usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so.3.2 (0x00007fce8a13f000)
	libopencv_ml.so.3.2 => /usr/lib/x86_64-linux-gnu/libopencv_ml.so.3.2 (0x00007fce896df000)
	libopencv_imgproc.so.3.4 => /usr/local/lib/libopencv_imgproc.so.3.4 (0x00007fce82950000)
```

发现一个问题，由于ROS系统安装的时候自带了OpenCV 3.2.0 （lib文件安装位置位于 /usr/lib/x86_64-linux-gnu/），而我自用的OpenCV 3.4.15 （安装在/usr/local/lib/）。**在编译链接orbslam工程的时候，由于不恰当的配置造成了一部分文件引用了OpenCV 3.2.0，而一部分文件引用了OpenCV 3.4.15** 。

那么查看3.2.0版本的 libopencv_highgui.so 依赖（实际上为了稳妥起见，我将所有依赖的OpenCV库文件都排查过一遍），

```bash
cd /usr/lib/x86_64-linux-gnu/
ldd libopencv_highgui.so.3.2 | grep gtk
```

从输出可发现，确实是3.2.0的libopencv_highgui.so文件依赖了gtk3版本库，而其他文件均依赖gtk2版本库。在绘制界面时，gtk2和3版本同时调用，产生了冲突。

问题转到：**查找ROS自带 OpenCV 库与 自行安装 OpenCV 冲突问题**。

[单纯](https://blog.csdn.net/qq_34570910/article/details/78245795) 修改/opt/ros/kinetic/share/cv_bridge/cmake下*Config.cmake文件版本库，或者更改CmakeLists.txt中的 find_package() 并不能完全解决问题。

经排查，是编译orbslam工程的CmakeLists.txt中使用的ros功能包 opencv_bridge 和 image_geometry 的库依赖了OpenCV 3.2.0

------

### 4、解决办法

懒得详细写了。俩方法：

#### 4.1、单独卸载这俩ROS功能包

去github下载相应的包，放到ros工作空间的src文件夹下，配置他们的CmakeLists.txt文件中相应的编译路径，使他们编译的时候使用自己安装的 OpenCV 3.4.15 库。

#### 4.2、直接卸载ros自带的OpenCV（[我使用的](https://luyanan.com/news/info/20091648620111)）

**简述**：

卸载该 OpenCV（即3.2.0版本） 会使得所有依赖此OpenCV 的ROS功能包全部卸载。卸载完成后，去github下载工程所需要的缺失的功能包（opencv_bridge 和 image_geometry），放到ros工作空间的src文件夹下，配置他们的CmakeLists.txt文件，使他们编译的时候使用自己安装的 OpenCV 库。

**步骤**：

##### 1、使用 `dpkg --get-selections | grep opencv` 指令查找所有opencv相关包

输出可能如下所示：

```bash
libopencv-calib3d-dev:amd64			install
libopencv-calib3d3.2:amd64			install
libopencv-contrib-dev:amd64			install
libopencv-contrib3.2:amd64			install
libopencv-core-dev:amd64			install
libopencv-core3.2:amd64				install
libopencv-dev					install
libopencv-features2d-dev:amd64			install
libopencv-features2d3.2:amd64			install
libopencv-flann-dev:amd64			install
libopencv-flann3.2:amd64			install
libopencv-highgui-dev:amd64			install
libopencv-highgui3.2:amd64			install
libopencv-imgcodecs-dev:amd64			install
libopencv-imgcodecs3.2:amd64			install
libopencv-imgproc-dev:amd64			install
libopencv-imgproc3.2:amd64			install
libopencv-ml-dev:amd64				install
libopencv-ml3.2:amd64				install
libopencv-objdetect-dev:amd64			install
libopencv-objdetect3.2:amd64			install
libopencv-photo-dev:amd64			install
libopencv-photo3.2:amd64			install
libopencv-shape-dev:amd64			install
libopencv-shape3.2:amd64			install
libopencv-stitching-dev:amd64			install
libopencv-stitching3.2:amd64			install
libopencv-superres-dev:amd64			install
libopencv-superres3.2:amd64			install
libopencv-ts-dev:amd64				install
libopencv-video-dev:amd64			install
libopencv-video3.2:amd64			install
libopencv-videoio-dev:amd64			install
libopencv-videoio3.2:amd64			install
libopencv-videostab-dev:amd64			install
libopencv-videostab3.2:amd64			install
libopencv-viz-dev:amd64				install
libopencv-viz3.2:amd64				install
libopencv3.2-java				install
libopencv3.2-jni				install
python-opencv					install
ros-melodic-vision-opencv			install
```

##### 2、从输出中看到  libopencv-core3.2:amd64				install  

使用命令 `sudo apt-get remove libopencv-core3.2` 卸载opencv3.2.0，注意不要添加 purge 参数

##### 3、输完指令后会出现如下类似输出

```bash
下列软件包将被【卸载】：
  libopencv-dev ros-melodic-camera-calibration ros-melodic-compressed-depth-image-transport
…………………………………………略
 升级了 0 个软件包，新安装了 0 个软件包，要卸载 29 个软件包，有 204 个软件包未被升级。
解压缩后将会空出 23.7 MB 的空间。
您希望继续执行吗？ [Y/n]
```

仔细检查上述要卸载的软件包，若没问题输入Y，然后点回车。ROS里依赖OpenCV 的包全都会被卸载。

卸载完成后，新开一个终端，输入`roscore`，若成功运行，则基本无问题。

##### 4、下载ros对应功能包

网址：https://github.com/ros-perception/vision_opencv

将 opencv_bridge 和 image_geometry ，放到ros工作空间的src文件夹下。

使用`pkg-config --modversion opencv` 查看当前 OpenCV 版本，输出3.4.15则无问题。

```bash
$ pkg-config --modversion opencv
3.4.15
```



### 5、后续

删除ROS工作空间之前的build和devel文件，重新编译（catkin_make）整个工程。

### 6、附

一些相关问题网页链接：[1](https://www.cnblogs.com/cv-pr/p/5366764.html)    [2](https://github.com/raulmur/ORB_SLAM2/issues/118)    [3](https://github.com/raulmur/ORB_SLAM2/issues/103)    [4](https://blog.csdn.net/hansry/article/details/76764491)    [5](https://blog.csdn.net/bigdog_1027/article/details/79092263)

