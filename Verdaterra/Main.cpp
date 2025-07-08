#include "iostream"

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GLFW/glfw3.h"

// ------------------- Camera Globals -------------------
glm::vec3 CameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float Yaw = -90.0f;
float Pitch = 0.0f;
float LastX = 1280.0f / 2.0f;
float LastY = 720.0f / 2.0f;
bool FirstMouse = true;

float CameraSpeed = 0.05f;
float MouseSensitivity = 0.1f;

void MouseCallback(GLFWwindow *window, double xpos, double ypos) {
    if (FirstMouse) {
        LastX = (float)xpos;
        LastY = (float)ypos;
        FirstMouse = false;
    }

    float xoffset = (float)(xpos - LastX);
    float yoffset = (float)(LastY - ypos); // reversed: y-coordinates go bottom to top

    LastX = (float)xpos;
    LastY = (float)ypos;

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    CameraFront = glm::normalize(front);
}

inline void ResizeCallback(GLFWwindow *Window, int32_t FramebufferWidth, int32_t FramebufferHeight)
{
    glViewport(0, 0, FramebufferWidth, FramebufferHeight);
}

glm::vec3 ToGLMVec3(aiVector3D Vector3D)
{
    return glm::vec3(
        Vector3D.x,
        Vector3D.y,
        Vector3D.z
    );
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

    glfwSetCursorPosCallback(Window, MouseCallback);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Assimp::Importer Importer;
    const aiScene *AssimpScene = Importer.ReadFile(
        "Assets/Models/Map/scene.gltf",
        aiProcess_GenNormals |
        aiProcess_Triangulate |
        aiProcess_GenBoundingBoxes |
        aiProcess_SortByPType
    );

    uint32_t MeshCount = AssimpScene->mNumMeshes;
    uint32_t MaterialCount = AssimpScene->mNumMaterials;

    std::vector<CMesh *> Meshes(MeshCount);
    std::vector<glm::vec4> Materials(MaterialCount);

    for (uint32_t I = 0; I < MeshCount; ++I) {
        aiMesh *AssimpMesh = AssimpScene->mMeshes[I];

        assert(AssimpMesh->HasTextureCoords(0));

        int32_t VertexCount = AssimpMesh->mNumVertices;

        std::vector<SVertex> Vertices(VertexCount);
        for (int32_t V = 0; V < VertexCount; ++V) {
            glm::vec3 Location = ToGLMVec3(AssimpMesh->mVertices[V]);
            glm::vec3 Normal = ToGLMVec3(AssimpMesh->mNormals[V]);
            glm::vec2 UV = glm::vec2(1.0f);
            if (AssimpMesh->HasTextureCoords(0)) {
                UV = glm::vec2(AssimpMesh->mTextureCoords[0][V].x, AssimpMesh->mTextureCoords[0][V].y);
            }
            Vertices[V] = SVertex(
                Location, Normal, UV
            );
        }

        assert(AssimpMesh->mFaces->mNumIndices == 3);
        uint32_t IndicesCount = AssimpMesh->mNumFaces * AssimpMesh->mFaces->mNumIndices;
        uint32_t IndicesCounter = 0;
        std::vector<uint32_t> Indices(IndicesCount);
        for (uint32_t J = 0; J < AssimpMesh->mNumFaces; ++J) {
            aiFace &Face = AssimpMesh->mFaces[J];
            for (uint32_t K = 0; K < Face.mNumIndices; ++K) {
                Indices[IndicesCounter++] = Face.mIndices[K];
            }
        }

        aiMaterial *AssimpMaterial = AssimpScene->mMaterials[AssimpMesh->mMaterialIndex];
        aiColor4D DiffuseColor;
        AssimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor);

        Meshes[I] = new CMesh(Vertices, Indices, EBufferUsage::StaticDraw);
        Materials[I] = glm::vec4(DiffuseColor.r, DiffuseColor.g, DiffuseColor.b, DiffuseColor.a);
    }

    glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
    glm::mat4 Model = glm::mat4(1.0f);

    CShader DefaultShader;
    DefaultShader.Load("Assets/Shaders/Default.vert", EShaderType::Vertex);
    DefaultShader.Load("Assets/Shaders/Default.frag", EShaderType::Fragment);

    DefaultShader.Bind();
    DefaultShader.SetUniform(0, "UHasTexture");
    DefaultShader.SetUniform(Projection, "UProjection");
    DefaultShader.SetUniform(Model, "UTransform");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
            CameraPosition += CameraSpeed * CameraFront;
        if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
            CameraPosition -= CameraSpeed * CameraFront;
        if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
            CameraPosition -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
        if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
            CameraPosition += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;

        glm::mat4 View = glm::lookAt(CameraPosition, CameraPosition + CameraFront, CameraUp);
        DefaultShader.SetUniform(View, "UView");

        for (uint32_t I = 0; I < MeshCount; ++I) {
            DefaultShader.SetUniform(Materials[I], "UColorDiffuse");
            Meshes[I]->Draw();
        }

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    DefaultShader.Unbind();
    glfwTerminate();
    return 0;
}
