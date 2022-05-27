//
// Created by xiang on 18-11-19.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <ceres/ceres.h>
#include <chrono>

using namespace std;

/*

    现有曲线：y = exp(ax^2+bx+c)+w     
           
    a,b,c为待优化曲线参数，w为高斯噪声，满足分布：w~(0,\sigma^2)。现假设有N个已知的关于x，y的观测数据点，根据这些点求出曲线的参数a,b,c
    构建最小二乘问题：  min(a,b,c)  \sum_{i=1}^{N} || y_i - exp(a * x_i^2 + b * x_i + c) ||^2 
    定义残差为 e_i = y_i - exp(a * x_i^2 + b * x_i + c)
    求残差对每个待优化变量的倒数：
        de_i/da = - x_i^2 * exp(a * x_i^2 + b * x_i + c)
        de_i/db = - x_i   * exp(a * x_i^2 + b * x_i + c)
        de_i/dc = -         exp(a * x_i^2 + b * x_i + c)
    于是，雅可比矩阵J_i = [de_i/da, de_i/db, de_i/dc]^T
    
    高斯牛顿法：J * J^T dx = - J * e_i
 
    Ceres编写流程：
        1. 构建 cost funtion 计算模型类，设计残差计算仿函数
        2. 通过 cost funtion 构建待求解的最小二乘问题，向问题中添加误差项
        3. 配置求解器，开始优化

*/

// 1. 构建代价函数cost funtion的计算模型
struct CURVE_FITTING_COST {
  CURVE_FITTING_COST(double x, double y) : _x(x), _y(y) {}  //构造函数，输入为已知的观测值

  // 设计残差计算仿函数，使用模板函数
  template<typename T>
  bool operator()(const T *const abc, T *residual) const {   // 模型参数，有3维
    residual[0] = T(_y) - ceres::exp(abc[0] * T(_x) * T(_x) + abc[1] * T(_x) + abc[2]); // e_i = y_i - exp(a * x_i^2 + b * x_i + c)
    return true;
  }

  const double _x, _y;    // x,y数据
};


int main(int argc, char **argv) {
  double ar = 1.0, br = 2.0, cr = 1.0;         // 真实参数值
  double ae = 2.0, be = -1.0, ce = 5.0;        // 估计参数值
  int N = 100;                                 // 数据点
  double w_sigma = 1.0;                        // 噪声Sigma值
  double inv_sigma = 1.0 / w_sigma;
  cv::RNG rng;                                 // OpenCV随机数产生器

  vector<double> x_data, y_data;               // 已知数据存储
  for (int i = 0; i < N; i++) {
    double x = i / 100.0;
    x_data.push_back(x);
    y_data.push_back(exp(ar * x * x + br * x + cr) + rng.gaussian(w_sigma * w_sigma));
  }

  double abc[3] = {ae, be, ce};                // 初始化待估计参数        

  // 2. 构建最小二乘问题
  ceres::Problem problem;
  // 向问题中添加误差项。1. 使用自动求导，模板参数：残差类型，输出维度，输入维度，维数要与前面struct中一致
  for (int i = 0; i < N; i++) {
    problem.AddResidualBlock(
      new ceres::AutoDiffCostFunction<CURVE_FITTING_COST, 1, 3>(new CURVE_FITTING_COST(x_data[i], y_data[i])),
      nullptr,            // 2. 核函数，这里不使用，为空
      abc                 // 3. 待估计参数
    );
  }

  // 3. 配置求解器
  ceres::Solver::Options options;     // 这里有很多配置项可以填
  options.linear_solver_type = ceres::DENSE_NORMAL_CHOLESKY;  // 增量方程如何求解
  options.minimizer_progress_to_stdout = true;   // 输出到cout

  ceres::Solver::Summary summary;                // 优化信息
  chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
  ceres::Solve(options, &problem, &summary);     // 开始优化
  chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
  chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
  cout << "solve time cost = " << time_used.count() << " seconds. " << endl;

  // 输出结果
  cout << summary.BriefReport() << endl;
  cout << "estimated a,b,c = ";
  for (auto a:abc) cout << a << " ";
  cout << endl;

  return 0;
}
