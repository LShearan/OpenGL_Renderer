#pragma once
#include "Renderer.h"

#include <iostream>

// ------------ ERROR CHECKING ---------------

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << "; " << line << std::endl;
        return false;
    }

    return true;
}

// ------------------------------------------

void Renderer::Clear() const
{
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Reset()
{
    GLCALL(glBindVertexArray(0));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCALL(glUseProgram(0));
}

void Renderer::DrawArray(const VertexArray& va, const Shader& shader, int first, int size) const
{
    va.Bind();
    shader.Bind();
    GLCALL(glDrawArrays(GL_TRIANGLES, first, size));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    /* Bind the vertex array and index buffer and shader*/
    va.Bind();
    ib.Bind();
    shader.Bind();

    GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawWithType(unsigned int type, const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    /* Bind the vertex array and index buffer and shader*/
    va.Bind();
    ib.Bind();
    shader.Bind();

    GLCALL(glDrawElements(type, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
