#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <utility>
#include <glm/glm.hpp>
#include <string>
#include <fstream>

struct Buffer {
	unsigned int VAO, VBO, EBO;
};

struct Vertex {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> weights;
	std::vector<glm::vec3> texCoord;
	std::vector<unsigned int> indices;
	size_t vertexSize;
	size_t weightSize;
	size_t textureSize;
	size_t indicesSize;
	size_t totalSize;
};

class SOM {
public:
	SOM(int width, int height, int wnd_width, int wnd_height, int Ninteration);
	void Render();
	bool Train();
	void Test();
	void TestDraw();
	void ReadTestData(std::string filePath);
	void ReadlatticeData(std::string filePath);
	unsigned int GetVAO() { return buffer.VAO; }
	bool isFinished() { return isDone; }
	bool cFlag = false;
private:
	//the nodes number would be width * height
	int m_width, m_height, wnd_width, wnd_height;
	struct Buffer buffer, tbuffer;
	unsigned int VAO2, VBO2, EBO2;
	int epoch, m_NumIter;
	//(x, y, z, r, g, b) -> the (r,g,b) would be the weight of each node
	//this double pointer is used for rendering (VAO VBO EBO stuff like that)
	struct Vertex m_data;
	//double* vertex;
	//store original vertex data
	std::vector<glm::vec3> originalVertex;
	//EBO indices
	unsigned int/** indices,*/ *indices2;
	//The color data you want to cluster
	struct Vertex m_TrainingData;
	//This would be a size of six double array (x, y, z, r, g, b)
	//double* m_WinnerPoint;
	glm::vec3 m_WinnerPoint;
	//The tracker of training
	bool isDone;
	//The Radius of the feature map, it's half of the max(width, height)
	double m_MapRadius;
	//variable of calculation 
	double m_TimeConstant;
	//current winner node's radius 
	double m_NeighborhoodRadius;
	//the coefficient of the weight get influence
	double m_Influence;
	double m_LearningRate;
	const double m_ConstStartLearningRate = 0.1f;
	//Calculate the Best Match Unit, input is (r,g,b) value
	void FindBMU(glm::vec3& DataValue);
	//Get the distance between Node and Training Data Set
	double CalcDist(const glm::vec3& NodeData, const glm::vec3& DataValue);
	//update node's (r,g,b) value
	void UpdateWeight(glm::vec3& nData, const glm::vec3& target);
	void CreateTrainingData();
	void initNodeData();
	const std::vector<std::string> split(const std::string& str, const std::string& pattern);
};