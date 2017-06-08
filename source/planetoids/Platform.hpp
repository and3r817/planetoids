#pragma once

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

// GLM
//#include <glm/setup.hpp>
#define GLM_SWIZZLE
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

// GLEW
#include <GL/glew.h>
#include <GL/wglew.h>

// GLFW
#define GLFW_DLL
#include <GL/glfw.h>

// Math consts
#define M_PI        3.14159265358979323846264338327950288
#define M_PI_2      1.57079632679489661923132169163975144
