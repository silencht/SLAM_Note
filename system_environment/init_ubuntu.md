### 1、安装常用软件

- 搜狗输入法（[搜狗输入法官网](https://pinyin.sogou.com/linux/)）

  注：搜狗输入法不支持Nvidia开发板的ARM64架构，所以输入法参见[nvidia_jetson_agx_xavier文件](./nvidia_jetson_agx_xavier.md)

- typora（[typora官网](https://www.typora.io/)）

  注：typora不支持ARM64架构。所以Markdown阅读器参见[nvidia_jetson_agx_xavier文件](./nvidia_jetson_agx_xavier.md)

- visual studio code（[visual studio code官网](https://code.visualstudio.com/)）

  注：英伟达板子应该下载Linux，Arm64版本的

- chrome（[谷歌浏览器官网](https://www.google.com/chrome/)）

  注：英伟达板子自带chromium

### 2、换源

- 普通电脑换源

```bash
# 首先备份源列表
sudo cp /etc/apt/sources.list /etc/apt/sources.list_backup
# 打开sources.list文件
sudo gedit /etc/apt/sources.list
#编辑/etc/apt/sources.list文件, 删除原文内容，复制添加下列镜像源
# 清华源
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-updates main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-updates main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-backports main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-backports main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-security main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-security main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-proposed main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-proposed main restricted universe multiverse
#更新软件列表
sudo apt-get update
```

- NVIDIA Jetson AGX Xavier开发板ARM架构CPU系统换源

```bash
# 默认注释了源码镜像以提高 apt update 速度，如有需要可自行取消注释
deb [arch=arm64] https://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic main restricted universe multiverse
# deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic main restricted universe multiverse
deb [arch=arm64] https://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-updates main restricted universe multiverse
# deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-updates main restricted universe multiverse
deb [arch=arm64] https://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-backports main restricted universe multiverse
# deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-backports main restricted universe multiverse
deb [arch=arm64] https://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-security main restricted universe multiverse
# deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-security main restricted universe multiverse
```

一定要指明架构：[arch=arm64] ，不然 apt-get update 会陷入 amd等架构目录的获取。

### 3、安装基础工具

```bash
#安装C/C++等软件依赖
sudo apt-get install build-essential

#安装vim
sudo apt install vim
#测试
vim #进入后输入:q按回车退出

#安装git
sudo apt install git-all
#测试
git --version

#安装miniconda
#下载网址https://docs.conda.io/en/latest/miniconda.html ，根据系统和版本自行选择
#下载好一个后缀为*.sh文件，添加权限并运行，输入命令（无权限的话，加上sudo提升权限）
chmod 777 *.sh #*为你刚下载的路径+文件名
bash *.sh
#根据脚本提示输入enter、yes等回答
#测试是否安装成功，若有conda信息说明成功
conda
#若显示conda命令不存在，先重启环境变量，然后继续测试conda命令
source ~/.bashrc
conda
#若还是显示conda命令不存在，则自行修改环境变量，具体方法使用搜索引擎.
#conda使用教程见 ./SilenchtNote/Conda/Conda.md

#下载密码管理器enpass
# For Ubuntu and Debian based systems，To install Enpass, add a new repository to /etc/apt/sources.list:
sudo -i
echo "deb https://apt.enpass.io/ stable main" >  /etc/apt/sources.list.d/enpass.list
# And import key that is used to sign the release:
wget -O - https://apt.enpass.io/keys/enpass-linux.key | apt-key add -
# After that, you can install Enpass as any other software package:
apt-get update
apt-get install enpass
exit
# Seclect WebDav and input URL: https://dav.jianguoyun.com/dav/
# input username and password
```



