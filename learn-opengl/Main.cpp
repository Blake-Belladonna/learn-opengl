#define STB_IMAGE_IMPLEMENTATION
#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 800.0f

#include "stb_image.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

void frameBufferSizeCallback(GLFWwindow*, int, int);
void processInput(GLFWwindow* window);

int main()
{
	//glfw初始化
	glfwInit();
	//告知glfw opengl所使用的版本号，以用于创建context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//告知glfw opengl使用的是核心版本
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, "OpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "failed to create GLFW window." << std::endl;
		//释放先前分配好的资源
		glfwTerminate();
		return -1;
	}

	//将窗口对象设置为当前线程的上下文对象
	glfwMakeContextCurrent(window);

	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD." << std::endl;
		glfwTerminate();
		return -1;
	}

	//注册窗口尺寸变化监听
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	Shader shader("./shader.vs", "./shader.fs");

	//定义顶点原始数据
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLuint VAO, VB0;
	//创建顶点数组对象
	glGenVertexArrays(1, &VAO);
	//绑定顶点数组对象
	glBindVertexArray(VAO);

	//创建顶点缓冲对象
	glGenBuffers(1, &VB0);
	//绑定顶点缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, VB0);
	//将原始顶点数据拷贝至显存中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//告诉Opengl如何解析顶点数据，并链接顶点数据到顶点属性上（location = 0）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//激活location = 0的顶点属性
	glEnableVertexAttribArray(0);
	//配置纹理坐标指针
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //陷阱：VAO会存储element的bind事件
	glBindVertexArray(0);
	//glVertexAttribPointer()函数执行完成后，可安全解绑顶点缓冲对象和索引缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//创建两个纹理对象
	GLuint textures[2];
	glGenTextures(sizeof(textures) / sizeof(textures[0]), textures);

	//配置0号纹理对象
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	//纹理环绕方式：纹理坐标超出范围后，以重复采样的模式继续采样原图(GL_REPEAT/GL_MIRRORED_REPEAT/GL_CLAMP_TO_EDGE/GL_CLAMP_TO_BORDER)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//纹理过滤：纹理坐标在采样时以线性插值的方式来获取纹理颜色(GL_NEAREST/GL_LINEAR/GL_X_MIPMAP_X)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//如果选用GL_CLAMP_TO_BORDER环绕方式，请额外设置环绕颜色
	//const float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//告诉stb_image.h加载图像时将图像Y轴进行翻转
	stbi_set_flip_vertically_on_load(true);
	int imageWidth, imageHeight, imageChannels;
	//使用stb_image.h来加载外部图像数据
	unsigned char* imageData = stbi_load("wall.jpg", &imageWidth, &imageHeight, &imageChannels, 0);
	if (imageData)
	{
		//生成GL纹理在当前绑定的纹理对象上
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		//生成当前纹理的MIPMAP数据
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE[0]" << std::endl;
	}

	//释放多余的图像内存
	stbi_image_free(imageData);
	//解绑当前纹理对象
	glGenTextures(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	imageData = stbi_load("awesomeface.png", &imageWidth, &imageHeight, &imageChannels, 0);
	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE[1]" << std::endl;
	}
	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);

	shader.Use();
	shader.SetInt("texture0", GL_TEXTURE0 - GL_TEXTURE0);
	shader.SetInt("texture1", GL_TEXTURE1 - GL_TEXTURE0);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//保持程序的运行，直至用户请求退出
	while (!glfwWindowShouldClose(window))
	{
		//输入处理函数
		processInput(window);

		//渲染开始

		//设置清屏颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//清屏
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//启动线框模式
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //启动完全绘制模型
		//激活纹理单元 0
		glActiveTexture(GL_TEXTURE0); //关联纹理单元与纹理对象
		//绑定纹理对象 0
		glBindTexture(GL_TEXTURE_2D, textures[0]); //关联纹理单元与纹理对象
		//激活纹理单元 1
		glActiveTexture(GL_TEXTURE1);
		//绑定纹理对象 1
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		//使用着色器程序
		shader.Use();
		//绑定顶点数组对象
		glBindVertexArray(VAO);

		size_t cubeCount = sizeof(cubePositions) / sizeof(cubePositions[0]);
		for (size_t i = 0; i < cubeCount; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f * (i + 1)), glm::vec3(0.5f, 1.0f, 0.0f));
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
			projection = glm::perspective(45.0f, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);
			shader.SetMatrix4("model", glm::value_ptr(model));
			shader.SetMatrix4("view", glm::value_ptr(view));
			shader.SetMatrix4("projection", glm::value_ptr(projection));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//交换颜色缓冲
		glfwSwapBuffers(window);
		//触发新一轮事件
		glfwPollEvents();
	}

	//移除顶点缓冲对象
	glDeleteBuffers(1, &VB0);
	//移除顶点数组对象
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();

	return 0;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	//设置gl渲染视窗位置与大小
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	//检测ESC按键是否被按下
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		//通知窗口将被关闭
		glfwSetWindowShouldClose(window, true);
	}
}
