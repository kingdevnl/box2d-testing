#pragma once
#include <iostream>
#include <spdlog/spdlog.h>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>




struct Init
{
	GLFWwindow* window;
	glm::vec2 window_size;
};

void Initialize();
void OnUpdate(float ts);
void Shutdown();