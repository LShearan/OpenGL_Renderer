#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// ------------ ERROR CHECKING ---------------

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
                  x;\
                  ASSERT(GLLogCall(#x,__FILE__,__LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

// ------------------------------------------

#define GLSL_VERSION "#version 330"

class Renderer
{
private:

public:

    void Clear() const;

    void Reset();

    void DrawArray(const VertexArray& va, const Shader& shader, int first, int size) const;

    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

    void EndDraw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    
    void DrawWithType(unsigned int type, const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};