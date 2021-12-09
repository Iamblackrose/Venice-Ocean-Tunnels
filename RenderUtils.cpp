#include "RenderUtils.h"

unsigned int GE::createVBO(const std::vector<Vertexx>& vertices)
{
    unsigned int newBuffer;
    glCreateBuffers(1, &newBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, newBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertexx), (void*)&vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    return newBuffer;
}

unsigned int GE::createIBO(const std::vector<unsigned int>& indices)
{
    unsigned int newBuffer;
    glGenBuffers(1, &newBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, newBuffer);
    glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), (void*)&indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    return newBuffer;
}