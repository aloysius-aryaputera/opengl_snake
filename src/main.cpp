#include <ctime>
#include <iostream>
#include <math.h>
#include "../include/glad/glad.h"
#include "../include/learnopengl/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include "board.h"

float mixValue = 0.2;

void framebuffer_size_callback(
	GLFWwindow* window, int width, int height
) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (mixValue < 0.999) {
			mixValue += 1E-2;
		}
	}
	
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (mixValue > 0.001) {
			mixValue -= 1E-2;
		}
	}
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(
		800, 600, "LearnOpenGL", NULL, NULL
	);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	Shader myShader = Shader("shader.vs", "shader.fs");

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ===================================================================
	// Set up vertex data
	// ===================================================================

	float vertices[] = {
		// positions		// colors		// texture coords
		 0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	2.0f, 2.0f,
		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	2.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f
	};

	unsigned int firstIndices[] = {
		0, 1, 2
	};

	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	// Triangle setup
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(firstIndices), firstIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	myShader.use();

	while(!glfwWindowShouldClose(window)) {
		time_t now = time(0);

		processInput(window);

		glClearColor(0.1 * (now % 10), 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(
			trans, (float)glfwGetTime(), 
			glm::vec3(0.0f, 0.0f, 1.0f));

		myShader.setFloat("mixValue", mixValue);
		myShader.setFloat("horizontal_offset", -0.0);		
		myShader.setMat4("transform", trans);
		myShader.use();

		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glm::mat4 trans_2 = glm::mat4(1.0f);
		float scaling_factor = sin((float)glfwGetTime());
		trans_2 = glm::translate(trans_2, glm::vec3(-0.5f, 0.5f, 0.0f));
		trans_2 = glm::scale(
			trans_2, scaling_factor * glm::vec3(1.0f));
		myShader.setMat4("transform", trans_2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}
