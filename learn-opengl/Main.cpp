#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

void frameBufferSizeCallback(GLFWwindow*, int, int);
void processInput(GLFWwindow* window);

//������ɫ��
const char* VERTEX_SHADER_SOURCE =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

//Ƭ����ɫ��
const char* FRAGMENT_SHADER_SOURCE =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";

int main()
{
	//glfw��ʼ��
	glfwInit();
	//��֪glfw opengl��ʹ�õİ汾�ţ������ڴ���context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//��֪glfw openglʹ�õ��Ǻ��İ汾
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "failed to create GLFW window." << std::endl;
		//�ͷ���ǰ����õ���Դ
		glfwTerminate();
		return -1;
	}

	//�����ڶ�������Ϊ��ǰ�̵߳������Ķ���
	glfwMakeContextCurrent(window);

	//��ʼ��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD." << std::endl;
		glfwTerminate();
		return -1;
	}

	//ע�ᴰ�ڳߴ�仯����
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	int success = 0;
	char infoLog[512] = { '\0' };

	GLuint vertexShader, fragmentShader, shaderProgram;
	//����������ɫ��
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//�󶨶�����ɫ��Դ��
	glShaderSource(vertexShader, 1, &VERTEX_SHADER_SOURCE, NULL);
	//���붥����ɫ��
	glCompileShader(vertexShader);

	//��ȡ������ɫ��������
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//�������ʧ�ܣ������������Ϣ
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILE_FAILED\n" << infoLog << std::endl;
	}

	//����Ƭ����ɫ��
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//��Ƭ����ɫ��Դ��
	glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER_SOURCE, NULL);
	//����Ƭ����ɫ��
	glCompileShader(fragmentShader);
	//��ȡƬ����ɫ��������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILE_FAILED\n" << infoLog << std::endl;
	}

	//������ɫ������
	shaderProgram = glCreateProgram();
	//��������ɫ���������ɫ������
	glAttachShader(shaderProgram, vertexShader);
	//��Ƭ����ɫ���������ɫ������
	glAttachShader(shaderProgram, fragmentShader);
	//����ɫ������
	glLinkProgram(shaderProgram);
	//��ȡ���ӽ��
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
	//��ɫ������ɫ�����������ӳɹ��󣬿�ɾ��ԭ�е���ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//���������ζ�������
	//float vertices[] = {
	//	-0.5f,-0.5f,0f,
	//	0.5f,-0.5f,0f,
	//	0.0f,0.5f,0f
	//};

	//���嶥��ԭʼ����
	float vertices[] = {
		-0.5f, 0.5f, 0,
		0.5f, 0.5f, 0,
		0.5f, -0.5f, 0,
		-0.5f, -0.5f, 0,
	};

	//��������Դ������
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	GLuint VAO, VB0, EBO;
	//���������������
	glGenVertexArrays(1, &VAO);
	//�󶨶����������
	glBindVertexArray(VAO);

	//�������㻺�����
	glGenBuffers(1, &VB0);
	//�󶨶��㻺�����
	glBindBuffer(GL_ARRAY_BUFFER, VB0);
	//��ԭʼ�������ݿ������Դ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//���������������
	glGenBuffers(1, &EBO);
	//�������������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//��ԭʼ�������ݿ������Դ���
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//����Opengl��ν����������ݣ������Ӷ������ݵ����������ϣ�location = 0��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	//����location = 0�Ķ�������
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //���壺VAO��洢element��bind�¼�
	glBindVertexArray(0);
	//glVertexAttribPointer()����ִ����ɺ󣬿ɰ�ȫ��󶥵㻺�����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//���ֳ�������У�ֱ���û������˳�
	while (!glfwWindowShouldClose(window))
	{
		//���봦����
		processInput(window);

		//��Ⱦ��ʼ

		//����������ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//����
		glClear(GL_COLOR_BUFFER_BIT);
		//�����߿�ģʽ
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //������ȫ����ģ��
		//ʹ����ǰ������ɵ���ɫ������
		glUseProgram(shaderProgram);
		//�󶨶����������
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//������
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//������ɫ����
		glfwSwapBuffers(window);
		//������һ���¼�
		glfwPollEvents();
	}

	//�Ƴ������������
	glDeleteBuffers(1, &EBO);
	//�Ƴ����㻺�����
	glDeleteBuffers(1, &VB0);
	//�Ƴ������������
	glDeleteVertexArrays(1, &VAO);
	//�Ƴ���ɫ������
	glDeleteProgram(shaderProgram);

	glfwTerminate();

	return 0;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	//����gl��Ⱦ�Ӵ�λ�����С
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window)
{
	//���ESC�����Ƿ񱻰���
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		//֪ͨ���ڽ����ر�
		glfwSetWindowShouldClose(window, true);
	}
}
