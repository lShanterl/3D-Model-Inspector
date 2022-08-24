#include "Application.hpp"

GLFWwindow* window;
Camera* camera;
Model* mesh;
Renderer* renderer;
Light* light;


constexpr int width = 1920, height = 1080;

//mvp
glm::mat4 view;
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 proj;
glm::mat4 mvp;
char modelPath[100] = "";
char texPath[100] = "";

bool bIsRunning = true;
bool useTexAlbedo = false;

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void Application::Init()
{
    /* Initialize the glfw */

    if (!glfwInit())
        bIsRunning = false;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Obj Model Inspector", NULL, NULL);
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



    mesh = new Model("res/models/backpack.obj");
    mesh->LoadTexture("res/textures/backpack_albedo.jpg");


    light = new Light;
}

void Application::Update()
{
    while (!glfwWindowShouldClose(window))
    {
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

        mesh->m_shader->SetVec3f("lightPos", light->GetLightPos().x, light->GetLightPos().y, light->GetLightPos().z);

        mesh->m_shader->SetMatrix4f("model", model);
        mesh->m_shader->SetMatrix4f("view", camera->GetView());
        mesh->m_shader->SetMatrix4f("projection", proj);

        mesh->m_shader->SetVec3f("objectColor", 1.0f, 1.0f, 1.0f);
        mesh->m_shader->SetVec3f("lightColor", light->m_LightCol.x, light->m_LightCol.y, light->m_LightCol.z);
        mesh->m_shader->SetMatrix4f("u_MVP", mvp);

        GLCall(renderer->Draw(*mesh->m_vao, *mesh->m_ib, *mesh->m_shader));

        //ImGui
        ImGui::Begin("Model Inspector");
        if(ImGui::TreeNode("Lights"))
        {
            ImGui::ColorEdit3("lightColor", (float*)&light->m_LightCol);
            ImGui::TreePop();
        }
        ImGui::SetNextItemOpen(true);
        if (ImGui::TreeNode("Model Loader"))
        {

            ImGui::Spacing();

            ImGui::Checkbox("Use Albedo Texture", &useTexAlbedo);


            ImGui::InputText("model path", modelPath, sizeof(char) * 100);
            if (useTexAlbedo)
            {
                if (ImGui::InputText("albedo texture path", texPath, sizeof(char) * 100));

            }
            if (ImGui::Button("Load Model and Texture"))
            {
                if (useTexAlbedo)
                {
                    delete(mesh);
                    sl::BeginBenchmark("modelLoader");
                    mesh = new Model(modelPath);
                    mesh->LoadTexture(texPath);
                    sl::EndBenchmark("modelLoader");
                }
                else
                {
                    delete(mesh);
                    sl::BeginBenchmark("modelLoader");
                    mesh = new Model(modelPath);
                    mesh->LoadTexture("res/textures/white.png");
                    sl::EndBenchmark("modelLoader");
                }
            }
            ImGui::TreePop();

            if (ImGui::TreeNode("Camera"))
            {
                ImGui::InputFloat("Movement speed", &camera->m_movementSpeed, 0.1f);
                ImGui::TreePop();
            }

        }
               
        ImGui::End();

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
    delete(light);
}

bool Application::IsRunning()
{
    return bIsRunning;
}
