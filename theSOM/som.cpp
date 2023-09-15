#include "som.h"

float Pi = 3.14159;

SOM::SOM(int width, int height, int wnd_width, int wnd_height, int Niteration)
	: m_width(width), m_height(height), wnd_width(wnd_width), wnd_height(wnd_height), m_NumIter(Niteration)
{
	srand(time(NULL));
	epoch = 0;
	isDone = false;
	m_MapRadius = std::max((double)(m_width), (double)(m_height)) / 2.0f;
	std::cout << "MapRadius : " << m_MapRadius << std::endl;
	m_TimeConstant = m_NumIter / log2(m_MapRadius);
	m_NeighborhoodRadius = 0;
	m_Influence = 0;
	m_LearningRate = m_ConstStartLearningRate;

	//CreateTrainingData();
	initNodeData();
	//ReadlatticeData("objfiles\\cylinder.obj");
}

void SOM::CreateTrainingData()
{
	if (cFlag)
	{
		//create a data set
		glm::vec3 red, green, blue, yellow, orange, purple, dk_green, dk_blue;

		red = glm::vec3(1, 0, 0);

		green = glm::vec3(0, 1, 0);

		dk_green = glm::vec3(0, 0.5, 0.25);

		blue = glm::vec3(0, 0, 1);

		dk_blue = glm::vec3(0, 0, 0.5);

		yellow = glm::vec3(1, 1, 0.2);

		orange = glm::vec3(1, 0.4, 0.25);

		purple = glm::vec3(1, 0, 1);

		
		m_TrainingData.weights.push_back(red);
		m_TrainingData.weights.push_back(green);
		m_TrainingData.weights.push_back(blue);
		m_TrainingData.weights.push_back(yellow);
		m_TrainingData.weights.push_back(orange);
		m_TrainingData.weights.push_back(purple);
		m_TrainingData.weights.push_back(dk_green);
		m_TrainingData.weights.push_back(dk_blue);
	}
	else
	{
		for (int i = 0; i < 16; i++)
		{
			double rcoef = 0.5 * (i % 4);
			double ccoef = 0.5 * (i / 4);
			m_TrainingData.weights.push_back(glm::vec3(-0.75 + rcoef, 0.75 - ccoef, 0.0f));
		}
		/*for (int i = 0; i < 8; i++)
		{
			std::vector<double> tmp;
			tmp.push_back((rand() % 256 / 255.0f));
			tmp.push_back((rand() % 256 / 255.0f));
			tmp.push_back((rand() % 256 / 255.0f));
			m_TrainingDataSet.push_back(tmp);
		}*/
	}
}

void SOM::initNodeData()
{
	int pAngle = 360 / (m_width - 1);
	float circumference = (m_width - 1), r;
	r = circumference / (2 * Pi);
	std::cout << pAngle << std::endl;
	for (int i = 0, x = 0, y = 0, idx = 0; i < m_width * m_height; i ++, x++, idx += 6)
	{
		if (x == m_width)
		{
			x = 0;
			y += 1;
		}
		float rd, gd, bd;
		/*rd = (rand() % 256) / 255.0f;
		gd = (rand() % 256) / 255.0f;
		bd = (rand() % 256) / 255.0f;*/
		rd = r * cos((pAngle * x) * Pi / 180);
		gd = y;
		bd = r * sin((pAngle * x) * Pi / 180);
		//bd = 1.0f;
		m_data.vertices.push_back(glm::vec3(x, y, 0.0f));
		m_data.weights.push_back(glm::vec3(rd, gd, bd));
		//originalVertex.push_back(glm::vec3(x, y, 0));
	}
	for (int n = 0, pos = 0, idx = 0; n < (m_width - 1) * (m_height - 1); n++, pos++, idx += 6)
	{
		if (pos % m_width == m_width - 1)
		{//	hit the width edge, move on to the next row
			pos++;
		}
		m_data.indices.push_back(pos);
		m_data.indices.push_back(pos + 1);
		m_data.indices.push_back(pos + m_height);
		m_data.indices.push_back(pos + m_height + 1);
		m_data.indices.push_back(pos + m_height);
		m_data.indices.push_back(pos + 1);
	}

	m_data.vertexSize = m_data.vertices.size() * sizeof(glm::vec3);
	m_data.weightSize = m_data.weights.size() * sizeof(glm::vec3);
	m_data.indicesSize = m_data.indices.size() * sizeof(unsigned int);
	m_data.totalSize = m_data.vertexSize + m_data.weightSize;

	glGenVertexArrays(1, &buffer.VAO);
	glBindVertexArray(buffer.VAO);

	glGenBuffers(1, &buffer.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
	glBufferData(GL_ARRAY_BUFFER, m_data.weightSize, m_data.weights.data(), GL_STATIC_DRAW);
	/*glBufferData(GL_ARRAY_BUFFER, m_data.totalSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_data.vertexSize, m_data.vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, m_data.vertexSize, m_data.colorSize, m_data.weights.data());*/

	glGenBuffers(1, &buffer.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data.indicesSize, m_data.indices.data(), GL_STATIC_DRAW);

	GLint  size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	//fprintf(stderr, "size=%d\n", size);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(m_data.vertexSize));
	//glEnableVertexAttribArray(1);

}

void SOM::Test()
{

}

void SOM::TestDraw()
{
	glBindVertexArray(tbuffer.VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, m_TrainingData.indices.size(), GL_UNSIGNED_INT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SOM::Render()
{
	glBindVertexArray(buffer.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
	/*glBufferData(GL_ARRAY_BUFFER, m_data.totalSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_data.vertexSize, m_data.vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, m_data.vertexSize, m_data.weightSize, m_data.weights.data());*/
	glBufferData(GL_ARRAY_BUFFER, m_data.weightSize, m_data.weights.data(), GL_STATIC_DRAW);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, m_data.indices.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool SOM::Train()
{
	if (isDone)
		return isDone;
	if (m_NumIter-- > 0)
	{
		//std::cout << "epoch : " << epoch << std::endl;
		int randIdx = rand() % m_TrainingData.weights.size();
		FindBMU(m_TrainingData.weights[randIdx]);
		m_NeighborhoodRadius = m_MapRadius * exp(-(double)epoch / m_TimeConstant);
		
		for (int i = 0, idx = 0; i < m_width * m_height; i++, idx += 6)
		{
			/*double dist = pow((m_WinnerPoint.x - m_data.vertices[i].x), 2)
				+ pow((m_WinnerPoint.y - m_data.vertices[i].y), 2)
				+ pow((m_WinnerPoint.z - m_data.vertices[i].z), 2);
				
				this is for plane
				*/

			/*cylinder version*/
			double dist, diff;
			diff = fabs(m_WinnerPoint.x - m_data.vertices[i].x);
			if (diff > ((m_width - 1) / 2)) {
				dist = pow((m_width - 1) - (m_WinnerPoint.x - m_data.vertices[i].x), 2)
					+ pow((m_WinnerPoint.y - m_data.vertices[i].y), 2)
					+ pow((m_WinnerPoint.z - m_data.vertices[i].z), 2);
			}
			else
			{
				dist = pow((m_WinnerPoint.x - m_data.vertices[i].x), 2)
					+ pow((m_WinnerPoint.y - m_data.vertices[i].y), 2)
					+ pow((m_WinnerPoint.z - m_data.vertices[i].z), 2);
			}

			double widthSquare = m_NeighborhoodRadius * m_NeighborhoodRadius;

			if (dist < (widthSquare)) {
				m_Influence = exp(-1 * (dist) / (2 * widthSquare));
				glm::vec3 tmp = m_data.weights[i];
				UpdateWeight(tmp, m_TrainingData.weights[randIdx]);
				m_data.weights[i] = tmp;
			}
		}
		
		m_LearningRate = m_ConstStartLearningRate * exp(-(double)epoch / m_NumIter);

		epoch++;
	}
	else
	{
		isDone = true;
	}
	return true;
}

void SOM::FindBMU(glm::vec3& DataValue)
{
	double distance = DBL_MAX, x = 0, y = 0, z = 0, r = 0, g = 0, b = 0;
	for (int i = 0, idx = 0; i < m_width * m_height; i++, idx += 6)
	{
		double dist = CalcDist(m_data.weights[i], DataValue);
		if (dist < distance) {
			distance = dist;
			x = m_data.vertices[i].x;
			y = m_data.vertices[i].y;
			z = m_data.vertices[i].z;
		}
	}
	m_WinnerPoint.x = x;
	m_WinnerPoint.y = y;
	m_WinnerPoint.z = z;
	return;
}

double SOM::CalcDist(const glm::vec3& NodeData, const glm::vec3& DataValue)
{
	double dist = 0;
	for (int i = 0; i < 3; i++) {
		dist += pow((NodeData[i] - DataValue[i]), 2);
	}
	return dist;
}

void SOM::UpdateWeight(glm::vec3& nData, const glm::vec3& target)
{
	for (int i = 0; i < 3; i++)
	{
		nData[i] += m_LearningRate * m_Influence * (target[i] - nData[i]);
	}
}

void SOM::ReadlatticeData(std::string filePath)
{
	std::ifstream ifs;
	ifs.open(filePath);
	if (!ifs.is_open())
	{
		std::cout << "lattice obj open failed\n";
		return;
	}
	else
	{
		std::string s;
		while (std::getline(ifs, s))
		{
			//std::cout << s << std::endl;
			std::vector<std::string> tmp = this->split(s, " ");
			if (tmp[0] != "f" && tmp[0] != "v")
				continue;
			if (tmp[0] == "v") {
				float x, y, z;
				x = std::stof(tmp[1]);
				y = std::stof(tmp[2]);
				z = std::stof(tmp[3]);
				m_TrainingData.vertices.push_back(glm::vec3(x, y, z));
				m_data.weights.push_back(glm::vec3(x, y, z));
				//originalVertex.push_back(glm::vec3(x, y, z));
			}
			else if (tmp[0] == "f")
			{
				m_data.indices.push_back(std::stoi(tmp[1]) - 1);
				m_data.indices.push_back(std::stoi(tmp[2]) - 1);
				m_data.indices.push_back(std::stoi(tmp[3]) - 1);
			}
		}
		ifs.close();

		m_data.weightSize = m_data.weights.size() * sizeof(glm::vec3);
		m_data.indicesSize = m_data.indices.size() * sizeof(unsigned int);

		glGenVertexArrays(1, &buffer.VAO);
		glBindVertexArray(buffer.VAO);

		glGenBuffers(1, &buffer.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
		glBufferData(GL_ARRAY_BUFFER, m_data.weightSize, m_data.weights.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &buffer.EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data.indicesSize, m_data.indices.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);
	}
	return;
}

void SOM::ReadTestData(std::string filePath)
{
	std::ifstream ifs;
	ifs.open(filePath);
	if (!ifs.is_open())
	{
		std::cout << "open failed\n";
		return;
	}
	else
	{
		std::string s;
		while (std::getline(ifs, s))
		{
			//std::cout << s << std::endl;
			std::vector<std::string> tmp = this->split(s, " ");
			if (tmp[0] != "f" && tmp[0] != "v")
				continue;
			if (tmp[0] == "v") {
				float x, y, z;
				x = std::stof(tmp[1]);
				y = std::stof(tmp[2]);
				z = std::stof(tmp[3]);
				//m_TrainingData.vertices.push_back(glm::vec3(x, y, z));
				m_TrainingData.weights.push_back(glm::vec3(x, y, z));
			}
			else if (tmp[0] == "f")
			{
				m_TrainingData.indices.push_back(std::stoi(tmp[1]) - 1);
				m_TrainingData.indices.push_back(std::stoi(tmp[2]) - 1);
				m_TrainingData.indices.push_back(std::stoi(tmp[3]) - 1);
			}
		}
		ifs.close();

		m_TrainingData.weightSize = m_TrainingData.weights.size() * sizeof(glm::vec3);
		m_TrainingData.indicesSize = m_TrainingData.indices.size() * sizeof(unsigned int);

		glGenVertexArrays(1, &tbuffer.VAO);
		glBindVertexArray(tbuffer.VAO);

		glGenBuffers(1, &tbuffer.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, tbuffer.VBO);
		glBufferData(GL_ARRAY_BUFFER, m_TrainingData.weightSize, m_TrainingData.weights.data(), GL_STATIC_DRAW);
	
		glGenBuffers(1, &tbuffer.EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tbuffer.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_TrainingData.indicesSize, m_TrainingData.indices.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);
	}
	return;
}

const std::vector<std::string> SOM::split(const std::string& str, const std::string& pattern)
{
	std::vector<std::string> result;
	std::string::size_type begin, end;

	end = str.find(pattern);
	begin = 0;

	while (end != std::string::npos)
	{
		if (end - begin != 0)
		{
			result.push_back(str.substr(begin, end - begin));
		}
		begin = end + pattern.size();
		end = str.find(pattern, begin);
	}

	if (begin != str.length()) {
		result.push_back(str.substr(begin));
	}

	return result;
}