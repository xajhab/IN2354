# IN2354
---

## Directory Structure

```
IN2354/
├── Exercise-1/ # RGB-D mesh reconstruction
├── Exercise-2/ # Surface reconstruction
├── Exercise-3/ # Optimization with Ceres
├── Exercise-4/ # Coarse Alignment (Procrustes)
├── Exercise-5/ # ICP Optimization
├── Libs/
├── Data/
├── media/
└── .gitignore
```

## Build Instructions

```bash
cd Exercise-X
mkdir build && cd build
cmake ..
cmake --build .
Modify main.cpp to switch between tasks if applicable.
```

### Personal Tips

#### Generate plots and result files
```bash
python plot_gaussian.py --mu 1.57729 --sigma 0.605089
python plot_surface.py
python plot_dragon.py --deg 41.9745 --tx 1247.91 --ty 507.993
```

#### Dependencies via vcpkg

Installed packages (x64-windows triplet):

- `eigen3`
- `ceres`
- `glog`
- `lz4`
- `flann`

Toolchain file passed to CMake:
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/Users/xajha/DevTools/vcpkg/scripts/buildsystems/vcpkg.cmake
```

##  Visual Results 

### Exercise 1

| Task   | Visualization               |
| ------ | --------------------------- |
| Meshes | ![](./Media/snapshot00.png) |

### Exercise 2

| Method | Visualization           |
| ------ | ----------------------- |
| Sphere | ![](./Media/sphere.png) |
| Torus  | ![](./Media/torus.png)  |
| Hoppe  | ![](./Media/hoppe.png)  |
| RBF    | ![](./Media/rbf.png)    |

### Exercise 3

| Task    | Plot                      | Console Log                |
|---------|---------------------------|----------------------------|
| Task 1  | ![](./Media/gaussian.png) | [`output_gaussian.txt`](./Media/output_gaussian.txt) |
| Task 2  | ![](./Media/surface.png)  | [`output_surface.txt`](./Media/output_surface.txt)   |
| Task 3  | ![](./Media/dragon.png)   | [`output_dragon.txt`](./Media/output_dragon.txt)     |

### Exercise 4

| Task             | Visualization                     |
| ---------------- | --------------------------------- |
| Bunny procrustes | ![](./Media/bunny_procrustes.png) |

### Exercise 5

| Task             | Visualization                     |
| ---------------- | --------------------------------- |
| Bunny procrustes | ![](./Media/merged_output.png) |