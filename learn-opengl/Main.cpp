#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

void frameBufferSizeCallback(GLFWwindow*, int, int);
void processInput(GLFWwindow* window);

//顶点着色器
const char* VERTEX_SHADER_SOURCE =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

//片段着色器
const char* FRAGMENT_SHADER_SOURCE =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";

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
	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL", NULL, NULL);
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

	int success = 0;
	char infoLog[512] = { '\0' };

	GLuint vertexShader, fragmentShader, shaderProgram;
	//创建顶点着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//绑定顶点着色器源码
	glShaderSource(vertexShader, 1, &VERTEX_SHADER_SOURCE, NULL);
	//编译顶点着色器
	glCompileShader(vertexShader);

	//获取顶点着色器编译结果
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//如果编译失败，则输出错误信息
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILE_FAILED\n" << infoLog << std::endl;
	}

	//创建片段着色器
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//绑定片段着色器源码
	glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER_SOURCE, NULL);
	//编译片段着色器
	glCompileShader(fragmentShader);
	//获取片段着色器编译结果
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILE_FAILED\n" << infoLog << std::endl;
	}

	//创建着色器程序
	shaderProgram = glCreateProgram();
	//将顶点着色器添加至着色器程序
	glAttachShader(shaderProgram, vertexShader);
	//将片段着色器添加至着色器程序
	glAttachShader(shaderProgram, fragmentShader);
	//将着色器链接
	glLinkProgram(shaderProgram);
	//获取链接结果
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
	//着色器在着色器程序中链接成功后，可删除原有的着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//定义三角形定点数据
	//float vertices[] = {
	//	-0.5f,-0.5f,0f,
	//	0.5f,-0.5f,0f,
	//	0.0f,0.5f,0f
	//};

	//定义顶点原始数据
	float vertices[] = {
		-0.5f, 0.5f, 0,
		0.5f, 0.5f, 0,
		0.5f, -0.5f, 0,
		-0.5f, -0.5f, 0,
	};

	//定义索引源氏数据
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	GLuint VAO, VB0, EBO;
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

	//创建索引缓冲对象
	glGenBuffers(1, &EBO);
	//绑定索引缓冲对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//将原始索引数据拷贝至显存中
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//告诉Opengl如何解析顶点数据，并链接顶点数据到顶点属性上（location = 0）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	//激活location = 0的顶点属性
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //陷阱：VAO会存储element的bind事件
	glBindVertexArray(0);
	//glVertexAttribPointer()函数执行完成后，可安全解绑顶点缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//保持程序的运行，直至用户请求退出
	while (!glfwWindowShouldClose(window))
	{
		//输入处理函数
		processInput(window);

		//渲染开始

		//设置清屏颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//清屏
		glClear(GL_COLOR_BUFFER_BIT);
		//启动线框模式
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //启动完全绘制模型
		//使用先前链接完成的着色器程序
		glUseProgram(shaderProgram);
		//绑定顶点数组对象
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//画矩形
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//交换颜色缓冲
		glfwSwapBuffers(window);
		//触发新一轮事件
		glfwPollEvents();
	}

	//移除索引缓冲对象
	glDeleteBuffers(1, &EBO);
	//移除顶点缓冲对象
	glDeleteBuffers(1, &VB0);
	//移除顶点数组对象
	glDeleteVertexArrays(1, &VAO);
	//移除着色器程序
	glDeleteProgram(shaderProgram);

	glfwTerminate();

	return 0;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	//设置gl渲染视窗位置与大小
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window)
{
	//检测ESC按键是否被按下
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		//通知窗口将被关闭
		glfwSetWindowShouldClose(window, true);
	}
}
