
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <rg/Error.h>
#include <rg/Camera.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera;

bool firstMouse = true;
float lastX =  SCR_WIDTH / 2.0f;
float lastY =  SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//lighting
glm::vec3 poklopacPos(0.0f, 0.43f, 0.0f);

int main(){
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // komanda glfwu da prati misa
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //disabled ubagovan, ukljuci mouse integration
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    Shader shader("resources/shaders/2.model_lighting.vs", "resources/shaders/2.model_lighting.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices1[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f
//          duz     vis    sir
//        -0.53f,  0.5f, -0.53f,  0.0f, 1.0f,
//         0.53f,  0.5f, -0.53f,  1.0f, 1.0f,
//         0.53f,  0.5f,  0.53f,  1.0f, 0.0f,
//         0.53f,  0.5f,  0.53f,  1.0f, 0.0f,
//        -0.53f,  0.5f,  0.53f,  0.0f, 0.0f,
//        -0.53f,  0.5f, -0.53f,  0.0f, 1.0f
    };

    float vertices2[] = {
            -0.53f, -0.1f, -0.53f,  0.0f, 0.0f,
            0.53f, -0.1f, -0.53f,  1.0f, 0.0f,
            0.53f,  0.1f, -0.53f,  1.0f, 0.294f,
            0.53f,  0.1f, -0.53f,  1.0f, 0.294f,
            -0.53f,  0.1f, -0.53f,  0.0f, 0.294f,
            -0.53f, -0.1f, -0.53f,  0.0f, 0.0f,

            -0.53f, -0.1f,  0.53f,  0.0f, 0.0f,
            0.53f, -0.1f,  0.53f,  1.0f, 0.0f,
            0.53f,  0.1f,  0.53f,  1.0f, 0.294f,
            0.53f,  0.1f,  0.53f,  1.0f, 0.294f,
            -0.53f,  0.1f,  0.53f,  0.0f, 0.294f,
            -0.53f, -0.1f,  0.53f,  0.0f, 0.0f,

            -0.53f,  0.1f,  0.53f,  1.0f, 0.294f,
            -0.53f,  0.1f, -0.53f,  0.0f, 0.294f,
            -0.53f, -0.1f, -0.53f,  0.0f, 0.0f,
            -0.53f, -0.1f, -0.53f,  0.0f, 0.0f,
            -0.53f, -0.1f,  0.53f,  1.0f, 0.0f,
            -0.53f,  0.1f,  0.53f,  1.0f, 0.294f,

            0.53f,  0.1f,  0.53f,  1.0f, 0.294f,
            0.53f,  0.1f, -0.53f,  0.0f, 0.294f,
            0.53f, -0.1f, -0.53f,  0.0f, 0.0f,
            0.53f, -0.1f, -0.53f,  0.0f, 0.0f,
            0.53f, -0.1f,  0.53f,  1.0f, 0.0f,
            0.53f,  0.1f,  0.53f,  1.0f, 0.294f,

//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f
            //          duz     vis    sir
            -0.53f,  0.1f, -0.53f,  0.0f, 1.0f,
            0.53f,  0.1f, -0.53f,  1.0f, 1.0f,
            0.53f,  0.1f,  0.53f,  1.0f, 0.0f,
            0.53f,  0.1f,  0.53f,  1.0f, 0.0f,
            -0.53f,  0.1f,  0.53f,  0.0f, 0.0f,
            -0.53f,  0.1f, -0.53f,  0.0f, 1.0f
    };

//    glm::vec3 cubePositions[] = {
//            glm::vec3( 0.0f,  -0.5f,  0.0f),
//            glm::vec3( 0.0f,  -5.0f, 0.0f)
//    };

    //_______________________________________
    //crtanje kutije
    unsigned int VBO1, kutijaVAO;
    glGenVertexArrays(1, &kutijaVAO);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(kutijaVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //asd
    //_______________________________________
    //crtanje poklopca
    unsigned int VBO2, poklopacVAO;
    glGenVertexArrays(1, &poklopacVAO);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(poklopacVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //_______________________________________

    unsigned int texture1,texture2;
    // tekstura za boju ukrasne folije
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ucitavanje slike, pravljenje teksture i generisanje mipmapa
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/gift.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // tekstura za efekat lepka
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ucitavanje slike, pravljenje teksture i generisanje mipmapa
    data = stbi_load(FileSystem::getPath("resources/textures/glue.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    camera.Position = glm::vec3 (0,0,3);
    camera.Front = glm::vec3 (0,0,-1);
    camera.Up = glm::vec3 (0,1,0);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        unsigned int viewLoc  = glGetUniformLocation(shader.ID, "view");
        shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        // render kutije
        glBindVertexArray(kutijaVAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)1, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // render poklopca
        glBindVertexArray(poklopacVAO);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)1, glm::vec3(0.0f, 1.0f, 0.0f)); //open model coords
        model = glm::translate(model, poklopacPos);
        //model = glm::rotate(model, (float)1, glm::vec3(0.0f, 0.0f, 3.0f)); //open model coords
        //model = glm::translate(model, poklopacOpenPos); // open model coords

        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);


//        glBindVertexArray(poklopacVAO);
//        glDrawArrays(GL_TRIANGLES, 0, 36);










//        for (unsigned int i = 0; i < 2; i++){
//            // calculate the model matrix for each object and pass it to shader before drawing
//            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//            if (i==0)
//                model = glm::rotate(model, (float)1, glm::vec3(0.0f, 1.0f, 0.0f));
//            else
//                model = glm::rotate(model, (float)1, glm::vec3(0.0f, 1.0f, 0.0f));
//            model = glm::translate(model, cubePositions[i]);
//
//            shader.setMat4("model", model);
//
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        }



        //glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &kutijaVAO);//vrv kutijaVAO
    glDeleteVertexArrays(1, &poklopacVAO);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &VBO2);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // pomeranje kamere pomocu tastature
    float speed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, 1);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(yoffset);
}