/*
"Hello Triangle". Just the basics + shaders + 2 buffers
Dependencies:
GLEW and GLFW3 to start GL.
Based on OpenGL 4 Example Code.
*/

#include "gl_utils.h"		// utility stuff discussed in previous tutorials is here
#include <GL/glew.h>		// include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#define GL_LOG_FILE "gl.log"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow *g_window = NULL;

float camX = 2.0f;
float camY = 2.0f;
float camZ = 2.0f;

float speed = 0.001;

void keysPress()
{
	if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(g_window, 1);
	}

	if (glfwGetKey(g_window, GLFW_KEY_W) == GLFW_PRESS) 
	{
		camZ += speed;
	}
	if (glfwGetKey(g_window, GLFW_KEY_S) == GLFW_PRESS) 
	{
		camZ -= speed;
	}
	if (glfwGetKey(g_window, GLFW_KEY_A) == GLFW_PRESS) 
	{
		camX -= speed;
	}
	if (glfwGetKey(g_window, GLFW_KEY_D) == GLFW_PRESS) 
	{
		camX += speed;
	}
	if (glfwGetKey(g_window, GLFW_KEY_UP) == GLFW_PRESS) 
	{
		camY += speed;
	}
	if (glfwGetKey(g_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camY -= speed;
	}
}

int main() {
	restart_gl_log();
	// all the start-up code for GLFW and GLEW is called here
	start_gl();

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

	//criando vertices e cores numa s� estrutura (x, y, z, r, g, b)
	//GLfloat vertices[] = { 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	//	0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	//	-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	//	0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, };

	GLfloat vertices[] = {
		0.0f, 0.5f, 0.0f, 
					1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 
					1.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, 0.0f, 
					1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 
					1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.65f,
					0.0f, 0.0f, 1.0f,
		0.5f, 0.0f, -0.65f,
					0.0f, 0.0f, 1.0f,
		-0.5f, 0.0f, -0.65f,
					0.0f, 0.0f, 1.0f,
		0.0f, -0.5f, -0.65f,
					0.0f, 0.0f, 1.0f
	};

	//0 � o primeiro v�rtice (topo do triangulo) e 3 � o ponto adicional para fazer um triangulo invertido
	//ordem para renderiza��o, sempre em sentido hor�rio.
	GLint indice[] = { 0, 1, 2, 2, 1, 3, 
						4, 6, 5, 5, 6, 7,
						0, 4, 1, 1, 4, 5,
						3, 1, 7, 7, 1, 5,
						2, 3, 6, 6, 3, 7,
						6, 0, 2, 6, 4, 0
						};

	//criando vari�vel trans com rota��o, escala e transla��o.
	//glm::mat4 trans;
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	//trans = glm::translate(trans, glm::vec3(0.3f, -0.3f, 0.0f));

	//apenas um vbo
	GLuint points_vbo;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* create a second VBO, containing the array of colours.
	note that we could also put them both into a single vertex buffer. in this
	case we would use the pointer and stride parameters of glVertexAttribPointer()
	to describe the different data layouts */

	//GLuint colours_vbo;
	//glGenBuffers( 1, &colours_vbo );
	//glBindBuffer( GL_ARRAY_BUFFER, colours_vbo );
	//glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), indice, GL_STATIC_DRAW);

	/* create the VAO.
	we bind each VBO in turn, and call glVertexAttribPointer() to indicate where
	the memory should be fetched for vertex shader input variables 0, and 1,
	respectively. we also have to explicitly enable both 'attribute' variables.
	'attribute' is the older name for vertex shader 'in' variables. */
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);

	//usando somente um vbo e definindo o uso dos dados.
	//glBindBuffer( GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	char vertex_shader[1024 * 256];
	char fragment_shader[1024 * 256];
	parse_file_into_str("test_vs.glsl", vertex_shader, 1024 * 256);
	parse_file_into_str("test_fs.glsl", fragment_shader, 1024 * 256);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *p = (const GLchar *)vertex_shader;
	glShaderSource(vs, 1, &p, NULL);
	glCompileShader(vs);

	// check for compile errors
	int params = -1;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs);
		print_shader_info_log(vs);
		return 1; // or exit or something
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	p = (const GLchar *)fragment_shader;
	glShaderSource(fs, 1, &p, NULL);
	glCompileShader(fs);

	// check for compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", fs);
		print_shader_info_log(fs);
		return 1; // or exit or something
	}

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	glGetProgramiv(shader_programme, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: could not link shader programme GL index %i\n",
			shader_programme);
		print_programme_info_log(shader_programme);
		return false;
	}

	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK);		// cull back face
	glFrontFace(GL_CW);			// GL_CCW for counter clock-wise

	while (!glfwWindowShouldClose(g_window)) {
		_update_fps_counter(g_window);
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_gl_width, g_gl_height);

		glUseProgram(shader_programme);

		//aplicando as transforma��es.
		unsigned int transformLoc = glGetUniformLocation(shader_programme, "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
		// retrieve the matrix uniform locations
		unsigned int modelLoc = glGetUniformLocation(shader_programme, "model");
		unsigned int viewLoc = glGetUniformLocation(shader_programme, "view");
		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// camera/view transformation
		//float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader_programme, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader_programme, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));

		//usando ebo!!
		glDrawElements(GL_TRIANGLES, sizeof(indice), GL_UNSIGNED_INT, indice);

		// update other events like input handling
		glfwPollEvents();
		keysPress();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}