## ROS rosdep update报错解决方法

### 一、问题描述

**操作系统**：Ubuntu18.04
**ROS版本**：melodic
**错误描述**：

rosdep init 和 rosdep update命令因网络原因无法正确执行，错误信息可能如下：

```bash
[rosdep init]
ERROR: cannot download default sources list from:
https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/sources.list.d/20-default.list
Website may be down.

[rosdep update]
reading in sources list data from /etc/ros/rosdep/sources.list.d
ERROR: unable to process source [https://raw.github.com/ros/rosdistro/master/rosdep/osx-homebrew.yaml]:
				<urlopen error [Errno 111] Connection refused>……
```

### 二、解决方法

放弃从网络自动下载文件，手动下载所需文件到本地，然后在本地执行脚本即可。

步骤如下：

#### 2.1 下载文件到本地

```bash
# 假设你的用户名为 username
cd ~
git clone https://github.com/ros/rosdistro.git
# 此时主文件夹目录下出现rosdistro文件夹，路径为/home/username/rosdistro
```

#### 2.2 创建20-default.list文件

```bash
# 手动创建脚本所需的文件
sudo mkdir -p /etc/ros/rosdep/sources.list.d
cd /etc/ros/rosdep/sources.list.d
sudo gedit 20-default.list
# 输入以下内容并保存：

# os-specific listings first
yaml file:///home/username/rosdistro/rosdep/osx-homebrew.yaml osx

# generic
yaml file:///home/username/rosdistro/rosdep/base.yaml
yaml file:///home/username/rosdistro/rosdep/python.yaml
yaml file:///home/username/rosdistro/rosdep/ruby.yaml
gbpdistro file:///home/username/rosdistro/releases/fuerte.yaml fuerte

# newer distributions (Groovy, Hydro, ...) must not be listed anymore, they are being fetched from the rosdistro index.yaml instead
```

#### 2.3 修改脚本URL为本地路径

```bash
cd /usr/lib/python2.7/dist-packages/rosdep2/
sudo gedit gbpdistro_support.py
# 将 FUERTE_GBPDISTRO_URL 后面路径改为如下，并保存
FUERTE_GBPDISTRO_URL = 'file:///home/username/rosdistro/releases/fuerte.yaml'
```

```bash
cd /usr/lib/python2.7/dist-packages/rosdep2/
sudo gedit rep3.py
# 将 REP3_TARGETS_URL 后面路径改为如下，并保存
REP3_TARGETS_URL = 'file:///home/username/rosdistro/releases/targets.yaml'
```

```bash
cd /usr/lib/python2.7/dist-packages/rosdistro/
sudo gedit __init__.py
# 将 DEFAULT_INDEX_URL 后面路径改为如下，并保存
DEFAULT_INDEX_URL = 'file:///home/username/rosdistro/index-v4.yaml'
```

#### 2.4 启动更新命令

```bash
rosdep update
# 输出如下：
reading in sources list data from /etc/ros/rosdep/sources.list.d
Hit file:///home/silencht/rosdistro/rosdep/osx-homebrew.yaml
Hit file:///home/silencht/rosdistro/rosdep/base.yaml
Hit file:///home/silencht/rosdistro/rosdep/python.yaml
Hit file:///home/silencht/rosdistro/rosdep/ruby.yaml
Hit file:///home/silencht/rosdistro/releases/fuerte.yaml
Query rosdistro index file:///home/silencht/rosdistro/index-v4.yaml
Skip end-of-life distro "ardent"
Skip end-of-life distro "bouncy"
Skip end-of-life distro "crystal"
Skip end-of-life distro "dashing"
Skip end-of-life distro "eloquent"
Add distro "foxy"
Add distro "galactic"
Skip end-of-life distro "groovy"
Skip end-of-life distro "hydro"
Skip end-of-life distro "indigo"
Skip end-of-life distro "jade"
Skip end-of-life distro "kinetic"
Skip end-of-life distro "lunar"
Add distro "melodic"
Add distro "noetic"
Add distro "rolling"
updated cache in /home/silencht/.ros/rosdep/sources.cache
```

