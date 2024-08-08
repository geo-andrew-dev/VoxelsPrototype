#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

#include "shader.h"
#include "camera.h"
#include "chunk.h"
#include "chunkmanager.h"
#include "voxel.h"
#include "ui.h"
#include "terrain.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

float deltaTime = 0.0f; //time between current and last frame
float lastFrame = 0.0f;	//time of last frame
int frameCount = 0;
float fps = 0.0f;
float lastX = 400, lastY = 300; // Initialize mouse position to the center of the screen
bool firstMouse = true;

Camera* camera;
UI* ui;

Terrain* terrain = new Terrain();





// screen resolution
/*
* original default:
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
*/
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
int main()
{
    //Initializing window things.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Core profile
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "World of Chunks", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

    //Camera set up
    camera = new Camera(glm::vec3(5.0f, 5.0f, 15.0f)); //camera starts at 0,0,10
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    Shader currentShader = Shader("Shaders/simplevertexshader3D.vs", "Shaders/simplefragmentshader.fs");

    ChunkManager chunkManager = ChunkManager(16,16,16,16,10,16, currentShader);
    chunkManager.createChunks();
    chunkManager.loadChunks();

    /*
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    */


    bool renderChunks = true;
    int fpsUpdateCount = 0;
    int fpsUpdateRate = 80; //update fps display every n-1 frames
    
    ui = new UI(window);

    
    terrain->generateHeightMap();
   

    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 16; ++y) {
            std::cout << terrain->heightMap[x][y] << std::endl;
        }
    }



    //Render loop
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        frameCount++;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (deltaTime >= 1.0f) {
            fps = float(frameCount) / deltaTime;
            frameCount = 0;
            lastFrame = currentFrame;
        }
        else
            if (fpsUpdateCount >= fpsUpdateRate) {
                fps = 1.0 / deltaTime;
                fpsUpdateCount = 0;
            }
        fpsUpdateCount++;

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        /*
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        */

        //currentShader.use();
        if (renderChunks) {
            chunkManager.renderChunks(camera, SCR_WIDTH, SCR_HEIGHT);
        }
        ui->render(fps, &renderChunks);
        /*
        ImGui::Begin("UI Test");   
        ImGui::Text("FPS: %.1f", fps);
        ImGui::Checkbox("Render Chunks", &renderChunks);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

//handles keyboard inputs and sends to camera system
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->toggleLock();
}

//whenever the glfw window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

//tracks mouse movement across glfw window and sends info to camera system
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera->ProcessMouseMovement(xoffset, yoffset);
}
