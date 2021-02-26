#define STB_IMAGE_IMPLEMENTATION

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
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
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
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f
	};

	//��������ԭʼ����
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//����location = 0�Ķ�������
	glEnableVertexAttribArray(0);
	//���ö�����ɫָ��
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//������������ָ��
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //���壺VAO��洢element��bind�¼�
	glBindVertexArray(0);
	//glVertexAttribPointer()����ִ����ɺ󣬿ɰ�ȫ��󶥵㻺�����������������
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

	shader.Use();
	//����������������Ԫ
	shader.SetInt("texture0", GL_TEXTURE0 - GL_TEXTURE0);
	shader.SetInt("texture1", GL_TEXTURE1 - GL_TEXTURE0);

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

		//����4*4��λ����
		glm::mat4 trans = glm::mat4(1.0f);
		//����任λ��
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//����任��ת
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//����任����
		trans = glm::scale(trans, glm::vec3(0.6f, 0.6f, 0.6f));
		//���������������ɫ��(����)
		shader.SetMatrix4("trans", glm::value_ptr(trans));

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//������
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, NULL);

		glm::mat4 trans2 = glm::mat4(1.0f);
		trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
		float scale = (float)((cos(glfwGetTime())) * 3.0f + 3.0f) * 0.1f;
		trans2 = glm::scale(trans2, glm::vec3(scale, scale, scale));
		shader.SetMatrix4("trans", glm::value_ptr(trans2));

		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, NULL);

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
