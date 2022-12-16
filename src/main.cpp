/**

 Anton Gorshkov, CTS
 
 3D Model: Formula 1 featuring multi-texturing on multiple meshes made in OpenGL/C++
 as a 3D-project for the subject Computer Graphics, teacher: Prof. Alfred M. Franz

 Model created in Blender by Andrew Mezentsew (Teliri),
 licensed under Creative Commons Attribution 4.0 International (CC BY 4.0)
 https://skfb.ly/oBuEo
 The model was downloaded from the source: https://free3d.com/3d-model/formula-1-lowpoly-87257.html

 Code-template by Tomasz Gałaj (Copyright 2018)
 https://shot511.github.io/2018-05-29-how-to-setup-opengl-project-with-cmake/
 https://github.com/Shot511/OpenGLSampleCmake

 Original code for loading the model with Assimp by LearnOpenGL
 https://learnopengl.com/Model-Loading/Assimp

 **/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rendering/Shader.h"
#include "rendering/Texture.h"
#include "rendering/Model.h"

GLFWwindow *window;
const float WINDOW_SIZE_COEFFICIENT = 2.3f; //TODO created a window size coefficient
const int WINDOW_WIDTH = (int) (1024 * WINDOW_SIZE_COEFFICIENT);
const int WINDOW_HEIGHT = (int) (768 * WINDOW_SIZE_COEFFICIENT);

Model *mesh = nullptr;
Shader *shader = nullptr;

Texture *texture_1 = nullptr;
Texture *texture_2 = nullptr;
Texture *texture_3 = nullptr;
Texture *texture_4 = nullptr;

Texture *texture_5 = nullptr;
Texture *texture_6 = nullptr;
Texture *texture_7 = nullptr;
Texture *texture_8 = nullptr;


/* Matrices */
glm::vec3 cam_position = glm::vec3(0.0f, 12.0f, 12.0f); // TODO changed camera position in an appropriate way
glm::vec3 cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cam_up = glm::vec3(0.0f, 1.0f, 0.0f);
// TODO corrected downloaded object in order to recreate the whole car, not only one weel
glm::mat4 world_matrix = glm::mat4(1.0f); // TODO what is it?
glm::mat4 view_matrix = glm::lookAt(cam_position, cam_look_at, cam_up);
glm::mat4 projection_matrix = glm::perspectiveFov(glm::radians(60.0f), float(WINDOW_WIDTH), float(WINDOW_HEIGHT), 0.1f,
                                                  200.0f); // TODO blinds

glm::vec3 lightPos = glm::vec3(15.0f, 15.0f, 0.0f);

void window_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    projection_matrix = glm::perspectiveFov(glm::radians(60.0f), float(width), float(height), 0.1f, 200.0f);

    if (shader != nullptr) {
        shader->setUniformMatrix4fv("viewProj", projection_matrix * view_matrix);
    }
}

bool init() {
    /* Initialize the library */
    if (!glfwInit())
        return false;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Model: Formula 1", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetWindowSizeCallback(window, window_size_callback);

    /* Initialize glad */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    /* Set the viewport */
    glClearColor(0.6f, 0.6f, 0.6f, 0.0f); // TODO background color
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glEnable(GL_DEPTH_TEST); // TODO without it we can look through

    return true;
}

int loadContent() {

    mesh = new Model("res/models/F1_lowpoly/F1.obj"); // TODO works somehow: F1 car with a strange texture

    // Create and apply basic shaders
    shader = new Shader("Basic.vert", "Basic.frag");
    shader->apply();

    shader->setUniformMatrix4fv("world", world_matrix);
    shader->setUniformMatrix3fv("normalMatrix", glm::inverse(glm::transpose(glm::mat3(world_matrix))));
    shader->setUniformMatrix4fv("viewProj", projection_matrix * view_matrix);

    shader->setUniform3fv("lightPos", lightPos);
    shader->setUniform3fv("viewPos", cam_position);

    texture_1 = new Texture(1);
    texture_1->load("res/models/F1_lowpoly/texture/jeans-1161035_1920.png");
    texture_1->bind(1);

    texture_2 = new Texture(2);
    texture_2->load("res/models/F1_lowpoly/texture/FabricAnimalBlueVinyl001/FabricAnimalBlueVinyl001_NRM_2K.png");
    texture_2->bind(2);

    texture_3 = new Texture(3);
    texture_3->load("res/models/F1_lowpoly/texture/l.png");
    texture_3->bind(3);

    texture_4 = new Texture(4);
    texture_4->load("res/models/F1_lowpoly/texture/l_diff.png");
    texture_4->bind(4);

    texture_5 = new Texture(5);
    texture_5->load("res/models/F1_lowpoly/texture/Rubber 0390.jpg");
    texture_5->bind(5);

    texture_6 = new Texture(6);
    texture_6->load("res/models/F1_lowpoly/texture/Rubber 0390.jpg");
    texture_6->bind(6);

    texture_7 = new Texture(7);
    texture_7->load("res/models/F1_lowpoly/texture/Rubber 0390.jpg");
    texture_7->bind(7);

    texture_8 = new Texture(8);
    texture_8->load("res/models/F1_lowpoly/texture/Rubber 0390.jpg");
    texture_8->bind(8);

    return true;
}

void render(float time) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Draw our triangle */
    world_matrix = glm::rotate(glm::mat4(1.0f), time * glm::radians(-30.0f), glm::vec3(0, 1, 0));

    shader->setUniformMatrix4fv("world", world_matrix); //TODO not moving, but lightning
    shader->setUniformMatrix3fv("normalMatrix", glm::inverse(glm::transpose(glm::mat3(world_matrix))));

    shader->apply();
    //TODO makes colorful
    texture_1->bind(1);
    texture_2->bind(2);
    texture_3->bind(3);
    texture_4->bind(4);
    mesh->Draw(0); // draws the body of the car
    //TODO makes the whole car colorful
    texture_5->bind(1);
    texture_6->bind(2);
    texture_7->bind(3);
    texture_8->bind(4);
    mesh->Draw(1); // draws the wheels of the car
}

void keyboard() {
    // if the ESC key was pressed: close the window
    if (GetAsyncKeyState(VK_ESCAPE)) exit(0); //TODO made ESC to leave the scene
    std::cerr << "keyboard()" << std::endl;
}

void update() {
    auto startTime = static_cast<float>(glfwGetTime());
    float newTime = 0.0f;
    float gameTime = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Update game time value */
        newTime = static_cast<float>(glfwGetTime());
        gameTime = newTime - startTime;

        /* Render here */
        render(gameTime);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        std::cerr << "update(): before keyboard()" << std::endl;
        // input handling
        keyboard();
    }
}

int main() {
    if (!init())
        return -1;

    if (!loadContent())
        return -1;

    update();

    glfwTerminate();

    delete mesh;
    delete shader;

    delete texture_1;
    delete texture_2;
    delete texture_3;
    delete texture_4;

    delete texture_5;
    delete texture_6;
    delete texture_7;
    delete texture_8;

    return 0;
}