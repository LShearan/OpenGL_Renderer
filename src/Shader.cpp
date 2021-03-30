#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragSource);
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(m_FilePath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAG = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAG;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCALL(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));

    int result;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCALL(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "SHADER!" << std::endl;
        std::cout << message << std::endl;
        GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragShader)
{
    GLCALL(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);

    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));

    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCALL(glUseProgram(0));

}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1iv(const std::string& name, int count, int values[])
{
    GLCALL(glUniform1iv(GetUniformLocation(name), count, values));
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
    GLCALL(glUniform1f(GetUniformLocation(name), v0));

}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCALL(glUniform3f(GetUniformLocation(name), v0, v1, v2));

}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
    GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }

    GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "WARNING: uniform " << name << " doesn't exsist" << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}
