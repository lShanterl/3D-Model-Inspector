#include "Application.hpp"

GLFWwindow* window;
Camera* camera;
Model* mesh;
Renderer* renderer;
Light* light;
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


char modelPath[100] = "";
char albedoTexPath[100] = "";
char specularTexPath[100] = "";

bool bIsRunning = true;
bool useTexAlbedo = false;
bool useTexSpecular = false;

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

    material = new Material(albedoTexture, nullptr , shader);

    mesh = new Model("res/models/backpack.obj", material);

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

        material->m_shader->SetVec3f("lightDir", light->GetlightDir().x, light->GetlightDir().y, light->GetlightDir().z);

        material->m_shader->SetMatrix4f("model", model);
        material->m_shader->SetMatrix4f("view", camera->GetView());
        material->m_shader->SetMatrix4f("projection", proj);

        material->m_shader->SetVec3f("objectColor", 1.0f, 1.0f, 1.0f);
        material->m_shader->SetVec3f("viewPos", camera->m_Position.x,camera->m_Position.y, camera->m_Position.z);
        material->m_shader->SetVec3f("lightColor", light->m_LightCol.x, light->m_LightCol.y, light->m_LightCol.z);
        material->m_shader->SetMatrix4f("u_MVP", mvp);


        GLCall(renderer->Draw(*mesh->m_vao, *mesh->m_ib, *material->m_shader));

        //ImGui     

        ImGui::Begin("Model Inspector");
        if(ImGui::TreeNode("Lights"))
        {
            ImGui::ColorEdit3("lightColor", (float*)&light->m_LightCol);
            ImGui::SliderFloat3("light Position", (float*) &light->m_lightDir, -10.0f, 10.0f);
            ImGui::TreePop();
        }
        ImGui::SetNextItemOpen(true);
        if (ImGui::TreeNode("Model Loader"))
        {

            ImGui::Spacing();

            ImGui::Checkbox("Use Albedo Texture", &useTexAlbedo);
            ImGui::Checkbox("Use Specular Texture", &useTexSpecular);


            ImGui::InputText("model path", modelPath, sizeof(char) * 100);
            if (useTexAlbedo)
            {
                if (ImGui::InputText("albedo texture path", albedoTexPath, sizeof(char) * 100));

            }
            if (useTexSpecular)
            {
                if (ImGui::InputText("specular texture path", specularTexPath, sizeof(char) * 100));

            }
            if (ImGui::Button("Load Model and Texture"))
            {
                delete(mesh);
                delete(material);
                delete(albedoTexture);
                delete(specularTexture);
                
                sl::BeginBenchmark("modelLoader");
                

                if (useTexAlbedo)
                {                   
                    albedoTexture = new Texture(albedoTexPath);
                }
                else
                {
                    albedoTexture = new Texture("res/textures/white.png");
                }
                if (useTexSpecular)
                {
                    specularTexture = new Texture(albedoTexPath);
                }
                else
                {
                    specularTexture = new Texture("res/textures/white.png");
                }
                
                material = new Material(albedoTexture, specularTexture, shader);

                mesh = new Model(modelPath, material);

                sl::EndBenchmark("modelLoader");
            }
            ImGui::TreePop();

            if (ImGui::TreeNode("Camera"))
            {
                ImGui::DragFloat("Movement speed", &camera->m_movementSpeed, 0.1f, 0.1f, 25.0f,"%.3f", ImGuiSliderFlags_AlwaysClamp);
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
    delete(material);
    delete(shader);
    delete(albedoTexture);
    delete(specularTexture);

}

bool Application::IsRunning()
{
    return bIsRunning;
}
