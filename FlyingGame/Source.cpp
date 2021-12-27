#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main()
{
	glfwInit();//initialize glfw with this function
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //configure glfw using this
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "FLIGHT SIMULATION ENGINE", NULL, NULL);
	if (window==NULL)
	{
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glad manages function pointers for OpenGL so we want to initialize glad before we call any OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	//Tell OpenGL the size of the rendering window
	
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
	}





	glfwTerminate();
	return 0;
}