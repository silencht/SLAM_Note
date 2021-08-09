## 零、参考链接

https://www.bilibili.com/video/BV1zt411G7Vn

http://wiki.ros.org/cn/ROS/Tutorials/

## 一、ROS安装

### 1.1 参考网址

http://wiki.ros.org/melodic/Installation/Ubuntu

### 1.2 安装步骤

#### 1.2.1 Setup your sources.list

软件源镜像版参考页面：http://wiki.ros.org/ROS/Installation/UbuntuMirrors

```bash
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
#下面这个是清华镜像版，推荐
sudo sh -c '. /etc/lsb-release && echo "deb http://mirrors.tuna.tsinghua.edu.cn/ros/ubuntu/ `lsb_release -cs` main" > /etc/apt/sources.list.d/ros-latest.list'
```

#### 1.2.2 Set up your keys

```bash
sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
```

#### 1.2.3 Installation ROS

- First, make sure your Debian package index is up-to-date:

```bash
sudo apt update
```

- **Desktop-Full Install: (Recommended)** : ROS, [rqt](http://wiki.ros.org/rqt), [rviz](http://wiki.ros.org/rviz), robot-generic libraries, 2D/3D simulators and 2D/3D perception

```bash
sudo apt install ros-melodic-desktop-full
```

#### 1.2.4 Environment setup

It's convenient if the ROS environment variables are automatically added to your bash session every time a new shell is launched:

```bash
echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

#### 1.2.5 Dependencies for building packages

Up to now you have installed what you need to run the core ROS packages. To create and manage your own ROS workspaces, there are various tools and requirements that are distributed separately. For example, [rosinstall](http://wiki.ros.org/rosinstall) is a frequently used command-line tool that enables you to easily download many source trees for ROS packages with one command.

To install this tool and other dependencies for building ROS packages, run:

```bash
sudo apt install python-rosinstall python-rosinstall-generator python-wstool build-essential
```

#### 1.2.6 Initialize rosdep（optional）

Before you can use many ROS tools, you will need to initialize `rosdep`. `rosdep` enables you to easily install system dependencies for source you want to compile and is required to run some core components in ROS. If you have not yet installed `rosdep`, do so as follows.

```bash
sudo apt install python-rosdep
```

With the following, you can initialize `rosdep`.

```bash
# 若因网络问题执行不通，利用rosdep_update.md文件解决
sudo rosdep init
rosdep update
```

### 1.3 测试安装

- 启动ROS Master，输出如下图

```bash
roscore
```

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210520172849724.png" alt="image-20210520172849724" style="zoom: 50%;" />

- 启动小海龟仿真器

```bash
rosrun turtlesim turtlesim_node
```

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210520173025132.png" alt="image-20210520173025132" style="zoom: 67%;" />

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210520173142478.png" alt="image-20210520173142478" style="zoom:50%;" />

- 启动海龟控制节点

```bash
rosrun turtlesim turtle_teleop_key
```

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210520173127843.png" alt="image-20210520173127843" style="zoom: 80%;" />

## 二、ROS概念

### 2.1 节点

#### 2.1.1 Node——执行单元

节点实际上只不过是ROS软件包中的一个可执行文件。ROS节点使用ROS[客户端库](http://wiki.ros.org/cn/Client Libraries)与其他节点通信。节点可以发布或订阅话题，也可以提供或使用[服务](http://wiki.ros.org/Services)。

- 执行具体任务的进程、独立运行的可执行文件
- 不同节点可使用不同编程语言，可分布式运行在不同主机
- 节点在系统中命名必须唯一

#### 2.1.2 ROS Master——控制中心

- 为节点提供命名和注册服务
- 跟踪和记录**话题/服务**通信，辅助节点相互查找、建立连接
- 提供参数服务器，节点使用此服务器存储和检索运行时的参数

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210520174556384.png" alt="image-20210520174556384" style="zoom: 67%;" />

### 2.2 通信

#### 2.2.1 话题通信

##### 1、 话题Topic——异步通信

- 节点间用来传输数据的重要总线
- 使用 **发布publish/订阅subscribe**模型，数据由发布者传输到订阅者，同一个话题的订阅者或发布者可以**不唯一**

##### 2、 消息Message——话题数据

- 具有一定的类型和数据结构，包括ROS提供的标准类型和用户自定义类型
- 使用编程语言无关的.msg文件定义，编译过程中产生对应的代码文件

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210521160350305.png" alt="image-20210521160350305" style="zoom:67%;" />

#### 2.2.2 服务通信

##### 服务Service——同步通信

- 使用**客户端/服务器**模型，客户端发送**请求**数据，服务器完成处理后返回**应答**数据
- 使用编程语言无关的.srv文件定义请求和应答数据结构，编译过程中生成对应的代码文件

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210521160849024.png" alt="image-20210521160849024" style="zoom:67%;" />

#### 2.2.3 话题 vs 服务

|          |     topic     |        service        |
| :------: | :-----------: | :-------------------: |
|  同步性  |     异步      |         同步          |
| 通信模型 |   发布/订阅   |     服务器/客户端     |
| 底层协议 | ROSTCP/ROSUDP |     ROSTCP/ROSUDP     |
| 反馈机制 |      无       |          有           |
|  缓冲区  |      有       |          无           |
|  实时性  |      弱       |          强           |
| 节点关系 |    多对多     | 一对多（一个service） |
| 适用场景 |   数据传输    |       逻辑处理        |

#### 2.2.4 参数

##### 参数Parameter——全局共享字典

- 可通过**网络**访问的共享、多变量字典
- 节点使用此服务器来存储和检索**运行时的参数**
- 适合存储**静态**、非二进制的配置参数，不适合存储动态配置的数据

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210521161720441.png" alt="image-20210521161720441" style="zoom: 50%;" />

### 2.3 文件系统

- 功能包Package

  ROS软件中的基本单元，包含节点源码、配置文件、数据定义等

- 清单Manifest

  对软件包的描述，记录功能包的基本信息：作者信息、许可信息、依赖选项、编译标志等

- 元功能包Meta Package 

  组织多个用于同一目的的功能包

  <img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210521162611135.png" alt="image-20210521162611135" style="zoom: 50%;" />

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210521162639077.png" alt="image-20210521162639077" style="zoom:50%;" />

### 2.4 主要命令

- 以海龟仿真器为例：

```bash
#运行,其中rosrun usage:rosrun <package> <executable>
roscore
rosrun turtlesim turtlesim_node
rosrun turtlesim turtle_teleop_key
#查看系统中运行的计算图
rtq_graph
#查看节点列表
rosnode list
#查看/turtlesim节点信息
rosnode info /turtlesim 
#查看话题列表
rostopic list
#发布/turtle1/cmd_vel话题关于坐标的消息，消息频率为10次/秒 (geo开始的内容使用tab补全)
rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist "linear:
  x:1.0
  y:0.0
  z:0.0
angular:
  x:0.0
  y:0.0
  z:0.0"
#查看话题的消息结构
rosmsg show geometry_msgs/Twist
#查看服务列表
rosservice list
#发布某服务请求,在（5.0,5.0）坐标位置产生一个初试角度为0的新海龟
rosservice call /spawn "x:5.0
y:5.0
theta:0.0
name:'turtle2'"
#话题记录,-all全部记录,-O压缩包
rosbag record -a -O cmd_record
#话题复现
rosbag play cmd_record.bag
```

```bash
#寻找软件包位置命令
rospack find [package_name]
#示例
rospack find roscpp
#输出
/opt/ros/<distro>/share/roscpp

#roscd是rosbash命令集的一部分，它允许你直接切换目录（cd）到某个软件包或者软件包集当中
roscd [locationname[/subdir]]
#示例
roscd roscpp
pwd
#输出
YOUR_INSTALL_PATH/share/roscpp
#roscd只能切换到那些路径已经包含在ROS_PACKAGE_PATH环境变量中的软件包
#如此种种，你也许已经注意到了ROS命令工具的的命名方式：
rospack = ros + pack(age)
roscd = ros + cd
rosls = ros + ls
```

## 三、话题应用

### 3.1 创建工作空间和功能包

- 工作目录结构

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210522161705472.png" alt="image-20210522161705472" style="zoom:80%;" />

工作空间workspace：一个存放工程开发相关文件的文件夹。

工作空间下共有四个主要文件夹：

- src：代码空间，放功能包的源码等
- build：编译空间，编译过程中的中间二进制文件等
- devel：开发空间，开发过程中生成的可执行文件和库等，与install功能类似
- install：安装空间，开发结束后生成的可执行文件等

```bash
#创建一个名为catkin_ws(自己命名)的工作空间，参数-p 确保目录名称存在，不存在的就新建一个。
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace  #将该目录初始化为ros工作空间
#编译工作空间，你可以认为catkin_make是在标准CMake工作流程中依次调用了cmake和make
cd ~/catkin_ws/
catkin_make
#编译安装命令
catkin_make install

#创建功能包：catkin_create_pkg <package_name> [depend1][depend2]···
#功能包需要用到c++、python语言所以需要roscpp和rospy依赖，std_msgs是ros官方提供的标准消息结构
cd ~/catkin_ws/src
catkin_create_pkg test_pkg std_msgs rospy roscpp
#编译构建功能包
cd ~/catkin_ws
catkin_make
#设置工作空间的环境变量
source ~/catkin_ws/devel/setup.bash
#检查环境变量
echo $ROS_PACKAGE_PATH
#输出示例
/home/<username>/catkin_ws/src:/opt/ros/<distro>/share

#注：ROS_PACKAGE_PATH环境变量应该包含那些保存有ROS软件包的路径，并且每个路径之间用冒号（:）分隔开来
export ROS_PACKAGE_PATH=/home/user/ros/ros-pkg:/another/path
#注：在 shell 中执行程序时，shell 会提供一组环境变量。export 可新增，修改或删除环境变量，供后续执行的程序使用。export 的效力仅限于该次登陆操作。若要每次启动可用，可将之加入~/.bashrc文件
```

### 3.2 Publisher实现

```bash
#创建名为learning_topic的功能包
cd ~/catkin_ws/src
catkin_create_pkg learning_topic std_msgs rospy roscpp geometry_msgs turtlesim
```

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210522151830922.png" alt="image-20210522151830922" style="zoom: 80%;" />

#### 3.2.1 C++版

```c++
// ~/catkin_ws/src/learning_topic/src/velocity_publisher.cpp代码如下
/*
 * 该例程将发布turtle1/cmd_vel话题，消息类型geometry_msgs::Twist
 */
 
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

int main(int argc, char **argv)
{
	// ROS节点初始化
	ros::init(argc, argv, "velocity_publisher");

	// 创建节点句柄
	ros::NodeHandle n;

	// 创建一个Publisher对象，发布名为/turtle1/cmd_vel的topic，消息类型为geometry_msgs::Twist，队列长度10
	ros::Publisher turtle_vel_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);

	// 设置循环的频率，类似2.4节中的 -r 10
	ros::Rate loop_rate(10);

	int count = 0;
	while (ros::ok())
	{
	    // 初始化geometry_msgs::Twist类型的消息
		geometry_msgs::Twist vel_msg;
		vel_msg.linear.x = 0.5;
		vel_msg.angular.z = 0.2;

	    // 发布消息
		turtle_vel_pub.publish(vel_msg);
		ROS_INFO("Publsh turtle velocity command[%0.2f m/s, %0.2f rad/s]", 
				vel_msg.linear.x, vel_msg.angular.z);

	    // 按照循环频率延时
	    loop_rate.sleep();
	}

	return 0;
}

```

```cmake
# ~/catkin_ws/src/learning_topic/src/CMakeLists.txt 加入下面的编译选项内容
add_executable(velocity_publisher src/velocity_publisher.cpp)
target_link_libraries(velocity_publisher ${catkin_LIBRARIES})
```

```bash
#进入working space根目录编译，并运行publisher
cd ~/catkin_ws
catkin_make
source devel/setup.bash #不想每次设置环境变量，可以直接写入 ~/.bashrc的最后一行
roscore
rosrun turtlesim turtlesim_node
rosrun learning_topic velocity_publisher
```

#### 3.2.2 python版

~/catkin_ws/src/learning_topic/scripts/velocity_publisher.py代码如下

第三行代码的python解释器由于版本问题可能要写为python3

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 该例程将发布turtle1/cmd_vel话题，消息类型geometry_msgs::Twist
import rospy
from geometry_msgs.msg import Twist

def velocity_publisher():
	# ROS节点初始化
    rospy.init_node('velocity_publisher', anonymous=True)

	# 创建一个Publisher，发布名为/turtle1/cmd_vel的topic，消息类型为geometry_msgs::Twist，队列长度10
    turtle_vel_pub = rospy.Publisher('/turtle1/cmd_vel', Twist, queue_size=10)

	#设置循环的频率
    rate = rospy.Rate(10) 

    while not rospy.is_shutdown():
		# 初始化geometry_msgs::Twist类型的消息
        vel_msg = Twist()
        vel_msg.linear.x = 0.5
        vel_msg.angular.z = 0.2

		# 发布消息
        turtle_vel_pub.publish(vel_msg)
    	rospy.loginfo("Publsh turtle velocity command[%0.2f m/s, %0.2f rad/s]", 
				vel_msg.linear.x, vel_msg.angular.z)

		# 按照循环频率延时
        rate.sleep()

if __name__ == '__main__':
    try:
        velocity_publisher()
    except rospy.ROSInterruptException:
        pass
```

```bash
#给.py文件添加执行权限,编译过程等暂时存疑
cd ~/catkin_ws/src/learning_topic/scripts/
sudo chmod 777 velocity_publisher.py
source devel/setup.bash #不想每次设置环境变量，可以直接写入 ~/.bashrc的最后一行
roscore
rosrun turtlesim turtlesim_node
rosrun learning_topic velocity_publisher.py
```

### 3.3 Subscriber实现

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210522151914764.png" alt="image-20210522151914764" style="zoom:80%;" />

#### 3.3.1 C++版

```c++
// ~/catkin_ws/src/learning_topic/src/pose_subscriber.cpp代码如下
/*
 * 该例程将订阅/turtle1/pose话题，消息类型turtlesim::Pose
 */
 
#include <ros/ros.h>
#include "turtlesim/Pose.h"

// 接收到订阅的消息后，会进入消息回调函数
void poseCallback(const turtlesim::Pose::ConstPtr& msg)
{
    // 将接收到的消息打印出来
    ROS_INFO("Turtle pose: x:%0.6f, y:%0.6f", msg->x, msg->y);
}

int main(int argc, char **argv)
{
    // 初始化ROS节点
    ros::init(argc, argv, "pose_subscriber");

    // 创建节点句柄
    ros::NodeHandle n;

    // 创建一个Subscriber，订阅名为/turtle1/pose的topic，注册回调函数poseCallback
    ros::Subscriber pose_sub = n.subscribe("/turtle1/pose", 10, poseCallback);

    // 循环等待回调函数
    ros::spin();

    return 0;
}

```

```cmake
# ~/catkin_ws/src/learning_topic/src/CMakeLists.txt 加入下面的编译选项内容
add_executable(pose_subscriber src/pose_subscriber.cpp)
target_link_libraries(pose_subscriber ${catkin_LIBRARIES})
```

```bash
#进入working space根目录编译，并运行subscriber
cd ~/catkin_ws
catkin_make
source devel/setup.bash #不想每次设置环境变量，可以直接写入 ~/.bashrc的最后一行
roscore
rosrun turtlesim turtlesim_node
rosrun learning_topic velocity_publisher
rosrun learning_topic pose_subscriber #开启订阅者
```

#### 3.3.2 python版

~/catkin_ws/src/learning_topic/scripts/pose_subscriber.py代码如下

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 该例程将订阅/turtle1/pose话题，消息类型turtlesim::Pose

import rospy
from turtlesim.msg import Pose

def poseCallback(msg):
    rospy.loginfo("Turtle pose: x:%0.6f, y:%0.6f", msg.x, msg.y)

def pose_subscriber():
	# ROS节点初始化
    rospy.init_node('pose_subscriber', anonymous=True)

	# 创建一个Subscriber，订阅名为/turtle1/pose的topic，注册回调函数poseCallback
    rospy.Subscriber("/turtle1/pose", Pose, poseCallback)

	# 循环等待回调函数
    rospy.spin()

if __name__ == '__main__':
    pose_subscriber()
```

```bash
#给.py文件添加执行权限,编译过程等暂时存疑
cd ~/catkin_ws/src/learning_topic/scripts/
sudo chmod 755 pose_subscriber.py
source devel/setup.bash #不想每次设置环境变量，可以直接写入 ~/.bashrc的最后一行
roscore
rosrun turtlesim turtlesim_node
rosrun turtlesim turtle_teleop_key
rosrun learning_topic pose_subscriber.py
```

### 3.4 话题消息自定义



<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210522160821468.png" alt="image-20210522160821468" style="zoom:80%;" />

#### 3.4.1 定义msg文件

```bash
cd ~/catkin_ws/src/learning_topic/
mkdir msg
cd msg
touch Person.msg
#在Person.msg文件中输入以下内容并保存：
string name
uint8  age
uint8  sex

uint8 unknown = 0
uint8 male    = 1
uint8 female  = 2
```

#### 3.4.2 在package.xml添加功能包依赖

```bash
cd ~/catkin_ws/src/learning_topic/
#打开package.xml文件，在文件下方依赖位置，输入以下内容并保存：
<build_depend>message_generation</build_depend>
<exec_depend>message_runtime</exec_depend>
```

#### 3.4.3 在CMakeLists.txt添加编译内容

```cmake
cd ~/catkin_ws/src/learning_topic/
#打开CMakeLists.txt，在适当位置，输入以下内容并保存：
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  rospy
  std_msgs
  turtlesim
  message_generation
)
add_message_files(
  FILES
  Person.msg
)
generate_messages(
  DEPENDENCIES
  std_msgs
)
catkin_package(CATKIN_DEPENDS 
  geometry_msgs roscpp rospy std_msgs turtlesim message_runtime
)
```

#### 3.4.4 编译生成语言相关文件

```bash
cd ~/catkin_ws
catkin_make
#编译结果：路径~/catkin_ws/devel/include/learning_topic下生成Person.h头文件
```

#### 3.4.5 自定义话题消息的使用

##### 1、publisher（c++）

```c++
// ~/catkin_ws/src/learning_topic/src/person_publisher.cpp代码如下
/*
 * 该例程将发布/person_info话题，自定义消息类型learning_topic::Person
 */
 
#include <ros/ros.h>
#include "learning_topic/Person.h"

int main(int argc, char **argv)
{
    // ROS节点初始化
    ros::init(argc, argv, "person_publisher");

    // 创建节点句柄
    ros::NodeHandle n;

    // 创建一个Publisher，发布名为/person_info的topic，消息类型为learning_topic::Person，队列长度10
    ros::Publisher person_info_pub = n.advertise<learning_topic::Person>("/person_info", 10);

    // 设置循环的频率
    ros::Rate loop_rate(1);

    int count = 0;
    while (ros::ok())
    {
        // 初始化learning_topic::Person类型的消息
    	learning_topic::Person person_msg;
		person_msg.name = "Tom";
		person_msg.age  = 18;
		person_msg.sex  = learning_topic::Person::male;

        // 发布消息
		person_info_pub.publish(person_msg);

       	ROS_INFO("Publish Person Info: name:%s  age:%d  sex:%d", 
				  person_msg.name.c_str(), person_msg.age, person_msg.sex);

        // 按照循环频率延时
        loop_rate.sleep();
    }

    return 0;
}
```

##### 2、subscriber（c++）

```c++
// ~/catkin_ws/src/learning_topic/src/person_subscriber.cpp代码如下
/*
 * 该例程将订阅/person_info话题，自定义消息类型learning_topic::Person
 */
 
#include <ros/ros.h>
#include "learning_topic/Person.h"

// 接收到订阅的消息后，会进入消息回调函数
void personInfoCallback(const learning_topic::Person::ConstPtr& msg)
{
    // 将接收到的消息打印出来
    ROS_INFO("Subcribe Person Info: name:%s  age:%d  sex:%d", 
			 msg->name.c_str(), msg->age, msg->sex);
}

int main(int argc, char **argv)
{
    // 初始化ROS节点
    ros::init(argc, argv, "person_subscriber");

    // 创建节点句柄
    ros::NodeHandle n;

    // 创建一个Subscriber，订阅名为/person_info的topic，注册回调函数personInfoCallback
    ros::Subscriber person_info_sub = n.subscribe("/person_info", 10, personInfoCallback);

    // 循环等待回调函数
    ros::spin();

    return 0;
}
```

##### 3、在CMakeLists.txt添加编译内容

```cmake
cd ~/catkin_ws/src/learning_topic/
#打开CMakeLists.txt，在适当位置，输入以下内容并保存：
add_executable(person_publisher src/person_publisher.cpp)
target_link_libraries(person_publisher ${catkin_LIBRARIES})
add_dependencies(person_publisher ${PROJECT_NAME}_generate_messages_cpp)

add_executable(person_subscriber src/person_subscriber.cpp)
target_link_libraries(person_subscriber ${catkin_LIBRARIES})
add_dependencies(person_subscriber ${PROJECT_NAME}_generate_messages_cpp)
```

##### 4、编译与运行

```bash
cd ~/catkin_ws
catkin_make
roscore
rosrun learning_topic person_subscriber
rosrun learning_topic person_publisher
#一旦pub和sub建立连接,就算关闭roscore,也不会影响两者的连接
#因为roscore只负责两者前期的“牵手",相当于一个”婚介所“,一旦连接之后,就与roscore就无关了
```

##### 5、publisher（python）

~/catkin_ws/src/learning_topic/scripts/person_publisher.py

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 该例程将发布/person_info话题，自定义消息类型learning_topic::Person

import rospy
from learning_topic.msg import Person

def velocity_publisher():
	# ROS节点初始化
    rospy.init_node('person_publisher', anonymous=True)

	# 创建一个Publisher，发布名为/person_info的topic，消息类型为learning_topic::Person，队列长度10
    person_info_pub = rospy.Publisher('/person_info', Person, queue_size=10)

	#设置循环的频率
    rate = rospy.Rate(10) 

    while not rospy.is_shutdown():
		# 初始化learning_topic::Person类型的消息
    	person_msg = Person()
    	person_msg.name = "Tom";
    	person_msg.age  = 18;
    	person_msg.sex  = Person.male;

		# 发布消息
        person_info_pub.publish(person_msg)
    	rospy.loginfo("Publsh person message[%s, %d, %d]", 
				person_msg.name, person_msg.age, person_msg.sex)

		# 按照循环频率延时
        rate.sleep()

if __name__ == '__main__':
    try:
        velocity_publisher()
    except rospy.ROSInterruptException:
        pass
```

##### 6、subscriber（python）

~/catkin_ws/src/learning_topic/scripts/person_subscriber.py

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 该例程将订阅/person_info话题，自定义消息类型learning_topic::Person

import rospy
from learning_topic.msg import Person

def personInfoCallback(msg):
    rospy.loginfo("Subcribe Person Info: name:%s  age:%d  sex:%d", 
			 msg.name, msg.age, msg.sex)

def person_subscriber():
	# ROS节点初始化
    rospy.init_node('person_subscriber', anonymous=True)

	# 创建一个Subscriber，订阅名为/person_info的topic，注册回调函数personInfoCallback
    rospy.Subscriber("/person_info", Person, personInfoCallback)

	# 循环等待回调函数
    rospy.spin()

if __name__ == '__main__':
    person_subscriber()
```

## 四、服务应用

### 4.1 创建功能包

```bash
cd ~/catkin_ws/src
catkin_create_pkg learning_service roscpp rospy std_msgs geometry_msgs turtlesim
```

### 4.2 Client实现

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210522174729345.png" alt="image-20210522174729345" style="zoom:80%;" />

#### 4.2.1 C++版

```c++
// ~/catkin_ws/src/learning_service/src/turtle_spawn.cpp代码如下
/*
 * 该例程将请求/spawn服务，服务数据类型turtlesim::Spawn
 */

#include <ros/ros.h>
#include <turtlesim/Spawn.h>

int main(int argc, char** argv)
{
    // ROS节点初始化
	ros::init(argc, argv, "turtle_spawn");

    // 创建节点句柄
	ros::NodeHandle node;

    // 发现/spawn服务后，创建一个服务客户端，连接名为/spawn的service
	ros::service::waitForService("/spawn"); //阻塞型函数
	ros::ServiceClient add_turtle = node.serviceClient<turtlesim::Spawn>("/spawn");

    // 初始化turtlesim::Spawn的请求数据
	turtlesim::Spawn srv;
	srv.request.x = 2.0;
	srv.request.y = 2.0;
	srv.request.name = "turtle2";

    // 请求服务调用
	ROS_INFO("Call service to spawn turtle[x:%0.6f, y:%0.6f, name:%s]", 
			 srv.request.x, srv.request.y, srv.request.name.c_str());

	add_turtle.call(srv);

	// 显示服务调用结果
	ROS_INFO("Spawn turtle successfully [name:%s]", srv.response.name.c_str());

	return 0;
};
```

```bash
# ~/catkin_ws/src/learning_service/src/CMakeLists.txt 加入下面的编译选项内容
add_executable(turtle_spawn src/turtle_spawn.cpp)
target_link_libraries(turtle_spawn ${catkin_LIBRARIES})
```

```bash
#进入working space根目录编译，生成可执行文件
cd ~/catkin_ws
catkin_make
source devel/setup.bash #不想每次设置环境变量，可以直接写入 ~/.bashrc的最后一行
roscore
rosrun turtlesim turtlesim_node
rosrun learning_service turtle_spawn
```

#### 4.2.2 python版

~/catkin_ws/src/learning_service/scripts/turtle_spawn.py内容如下（注意添加执行权限）

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 该例程将请求/spawn服务，服务数据类型turtlesim::Spawn

import sys
import rospy
from turtlesim.srv import Spawn

def turtle_spawn():
	# ROS节点初始化
    rospy.init_node('turtle_spawn')

	# 发现/spawn服务后，创建一个服务客户端，连接名为/spawn的service
    rospy.wait_for_service('/spawn')
    try:
        add_turtle = rospy.ServiceProxy('/spawn', Spawn)

		# 请求服务调用，输入请求数据
        response = add_turtle(2.0, 2.0, 0.0, "turtle2")
        return response.name
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

if __name__ == "__main__":
	#服务调用并显示调用结果
    print "Spwan turtle successfully [name:%s]" %(turtle_spawn())
```

### 4.3 Server实现

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210522174838657.png" alt="image-20210522174838657" style="zoom:80%;" />

#### 4.3.1 C++版

```c++
// ~/catkin_ws/src/learning_service/src/turtle_command_server.cpp代码如下
/*
 * 该例程将执行/turtle_command服务，服务数据类型std_srvs/Trigger
 */
 
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_srvs/Trigger.h>

ros::Publisher turtle_vel_pub;
bool pubCommand = false;

// service回调函数，输入参数req，输出参数res
bool commandCallback(std_srvs::Trigger::Request  &req,
         			std_srvs::Trigger::Response &res)
{
	pubCommand = !pubCommand;

    // 显示请求数据
    ROS_INFO("Publish turtle velocity command [%s]", pubCommand==true?"Yes":"No");

	// 设置反馈数据
	res.success = true;
	res.message = "Change turtle command state!";

    return true;
}

int main(int argc, char **argv)
{
    // ROS节点初始化
    ros::init(argc, argv, "turtle_command_server");

    // 创建节点句柄
    ros::NodeHandle n;

    // 创建一个名为/turtle_command的server，注册回调函数commandCallback
    ros::ServiceServer command_service = n.advertiseService("/turtle_command", commandCallback);

	// 创建一个Publisher，发布名为/turtle1/cmd_vel的topic，消息类型为geometry_msgs::Twist，队列长度10
	turtle_vel_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);

    // 循环等待回调函数
    ROS_INFO("Ready to receive turtle command.");

	// 设置循环的频率，类似2.4节中的 -r 10
	ros::Rate loop_rate(10);

	while(ros::ok())
	{
		// 查看一次回调函数队列
    	ros::spinOnce();
		
		// 如果标志为true，则发布速度指令
		if(pubCommand)
		{
			geometry_msgs::Twist vel_msg;
			vel_msg.linear.x = 0.5;
			vel_msg.angular.z = 0.2;
			turtle_vel_pub.publish(vel_msg);
		}

		//按照循环频率延时
	    loop_rate.sleep();
	}

    return 0;
}
```

```cmake
# ~/catkin_ws/src/learning_service/src/CMakeLists.txt 加入下面的编译选项内容
add_executable(turtle_command_server src/turtle_command_server.cpp)
target_link_libraries(turtle_command_server ${catkin_LIBRARIES})
```

```bash
#进入working space根目录编译，生成可执行文件
cd ~/catkin_ws
catkin_make
source devel/setup.bash #不想每次设置环境变量，可以直接写入 ~/.bashrc的最后一行
roscore
rosrun turtlesim turtlesim_node
rosrun learning_service turtle_command_server
rosservice call /turtle_command "{}"
```

#### 4.3.2 python版

~/catkin_ws/src/learning_service/scripts/turtle_command_server.py内容如下（注意添加执行权限）

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 该例程将执行/turtle_command服务，服务数据类型std_srvs/Trigger

import rospy
import thread,time
from geometry_msgs.msg import Twist
from std_srvs.srv import Trigger, TriggerResponse

pubCommand = False;
turtle_vel_pub = rospy.Publisher('/turtle1/cmd_vel', Twist, queue_size=10)

def command_thread():	
	while True:
		if pubCommand:
			vel_msg = Twist()
			vel_msg.linear.x = 0.5
			vel_msg.angular.z = 0.2
			turtle_vel_pub.publish(vel_msg)
			
		time.sleep(0.1)

def commandCallback(req):
	global pubCommand
	pubCommand = bool(1-pubCommand)

	# 显示请求数据
	rospy.loginfo("Publish turtle velocity command![%d]", pubCommand)

	# 反馈数据
	return TriggerResponse(1, "Change turtle command state!")

def turtle_command_server():
	# ROS节点初始化
    rospy.init_node('turtle_command_server')

	# 创建一个名为/turtle_command的server，注册回调函数commandCallback
    s = rospy.Service('/turtle_command', Trigger, commandCallback)

	# 循环等待回调函数
    print "Ready to receive turtle command."

    thread.start_new_thread(command_thread, ())
    rospy.spin()

if __name__ == "__main__":
    turtle_command_server()
```

### 4.4 服务数据自定义

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210522195616472.png" alt="image-20210522195616472" style="zoom:80%;" />

#### 4.4.1 定义srv文件

```bash
cd ~/catkin_ws/src/learning_service/
mkdir srv
cd srv
touch Person.srv
#在Person.srv文件中输入以下内容并保存：
#---上方是request数据，下方是response数据
string name
uint8  age
uint8  sex

uint8 unknown = 0
uint8 male    = 1
uint8 female  = 2

---
string result
```

#### 4.4.2 在package.xml添加功能包依赖

```bash
cd ~/catkin_ws/src/learning_service/
#打开package.xml文件，在文件下方依赖位置，输入以下内容并保存：
<build_depend>message_generation</build_depend>
<exec_depend>message_runtime</exec_depend>
```

#### 4.4.3 在CMakeLists.txt添加编译内容

```cmake
cd ~/catkin_ws/src/learning_service/
#打开CMakeLists.txt，在适当位置，输入以下内容并保存：
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  rospy
  std_msgs
  turtlesim
  message_generation
)
add_service_files(
  FILES
  Person.srv
)
generate_messages(
  DEPENDENCIES
  std_msgs
)
catkin_package(CATKIN_DEPENDS  
  geometry_msgs roscpp rospy std_msgs turtlesim message_runtime
)
```

#### 4.4.4 编译生成语言相关文件

```bash
cd ~/catkin_ws
catkin_make
#编译结果：路径~/catkin_ws/devel/include/learning_service下生成PersonRequest.h和PersonResponse.h头文件
```

#### 4.4.5 自定义服务数据的使用

##### 1、server（c++）

```c++
// ~/catkin_ws/src/learning_service/src/person_server.cpp代码如下
/*
 * 该例程将执行/show_person服务，服务数据类型learning_service::Person
 */
 
#include <ros/ros.h>
#include "learning_service/Person.h"

// service回调函数，输入参数req，输出参数res
bool personCallback(learning_service::Person::Request  &req,
         			learning_service::Person::Response &res)
{
    // 显示请求数据
    ROS_INFO("Person: name:%s  age:%d  sex:%d", req.name.c_str(), req.age, req.sex);

	// 设置反馈数据
	res.result = "OK";

    return true;
}

int main(int argc, char **argv)
{
    // ROS节点初始化
    ros::init(argc, argv, "person_server");

    // 创建节点句柄
    ros::NodeHandle n;

    // 创建一个名为/show_person的server，注册回调函数personCallback
    ros::ServiceServer person_service = n.advertiseService("/show_person", personCallback);

    // 循环等待回调函数
    ROS_INFO("Ready to show person informtion.");
    ros::spin();

    return 0;
}
```

##### 2、client（c++）

```c++
// ~/catkin_ws/src/learning_service/src/person_client.cpp代码如下
/*
 * 该例程将请求/show_person服务，服务数据类型learning_service::Person
 */

#include <ros/ros.h>
#include "learning_service/Person.h"

int main(int argc, char** argv)
{
    // 初始化ROS节点
	ros::init(argc, argv, "person_client");

    // 创建节点句柄
	ros::NodeHandle node;

    // 发现/spawn服务后，创建一个服务客户端，连接名为/spawn的service
	ros::service::waitForService("/show_person");
	ros::ServiceClient person_client = node.serviceClient<learning_service::Person>("/show_person");

    // 初始化learning_service::Person的请求数据
	learning_service::Person srv;
	srv.request.name = "Tom";
	srv.request.age  = 20;
	srv.request.sex  = learning_service::Person::Request::male;

    // 请求服务调用
	ROS_INFO("Call service to show person[name:%s, age:%d, sex:%d]", 
			 srv.request.name.c_str(), srv.request.age, srv.request.sex);

	person_client.call(srv);

	// 显示服务调用结果
	ROS_INFO("Show person result : %s", srv.response.result.c_str());

	return 0;
};
```

##### 3、在CMakeLists.txt添加编译内容

```cmake
cd ~/catkin_ws/src/learning_service/
#打开CMakeLists.txt，在适当位置，输入以下内容并保存：
add_executable(person_server src/person_server.cpp)
target_link_libraries(person_server ${catkin_LIBRARIES})
add_dependencies(person_server ${PROJECT_NAME}_gencpp)

add_executable(person_client src/person_client.cpp)
target_link_libraries(person_client ${catkin_LIBRARIES})
add_dependencies(person_client ${PROJECT_NAME}_gencpp)
```

##### 4、编译与运行

```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
roscore
rosrun learning_service person_server
rosrun learning_service person_client
```

##### 5、server（python）

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 该例程将执行/show_person服务，服务数据类型learning_service::Person

import rospy
from learning_service.srv import Person, PersonResponse

def personCallback(req):
	# 显示请求数据
    rospy.loginfo("Person: name:%s  age:%d  sex:%d", req.name, req.age, req.sex)

	# 反馈数据
    return PersonResponse("OK")

def person_server():
	# ROS节点初始化
    rospy.init_node('person_server')

	# 创建一个名为/show_person的server，注册回调函数personCallback
    s = rospy.Service('/show_person', Person, personCallback)

	# 循环等待回调函数
    print "Ready to show person informtion."
    rospy.spin()

if __name__ == "__main__":
    person_server()
```

##### 6、client（python）

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 该例程将请求/show_person服务，服务数据类型learning_service::Person

import sys
import rospy
from learning_service.srv import Person, PersonRequest

def person_client():
	# ROS节点初始化
    rospy.init_node('person_client')

	# 发现/spawn服务后，创建一个服务客户端，连接名为/spawn的service
    rospy.wait_for_service('/show_person')
    try:
        person_client = rospy.ServiceProxy('/show_person', Person)

		# 请求服务调用，输入请求数据
        response = person_client("Tom", 20, PersonRequest.male)
        return response.result
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

if __name__ == "__main__":
	#服务调用并显示调用结果
    print "Show person result : %s" %(person_client())
```

## 五、参数应用

<img src="https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210524175228081.png" alt="image-20210524175228081" style="zoom:80%;" />

### 5.1 创建功能包

```bash
cd ~/catkin_ws/src
catkin_create_pkg learning_parameter roscpp rospy std_srvs
```

### 5.2 参数命令行使用

```bash
#列出当前所有参数
rosparam list
#显示某个参数的值
rosparam get param_key
#设置某个参数的值
rosparam set param_key param_value
#保存参数到文件
rosparam dump file_name
#从文件读取参数
rosparam load file_name
#删除参数
rosparam delete param_key
```

### 5.3 实现

#### 5.3.1 C++版

```c++
// ~/catkin_ws/src/learning_parameter/src/parameter_config.cpp代码如下
/*
 * 该例程设置/读取海龟例程中的参数
 */
#include <string>
#include <ros/ros.h>
#include <std_srvs/Empty.h>

int main(int argc, char **argv)
{
	int red, green, blue;

    // ROS节点初始化
    ros::init(argc, argv, "parameter_config");

    // 创建节点句柄
    ros::NodeHandle node;

    // 读取背景颜色参数
	ros::param::get("/background_r", red);
	ros::param::get("/background_g", green);
	ros::param::get("/background_b", blue);

	ROS_INFO("Get Backgroud Color[%d, %d, %d]", red, green, blue);

	// 设置背景颜色参数
	ros::param::set("/background_r", 255);
	ros::param::set("/background_g", 255);
	ros::param::set("/background_b", 255);

	ROS_INFO("Set Backgroud Color[255, 255, 255]");

    // 读取背景颜色参数
	ros::param::get("/background_r", red);
	ros::param::get("/background_g", green);
	ros::param::get("/background_b", blue);

	ROS_INFO("Re-get Backgroud Color[%d, %d, %d]", red, green, blue);

	//发现/clear服务后，创建一个服务客户端，服务数据类型empty调用服务，刷新背景颜色
	ros::service::waitForService("/clear");
	ros::ServiceClient clear_background = node.serviceClient<std_srvs::Empty>("/clear");
	std_srvs::Empty srv;
	clear_background.call(srv);
	
	sleep(1);

    return 0;
}
```

```cmake
cd ~/catkin_ws/src/learning_parameter/
#打开CMakeLists.txt，在适当位置，输入以下内容并保存：
add_executable(parameter_config src/parameter_config.cpp)
target_link_libraries(parameter_config ${catkin_LIBRARIES})
```

```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
roscore
rosrun turtlesim turtlesim_node
rosrun learning_parameter parameter_config
```

#### 5.3.2 python版

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 该例程设置/读取海龟例程中的参数

import sys
import rospy
from std_srvs.srv import Empty

def parameter_config():
	# ROS节点初始化
    rospy.init_node('parameter_config', anonymous=True)

	# 读取背景颜色参数
    red   = rospy.get_param('/background_r')
    green = rospy.get_param('/background_g')
    blue  = rospy.get_param('/background_b')

    rospy.loginfo("Get Backgroud Color[%d, %d, %d]", red, green, blue)

	# 设置背景颜色参数
    rospy.set_param("/background_r", 255);
    rospy.set_param("/background_g", 255);
    rospy.set_param("/background_b", 255);

    rospy.loginfo("Set Backgroud Color[255, 255, 255]");

	# 读取背景颜色参数
    red   = rospy.get_param('/background_r')
    green = rospy.get_param('/background_g')
    blue  = rospy.get_param('/background_b')

    rospy.loginfo("Get Backgroud Color[%d, %d, %d]", red, green, blue)

	# 发现/spawn服务后，创建一个服务客户端，连接名为/spawn的service
    rospy.wait_for_service('/clear')
    try:
        clear_background = rospy.ServiceProxy('/clear', Empty)

		# 请求服务调用，输入请求数据
        response = clear_background()
        return response
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

if __name__ == "__main__":
    parameter_config()
```

## 六、tf坐标系

### 6.1 创建功能包

```bash
sudo apt-get install ros-melodic-turtle-tf
cd ~/catkin_ws/src
catkin_create_pkg learning_tf roscpp rospy tf turtlesim
```

### 6.2 实现

#### 6.2.1 broadcaster（c++）

```c++
// ~/catkin_ws/src/learning_tf/src/turtle_tf_broadcaster.cpp代码如下
/*
 * 该例程产生tf数据，并计算、发布turtle2的速度指令
 */

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>

std::string turtle_name;

void poseCallback(const turtlesim::PoseConstPtr& msg)
{
	// 创建tf的广播器
	static tf::TransformBroadcaster br;

	// 初始化tf数据
	tf::Transform transform;
	transform.setOrigin( tf::Vector3(msg->x, msg->y, 0.0) );
	tf::Quaternion q;
	q.setRPY(0, 0, msg->theta);
	transform.setRotation(q);

	// 广播world与海龟坐标系之间的tf数据
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", turtle_name));
}

int main(int argc, char** argv)
{
    // 初始化ROS节点
	ros::init(argc, argv, "my_tf_broadcaster");

	// 输入参数作为海龟的名字
	if (argc != 2)
	{
		ROS_ERROR("need turtle name as argument"); 
		return -1;
	}

	turtle_name = argv[1];

	// 订阅海龟的位姿话题
	ros::NodeHandle node;
	ros::Subscriber sub = node.subscribe(turtle_name+"/pose", 10, &poseCallback);

    // 循环等待回调函数
	ros::spin();

	return 0;
};
```

#### 6.2.2 listener（c++）

```c++
// ~/catkin_ws/src/learning_tf/src/turtle_tf_listener.cpp代码如下
/*
 * 该例程监听tf数据，并计算、发布turtle2的速度指令
 */

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>

int main(int argc, char** argv)
{
	// 初始化ROS节点
	ros::init(argc, argv, "my_tf_listener");

    // 创建节点句柄
	ros::NodeHandle node;

	// 请求产生turtle2
	ros::service::waitForService("/spawn");
	ros::ServiceClient add_turtle = node.serviceClient<turtlesim::Spawn>("/spawn");
	turtlesim::Spawn srv;
	add_turtle.call(srv);

	// 创建发布turtle2速度控制指令的发布者
	ros::Publisher turtle_vel = node.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel", 10);

	// 创建tf的监听器
	tf::TransformListener listener;

	ros::Rate rate(10.0);
	while (node.ok())
	{
		// 获取turtle1与turtle2坐标系之间的tf数据
		tf::StampedTransform transform;
		try
		{
			listener.waitForTransform("/turtle2", "/turtle1", ros::Time(0), ros::Duration(3.0));
			listener.lookupTransform("/turtle2", "/turtle1", ros::Time(0), transform);
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
			continue;
		}

		// 根据turtle1与turtle2坐标系之间的位置关系，发布turtle2的速度控制指令
		geometry_msgs::Twist vel_msg;
		vel_msg.angular.z = 4.0 * atan2(transform.getOrigin().y(),
				                        transform.getOrigin().x());
		vel_msg.linear.x = 0.5 * sqrt(pow(transform.getOrigin().x(), 2) +
				                      pow(transform.getOrigin().y(), 2));
		turtle_vel.publish(vel_msg);

		rate.sleep();
	}
	return 0;
};
```

#### 6.2.3 在CMakeLists.txt添加编译内容

```cmake
cd ~/catkin_ws/src/learning_tf/
#打开CMakeLists.txt，在适当位置，输入以下内容并保存：
add_executable(turtle_tf_broadcaster src/turtle_tf_broadcaster.cpp)
target_link_libraries(turtle_tf_broadcaster ${catkin_LIBRARIES})

add_executable(turtle_tf_listener src/turtle_tf_listener.cpp)
target_link_libraries(turtle_tf_listener ${catkin_LIBRARIES})
```

#### 6.2.4 编译与运行

```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
roscore
rosrun turtlesim turtlesim_node
#最后的/turtle1是main函数中的输入参数，前面(__name:=turtle1_tf_broadcaster)
#使用了节点名的重映射，即turtle1_tf_broadcaster会代替6.2.1节中节点初始化名
#my_tf_broadcaster，这就方便了相同程序同时（使用不同节点名）重复运行
rosrun learning_tf turtle_tf_broadcaster __name:=turtle1_tf_broadcaster /turtle1
rosrun learning_tf turtle_tf_broadcaster __name:=turtle2_tf_broadcaster /turtle2
rosrun learning_tf turtle_tf_listener
rosrun turtlesim turtle_teleop_key
```

#### 6.2.5 broadcaster（python）

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 该例程将请求/show_person服务，服务数据类型learning_service::Person

import roslib
roslib.load_manifest('learning_tf')
import rospy

import tf
import turtlesim.msg

def handle_turtle_pose(msg, turtlename):
    br = tf.TransformBroadcaster()
    br.sendTransform((msg.x, msg.y, 0),
                     tf.transformations.quaternion_from_euler(0, 0, msg.theta),
                     rospy.Time.now(),
                     turtlename,
                     "world")

if __name__ == '__main__':
    rospy.init_node('turtle_tf_broadcaster')
    turtlename = rospy.get_param('~turtle')
    rospy.Subscriber('/%s/pose' % turtlename,
                     turtlesim.msg.Pose,
                     handle_turtle_pose,
                     turtlename)
    rospy.spin()
```

#### 6.2.6 listener（python）

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 该例程将请求/show_person服务，服务数据类型learning_service::Person

import roslib
roslib.load_manifest('learning_tf')
import rospy
import math
import tf
import geometry_msgs.msg
import turtlesim.srv

if __name__ == '__main__':
    rospy.init_node('turtle_tf_listener')

    listener = tf.TransformListener()

    rospy.wait_for_service('spawn')
    spawner = rospy.ServiceProxy('spawn', turtlesim.srv.Spawn)
    spawner(4, 2, 0, 'turtle2')

    turtle_vel = rospy.Publisher('turtle2/cmd_vel', geometry_msgs.msg.Twist,queue_size=1)

    rate = rospy.Rate(10.0)
    while not rospy.is_shutdown():
        try:
            (trans,rot) = listener.lookupTransform('/turtle2', '/turtle1', rospy.Time(0))
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            continue

        angular = 4 * math.atan2(trans[1], trans[0])
        linear = 0.5 * math.sqrt(trans[0] ** 2 + trans[1] ** 2)
        cmd = geometry_msgs.msg.Twist()
        cmd.linear.x = linear
        cmd.angular.z = angular
        turtle_vel.publish(cmd)

        rate.sleep()
```

#### 6.2.7 运行

```bash
rosrun learning_tf turtle_tf_broadcaster.py __name:=turtle1_tf_broadcaster _turtle:=/turtle1
```

## 七、launch启动文件

#### 7.1 简单启动文件

```bash
cd ~/catkin_ws/src
catkin_create_pkg learning_launch
cd learning_launch
mkdir launch
cd launch
touch simple.launch
sudo vim simple.launch
#输入以下内容
```

```xml
<launch>
    <node pkg="learning_topic" type="person_subscriber" name="talker" />
    <node pkg="learning_topic" type="person_publisher" name="listener" /> 
</launch>
```

```bash
cd ~/catkin_ws
catkin_make
#命令：roslaunch [package] [filename.launch]
roslaunch learning_launch simple.launch
```

#### 7.2 官方参考网址

https://wiki.ros.org/roslaunch/XML

#### 7.3 常用标签注释

```xml
<!--launch文件中的根元素用该标签定义，只作为其他自标签容器-->
<launch> 
    <!--node启动节点;pkg:节点所在功能包名;type:可执行文件名称;name:重定义节点名
	【注：name会替换掉程序初始化时的节点名】
	【可选:output(日志输出到屏幕，若不设置，将写入日志文件)
		  respawn(节点失效是否重启)
		  required(节点失效关闭整个launch文件)
		  ns(在命名空间中运行此节点)
		  args(给节点输入参数)】
	模板： <node pkg="package_name" type="executable|python" name="node_name" />    
 	-->
	<node name="bar1" pkg="foo_pkg" type="bar" args="--test" respawn="true" output="sceen"/>
        
    <!--若干个节点划分进同一个命名空间 -->
    <group ns="namespace">   
    	<node pkg="pkg_name1" .../>
    	<node pkg="pkg_name2" .../>
    	...
    </group>
	<!--条件判断执行 -->
    <group if="$(arg foo1)">  
        <node pkg="pkg_name1" .../>
    </group> 
    <group unless="$(arg foo2)">
    	<node pkg="pkg_name2" .../>
	</group>
      
	<!--重命名，from 原始名称 to 新名称
	可以这样理解这个替换标签（存疑）：你有一个节点订阅了「chatter」话题，但是你只有一个节点发布 「hello」话题，而「hello」和「chatter」的类型相同，所以我们可以将「chatter」简单地替换为「hello」，从而实现订阅「hello」话题
 	-->        
    <remap from = "/different_topic" to = "/needed_topic" />  
     
    <!--定义节点所运行的机器信息，如果只在本地运行则不需要配置，它主要使用在SSH和远程机器
	【参数:name：机器名称
		  address：机器的网络地址
		  env-loader：设置机器的环境变量，必须是一个设置了所有要求变量的 shell 脚本
		  user：用户名称】 
	-->
    <machine name="foo" address="foo-address" env-loader="/opt/ros/kinetic/env.sh" user="someone">
    
    <!--可以在启动的节点上设置环境变量，这个标签基本只会使用在 launch、include、node、machine 这 4 个标签内部，当使用在 launch 内部时，env 设置的环境标量会应用到内部定义的节点。  
	【参数:name：环境变量名称
		  value：环境变量值	
	-->
    <env name="ENV_EXAMPLE" value="some value" />
        
    <!--设置全局参数，存储在参数服务器中:name(参数名)type(参数类型)value(参数值)
	param 标签也可以为一组 group 节点同时设置参数。-->
    <param name="publish_frequency" type="double" value="10.0"/>
        
    <!--允许节点从参数服务器上增删改YAML文件，也可以使用在远程机器上，需要注意的是 delete 必须在 load 或者 dump 之后进行导入之后进行
	【参数:command：load，dump，delete
		  file：参数文件的路径
	-->
    <rosparam file="params.yaml" command="load" ns="params"/>
        
    <!--设置launch文件内部的局部参数:name(参数名)value(参数值)-->
    <arg name="arg-name" default="arg-value"/>
    <!--两个调用arg的例子-->
    <param name="foo" value="$(arg arg-name)"/>
    <node name="node" pkg="package" type="type" args="$(arg arg-name)"/>
        
    <!--包含其他launch文件，类似C语言的头文件包含:file是路径-->
    <include file="$(dirname)/other.launch"/>
</launch>
```

## 八、可视化工具

```bash
#工具集合
rqt
#日志输出
rqt_console
#计算图可视化
rqt_graph
#数据绘图
rqt_plot
#图像渲染工具
rqt_image_view
#三维可视化工具（数据显示）
rviz
#三维物理仿真平台
roslaunch gazebo_ros + tab补全
```
