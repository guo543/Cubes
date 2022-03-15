#include "Util.h"

unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error Handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << ((type == GL_VERTEX_SHADER) ? " vertex " : " fragment ") << "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	std::ifstream filev(vertexShader);
	if (!filev.is_open()) {
		std::stringstream ss;
		ss << "Failed to open " << vertexShader << std::endl;
		throw std::runtime_error(ss.str());
	}

	// Read the shader source
	std::stringstream bufferv;
	bufferv << filev.rdbuf();
	std::string bufStrv = bufferv.str();
	const char* bufCStrv = bufStrv.c_str();

	std::ifstream filef(fragmentShader);
	if (!filef.is_open()) {
		std::stringstream ss;
		ss << "Failed to open " << vertexShader << std::endl;
		throw std::runtime_error(ss.str());
	}

	// Read the shader source
	std::stringstream bufferf;
	bufferf << filef.rdbuf();
	std::string bufStrf = bufferf.str();
	const char* bufCStrf = bufStrf.c_str();

	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, bufCStrv);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, bufCStrf);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}