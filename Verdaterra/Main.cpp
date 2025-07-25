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
glm::vec3 CameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float Yaw = -90.0f;
float Pitch = 0.0f;
float LastX = 1280.0f / 2.0f;
float LastY = 720.0f / 2.0f;
bool FirstMouse = true;

float CameraSpeed = 4.0f;
float MouseSensitivity = 0.1f;

inline void MouseCallback(GLFWwindow *Window, double X, double Y)
{
    if (FirstMouse) {
        LastX = static_cast<float>(X);
        LastY = static_cast<float>(Y);
        FirstMouse = false;
    }

    float xoffset = static_cast<float>(X - LastX);
    float yoffset = static_cast<float>(LastY - Y);

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

    glm::vec3 Forward;
    Forward.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Forward.y = sin(glm::radians(Pitch));
    Forward.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    CameraForward = glm::normalize(Forward);
}

inline void ResizeCallback(GLFWwindow *Window, int32_t FramebufferWidth, int32_t FramebufferHeight)
{
    glViewport(0, 0, FramebufferWidth, FramebufferHeight);
}

int main() {
    CConsoleLogger ConsoleOut;
    CLogger::GetInstance().AddOutput(&ConsoleOut);
    CLogger::GetInstance().SetLevel(ELogLevel::Debug);

    if (!glfwInit()) {
        LOG_CRITICAL("Failed to initialize GLFW");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *Monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *VideoMode = glfwGetVideoMode(Monitor);

    GLFWwindow *Window = glfwCreateWindow(VideoMode->width, VideoMode->height, "Verdaterra", Monitor, NULL);

    if (!Window) {
        LOG_CRITICAL("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        LOG_CRITICAL("Failed to initialize GLAD");
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
        SLinkRule(0, 3, EVertexComponentType::Float32, false, offsetof(SVertex, MPosition)),
        SLinkRule(1, 3, EVertexComponentType::Float32, false, offsetof(SVertex, MNormal)),
        SLinkRule(2, 2, EVertexComponentType::Float32, false, offsetof(SVertex, MUV))
    };

    for (uint32_t I = 0; I < MeshCount; ++I) {
        aiMesh *AssimpMesh = AssimpScene->mMeshes[I];

        int32_t VertexCount = AssimpMesh->mNumVertices;

        std::vector<SVertex> Vertices(VertexCount);
        for (int32_t V = 0; V < VertexCount; ++V) {
            glm::vec3 Location = *((glm::vec3 *)&AssimpMesh->mVertices[V]);
            glm::vec3 Normal = *((glm::vec3 *)&AssimpMesh->mNormals[V]);
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

    DMesh Plane;
    glm::vec4 PlaneColor = glm::vec4(0.0, 0.3, 0.0, 1.0);
    std::vector<SVertex> PlaneVertices = {
        { glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 1.0f) }
    };

    std::vector<uint32_t> PlaneIndices = {
        2, 1, 0,
        3, 2, 0
    };

    Plane.Create();
    Plane.Write(PlaneVertices, PlaneIndices, LinkRules, EBufferMode::StaticDraw);


    glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
    glm::mat4 Model1 = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 Model2 = glm::scale(glm::mat4(1.0f), glm::vec3(40.0f, 40.0f, 40.0f));

    glm::vec4 LightColor = glm::vec4(1.0);
    glm::vec3 LightPosition = glm::vec3(-4.0f, 2.0f, 0.0f);
    glm::vec3 initialLightPos = glm::vec3(-4.0f, 2.0f, 0.0f);
    float angle = 0.0f;
    float rotationSpeed = 0.628f; // Approx. 1 rotation every 10 seconds (2π / 10)

    CPipeline DefaultPipeline;
    DefaultPipeline.Create();
    DefaultPipeline.AddStage("Assets/Shaders/Default.vert", EShaderType::Vertex);
    DefaultPipeline.AddStage("Assets/Shaders/Default.frag", EShaderType::Fragment);

    DefaultPipeline.Bind();
    DefaultPipeline.SetUniform(0, "UBTexture");
    DefaultPipeline.SetUniform(Projection, "UProjection");
    DefaultPipeline.SetUniform(LightColor, "ULightColor");
    DefaultPipeline.SetUniform(LightPosition, "ULightPosition");

    unsigned int ShadowMapFBO;
    glGenFramebuffers(1, &ShadowMapFBO);

    unsigned int ShadowMapWidth = 2048, ShadowMapHeight = 2048;
    unsigned int ShadowMap;
    glGenTextures(1, &ShadowMap);
    glBindTexture(GL_TEXTURE_2D, ShadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowMapWidth, ShadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float ClampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, ClampColor);

    glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glm::mat4 OrthgonalProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 75.0f);
    glm::mat4 LightView = glm::lookAt(LightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 LightProjection = OrthgonalProjection * LightView;

    CPipeline ShadowMapPipeline;
    ShadowMapPipeline.Create();
    ShadowMapPipeline.AddStage("Assets/Shaders/ShadowMap.vert", EShaderType::Vertex);
    ShadowMapPipeline.AddStage("Assets/Shaders/ShadowMap.frag", EShaderType::Fragment);

    float lastFrameTime = static_cast<float>(glfwGetTime());

    glEnable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(Window)) {
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        glCullFace(GL_FRONT);

        angle += rotationSpeed * deltaTime;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        LightPosition = glm::vec3(rotation * glm::vec4(initialLightPos, 1.0f));
        LightView = glm::lookAt(LightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        LightProjection = OrthgonalProjection * LightView;

        // Shadow Pass
        ShadowMapPipeline.Bind();
        ShadowMapPipeline.SetUniform(LightProjection, "ULightProjection");
        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, ShadowMapWidth, ShadowMapHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        ShadowMapPipeline.SetUniform(Model1, "UTransform");
        for (uint32_t I = 0; I < MeshCount; ++I) {
            Meshes[I].Bind();
            Meshes[I].Draw();
            Meshes[I].Unbind();
        }

       /* ShadowMapPipeline.SetUniform(Model2, "UTransform");
        Plane.Bind();
        Plane.Draw();
        Plane.Unbind();*/

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // Base Pass
        glViewport(0, 0, VideoMode->width, VideoMode->height);
        glCullFace(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
            CameraPosition += CameraSpeed * deltaTime * CameraForward;
        if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
            CameraPosition -= CameraSpeed * deltaTime * CameraForward;
        if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
            CameraPosition -= glm::normalize(glm::cross(CameraForward, CameraUp)) * CameraSpeed * deltaTime;
        if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
            CameraPosition += glm::normalize(glm::cross(CameraForward, CameraUp)) * CameraSpeed * deltaTime;
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(Window, true);

        DefaultPipeline.Bind();
        DefaultPipeline.SetUniform(LightProjection, "ULightProjection");
        DefaultPipeline.SetUniform(LightPosition, "ULightPosition");

        glm::mat4 View = glm::lookAt(CameraPosition, CameraPosition + CameraForward, CameraUp);
        DefaultPipeline.SetUniform(View, "UView");
        DefaultPipeline.SetUniform(CameraPosition, "UCameraPosition");

        DefaultPipeline.SetUniform(Model1, "UTransform");
        for (uint32_t I = 0; I < MeshCount; ++I) {
            DefaultPipeline.SetUniform(0, "UBTexture");
            DefaultPipeline.SetUniform(Materials[I], "UColorDiffuse");
            Meshes[I].Bind();
            Meshes[I].Draw();
            Meshes[I].Unbind();
        }
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, ShadowMap);
        uint32_t Location = DefaultPipeline.GetUniformLocation("UShadowMap");
        glUniform1i(Location, 2);
        DefaultPipeline.SetUniform(0, "UBTexture");
        DefaultPipeline.SetUniform(Model2, "UTransform");
        DefaultPipeline.SetUniform(PlaneColor, "UColorDiffuse");
        Plane.Bind();
        Plane.Draw();
        Plane.Unbind();

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    DefaultPipeline.Unbind();
    DefaultPipeline.Destroy();
    for (DMesh &Mesh : Meshes) {
        Mesh.Destroy();
    }
    Plane.Destroy();
    glfwTerminate();
    return 0;
}