#include "Application.hpp"

GLFWwindow* window;
Camera* camera;
Model* mesh;
Renderer* renderer;

Material* material;
Texture* albedoTexture;
Texture* specularTexture;
Shader* shader;

constexpr int width = 1920, height = 1080;

//mvp
glm::mat4 view;
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 proj;
glm::mat4 mvp;

//double lastTime = glfwGetTime();
//int nbFrames = 0;

char modelPath[100] = "";
char albedoTexPath[100] = "";
char specularTexPath[100] = "";

bool bIsRunning = true;
bool useTexAlbedo = false;
bool useTexSpecular = false;

std::vector<PointLight>  pointLights;

std::vector<DirLight> dirLights;

std::vector<SpotLight> spotLights;

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
    model = glm::scale(model, glm::vec3(1));
    shader = new Shader("res/shaders/basicVertex.glsl","res/shaders/LightFrag.glsl");

    albedoTexture = new Texture("res/textures/backpack_albedo.jpg");
    specularTexture = new Texture("res/textures/backpack_specular.jpg");

    material = new Material(albedoTexture, specularTexture , shader);

    mesh = new Model("res/models/backpack.obj", material);

    pointLights.reserve(7);

    dirLights.reserve(4);

    spotLights.reserve(3);

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
        glfwGetFramebufferSize(window, &width, &height);
        proj = camera->GetProj(width, height);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        camera->ProcessInput(window);

        camera->CameraFocusing(window);

        mvp = proj * camera->GetView() * model;

        renderer->Clear();

        Application::SetUniforms();

        GLCall(renderer->Draw(*mesh->m_vao, *mesh->m_ib, *material->m_shader));

        //ImGui     

        //FLAGS
        
        ImGuiWindowFlags window_flags = 0;
        window_flags =
            ImGuiWindowFlags_NoTitleBar;
            //ImGuiWindowFlags_NoMove;
                   
        ImGui::BeginMainMenuBar();
        ImGui::Begin("Model Inspector",0, window_flags);
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("Scene"))
            {

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::Separator();

        editor::RenderLightButtons(pointLights, dirLights, spotLights, camera);

        if (ImGui::CollapsingHeader("ModelLoader", ImGuiStyleVar_PopupRounding))
        {

            ImGui::Spacing();

            ImGui::Checkbox("Use Albedo Texture", &useTexAlbedo);
            ImGui::Checkbox("Use Specular Texture", &useTexSpecular);

            ImGui::InputText("model path", modelPath, sizeof(char) * 65);
            if (useTexAlbedo)
            {
                if (ImGui::InputText("albedo texture path", albedoTexPath, sizeof(char) * 65));
            }
            if (useTexSpecular)
            {
                if (ImGui::InputText("specular texture path", specularTexPath, sizeof(char) * 65));

            }
            if (ImGui::Button("Load Model and Texture"))
            {
                sl::BeginBenchmark("modelLoader");
                delete(mesh);
                delete(material);
                delete(albedoTexture);
                delete(specularTexture);
                           
                if (useTexAlbedo)
                {                   
                    albedoTexture = new Texture(albedoTexPath);
                }
                else
                {
                    unsigned char white[4] = { 255,255,255,255 };
                    albedoTexture = new Texture(white, 1, 1);
                }
                if (useTexSpecular)
                {
                    specularTexture = new Texture(albedoTexPath);
                }
                else
                {
                    unsigned char darkGray[4] = {36,36,36,255};
                    specularTexture = new Texture(darkGray,1,1);
                }
                
                material = new Material(albedoTexture, specularTexture, shader);

                mesh = new Model(modelPath, material);

                std::cout << "model loaded in " << sl::EndBenchmark("modelLoader") << " seconds\n";
            }


            if (ImGui::TreeNode("Camera"))
            {
                ImGui::DragFloat("Movement speed", &camera->m_movementSpeed, 0.1f, 0.1f, 25.0f,"%.3f", ImGuiSliderFlags_AlwaysClamp);
                ImGui::TreePop();
            }

        }
        ImGui::End();

        ImGui::EndMainMenuBar();

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
    delete(mesh);
    delete(material);
    delete(shader);
    delete(albedoTexture);
    delete(specularTexture);
    editor::m_LightPointer = nullptr;

}

void Application::SetUniforms()
{
    const float linear = 0.027f;
    const float quadratic = 0.028f;
    //mvp
    material->m_shader->SetMatrix4f("model", model);
    material->m_shader->SetMatrix4f("view", camera->GetView());
    material->m_shader->SetMatrix4f("projection", proj);
    material->m_shader->SetMatrix4f("u_MVP", mvp);

    //lighting
    material->m_shader->SetVec3f("viewPos", camera->m_Position.x, camera->m_Position.y, camera->m_Position.z);

    material->m_shader->SetInt("scene.mPointLights", pointLights.size());
    material->m_shader->SetInt("scene.mDirLights", dirLights.size());

    for (uint32_t i = 0; i < pointLights.size(); i++)
    {

        if (pointLights[i].isWorking)
        {
            material->m_shader->SetVec3f(("pointLights[" + std::to_string(i) + "].position"), pointLights[i].m_lightPos.x, pointLights[i].m_lightPos.y, pointLights[i].m_lightPos.z);
            material->m_shader->SetVec3f(("pointLights[" + std::to_string(i) + "].lightColor"), pointLights[i].m_LightCol.x, pointLights[i].m_LightCol.y, pointLights[i].m_LightCol.z);
            material->m_shader->SetFloat(("pointLights[" + std::to_string(i) + "].linear"), linear);
            material->m_shader->SetFloat(("pointLights[" + std::to_string(i) + "].quadratic"), quadratic);
            material->m_shader->SetFloat(("pointLights[" + std::to_string(i) + "].intensity"), pointLights[i].intensity);
        }
        else
        {
            material->m_shader->SetVec3f(("pointLights[" + std::to_string(i) + "].position"), 0, 0, 0);
            material->m_shader->SetVec3f(("pointLights[" + std::to_string(i) + "].lightColor"), 0, 0, 0);
            material->m_shader->SetFloat(("pointLights[" + std::to_string(i) + "].linear"), 0);
            material->m_shader->SetFloat(("pointLights[" + std::to_string(i) + "].quadratic"), 0);
            material->m_shader->SetFloat(("pointLights[" + std::to_string(i) + "].intensity"), 0);
        }

    }
    for (uint32_t i = pointLights.size(); i < 25; i++)
    {
        material->m_shader->SetVec3f(("pointLights[" + std::to_string(i) + "].position"), 0, 0, 0);
        material->m_shader->SetVec3f(("pointLights[" + std::to_string(i) + "].lightColor"), 0, 0, 0);
        material->m_shader->SetFloat(("pointLights[" + std::to_string(i) + "].linear"), 0);
        material->m_shader->SetFloat(("pointLights[" + std::to_string(i) + "].quadratic"), 0);
        material->m_shader->SetFloat(("pointLights[" + std::to_string(i) + "].intensity"), 0);
    }

    for (uint32_t i = 0; i < dirLights.size(); i++)
    {
        if (dirLights[i].isWorking)
        {
            material->m_shader->SetVec3f("dirLights[" + std::to_string(i) + "].direction", dirLights[i].m_Direction.x, dirLights[i].m_Direction.y, dirLights[i].m_Direction.z);
            material->m_shader->SetVec3f("dirLights[" + std::to_string(i) + "].lightColor", dirLights[i].m_LightCol.x, dirLights[i].m_LightCol.y, dirLights[i].m_LightCol.z);
        }
        else
        {
            material->m_shader->SetVec3f("dirLights[" + std::to_string(i) + "].direction", 0, 0, 0);
            material->m_shader->SetVec3f("dirLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
        }

    }
    for (uint32_t i = dirLights.size(); i < 25; i++)
    {
        material->m_shader->SetVec3f("dirLights[" + std::to_string(i) + "].direction", 0, 0, 0);
        material->m_shader->SetVec3f("dirLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
    }

    for (uint32_t i = 0; i < spotLights.size(); i++)
    {
        if (spotLights[i].isWorking)
        {
            material->m_shader->SetVec3f("spotLights[" + std::to_string(i) + "].direction", spotLights[i].m_Direction.x, spotLights[i].m_Direction.y, spotLights[i].m_Direction.z);
            material->m_shader->SetVec3f("spotLights[" + std::to_string(i) + "].lightColor", spotLights[i].m_LightCol.x, spotLights[i].m_LightCol.y, spotLights[i].m_LightCol.z);
            material->m_shader->SetVec3f(("spotLights[" + std::to_string(i) + "].position"),spotLights[i].m_lightPos.x, spotLights[i].m_lightPos.y, spotLights[i].m_lightPos.z);

            //material->m_shader->SetVec3f("spotLights[" + std::to_string(i) + "].direction", camera->cameraFront.x, camera->cameraFront.y, camera->cameraFront.z);
            //material->m_shader->SetVec3f(("spotLights[" + std::to_string(i) + "].position"), camera->m_Position.x, camera->m_Position.y, camera->m_Position.z);

            material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].cutOff"), glm::cos(glm::radians(spotLights[i].cutOff)));
            material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].outerCutOff"), glm::cos(glm::radians(spotLights[i].outerCutOff)));

            material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].linear"), linear);
            material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].quadratic"),quadratic);
        }
        else
        {
            material->m_shader->SetVec3f("spotLights[" + std::to_string(i) + "].direction", 0, 0, 0);
            material->m_shader->SetVec3f("spotLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
            material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].cutOff"), 0);
            material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].outerCutOff"), 0);
            material->m_shader->SetVec3f(("spotLights[" + std::to_string(i) + "].position"), 0,0,0);
            material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].linear"), 0);
            material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].quadratic"), 0);

        }

    }
    for (uint32_t i = spotLights.size(); i < 10; i++)
    {
        material->m_shader->SetVec3f("spotLights[" + std::to_string(i) + "].direction", 0, 0, 0);
        material->m_shader->SetVec3f("spotLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
        material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].cutOff"), 0);
        material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].outerCutOff"), 0);
        material->m_shader->SetVec3f(("spotLights[" + std::to_string(i) + "].position"), 0, 0, 0);
        material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].linear"), 0);
        material->m_shader->SetFloat(("spotLights[" + std::to_string(i) + "].quadratic"), 0);
    }

    //material properties
    material->m_shader->SetVec3f("material.ambient", material->ambient.x, material->ambient.y, material->ambient.z);
    material->m_shader->SetFloat("material.shininess", material->shininess);
}

bool Application::IsRunning()
{
    return bIsRunning;
}
