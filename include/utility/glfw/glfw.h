#pragma once

// wrapper file for including GLFW so that this GLFW_INCLUDE_NONE macro is always defined. 
// needed so that the order of GLFW and GLAD includes does not matter

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
