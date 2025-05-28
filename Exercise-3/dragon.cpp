#include "utils/io.h"
#include "utils/points.h"

#include "ceres/ceres.h"
#include <math.h>


// TODO: Implement the cost function (check gaussian.cpp for reference)
struct RegistrationCostFunction
{
	RegistrationCostFunction(const Point2D& point1_, const Point2D& point2_, const Weight& weight_)
		: point1(point1_), point2(point2_), weight(weight_)
	{
	}

	template<typename T>
	bool operator()(const T* const angle, const T* const tx, const T* const ty, T* residual) const
	{
		// 1. 获取点坐标并转换为 T 类型
		const T x1 = T(point1.x);
		const T y1 = T(point1.y);
		const T x2 = T(point2.x);
		const T y2 = T(point2.y);

		// 2. 获取变换参数
		T angle_rad = angle[0];
		T tx_val = tx[0];
		T ty_val = ty[0];

		// 3. 应用旋转变换
		T x1_rotated = x1 * cos(angle_rad) - y1 * sin(angle_rad);
		T y1_rotated = x1 * sin(angle_rad) + y1 * cos(angle_rad);

		// 4. 应用平移变换
		T x1_translated = x1_rotated + tx_val;
		T y1_translated = y1_rotated + ty_val;

		// 5. 计算加权残差（欧氏距离的平方再乘以权重）
		T weight_val = T(weight.w);
		residual[0] = weight_val * ((x2 - x1_translated) * (x2 - x1_translated) + (y2 - y1_translated) * (y2 - y1_translated));
		return true;
	}

private:
	const Point2D point1;
	const Point2D point2;
	const Weight weight;
};


int main(int argc, char** argv)
{
	google::InitGoogleLogging(argv[0]);

	// Read data points and the weights, and define the parameters of the problem
	const std::string file_path_1 = "/config/workspace/Data/points_dragon_1.txt";
	const auto points1 = read_points_from_file<Point2D>(file_path_1);
	
	const std::string file_path_2 = "/config/workspace/Data/points_dragon_2.txt";
	const auto points2 = read_points_from_file<Point2D>(file_path_2);
	
	const std::string file_path_weights = "/config/workspace/Data/weights_dragon.txt";
	const auto weights = read_points_from_file<Weight>(file_path_weights);
	
	const double angle_initial = 0.0;
	const double tx_initial = 0.0;
	const double ty_initial = 0.0;
	
	double angle = angle_initial;
	double tx = tx_initial;
	double ty = ty_initial;

	ceres::Problem problem;

	// 为每对点添加残差块
	auto p1 = points1.begin();
	auto p2 = points2.begin();
	auto w = weights.begin();
	for (; p1 != points1.end() && p2 != points2.end() && w != weights.end(); ++p1, ++p2, ++w)
	{
		problem.AddResidualBlock(
			new ceres::AutoDiffCostFunction<RegistrationCostFunction, 1, 1, 1, 1>(
				new RegistrationCostFunction(*p1, *p2, *w)
			),
			nullptr, &angle, &tx, &ty
		);
	}

	ceres::Solver::Options options;
	options.max_num_iterations = 25;
	options.linear_solver_type = ceres::DENSE_QR;
	options.minimizer_progress_to_stdout = true;

	ceres::Solver::Summary summary;
	ceres::Solve(options, &problem, &summary);

	std::cout << summary.BriefReport() << std::endl;

	// Output the final values of the translation and rotation (in degree)
	std::cout << "Initial angle: " << angle_initial << "\ttx: " << tx_initial << "\tty: " << ty_initial << std::endl;
	std::cout << "Final angle: " << std::fmod(angle * 180 / M_PI, 360.0) << "\ttx: " << tx << "\tty: " << ty << std::endl;

	system("pause");
	return 0;
}
