#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Cube.h"
#include "Util.h"

Cube::Cube()
{
	vao = 0;
	vbuf = 0;
	vcount = 0;

	loadMesh("models/cube.obj");
	loadTexture("textures/cube.jpg");
}

Cube::~Cube()
{
	vertices.clear();
	if (vao) { glDeleteVertexArrays(1, &vao); vao = 0; }
	if (vbuf) { glDeleteBuffers(1, &vbuf); vbuf = 0; }
	vcount = 0;
}

void Cube::loadMesh(std::string filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::stringstream ss;
		ss << "Error reading " << filename << ": failed to open file";
		throw std::runtime_error(ss.str());
	}
	
	std::vector<std::vector<unsigned int>> v_elements;
	std::vector<glm::vec3> raw_vertices;
	std::vector<glm::vec3> raw_normals;
	std::vector<glm::vec2> raw_uvs;

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
		else if (line.substr(0, 2) == "vt") {
			// Read texture coorindates
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			std::vector<std::string> values = split(line.substr(index1, index2 - index1 + 1), ' ');
			glm::vec2 uv(stof(values[0]), stof(values[1]));
			raw_uvs.push_back(uv);
		}
		else if (line.substr(0, 2) == "vn") {
			// Read the normals
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			std::vector<std::string> values = split(line.substr(index1, index2 - index1 + 1), ' ');
			glm::vec3 norm(stof(values[0]), stof(values[1]), stof(values[2]));
			raw_normals.push_back(norm);
		}
		else if (line.substr(0, 2) == "f ") {
			// Read face data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			std::vector<std::string> values = split(line.substr(index1, index2 - index1 + 1), ' ');
			for (int i = 0; i < int(values.size()) - 2; i++) {
				// Split up vertex indices
				std::vector<std::string> v1 = split(values[0], '/');  // Triangle fan for ngons
				std::vector<std::string> v2 = split(values[i + 1], '/');
				std::vector<std::string> v3 = split(values[i + 2], '/');

				// Store position indices
				std::vector<unsigned int> indices1, indices2, indices3;
				indices1.push_back(stoul(v1[0]) - 1);
				indices1.push_back(stoul(v1[1]) - 1);
				indices1.push_back(stoul(v1[2]) - 1);
				indices2.push_back(stoul(v2[0]) - 1);
				indices2.push_back(stoul(v2[1]) - 1);
				indices2.push_back(stoul(v2[2]) - 1);
				indices3.push_back(stoul(v3[0]) - 1);
				indices3.push_back(stoul(v3[1]) - 1);
				indices3.push_back(stoul(v3[2]) - 1);
				v_elements.push_back(indices1);
				v_elements.push_back(indices2);
				v_elements.push_back(indices3);
				//std::cout << v1 << ", " << v2 << ", " << v3 << std::endl;
			}
		}
	}
	file.close();

	vertices = std::vector<Vertex>(v_elements.size());
	for (int i = 0; i < int(v_elements.size()); i += 3) {
		// Store positions
		vertices[i + 0].pos = raw_vertices[v_elements[i + 0][0]];
		vertices[i + 1].pos = raw_vertices[v_elements[i + 1][0]];
		vertices[i + 2].pos = raw_vertices[v_elements[i + 2][0]];

		// Store normals
		vertices[i + 0].norm = raw_normals[v_elements[i + 0][2]];
		vertices[i + 1].norm = raw_normals[v_elements[i + 1][2]];
		vertices[i + 2].norm = raw_normals[v_elements[i + 2][2]];

		// Store texture coordinates:
		vertices[i + 0].uv = raw_uvs[v_elements[i + 0][1]];
		vertices[i + 1].uv = raw_uvs[v_elements[i + 1][1]];
		vertices[i + 2].uv = raw_uvs[v_elements[i + 2][1]];
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
	glEnableVertexAttribArray(2);  // uv
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(2 * sizeof(glm::vec3)));  // the last parameter: offset

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::loadTexture(std::string filename)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Cube::activateTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);  // bind the texture
}

void Cube::draw()
{
	//glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vcount);
	glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}