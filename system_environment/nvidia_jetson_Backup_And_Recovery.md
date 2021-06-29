https://www.jianshu.com/p/0f00f6d0015e

## 0、部署和启动ssh

在首先确保主机与nvidia板载系统上已安装ssh-server

```bash
sudo apt-get install ssh
sudo ps -e |grep ssh
若存在ssh和sshd输出，则说明已启动ssh服务
否则手动启动ssh服务
sudo service ssh start
```

## 1、互通主从机

1. 将主机和nvidia从机均加入同一局域网（同一路由器网络下）

2. 用 nvidia自带原装 type-c—usb的线将xavier电源灯右侧的接口与host主机的usb接口相连

3. 通过 `ssh username@ip`　指令互相访问测试，通过后可进行下一步，主从机ip地址可通过系统网络连接或者登陆路由器控制页面查看，命令示例如下

   ```bash
   silencht@silencht-pc:~$ ssh silencht@192.168.1.104 #注释：该ip为nvidia板子网络ip
   silencht@silencht-pc:~$ ……Are you sure you want to……（Yes/no）? 输入Yes并按回车
   silencht@silencht-nvidia:~$ #注释：注意此时口令前面用户名已经发生变化，登陆成功！
   silencht@silencht-nvidia:~$ ssh silencht@192.168.1.107 #注释：该ip为主机网络ip
   silencht@silencht-nvidia:~$ ……Are you sure you want to……（Yes/no）?输入Yes并按回车
   silencht@silencht-pc:~$ #注释：注意此时口令前面用户名已经变为pc，登陆成功！
   ```

## 2、导出镜像

- 使用主机利用ssh登陆nvidia系统，然后将系统设置为只读并导出镜像

  ```bash
  silencht@silencht-pc:~$ ssh silencht@192.168.1.104 #注释：该ip为nvidia板子网络ip
  silencht@silencht-nvidia:~$ echo u > /proc/sysrq-trigger 
  silencht@silencht-nvidia:~$ dd if=/dev/mmcblk0p1 | ssh silencht@192.168.1.107 dd of=/home/silencht/xavier-image.raw
  # wait long time and output infomation below:
  记录了58720256+0 的读入
  记录了 xxxx      的写出
  30064771072 bytes（30GB,28GiB）copied，2574.2s，11.7 MB/s
  ```

  注意：下方步骤暂未经过实验

  ```bash
  silencht@silencht-pc:~$ cd /home/silencht/nvidia/nvidia_sdk/JetPack_4.5.1_Linux_JETSON_AGX_XAVIER_TARGETS/Linux_for_Tegra/bootloader/ 
  silencht@silencht-pc:~$ sudo ./mksparse -v --fillpattern=0 /home/silencht/xavier-image.raw system1.img
  ```



## 3、恢复镜像

```bash
silencht@silencht-pc:~$ cd /home/silencht/nvidia/nvidia_sdk/JetPack_4.5.1_Linux_JETSON_AGX_XAVIER_TARGETS/Linux_for_Tegra/bootloader/ 
#对目录下原始system.img进行备份,同时对system1.img重命名为system.img
silencht@silencht-pc:~$ mv system.img /home/silencht/nvidia/nvidia_sdk/JetPack_4.5.1_Linux_JETSON_AGX_XAVIER_TARGETS/Linux_for_Tegra/bootloader/system_backup.img
silencht@silencht-pc:~$ mv system1.img system.img
silencht@silencht-pc:~$ mv system.img /home/silencht/nvidia/nvidia_sdk/JetPack_4.5.1_Linux_JETSON_AGX_XAVIER_TARGETS/Linux_for_Tegra/bootloader/system.img
```

- 进入recovery模式方法
  1.xavier关机
  2.按住recovery 按钮
  3.按住电源按钮
  4.同时松开
  将xavier切换到recovery模式,执行命令恢复镜像系统

```bash
silencht@silencht-pc:~$ cd  /home/silencht/nvidia/nvidia_sdk/JetPack_4.5.1_Linux_JETSON_AGX_XAVIER_TARGETS/Linux_for_Tegra
silencht@silencht-pc:~$ sudo ./flash.sh -r jetson-xavier mmcblk0p1
```

