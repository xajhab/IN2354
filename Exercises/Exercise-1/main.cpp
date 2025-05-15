#include <iostream>
#include <fstream>
#include <array>

#include "Eigen.h"
#include "VirtualSensor.h"

struct Vertex
{
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	// position stored as 4 floats (4th component is supposed to be 1.0)
	Vector4f position;
	// color stored as 4 unsigned char
	Vector4uc color;
};

bool WriteMesh(Vertex* vertices, unsigned int width, unsigned int height, const std::string& filename)
{
	float edgeThreshold = 0.01f; // 1cm

	// TODO 2: use the OFF file format to save the vertices grid (http://www.geomview.org/docs/html/OFF.html)
	// - have a look at the "off_sample.off" file to see how to store the vertices and triangles
	// - for debugging we recommend to first only write out the vertices (set the number of faces to zero)
	// - for simplicity write every vertex to file, even if it is not valid (position.x() == MINF) (note that all vertices in the off file have to be valid, thus, if a point is not valid write out a dummy point like (0,0,0))
	// - use a simple triangulation exploiting the grid structure (neighboring vertices build a triangle, two triangles per grid cell)
	// - you can use an arbitrary triangulation of the cells, but make sure that the triangles are consistently oriented
	// - only write triangles with valid vertices and an edge length smaller then edgeThreshold

	auto isValid = [](const Vector4f& v) {
		return v[0] != MINF;
	};

	auto edgeLen2 = [](const Vector4f& a, const Vector4f& b) {
		return (a - b).squaredNorm();
	};

	// TODO: Get number of vertices
	unsigned int nVertices = width * height;

	// TODO: Determine number of valid faces
	unsigned nFaces = 0;

	// First pass: count valid faces
	for (unsigned int y = 0; y < height - 1; ++y) {
		for (unsigned int x = 0; x < width - 1; ++x) {
			unsigned int idx0 = y * width + x;
			unsigned int idx1 = y * width + (x + 1);
			unsigned int idx2 = (y + 1) * width + x;
			unsigned int idx3 = (y + 1) * width + (x + 1);

			const Vector4f& v0 = vertices[idx0].position;
			const Vector4f& v1 = vertices[idx1].position;
			const Vector4f& v2 = vertices[idx2].position;
			const Vector4f& v3 = vertices[idx3].position;

			if (isValid(v0) && isValid(v1) && isValid(v2) &&
				edgeLen2(v0, v1) < edgeThreshold * edgeThreshold &&
				edgeLen2(v0, v2) < edgeThreshold * edgeThreshold &&
				edgeLen2(v1, v2) < edgeThreshold * edgeThreshold)
			{
				nFaces++;
			}

			if (isValid(v1) && isValid(v2) && isValid(v3) &&
				edgeLen2(v1, v2) < edgeThreshold * edgeThreshold &&
				edgeLen2(v1, v3) < edgeThreshold * edgeThreshold &&
				edgeLen2(v2, v3) < edgeThreshold * edgeThreshold)
			{
				nFaces++;
			}
		}
	}

	// Write off file
	std::ofstream outFile(filename);
	if (!outFile.is_open()) return false;


	// write header
	outFile << "COFF" << std::endl;

	outFile << "# numVertices numFaces numEdges" << std::endl;

	outFile << nVertices << " " << nFaces << " 0" << std::endl;

	// TODO: save vertices


	for (unsigned int i = 0; i < nVertices; ++i) {
		const Vector4f& pos = vertices[i].position;
		if (pos.x() == MINF)
			outFile << "0.0 0.0 0.0 0 0 0 0\n";
		else {
			outFile << pos.x() << " " << pos.y() << " " << pos.z();
			const Vector4uc& col = vertices[i].color;
			outFile << " " << (int)col[0] << " " << (int)col[1] << " " << (int)col[2] << " " << (int)col[3] << std::endl;
		}
	}


	// TODO: save valid faces
	for (unsigned int y = 0; y < height - 1; ++y) {
		for (unsigned int x = 0; x < width - 1; ++x) {
			unsigned int idx0 = y * width + x;
			unsigned int idx1 = y * width + (x + 1);
			unsigned int idx2 = (y + 1) * width + x;
			unsigned int idx3 = (y + 1) * width + (x + 1);

			const Vector4f& v0 = vertices[idx0].position;
			const Vector4f& v1 = vertices[idx1].position;
			const Vector4f& v2 = vertices[idx2].position;
			const Vector4f& v3 = vertices[idx3].position;

			// triangle 1: v0, v2, v1
			if (isValid(v0) && isValid(v1) && isValid(v2) &&
				edgeLen2(v0, v1) < edgeThreshold * edgeThreshold &&
				edgeLen2(v0, v2) < edgeThreshold * edgeThreshold &&
				edgeLen2(v1, v2) < edgeThreshold * edgeThreshold)
			{
				outFile << "3 " << idx0 << " " << idx2 << " " << idx1 << std::endl;
			}

			// triangle 2: v1, v2, v3
			if (isValid(v1) && isValid(v2) && isValid(v3) &&
				edgeLen2(v1, v2) < edgeThreshold * edgeThreshold &&
				edgeLen2(v1, v3) < edgeThreshold * edgeThreshold &&
				edgeLen2(v2, v3) < edgeThreshold * edgeThreshold)
			{
				outFile << "3 " << idx1 << " " << idx2 << " " << idx3 << std::endl;
			}
		}
	}

	std::cout << "# list of faces" << std::endl;
	std::cout << "# nVerticesPerFace idx0 idx1 idx2 ..." << std::endl;


	// close file
	outFile.close();

	return true;
}

int main()
{
	// Make sure this path points to the data folder
	// std::string filenameIn = "../Data/rgbd_dataset_freiburg1_xyz/";
	std::string filenameIn = "D:\\tum\\tum_02\\IN2354\\Data\\rgbd_dataset_freiburg1_xyz\\";

	std::string filenameBaseOut = "mesh_";

	// load video
	std::cout << "Initialize virtual sensor..." << std::endl;
	VirtualSensor sensor;
	if (!sensor.Init(filenameIn))
	{
		std::cout << "Failed to initialize the sensor!\nCheck file path!" << std::endl;
		return -1;
	}

	// convert video to meshes
	while (sensor.ProcessNextFrame())
	{
		// get ptr to the current depth frame
		// depth is stored in row major (get dimensions via sensor.GetDepthImageWidth() / GetDepthImageHeight())
		float* depthMap = sensor.GetDepth();
		// get ptr to the current color frame
		// color is stored as RGBX in row major (4 byte values per pixel, get dimensions via sensor.GetColorImageWidth() / GetColorImageHeight())
		BYTE* colorMap = sensor.GetColorRGBX();

		// get depth intrinsics
		Matrix3f depthIntrinsics = sensor.GetDepthIntrinsics();
		Matrix3f depthIntrinsicsInv = depthIntrinsics.inverse();

		float fX = depthIntrinsics(0, 0);
		float fY = depthIntrinsics(1, 1);
		float cX = depthIntrinsics(0, 2);
		float cY = depthIntrinsics(1, 2);

		// compute inverse depth extrinsics
		Matrix4f depthExtrinsicsInv = sensor.GetDepthExtrinsics().inverse();

		Matrix4f trajectory = sensor.GetTrajectory();
		Matrix4f trajectoryInv = sensor.GetTrajectory().inverse();

		if (sensor.GetCurrentFrameCnt() == 0 || sensor.GetCurrentFrameCnt() == 100)
		{
			std::cout << "Trajectory Frame " << sensor.GetCurrentFrameCnt() << ":\n";
			std::cout << trajectory << std::endl;
		}

		// TODO 1: back-projection
		// write result to the vertices array below, keep pixel ordering!
		// if the depth value at idx is invalid (MINF) write the following values to the vertices array
		// vertices[idx].position = Vector4f(MINF, MINF, MINF, MINF);
		// vertices[idx].color = Vector4uc(0,0,0,0);
		// otherwise apply back-projection and transform the vertex to world space, use the corresponding color from the colormap
		Vertex* vertices = new Vertex[sensor.GetDepthImageWidth() * sensor.GetDepthImageHeight()];

		unsigned int width = sensor.GetDepthImageWidth();
		unsigned int height = sensor.GetDepthImageHeight();

		for (unsigned int y = 0; y < height; ++y) {
			for (unsigned int x = 0; x < width; ++x) {
				unsigned int idx = y * width + x;
				float depth = depthMap[idx];

				if (depth == MINF) {
					vertices[idx].position = Vector4f(MINF, MINF, MINF, MINF);
					vertices[idx].color = Vector4uc(0, 0, 0, 0);
					continue;
				}

				// Backproject to camera space
				float X = (x - cX) * depth / fX;
				float Y = (y - cY) * depth / fY;
				float Z = depth;

				Vector4f camPoint(X, Y, Z, 1.0f);

				// Convert to world space
				Vector4f worldPoint = trajectoryInv * camPoint;

				vertices[idx].position = worldPoint;

				// color (RGBX), each pixel 4 bytes
				BYTE r = colorMap[4 * idx + 0];
				BYTE g = colorMap[4 * idx + 1];
				BYTE b = colorMap[4 * idx + 2];
				BYTE a = colorMap[4 * idx + 3];

				vertices[idx].color = Vector4uc(r, g, b, a);

			}
		}



		// write mesh file
		std::stringstream ss;
		ss << filenameBaseOut << sensor.GetCurrentFrameCnt() << ".off";
		if (!WriteMesh(vertices, sensor.GetDepthImageWidth(), sensor.GetDepthImageHeight(), ss.str()))
		{
			std::cout << "Failed to write mesh!\nCheck file path!" << std::endl;
			return -1;
		}

		// free mem
		delete[] vertices;
	}

	return 0;
}