#include "Application.hpp"

GLFWwindow* window;
Camera* camera;
Renderer* renderer;

constexpr int width = 1920, height = 1080;

//mvp
glm::mat4 view;

glm::mat4 proj;

//double lastTime = glfwGetTime();
//int nbFrames = 0;



bool bIsRunning = true;


void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Application::Init()
{
    /* Initialize the glfw */

    if (!glfwInit())
        bIsRunning = false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Obj Model Inspector",NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        bIsRunning = false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(window, Camera::mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Check if glew works
    if (glewInit() != GLEW_OK)
        std::cout << "Error! \n";


    //opengl version
    std::cout << glGetString(GL_VERSION) << std::endl;

    // ImGui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //initialize required objects
    camera = new Camera(glm::vec3(0.0f, -2.0f, 3.0f), 90.0f, width, height);
    renderer = new Renderer;
    view = camera->GetView();

    scene::GlobalShader = new Shader("res/shaders/basicVertex.glsl","res/shaders/LightFrag.glsl");

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5;
    style.FrameRounding = 5;
    //style.Colors[ImGuiCol_Header] = ImVec4(0.83, 0.5, 0.25, 1);
    


}

void Application::Update()
{
    while (!glfwWindowShouldClose(window))
    {
        // Measure speed
        //double currentTime = glfwGetTime();
        //nbFrames++;
        //if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
        //    // printf and reset timer
        //    printf("%f ms/frame\n", 1000.0 / double(nbFrames));
        //    nbFrames = 0;
        //    lastTime += 1.0;
        //}
         /* Render here */
        int width, height;
        glfwGetFramebufferSize(window, &width, &height); //??
        proj = camera->GetProj(width, height);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        camera->ProcessInput(window);

        camera->CameraFocusing(window);

        renderer->Clear();

        Application::SetUniforms();

        for (auto index : scene::m_Models)
        {
            //GLCall(renderer->Draw(index.m_vao, index.m_ib, index.m_material->m_shader)); //
            //renderer->Draw(index.m_vao, index.m_ib, index.m_material->m_shader);
        }
        //for (int i = 0; i < scene::m_Models.size(); i++)
        //{
        //    renderer->Draw(*scene::m_Models[i].m_vao, *scene::m_Models[i].m_ib, *scene::m_Models[i].m_material->m_shader);
        //} 
        
        //ImGui     

        //FLAGS

        editor::RenderLightButtons(camera);

        //ImGui::EndFrame();
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        bIsRunning = false;
    }
}



void Application::Exit()
{
    glfwTerminate();

    delete(camera);
    delete(renderer);
    editor::m_EditorPointer = nullptr;

}

void Application::SetUniforms()
{
    const float linear = 0.027f;
    const float quadratic = 0.028f;
    //mvp

    

    //lighting
    scene::GlobalShader->SetVec3f("viewPos", camera->m_Position.x, camera->m_Position.y, camera->m_Position.z);

    scene::GlobalShader->SetInt("scene.mPointLights", scene::m_PointLights.size());
    scene::GlobalShader->SetInt("scene.mDirLights", scene::m_DirectionalLights.size());

    for (uint32_t i = 0; i < scene::m_PointLights.size(); i++)
    {

        if (scene::m_PointLights[i].isWorking)
        {
            scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].position"), scene::m_PointLights[i].m_lightPos.x, scene::m_PointLights[i].m_lightPos.y, scene::m_PointLights[i].m_lightPos.z);
            scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].lightColor"), scene::m_PointLights[i].m_LightCol.x, scene::m_PointLights[i].m_LightCol.y, scene::m_PointLights[i].m_LightCol.z);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].linear"), linear);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].quadratic"), quadratic);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].intensity"), scene::m_PointLights[i].intensity);
        }
        else
        {
            scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].position"), 0, 0, 0);
            scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].lightColor"), 0, 0, 0);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].linear"), 0);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].quadratic"), 0);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].intensity"), 0);
        }

    }
    for (uint32_t i = scene::m_PointLights.size(); i < 25; i++)
    {
        scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].position"), 0, 0, 0);
        scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].lightColor"), 0, 0, 0);
        scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].linear"), 0);
        scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].quadratic"), 0);
        scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].intensity"), 0);
    }

    for (uint32_t i = 0; i < scene::m_DirectionalLights.size(); i++)
    {
        if (scene::m_DirectionalLights[i].isWorking)
        {
            scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].direction", scene::m_DirectionalLights[i].m_Direction.x, scene::m_DirectionalLights[i].m_Direction.y, scene::m_DirectionalLights[i].m_Direction.z);
            scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].lightColor", scene::m_DirectionalLights[i].m_LightCol.x, scene::m_DirectionalLights[i].m_LightCol.y, scene::m_DirectionalLights[i].m_LightCol.z);
        }
        else
        {
            scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].direction", 0, 0, 0);
            scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
        }

    }
    for (uint32_t i = scene::m_DirectionalLights.size(); i < 25; i++)
    {
        scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].direction", 0, 0, 0);
        scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
    }

    for (uint32_t i = 0; i < scene::m_SpotLights.size(); i++)
    {
        if (scene::m_SpotLights[i].isWorking)
        {
            scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].direction", scene::m_SpotLights[i].m_Direction.x, scene::m_SpotLights[i].m_Direction.y, scene::m_SpotLights[i].m_Direction.z);
            scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].lightColor", scene::m_SpotLights[i].m_LightCol.x, scene::m_SpotLights[i].m_LightCol.y, scene::m_SpotLights[i].m_LightCol.z);
            scene::GlobalShader->SetVec3f(("spotLights[" + std::to_string(i) + "].position"), scene::m_SpotLights[i].m_lightPos.x, scene::m_SpotLights[i].m_lightPos.y, scene::m_SpotLights[i].m_lightPos.z);

            //material->m_shader->SetVec3f("spotLights[" + std::to_string(i) + "].direction", camera->cameraFront.x, camera->cameraFront.y, camera->cameraFront.z);
            //material->m_shader->SetVec3f(("spotLights[" + std::to_string(i) + "].position"), camera->m_Position.x, camera->m_Position.y, camera->m_Position.z);

            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].cutOff"), glm::cos(glm::radians(scene::m_SpotLights[i].cutOff)));
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].outerCutOff"), glm::cos(glm::radians(scene::m_SpotLights[i].outerCutOff)));

            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].linear"), linear);
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].quadratic"),quadratic);
        }
        else
        {
            scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].direction", 0, 0, 0);
            scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].cutOff"), 0);
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].outerCutOff"), 0);
            scene::GlobalShader->SetVec3f(("spotLights[" + std::to_string(i) + "].position"), 0,0,0);
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].linear"), 0);
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].quadratic"), 0);

        }

    }
    for (uint32_t i = scene::m_SpotLights.size(); i < 10; i++)
    {
        scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].direction", 0, 0, 0);
        scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
        scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].cutOff"), 0);
        scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].outerCutOff"), 0);
        scene::GlobalShader->SetVec3f(("spotLights[" + std::to_string(i) + "].position"), 0, 0, 0);
        scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].linear"), 0);
        scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].quadratic"), 0);
    }

    scene::GlobalShader->SetMatrix4f("view", camera->GetView());
    scene::GlobalShader->SetMatrix4f("projection", proj);


    //material properties
    scene::GlobalShader->SetVec3f("material.ambient", scene::ambient.x, scene::ambient.y, scene::ambient.z);

    for (auto mesh : scene::m_Models)
    {
        mesh.modelView = glm::mat4(1.0f);
        mesh.modelView = glm::translate(mesh.modelView, mesh.position);
		mesh.modelView = glm::rotate(mesh.modelView, glm::radians(mesh.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		mesh.modelView = glm::rotate(mesh.modelView, glm::radians(mesh.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		mesh.modelView = glm::rotate(mesh.modelView, glm::radians(mesh.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        mesh.modelView = glm::scale(mesh.modelView, mesh.scale);
		
        scene::GlobalShader->SetMatrix4f("model", mesh.modelView);
        scene::GlobalShader->SetFloat("material.shininess", mesh.m_material->shininess);

        renderer->Draw(mesh);
    }
}

bool Application::IsRunning()
{
    return bIsRunning;
}
