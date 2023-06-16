#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

// Read shader files line by line and extract the vertex and fragments shader
// Both shaders are in the same files and seprated with "#"
// for example before a vertex shader sourse code, it is written #shader vertex
static ShaderProgramSource ParseShader(const std::string& filepath)
{

	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				//set mode to vertex
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				//set mode to fragment
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}
