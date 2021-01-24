#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Shader.h"
#include <iostream>

void frameBufferSizeCallback(GLFWwindow*, int, int);
void processInput(GLFWwindow* window);

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

	Shader shader("./shader.vs", "./shader.fs");

	//���嶥��ԭʼ����
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f	
	};

	//��������Դ������
	unsigned int indices[] = {
		0, 1, 2,
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//����location = 0�Ķ�������
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //������ȫ����ģ��
		//ʹ����ǰ������ɵ���ɫ������
		shader.Use();
		float movePos = cos((float)glfwGetTime());
		shader.SetFloat("offset", movePos * 0.5f);
		//�󶨶����������
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//������
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

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

	glfwTerminate();

	return 0;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	//����gl��Ⱦ�Ӵ�λ�����С
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	//���ESC�����Ƿ񱻰���
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		//֪ͨ���ڽ����ر�
		glfwSetWindowShouldClose(window, true);
	}
}
