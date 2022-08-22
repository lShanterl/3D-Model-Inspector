#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <memory>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.hpp"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Camera.hpp"
#include "Model.h"
#include "Light.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

}

int main()
{
    GLFWwindow* window;
    constexpr int width = 640,height = 480;


    /* Initialize the library */

    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Obj Model Inspector", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error! \n";

    std::cout << glGetString(GL_VERSION) << std::endl;

    
        Camera camera(glm::vec3(0.0f, -2.0f, 3.0f), 90.0f, width, height);
        glm::mat4 view = camera.GetView();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1));
	    Renderer renderer;

        glEnable(GL_DEPTH_TEST);

        Model modelObj("res/models/backpack.obj");

        glfwSetCursorPosCallback(window, Camera::mouse_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        Light light;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glm::mat4 proj = camera.GetProj(width, height);
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            camera.ProcessInput(window);

            camera.CameraFocusing(window);

            glm::mat4 mvp = proj * camera.GetView() * model;
            
            modelObj.m_shader->SetVec3f("lightPos", light.GetLightPos().x, light.GetLightPos().y, light.GetLightPos().z);

            modelObj.m_shader->SetMatrix4f("model", model);
            modelObj.m_shader->SetMatrix4f("view", camera.GetView());
            modelObj.m_shader->SetMatrix4f("projection", proj);
            modelObj.m_shader->SetVec4f("u_Color", 1.0f, 0.5f, 0.31f, 1.0f);
            modelObj.m_shader->SetVec3f("objectColor", 1.0f, 1.0f, 1.0f);
            modelObj.m_shader->SetVec3f("lightColor", 0.75f, 1.0f, 0.85f);
            modelObj.m_shader->SetMatrix4f("u_MVP", mvp);

            GLCall(renderer.Draw(*modelObj.m_vao, *modelObj.m_ib, *modelObj.m_shader));

            ImGui::Begin("Hello World!");
            ImGui::Text("Waltuh");
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    
        glfwTerminate();
        return 0;
}