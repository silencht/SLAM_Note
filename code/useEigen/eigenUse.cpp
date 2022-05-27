#include <iostream>

using namespace std;

#include <cmath>
#include <ctime>
// Eigen 核心部分
#include <Eigen/Core>
// 稠密矩阵的代数运算（逆，特征值等）
#include <Eigen/Dense>
// Eigen/Geometry 模块提供了各种旋转和平移的表示
#include <Eigen/Geometry>

using namespace Eigen;

#define MATRIX_SIZE 50

/****************************
* 本程序演示了 Eigen 基本类型的使用
****************************/

int main(int argc, char **argv) {
  // Eigen 中所有向量和矩阵都是Eigen::Matrix，它是一个模板类。它的前三个参数为：数据类型，行，列
  // 声明一个2*3的float矩阵
  Matrix<float, 2, 3> matrix_23f;

  // 同时，Eigen 通过 typedef 提供了许多内置类型，不过底层仍是Eigen::Matrix
  // 例如 Vector3d 实质上是 Eigen::Matrix<double, 3, 1>，即三维向量
  Vector3d v_3d;
  // 这是一样的
  Matrix<float, 3, 1> mv_3f;  //vd_3d

  // Matrix3d 实质上是 Eigen::Matrix<double, 3, 3>
  Matrix3d matrix_33d = Matrix3d::Zero(); //初始化为零
  // 如果不确定矩阵大小，可以使用动态大小的矩阵
  Matrix<double, Dynamic, Dynamic> matrix_dynamicd;
  // 更简单的
  MatrixXd matrix_xd;

  // 下面是对Eigen阵的操作
  // 输入数据（初始化），输入顺序：从左到右，从上到下
  matrix_23f << 1, 2, 3, 4, 5, 6;
  // 输出
  cout << "matrix 2x3 from 1 to 6: \n" << matrix_23f << endl;

  // 用(行，列)访问矩阵中的元素
  cout << "print matrix 2x3: " << endl;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) cout << matrix_23f(i, j) << "\t";
    cout << endl;
  }

  // 矩阵和向量相乘（实际上仍是矩阵和矩阵）
  v_3d << 3, 2, 1;
  mv_3f << 4, 5, 6;

  // 但是在Eigen里你不能混合两种不同类型的矩阵，像这样是错的
  // Matrix<double, 2, 1> result_wrong_type = matrix_23f * v_3d;
  // 应该显式转换
  Matrix<double, 2, 1> result = matrix_23f.cast<double>() * v_3d;
  cout << "[1,2,3;4,5,6]*[3,2,1]=" << result.transpose() << endl;

  Matrix<float, 2, 1> result2 = matrix_23f * mv_3f;
  cout << "[1,2,3;4,5,6]*[4,5,6]: " << result2.transpose() << endl;

  // 同样你不能搞错矩阵的维度
  // 试着取消下面的注释，看看Eigen会报什么错
  // Eigen::Matrix<double, 2, 3> result_wrong_dimension = matrix_23.cast<double>() * v_3d;

  // 一些矩阵运算
  // 四则运算就不演示了，直接用+-*/即可。
  matrix_33d = Matrix3d::Random();                               
  cout << "random matrix: \n" << matrix_33d << endl;              // 随机数矩阵
  cout << "transpose: \n" << matrix_33d.transpose() << endl;      // 转置
  cout << "sum: " << matrix_33d.sum() << endl;                    // 各元素和
  cout << "trace: " << matrix_33d.trace() << endl;                // 迹
  cout << "times 10: \n" << 10 * matrix_33d << endl;              // 数乘
  cout << "inverse: \n" << matrix_33d.inverse() << endl;          // 逆
  cout << "det: " << matrix_33d.determinant() << endl;            // 行列式

  // 特征值、特征向量
  // 实对称矩阵可以保证对角化成功
  SelfAdjointEigenSolver<Matrix3d> eigen_solver(matrix_33d.transpose() * matrix_33d);
  cout << "Eigen values = \n" << eigen_solver.eigenvalues() << endl;   //输出特征值
  cout << "Eigen vectors = \n" << eigen_solver.eigenvectors() << endl; //输出特征向量

  // 解方程
  // 我们求解 matrix_NN * x = v_Nd 这个方程
  // N的大小在前边的宏里定义，它由随机数生成
  // 直接求逆自然是最直接的，但是求逆运算量大

  Matrix<double, MATRIX_SIZE, MATRIX_SIZE> matrix_NNd = MatrixXd::Random(MATRIX_SIZE, MATRIX_SIZE);
  matrix_NNd = matrix_NNd * matrix_NNd.transpose();  // 保证半正定
  Matrix<double, MATRIX_SIZE, 1> v_Nd = MatrixXd::Random(MATRIX_SIZE, 1);

  clock_t time_stt = clock(); // 计时
  // 直接求逆
  Matrix<double, MATRIX_SIZE, 1> x = matrix_NNd.inverse() * v_Nd;
  cout << "time of normal inverse is "<< 1000 * (clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << endl;
  cout << "x = " << x.transpose() << endl;

  // 通常用矩阵分解来求，例如QR分解，速度会快很多
  time_stt = clock();
  x = matrix_NNd.colPivHouseholderQr().solve(v_Nd);
  cout << "time of Qr decomposition is "<< 1000 * (clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << endl;
  cout << "x = " << x.transpose() << endl;

  // 对于正定矩阵，还可以用cholesky分解来解方程
  time_stt = clock();
  x = matrix_NNd.ldlt().solve(v_Nd);
  cout << "time of ldlt decomposition is "<< 1000 * (clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << endl;
  cout << "x = " << x.transpose() << endl;


  // 3D 旋转矩阵直接使用 Matrix3d 或 Matrix3f
  Matrix3d rotation_matrixd = Matrix3d::Identity();             //3*3 double类型的单位矩阵
  // 旋转向量使用 AngleAxis, 它底层不直接是Matrix，但运算可以当作矩阵（因为重载了运算符）
  AngleAxisd rotation_vectord(M_PI / 4, Vector3d(0, 0, 1));     //沿 Z 轴旋转 45 度. AngleAxis(const Scalar & angle,const MatrixBase< Derived > & axis )	
  cout.precision(3);
  // 旋转向量转换为旋转矩阵
  cout << "rotation_vectord.matrix() =\n" << rotation_vectord.matrix() << endl;   //用.matrix(),继承自基类Eigen::RotationBase
  rotation_matrixd = rotation_vectord.toRotationMatrix();
  cout << "rotation_vectord.toRotationMatrix() =\n" << rotation_matrixd << endl;  //用.toRotationMatrix()，自身的成员函数


  Vector3d v(1, 0, 0);
  // 用 AngleAxis 进行坐标变换
  Vector3d v_rotated = rotation_vectord * v;
  cout << "(1,0,0) after rotation (by angle axis) = " << v_rotated.transpose() << endl;
  // 用旋转矩阵进行坐标变换
  v_rotated = rotation_matrixd * v;
  cout << "(1,0,0) after rotation (by matrix) = " << v_rotated.transpose() << endl;

  // 欧拉角: 将旋转矩阵直接转换成欧拉角
  Vector3d euler_anglesd = rotation_matrixd.eulerAngles(2, 1, 0); // ZYX顺序，即yaw-pitch-roll顺序
  cout << "yaw pitch roll = " << euler_anglesd.transpose() << endl;

  // 欧氏变换矩阵使用 Eigen::Isometry
  Isometry3d T = Isometry3d::Identity();                // 虽然称为3d，实质上是4＊4的矩阵
  T.rotate(rotation_vectord);                           // 按照rotation_vector进行旋转
  T.pretranslate(Vector3d(1, 3, 4));                    // 把平移向量设成(1,3,4)
  cout << "Transform matrix = \n" << T.matrix() << endl;

  // 用变换矩阵进行坐标变换
  Vector3d v_transformed = T * v;                              // 相当于R*v+t
  cout << "v tranformed = " << v_transformed.transpose() << endl;
  // 对于仿射和射影变换，使用 Eigen::Affine3d 和 Eigen::Projective3d 即可，略


  // 四元数
  // 直接把 AngleAxis 赋值给四元数，反之亦然
  Quaterniond q = Quaterniond(rotation_vectord);
  cout << "quaternion from rotation vector = " << q.coeffs().transpose() << endl;   // 请注意coeffs的顺序是(x,y,z,w),w为实部，前三者为虚部
  // 也可以把旋转矩阵赋给它
  q = Quaterniond(rotation_matrixd);
  cout << "quaternion from rotation matrix = " << q.coeffs().transpose() << endl;
  // 使用四元数旋转一个向量，使用重载的乘法即可
  v_rotated = q * v; // 注意数学上是qvq^{-1}
  cout << "(1,0,0) after rotation = " << v_rotated.transpose() << endl;
  // 用常规向量乘法表示，则应该如下计算 Quaternion (const Scalar &w, const Scalar &x, const Scalar &y, const Scalar &z)
  cout << "should be equal to " << (q * Quaterniond(0, 1, 0, 0) * q.inverse()).coeffs().transpose() << endl;
  // 将四元数单位化
  q.normalize();
  cout << "quaternion  normalized = " << q.coeffs().transpose() << endl;
  // 转换为欧式变换矩阵
  Isometry3d T2(q);
  cout << "Isometry3d from  = quaternion = \n" << T2.matrix() << endl;

  return 0;
}
