#include "iostream"

#include "Rendering/Mesh.h"
#include "Rendering/Pipeline.h"
#include "Rendering/Texture.h"
#include "Utils/Logger.h"

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

float CameraSpeed = 0.005f;
float MouseSensitivity = 0.1f;

inline void MouseCallback(GLFWwindow *Window, double X, double Y)
{
    if (FirstMouse) {
        LastX = static_cast<float>(X);
        LastY = static_cast<float>(Y);
        FirstMouse = false;
    }

    float xoffset = static_cast<float>(X - LastX);
    float yoffset = static_cast<float>(LastY - Y); // reversed: y-coordinates go bottom to top

    LastX = static_cast<float>(X);
    LastY = static_cast<float>(Y);

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

int main() {
    CConsoleLogger ConsoleOut;
    CLogger::GetInstance().AddOutput(&ConsoleOut);
    CLogger::GetInstance().SetLevel(ELogLevel::Debug);

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *Monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *VideoMode = glfwGetVideoMode(Monitor);

    GLFWwindow *Window = glfwCreateWindow(VideoMode->width, VideoMode->height, "Verdaterra", Monitor, NULL);

    if (!Window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSwapInterval(0);

    glfwSetFramebufferSizeCallback(Window, ResizeCallback);

    glfwSetCursorPosCallback(Window, MouseCallback);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Assimp::Importer Importer;
    const aiScene *AssimpScene = Importer.ReadFile(
        "Assets/Models/Crops/Apple_4.fbx",
        static_cast<uint32_t>(
            aiProcess_GenNormals |
            aiProcess_Triangulate |
            aiProcess_GenBoundingBoxes |
            aiProcess_SortByPType
        )
    );

    uint32_t MeshCount = AssimpScene->mNumMeshes;
    uint32_t MaterialCount = AssimpScene->mNumMaterials;

    std::vector<DMesh> Meshes(MeshCount);
    std::vector<glm::vec4> Materials(MaterialCount);

    std::vector<SLinkRule> LinkRules = {
        SLinkRule(0, 3, EVertexComponentType::Float32, false, (void *) offsetof(SVertex, MPosition)),
        SLinkRule(1, 3, EVertexComponentType::Float32, false, (void *) offsetof(SVertex, MNormal)),
        SLinkRule(2, 2, EVertexComponentType::Float32, false, (void *) offsetof(SVertex, MUV))
    };

    for (uint32_t I = 0; I < MeshCount; ++I) {
        aiMesh *AssimpMesh = AssimpScene->mMeshes[I];
        
        int32_t VertexCount = AssimpMesh->mNumVertices;

        std::vector<SVertex> Vertices(VertexCount);
        for (int32_t V = 0; V < VertexCount; ++V) {
            glm::vec3 Location = *((glm::vec3 *) &AssimpMesh->mVertices[V]);
            glm::vec3 Normal = *((glm::vec3 *) &AssimpMesh->mNormals[V]);
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

        Meshes[I].Create();
        Meshes[I].Write(Vertices, Indices, LinkRules, EBufferMode::StaticDraw);
        Materials[I] = glm::vec4(DiffuseColor.r, DiffuseColor.g, DiffuseColor.b, DiffuseColor.a);
    }

    glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
    glm::mat4 Model = glm::mat4(1.0f);

    CPipeline DefaultPipeline;
    DefaultPipeline.Create();
    DefaultPipeline.AddStage("Assets/Shaders/Default.vert", EShaderType::Vertex);
    DefaultPipeline.AddStage("Assets/Shaders/Default.frag", EShaderType::Fragment);

    DefaultPipeline.Bind();
    DefaultPipeline.SetUniform(0, "UBTexture");
    DefaultPipeline.SetUniform(Projection, "UProjection");
    DefaultPipeline.SetUniform(Model, "UTransform");

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;
    float currentFrameTime = static_cast<float>(glfwGetTime());
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
    
    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
            CameraPosition += CameraSpeed * deltaTime * CameraFront;
        if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
            CameraPosition -= CameraSpeed * deltaTime * CameraFront;
        if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
            CameraPosition -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed * deltaTime;
        if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
            CameraPosition += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed * deltaTime;
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(Window, true);

        glm::mat4 View = glm::lookAt(CameraPosition, CameraPosition + CameraFront, CameraUp);
        DefaultPipeline.SetUniform(View, "UView");
        
        for (uint32_t I = 0; I < MeshCount; ++I) {
            DefaultPipeline.SetUniform(Materials[I], "UColorDiffuse");
            Meshes[I].Bind();
            Meshes[I].Draw();
        }

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    DefaultPipeline.Unbind();
    DefaultPipeline.Destroy();
    for (TMesh<SVertex, uint32_t> &Mesh : Meshes) {
        Mesh.Destroy();
    }
    glfwTerminate();
    return 0;
}
