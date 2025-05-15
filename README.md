# TUM IN2354 – Exercise 1: 3D Mesh from RGB-D

This repository contains the full setup to generate OFF meshes from depth+color images using the TUM RGB-D SLAM dataset.

## Structure

- `Exercises/Exercise-1/`: Code to run Exercise 1 (main.cpp, helper headers)
- `Libs/`: Includes Eigen and FreeImage
- `Data/`: Place to put your dataset (e.g., `rgbd_dataset_freiburg1_xyz/`)
- `.gitignore`: Prevents tracking build/data artifacts

## Build

```bash
cd Exercises/Exercise-1
mkdir build && cd build
cmake ..
cmake --build . --config Release
