#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "examples/Scene.h"
#include "examples/SceneClearColour.h"
#include "examples/SceneTexturedCube.h"
#include "examples/SceneBatchRendering.h"
#include "examples/ModelLoading.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "OpenGL Renderer", NULL, NULL);
    if (!window)
    {
        std::cout << "ERROR GLFW FAILED TO CREATE A WINDOW!" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Enable vsync */
    glfwSwapInterval(1);

    /* Initialise GLEW needs a valid windows context to init */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR GLEW FAILED TO INITIALISE!" << std::endl;
    }

    /* Output the current OpenGL Version */
    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        /* Enable blending */
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Setup ImGui */
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(GLSL_VERSION);

        /* Time variables */
        float deltaTime = 0.f;
        float currentTime = 0.f;
        float lastTime = 0.f;

        /* Setup our scene menu */
        scene::Scene* currentScene = nullptr;
        scene::SceneMenu* sceneMenu = new scene::SceneMenu(currentScene);
        currentScene = sceneMenu;

        sceneMenu->RegisterTest<scene::ClearColour>("Clear Colour");
        sceneMenu->RegisterTest<scene::BatchRenderer>("2D Batch Renderer");
        sceneMenu->RegisterTest<scene::TexturedCube>("Textured Cube");
        sceneMenu->RegisterTest<scene::ModelLoader>("Model Loading");
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GLCALL(glClearColor(0.f, 0.f, 0.f, 1.f));
            GLCALL(glClear(GL_COLOR_BUFFER_BIT));

            /* Start the Dear ImGui frame */
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            int camIndex = -1;
            if (currentScene)
            {
                currentTime = glfwGetTime();
                deltaTime = currentTime - lastTime;
                lastTime = currentTime;

                currentScene->OnUpdate(currentTime, window);
                currentScene->OnRender();

                ImGui::Begin("Scene");
                if (currentScene != sceneMenu && ImGui::Button("<-"))
                {
                    delete currentScene;
                    currentScene = sceneMenu;
                }
                currentScene->OnImGuiRender();
                ImGui::End();
            }

            /* Render ImGui */
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        delete currentScene;
        if (currentScene != sceneMenu)
        {
            delete sceneMenu;
        }

    }

    /* Cleanup ImGui */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}