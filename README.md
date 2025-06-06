# IN2354
---

## Directory Structure

```
IN2354/
├── Exercise-1/ # RGB-D mesh reconstruction
├── Exercise-2/ # Surface reconstruction
├── Exercise-3/ # Optimization with Ceres
├── Exercise-4/ # Coarse Alignment (Procrustes)
├── Exercise-5/
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

```bash
cd Exercise-1
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/Users/xajha/DevTools/vcpkg/scripts/buildsystems/vcpkg.cmake -DEigen3_DIR=../Libs/eigen-3.4.0/cmake
cmake --build .


cmake .. `  -DCMAKE_PREFIX_PATH="C:/Users/xajha/DevTools/vcpkg/installed/x64-windows;D:/tum/tum_02/IN2354/Libs/Glog;D:/tum/tum_02/IN2354/Libs/Ceres" `  -DEigen3_DIR="D:/tum/tum_02/IN2354/Libs/Eigen/cmake" `  -DCeres_DIR="D:/tum/tum_02/IN2354/Libs/Ceres/lib/cmake/Ceres" `  -Dglog_USE_STATIC_LIBS=ON    


cmake .. `
  -DCMAKE_INSTALL_PREFIX=../../Ceres `
  -DEigen3_DIR=../../Eigen/cmake `
  -DCMAKE_PREFIX_PATH="C:/Users/xajha/DevTools/vcpkg/installed/x64-windows" `
  -DBUILD_EXAMPLES=OFF `
  -DBUILD_BENCHMARKS=OFF `
  -DBUILD_TESTING=OFF `
  -DGFLAGS=OFF `
  -DLAPACK=OFF `
  -DCUSTOM_BLAS=OFF `
  -DSUITESPARSE=OFF
```

#### Clone and bootstrap vcpkg
```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
```

#### Install required libraries via vcpkg
```bash
vcpkg install eigen3:x64-windows
vcpkg install glog:x64-windows
vcpkg install ceres:x64-windows
```

#### Configure CMake with fixed library paths
```bash
cmake .. ^
  -DCMAKE_PREFIX_PATH="C:/Users/xajha/DevTools/vcpkg/installed/x64-windows;D:/tum/tum_02/IN2354/Libs/Glog;D:/tum/tum_02/IN2354/Libs/Ceres" ^
  -DEigen3_DIR="D:/tum/tum_02/IN2354/Libs/Eigen/cmake" ^
  -DCeres_DIR="D:/tum/tum_02/IN2354/Libs/Ceres/lib/cmake/Ceres"
```

#### Build all exercises
```bash
cmake --build .
```

#### Fix missing DLLs (Debug mode)
```bash
copy D:\tum\tum_02\IN2354\Libs\Glog\bin\glogd.dll D:\tum\tum_02\IN2354\Exercise-3\build\Debug\
```

#### Run executables
```bash
cd build
.\Debug\gaussian.exe
.\Debug\surface.exe
.\Debug\dragon.exe
```

#### Generate plots and result files
```bash
python plot_gaussian.py --mu 1.57729 --sigma 0.605089
python plot_surface.py
python plot_dragon.py --deg 41.9745 --tx 1247.91 --ty 507.993
```

#### Git conflict handling to keep full local version
```bash
git stash
git checkout Bang
git stash pop
git add -A
git commit -m "Keep full local version of IN2354 directory and resolve all stash merge conflicts"
git push --force origin Bang
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