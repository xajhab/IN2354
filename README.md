# IN2354
---

## Directory Structure

```
IN2354/
├── Exercise-1/ # RGB-D mesh reconstruction
├── Exercise-2/ # Surface reconstruction
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

##  Visual Results 
### Exercise 2
| Method | Visualization           |
| ------ | ----------------------- |
| Sphere | ![](./Media/sphere.png) |
| Torus  | ![](./Media/torus.png)  |
| Hoppe  | ![](./Media/hoppe.png)  |
| RBF    | ![](./Media/rbf.png)    |
