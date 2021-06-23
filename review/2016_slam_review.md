#    2016 SLAM REVIEW

## 1. SLAM概念

### 1.1 SLAM是什么？

**同时定位与地图构建**（英语：Simultaneous localization and mapping，一般直接称SLAM）是一种概念：

> 希望机器人从未知环境的未知地点出发，在运动过程中通过重复观测到的地图特征（比如，墙角，柱子等）定位自身位置和姿态，再根据自身位置增量式的构建地图，从而达到同时定位和地图构建的目的。

- localization：我在什么地方？——**定位**

- mapping：周围环境什么样子？——**建图**

- simultaneous：先做什么？——**同时**

  > simultaneous map building and localization can be seen to present a question of which came fifirst, the chicken or the egg? (The map or the motion?)

  > starting from the unknown location of the unknown environment, the robot locates its own position and attitude through repeated observation of environmental features in the movement process, and then builds an incremental map of the surrounding environment according to its own position, so as to achieve the purpose of simultaneous positioning and map construction.

  **同时**建立地图和定位可以被视为一个是先有鸡还是先有蛋的问题。

  从未知环境的未知地点出发，通过运动过程中对环境特征的反复观察，定位自身位置和姿态，然后根据自身位置来构建周围环境的**增量地图**，实现 同时 定位和地图构造的目的。
  
  
  
  **SLAM问题的本质：对运动主体自身和周围环境空间不确定性的估计**
  
  

### 1.2 SLAM用在哪？

#### 1.2.1 适用场景

> SLAM finds applications in all scenarios in which a prior map is not available and needs to be built.

那些没有办法构建先验地图且需要构建地图的所有场景。

#### 1.2.2 不适用场景

> SLAM may not be required if localization can be done reliably with respect to the known landmarks.

那些能依靠已知地标进行有效定位的场景。

### 1.3 SLAM为何兴起？

> The popularity of the SLAM problem is connected with the emergence of indoor applications of mobile robotics.

与室内移动机器人的兴起息息相关。

>  The popularity of SLAM in the last 30 years is not surprising if one thinks about the manifold aspects that SLAM involves. 
>
> At the lower level (called the *front-end* ) SLAM naturally intersects other research fifields such as computer vision and signal processing;
>
> At the higher level (that we later call the *back-end* ), SLAM is an appealing mix of geometry, graph theory, optimization, and probabilistic estimation.

如果考虑到SLAM涉及的多方面问题，那么它在这三十年的流行也就不足为奇。

从低层次，即SLAM前端来说，它与计算机视觉和信号处理等领域很自然的交叉作用；

从高层次，即SLAM后端来说，它是一个吸引人的几何、图论、优化和概率估计的组合。

### 1.4 SLAM有研究意义吗？

#### 1.4.1 自动机器人需要SLAM吗？

##### 1.4.1.0 引言

> > SLAM aims at building a globally consistent representation of the environment, leveraging both ego-motion measurements and loop closures. The keyword here is “**loop closure**”: if we sacrififice loop closures, SLAM reduces to **odometry**. In early applications, odometry was obtained by integrating wheel encoders. The pose estimate obtained from wheel odometry quickly **drifts**, making the estimate unusable after few meters.
>
> SLAM旨在利用自我运动测量和闭环检测来构建整体一致的环境表示。这里的关键词是“闭环检测”：如果我们丢弃闭环检测，那么SLAM将会退化为里程测量 / 里程计。在早期的应用中，可以通过积分车轮编码器来获得里程。然而，从车轮里程测量所获得的姿态估计快速**漂移**，使得估计数在短短几米之内就不可用。
>
> - A. Kelly. Mobile Robotics: Mathematics, Models, and Methods . Cambridge University Press, 2013.
>
> > this was one of the main thrusts behind the development of SLAM: the observation of external landmarks is useful to reduce the trajectory drift and possibly correct it.
>
> 这个是SLAM发展背后的主要推动力之一：对于外部地标的观测有助于减少轨迹漂移并可能纠正之。
>
> - P. Newman, J. J. Leonard, J. D. Tardos, and J. Neira. Explore and Return: Experimental Validation of Real-Time Concurrent Mapping and Localization. In *Proceedings of the IEEE International Conferenceon Robotics and Automation (ICRA)*, pages 1802–1809. IEEE, 2002.
>
> >  However, more recent odometry algorithms are based on visual and inertial information, and have very small drift (*<* 0*.*5% of the trajectory length).
>
> 然而，很多最近的基于视觉和惯性信息的测速方法漂移越来越小。
>
> - C. Forster, L. Carlone, F. Dellaert, and D. Scaramuzza. On-Manifold Preintegration for Real-Time Visual–Inertial Odometry. *IEEE Transactions on Robotics (TRO)*, PP(99):1–21, 2016.
>
> 那么问题来了，
>
> **我们真的还需要SLAM这个算法吗？**
>
> **答案有三方面**

##### 1.4.1.1 Odometry就是SLAM的一部分

- **视觉-惯性导航（VIN）就是一个简化版SLAM**

​	首先，在过去十年的SLAM发展中，研究本身就产生了基于视觉-惰性测量算法，这种算法目前代表了最先进的水平[163,175]。在这种意义上，视觉-惯性导航（visual-inertial navigation）就是SLAM：**VIN可以被看作一个简化版SLAM系统，只不过这个系统的闭环检测（loop closure）或者位置识别（place recongnition）模块没有开启**。

​	更一般的，SLAM直接导致了在相比以前文献（如航空航天工程中的[惰性导航](../note/inertial_navigation.md)）更有挑战性的设置下（如没有GPS,低质量传感器等）关于传感器融合的研究。

##### 1.4.1.2 Odometry 和 Loop Closure 缺一不可

- **单纯的里程计会无限探索新区域，加入位置识别有助于理解环境真实拓扑**

 	第二个回答是关于环境的真实拓扑结构（true topology）的。一个只运行里程计（odometry）而关闭闭环检测（loop closure）的机器人会将世界解释为一个“无限走廊（infinite corridor）”，在这个“走廊”里，机器人一直保持着探索未定义的新区域，如图1左侧。回路闭环事件会通知机器人这个走廊与它自身发生相交（intersect），如图1右侧。闭环检测的优势现在开始变得清晰：**加入闭环检测后，机器人可以理解环境的真实拓扑，也就有能力找到位置之间的最短路径**（比如地图上的点B和点C）。

![image-20210331092855678](https://silencht.oss-cn-beijing.aliyuncs.com/img/image-20210331092855678.png)

<center> 图1</center>

*图一：**左图**：利用里程计构建的地图。这个地图可看作是一条从起始位置 A 到最终位置 B的一条长走廊。在现实中实际距离很近的点（例如 B和 C）可能在测距图（odometric map）中是任意远的。**右图**：利用 SLAM构建的地图。通过充分利用回路闭环（loop closures），SLAM可以估计环境的实际拓扑结构（topology）并在地图中发现（discovers）最短路径（shortcuts）。*

- **单纯的位置识别有可能被位置不同而场景相似的数据关联所欺骗**

​	那么，既然获得正确的环境拓扑是SLAM的优点之一，那为什么不简单删掉度量信息（metric information）就只做位置识别呢？答案很简单：度量信息可以使得位置识别更简单和鲁棒；度量的重建会通知机器人有关闭环检测的机会，并允许丢弃虚假的回路闭环[150]。因此，尽管SLAM在原理上可能是冗余的（一个甲骨文位置识别模块足以胜任构建拓扑地图），但是SLAM（中的度量信息）为那些错误的数据关联和感知混叠提供了自然的防御（此时，对于环境中的实际位置不同而场景却相似的情况将不会欺骗到位置识别）。

​	从这个意义上来说，**SLAM地图提供了一种既预测又验证未来测量结果的方法**：我们相信这个机制将是机器人稳定运行的关键。

##### 1.4.1.3 在要求全局一致地图中，SLAM是必须的

​	对于许多无论是内在还是外在需求全局一致（globally consistent）的地图（map）来说，SLAM是必需的。例如，在许多军用和民用应用中，机器人的目标是探索一个（未知）环境，然后向人类操作员报告一张确保全覆盖该环境的地图。另外一个例子是，机器人必须可以（对建筑、桥梁等）进行结构性检查；同样，在这种情况下，一个全局一致的三维重建是获得成功的需要。



#### 1.4.2 SLAM在学术上已经被解决了吗？

​	这个问题很难回答。因为SLAM已经成为一个很广泛的话题，所以这个问题适合在  **给定的 机器人/环境/性能 三者组合的情况下** 提问。

##### 1.4.2.1 比较成熟的领域：提高系统的精度、鲁棒性

- 配备 车轮编码器和激光扫描仪 的构建 二维地图 的 室内机器人已经有了相当的精度（<10cm）和足够的鲁棒性（如低故障率），可以被认为在很大程度上已经得到解决；
- 配备 基于视觉SLAM 的 缓慢移动机器人（例如火星漫游者、家用机器人等），以及视觉惯性里程计（visual-inertial odometry）可以被视为是比较成熟的领域；

##### 1.4.2.2 尚不成熟的领域：高速时失效

- 当机器人的 运动 或者 所处环境 具有挑战性的时候，SLAM算法很容易失效。例如，快速机器人动力学 、 高度动态的环境；
- SLAM算法往往无法满足严格性能要求场合。例如，快速闭环控制下的高速率估计（high rate estimation for fast closed-loop control）
- 如何平衡实时性和准确性是一个问题；
- 有关动态、非结构化、复杂、不确定和大规模的环境的解决方案仍有待探索；

### 1.5 视觉SLAM的相机决策

|   相机   |                             优点                             |                             缺点                             |
| :------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| 单目相机 |                     结构简单，成本特别低                     |            平移之后才能计算深度，无法确定真实尺度            |
| 双目相机 |                     克服了单目相机的缺点                     | 配置与标定较为复杂，深度量程和精度受双目基线与分辨率所限，视差计算非常消耗计算资源 |
| 深度相机 |                 相比双目相机节省大量计算资源                 | 测量范围窄、噪声大、视野小、易受日光干扰、无法测量透射材质等。主要用于室内，室外较难应用。 |
| 事件相机 | 相比传统相机具有更小的时间延迟，更快的更新速率、更强的动态范围、更低的功耗和更小的存储空间要求。有望成为继深度相机后的新型视觉传感器 |     完全依赖于场景动态,低动态环境下性能反而不如传统相机      |

## 2. SLAM的发展阶段

### 2.1 1986—2004：classical age（古典时期）

#### 2.1.1 研究内容

> the introduction of **the main probabilistic formulations** for SLAM , including approaches based on **Extended Kalman Filters**, **Rao-Blackwellised Particle Filters**, and **maximum likelihood estimation**.

引入了SLAM主要的概率公式，其中包括 扩展卡尔曼滤波、粒子滤波 和 最大似然估计

#### 2.1.2 论文

> A thorough historical review of the first 20 years of the SLAM problem

详细论述SLAM发展前二十年的review

- H. F. Durrant-Whyte and T. Bailey. Simultaneous Localisation and Mapping (SLAM): Part I. *IEEE Robotics and Automation Magazine*, 13(2):99–110, 2006.
- T. Bailey and H. F. Durrant-Whyte. Simultaneous Localisation and Mapping (SLAM): Part II. *Robotics and Autonomous Systems (RAS)*, 13(3):108–117, 2006.

> Two other excellent references describing the **three main SLAM formulations** of the classical age

描述三大SLAM主要公式的论文

- S. Thrun, W. Burgard, and D. Fox. Probabilistic Robotics. MIT Press,2005.
- C. Stachniss, S. Thrun, and J. J. Leonard. Simultaneous Localization and Mapping. In B. Siciliano and O. Khatib, editors, Springer Handbook of Robotics, chapter 46, pages 1153–1176. Springer, 2nd edition, 2016.  

#### 2.1.3 挑战

> the basic challenges connected to efficiency and robust data association.

效率和鲁棒的数据关联的挑战 。

### 2.2 2004—2015：algorithmic-analysis age（算法分析时期）

#### 2.2.1 研究内容

> the study of fundamental properties of SLAM, including observability, convergence, and consistency. In this period, the key role of **sparsity** towards effificient SLAM solvers was also understood, and the main open-source SLAM libraries were developed.

研究SLAM的基本特性，包括可观测性、收敛性和一致性。在这一时期，（人们）理解了对SLAM的高效率解决器？起到关键作用的**稀疏性**，并开发了主要的开源SLAM库。

#### 2.2.2 论文

> partially covered

覆盖部分该时期内容的review

- G. Dissanayake, S. Huang, Z. Wang, and R. Ranasinghe. A review of recent developments in Simultaneous Localization and Mapping. In International Conference on Industrial and Information Systems*, pages 477–482. IEEE, 2011.

#### 2.2.3 挑战



### 2.3 2015—至今：robust-perception age（稳健感知时期）

#### 2.3.1 研究内容

##### 2.3.1.1 稳健的性能

> the SLAM system operates with low failure rate for an extended period of time in a broad set of environments; the system includes fail-safe mechanisms and has self-tuning capabilities in that it can adapt the selection of the system parameters to the scenario.

系统在广泛的环境中长期运行且具有低故障率；系统包括故障-安全机制和自调整功能，可以针对适应场景修改参数。

##### 2.3.1.2 高级的理解

> the SLAM system goes beyond basic geometry reconstruction to obtain a high-level understanding of the environment

系统超越基本的几何重建来获得对环境的更高级别的理解。（高级图形学、语义学、物理学、affordances）

##### 2.3.1.3 资源意识

> the SLAM system is tailored to the available sensing and computational resources, and provides means to adjust the computation load depending on the available resources

系统针对可用传感器与计算资源进行裁剪，提供根据可用资源调整计算负载的方法

##### 2.3.1.4 任务驱动感知

> the SLAM system is able to select relevant perceptual information and fifilter out irrelevant sensor data, in order to support the task the robot has to perform; moreover, the SLAM system produces adaptive map representations, whose complexity may vary depending on the task at hand

系统有 选择 相关感知信息 和 滤除 不相关传感数据 的能力 来支持机器人需要执行的任务。此外，系统产生自适应的根据手头任务不同而复杂性不同的地图表示。

#### 2.3.2 论文



#### 2.3.3 挑战

> Visual SLAM still face some important obstacles like the illumination condition, high dynamic environment, fast motion, vigorous rotation and low texture environment.

视觉SLAM仍然面临着光照条件、高动态环境、运动快、旋转剧烈、低纹理环境等重要阻碍。



#### 2.3.4 未来

- 基于智能手机、嵌入式平台（如无人机）的SLAM
- 详细的三维重建，使用深度学习的场景理解
- 多传感器融合问题
- 语义SLAM：还可以支持全局优化、闭环检测和重新定位；传统的SLAM依赖于点线面等几何特征来推断环境结构，而语义SLAM是使机器人像人类一样思考，从而在大规模场景中实现高精度实时定位的目标。
- 算法和传感器的深度融合，集成、专用的传感器将发挥出巨大的潜力
- 端到端





## 0.1 英文名词翻译/解释

- metric 度量
- semantic 语义 Semantic is used to describe things that deal with the meanings of words and sentences.
- pose 姿态，这里一般指机器人的位置和姿势 position and orientation
- topology 拓扑the way the parts of sth are arranged and related 

