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
	//glfw��ʼ��
	glfwInit();
	//��֪glfw opengl��ʹ�õİ汾�ţ������ڴ���context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//��֪glfw openglʹ�õ��Ǻ��İ汾
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, "OpenGL", NULL, NULL);
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

	//����Opengl��ν����������ݣ������Ӷ������ݵ����������ϣ�location = 0��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//����location = 0�Ķ�������
	glEnableVertexAttribArray(0);
	//������������ָ��
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //���壺VAO��洢element��bind�¼�
	glBindVertexArray(0);
	//glVertexAttribPointer()����ִ����ɺ󣬿ɰ�ȫ��󶥵㻺�����������������
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//���������������
	GLuint textures[2];
	glGenTextures(sizeof(textures) / sizeof(textures[0]), textures);

	//����0���������
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	//�����Ʒ�ʽ���������곬����Χ�����ظ�������ģʽ��������ԭͼ(GL_REPEAT/GL_MIRRORED_REPEAT/GL_CLAMP_TO_EDGE/GL_CLAMP_TO_BORDER)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//������ˣ����������ڲ���ʱ�����Բ�ֵ�ķ�ʽ����ȡ������ɫ(GL_NEAREST/GL_LINEAR/GL_X_MIPMAP_X)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//���ѡ��GL_CLAMP_TO_BORDER���Ʒ�ʽ����������û�����ɫ
	//const float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//����stb_image.h����ͼ��ʱ��ͼ��Y����з�ת
	stbi_set_flip_vertically_on_load(true);
	int imageWidth, imageHeight, imageChannels;
	//ʹ��stb_image.h�������ⲿͼ������
	unsigned char* imageData = stbi_load("wall.jpg", &imageWidth, &imageHeight, &imageChannels, 0);
	if (imageData)
	{
		//����GL�����ڵ�ǰ�󶨵����������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		//���ɵ�ǰ�����MIPMAP����
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE[0]" << std::endl;
	}

	//�ͷŶ����ͼ���ڴ�
	stbi_image_free(imageData);
	//���ǰ�������
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

	//���ֳ�������У�ֱ���û������˳�
	while (!glfwWindowShouldClose(window))
	{
		//���봦����
		processInput(window);

		//��Ⱦ��ʼ

		//����������ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//����
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//�����߿�ģʽ
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //������ȫ����ģ��
		//��������Ԫ 0
		glActiveTexture(GL_TEXTURE0); //��������Ԫ���������
		//��������� 0
		glBindTexture(GL_TEXTURE_2D, textures[0]); //��������Ԫ���������
		//��������Ԫ 1
		glActiveTexture(GL_TEXTURE1);
		//��������� 1
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		//ʹ����ɫ������
		shader.Use();
		//�󶨶����������
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

		//������ɫ����
		glfwSwapBuffers(window);
		//������һ���¼�
		glfwPollEvents();
	}

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
