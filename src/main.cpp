#include <chrono>
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

void processInput(GLFWwindow *window, Board* board) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		board -> change_snake_direction(3);
	}
	
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		board -> change_snake_direction(1);
	}

	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		board -> change_snake_direction(0);
	}
	
	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		board -> change_snake_direction(2);
	}
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ===================================================================
	// Set up window
	// ===================================================================
	
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

	glViewport(0, 0, 800, 800);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ===================================================================
	// Set up vertex data
	// ===================================================================

	float vertices[] = {
		// positions		// colors		// texture coords
		 1.0f,  1.0f, 0.0f,	1.0f, 0.0f, 0.0f,	2.0f, 2.0f,
		-1.0f,  1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	2.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f
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

	// ===================================================================
	// Set up game
	// ===================================================================

	Board my_board = Board(120, 120);	
	bool allow_animation = true;
	int period = 10;
	float period_factor = 100;

	while(!glfwWindowShouldClose(window)) {
		const auto p1 = std::chrono::system_clock::now();
		int my_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                   p1.time_since_epoch()).count();
		bool animation_time = int(my_time / period_factor) % period == 0;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShader.use();

		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		processInput(window, &my_board);

		if (animation_time and allow_animation) {
			my_board.move_snake();
			allow_animation = false;
		}

		if (!animation_time) {
			allow_animation = true;
		}

		for (int i = 0; i < my_board.snake.x.size(); i++) {
			glm::mat4 trans = glm::mat4(1.0f);
			float scaling_factor = float(1.0 / my_board.x_size);
			float x = float(my_board.snake.x[i]) + .5;
			float y = float(my_board.snake.y[i]) + .5;
			float x_centre = my_board.x_size / 2;
			float y_centre = my_board.y_size / 2;
			glm::vec3 trans_vector = glm::vec3(
				(x - x_centre) * 2 * scaling_factor,
				(y - y_centre) * 2 * scaling_factor,
				0.0f
			);
			trans = glm::translate(trans, trans_vector);
			trans = glm::scale(
				trans,
				scaling_factor * glm::vec3(1.0f)
			);
			myShader.setMat4("transform", trans);	
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

			trans = glm::rotate(
				trans, float(M_PI), 
				glm::vec3(0.0f, 0.0f, 1.0f));
			myShader.setMat4("transform", trans);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

			//std::cout << x << ", " << y << "; " << x_centre << ", " << y_centre << "\n";
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}
