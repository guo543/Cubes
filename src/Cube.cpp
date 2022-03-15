#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Cube.h"

int indexOfNumberLetter(std::string& str, int offset);
int lastIndexOfNumberLetter(std::string& str);
std::vector<std::string> split(const std::string& s, char delim);

Cube::Cube()
{
	vao = 0;
	vbuf = 0;
	vcount = 0;

	loadMesh("models/cube.obj");
}

Cube::~Cube()
{
}

void Cube::loadMesh(std::string filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::stringstream ss;
		ss << "Error reading " << filename << ": failed to open file";
		throw std::runtime_error(ss.str());
	}

	std::vector<glm::vec3> raw_vertices;
	std::vector<unsigned int> v_elements;

	std::string line;
	while (getline(file, line)) {
		if (line.substr(0, 2) == "v ") {
			// Read position data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			std::vector<std::string> values = split(line.substr(index1, index2 - index1 + 1), ' ');
			glm::vec3 vert(stof(values[0]), stof(values[1]), stof(values[2]));
			raw_vertices.push_back(vert);
		}
		else if (line.substr(0, 2) == "f ") {
			// Read face data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			std::vector<std::string> values = split(line.substr(index1, index2 - index1 + 1), ' ');
			for (int i = 0; i < int(values.size()) - 2; i++) {
				// Split up vertex indices
				std::string v1 = values[0];		// Triangle fan for ngons
				std::string v2 = values[i + 1];
				std::string v3 = values[i + 2];

				// Store position indices
				v_elements.push_back(stoul(v1) - 1);
				v_elements.push_back(stoul(v2) - 1);
				v_elements.push_back(stoul(v3) - 1);
				//std::cout << v1 << ", " << v2 << ", " << v3 << std::endl;
			}
		}
	}
	file.close();

	vertices = std::vector<Vertex>(v_elements.size());
	for (int i = 0; i < int(v_elements.size()); i += 3) {
		// Store positions
		vertices[i + 0].pos = raw_vertices[v_elements[i + 0]];
		vertices[i + 1].pos = raw_vertices[v_elements[i + 1]];
		vertices[i + 2].pos = raw_vertices[v_elements[i + 2]];

		// Calculate normal
		glm::vec3 normal = normalize(cross(vertices[i + 1].pos - vertices[i + 0].pos, vertices[i + 2].pos - vertices[i + 0].pos));
		vertices[i + 0].norm = normal;
		vertices[i + 1].norm = normal;
		vertices[i + 2].norm = normal;
	}
	vcount = (int)vertices.size();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(glm::vec3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::draw()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vcount);
	glBindVertexArray(0);
}

int indexOfNumberLetter(std::string& str, int offset) {
	for (int i = offset; i < int(str.length()); ++i) {
		if ((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '.') return i;
	}
	return (int)str.length();
}

int lastIndexOfNumberLetter(std::string& str) {
	for (int i = int(str.length()) - 1; i >= 0; --i) {
		if ((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '.') return i;
	}
	return 0;
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;

	std::stringstream ss(s);
	std::string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}

	return elems;
}