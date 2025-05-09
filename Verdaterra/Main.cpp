#include "iostream"

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include "assimp/Importer.hpp"
#include "GLFW/glfw3.h"


inline void ResizeCallback(GLFWwindow *Window, int32_t FramebufferWidth, int32_t FramebufferHeight)
{
    glViewport(0, 0, FramebufferWidth, FramebufferHeight);
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *Window = glfwCreateWindow(1280, 720, "Verdaterra", NULL, NULL);
    
    if (!Window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(Window, ResizeCallback);

    SVertex StackVertices[4] = {
        SVertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
        SVertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
        SVertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
        SVertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f))
    };
    TArray<SVertex> *Vertices = TArray<SVertex>::From(StackVertices, sizeof(StackVertices) / sizeof(StackVertices[0]));

    uint32_t StackIndices[6] = {
        0, 1, 2,
        0, 2, 3
    };
    TArray<uint32_t> *Indices = TArray<uint32_t>::From(StackIndices, sizeof(StackIndices) / sizeof(StackIndices[0]));

    CMesh Mesh(Vertices, Indices, EBufferUsage::StaticDraw);

    CShader DefaultShader;
    DefaultShader.Load("Assets/Shaders/Default.vert", EShaderType::Vertex);
    DefaultShader.Load("Assets/Shaders/Default.frag", EShaderType::Fragment);

    CTexture Texture = CTexture("Assets/Textures/mta-map.jpg");

    DefaultShader.Bind();

    DefaultShader.SetUniform(&Texture, "UDiffuse");
    Texture.Bind();

    //Assimp::Importer Importer;
    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Mesh.Draw();

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    
    DefaultShader.Unbind();

    glfwTerminate();
    return 0;
}