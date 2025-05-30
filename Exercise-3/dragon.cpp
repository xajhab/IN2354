#include "utils/io.h"
#include "utils/points.h"

#include "ceres/ceres.h"
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// TODO: Implement the cost function (check gaussian.cpp for reference)
struct RegistrationCostFunction
{
    RegistrationCostFunction(const Point2D& p_, const Point2D& q_, double w_)
        : p(p_), q(q_), w(w_) {}

    template<typename T>
    bool operator()(const T* const angle, const T* const tx, const T* const ty, T* residual) const
    {
        T cos_theta = ceres::cos(angle[0]);
        T sin_theta = ceres::sin(angle[0]);

        T x = T(p.x);
        T y = T(p.y);
        T x_q = T(q.x);
        T y_q = T(q.y);

        T x_trans = cos_theta * x - sin_theta * y + tx[0];
        T y_trans = sin_theta * x + cos_theta * y + ty[0];

        residual[0] = T(std::sqrt(w)) * (x_trans - x_q);
        residual[1] = T(std::sqrt(w)) * (y_trans - y_q);

        return true;
    }

    const Point2D p;
    const Point2D q;
    const double w;
};


int main(int argc, char** argv)
{
	google::InitGoogleLogging(argv[0]);

	// Read data points and the weights, and define the parameters of the problem
	const std::string file_path_1 = "../../Data/points_dragon_1.txt";
	const auto points1 = read_points_from_file<Point2D>(file_path_1);
	
	const std::string file_path_2 = "../../Data/points_dragon_2.txt";
	const auto points2 = read_points_from_file<Point2D>(file_path_2);
	
	const std::string file_path_weights = "../../Data/weights_dragon.txt";
	const auto weights = read_points_from_file<Weight>(file_path_weights);
	
	const double angle_initial = 0.0;
	const double tx_initial = 0.0;
	const double ty_initial = 0.0;
	
	double angle = angle_initial;
	double tx = tx_initial;
	double ty = ty_initial;

	ceres::Problem problem;

	// TODO: For each weighted correspondence create one residual block (check gaussian.cpp for reference)

	for (size_t i = 0; i < points1.size(); ++i)
	{
		problem.AddResidualBlock(
			new ceres::AutoDiffCostFunction<RegistrationCostFunction, 2, 1, 1, 1>(
				new RegistrationCostFunction(points1[i], points2[i], weights[i].w)
			),
			nullptr,
			&angle, &tx, &ty
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
