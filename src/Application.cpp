#include "Application.hpp"

GLFWwindow* window;
Camera* camera;
Renderer* renderer;

constexpr int width = 1920, height = 1080;


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

    glEnable(GL_DEPTH_TEST);
    

    glfwSwapInterval(1);

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
    renderer = new Renderer(camera);
    
    scene::GlobalShader = new Shader("res/shaders/basicVertex.glsl","res/shaders/LightFrag.glsl");

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5;
    style.FrameRounding = 5;
    //style.Colors[ImGuiCol_Header] = ImVec4(0.32, 0.5, 0.25, 1);
}

void Application::Update()
{

        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            //glfwGetFramebufferSize(window, &camera->width, &camera->height);
            //
            //if(camera->width > 0 && camera->height > 0)

            glfwGetFramebufferSize(window, &camera->width, &camera->height);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            camera->ProcessInput(window);

            camera->CameraFocusing(window);

            renderer->Clear();

            if(camera->width >0 && camera->height > 0)
                renderer->SetUniformsAndRender();

            gui::RenderLightButtons(camera);

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
    gui::m_EditorPointer = nullptr;
}

bool Application::IsRunning()
{
    return bIsRunning;
}
