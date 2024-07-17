#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "shader.h"
#include "camera.h"
#include "chunk.h"
#include "chunkmanager.h"
#include "voxel.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

float deltaTime = 0.0f; //time between current and last frame
float lastFrame = 0.0f;	//time of last frame

float lastX = 400, lastY = 300; // Initialize mouse position to the center of the screen
bool firstMouse = true;

Camera* camera;

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    Shader currentShader = Shader("Shaders/simplevertexshader3D.vs", "Shaders/simplefragmentshader.fs");

    ChunkManager chunkManager = ChunkManager(16,16,16,3,3,3, currentShader);
    chunkManager.createChunks();
    chunkManager.loadChunks();

    //Render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        currentShader.use();
        //this part is something that I need to understand better
        //glm::mat4 model = glm::mat4(1.0f);
        //glm::mat4 view = camera->GetViewMatrix();
        //glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        //unsigned int modelLoc = glGetUniformLocation(currentShader.ID, "model");
        //unsigned int viewLoc = glGetUniformLocation(currentShader.ID, "view");
        //unsigned int projLoc = glGetUniformLocation(currentShader.ID, "projection");
        
        chunkManager.renderChunks(camera, SCR_WIDTH, SCR_HEIGHT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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
