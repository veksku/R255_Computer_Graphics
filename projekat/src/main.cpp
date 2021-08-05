
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <rg/Error.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));;

bool firstMouse = true;
float lastX =  SCR_WIDTH / 2.0f;
float lastY =  SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float currentTime = glfwGetTime();
float timeDiff = 0.0f;
int flag = 1;

//pozicija poklopca
glm::vec3 poklopacPos(0.0f, 0.43f, 0.0f);

//lightcube
glm::vec3 flashlightPos(2.0f, 0.1f, 1.3f);

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
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // komanda glfwu da prati misa
     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //disabled ubagovan, ukljuci mouse integration
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    Shader lightingShader("resources/shaders/all_lights.vs", "resources/shaders/all_lights.fs");
    Shader flashlightShader("resources/shaders/flashlight.vs", "resources/shaders/flashlight.fs");

    Model ourModel = (FileSystem::getPath("resources/objects/flashlight/Linterna.obj"));
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices1[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

            //nema gore
            -0.50f,  0.5f, -0.50f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
            0.50f,  0.5f, -0.50f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
            0.50f,  0.5f,  0.50f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
            0.50f,  0.5f,  0.50f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
            -0.50f,  0.5f,  0.50f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
            -0.50f,  0.5f, -0.50f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };

    float vertices2[] = {
            -0.53f, -0.1f, -0.53f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
             0.53f, -0.1f, -0.53f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
             0.53f,  0.1f, -0.53f, 0.0f,  0.0f, -1.0f, 1.0f, 0.294f,
             0.53f,  0.1f, -0.53f, 0.0f,  0.0f, -1.0f, 1.0f, 0.294f,
            -0.53f,  0.1f, -0.53f, 0.0f,  0.0f, -1.0f, 0.0f, 0.294f,
            -0.53f, -0.1f, -0.53f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

            -0.53f, -0.1f,  0.53f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
             0.53f, -0.1f,  0.53f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
             0.53f,  0.1f,  0.53f, 0.0f,  0.0f,  1.0f, 1.0f, 0.294f,
             0.53f,  0.1f,  0.53f, 0.0f,  0.0f,  1.0f, 1.0f, 0.294f,
            -0.53f,  0.1f,  0.53f, 0.0f,  0.0f,  1.0f, 0.0f, 0.294f,
            -0.53f, -0.1f,  0.53f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

            -0.53f,  0.1f,  0.53f, -1.0f,  0.0f, 0.0f, 1.0f, 0.294f,
            -0.53f,  0.1f, -0.53f, -1.0f,  0.0f, 0.0f, 0.0f, 0.294f,
            -0.53f, -0.1f, -0.53f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,
            -0.53f, -0.1f, -0.53f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,
            -0.53f, -0.1f,  0.53f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,
            -0.53f,  0.1f,  0.53f, -1.0f,  0.0f, 0.0f, 1.0f, 0.294f,

             0.53f,  0.1f,  0.53f, 1.0f,  0.0f,  0.0f, 1.0f, 0.294f,
             0.53f,  0.1f, -0.53f, 1.0f,  0.0f,  0.0f, 0.0f, 0.294f,
             0.53f, -0.1f, -0.53f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
             0.53f, -0.1f, -0.53f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
             0.53f, -0.1f,  0.53f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
             0.53f,  0.1f,  0.53f, 1.0f,  0.0f,  0.0f, 1.0f, 0.294f,

            //nema dole

            //duz     vis    sir
            -0.53f,  0.1f, -0.53f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
             0.53f,  0.1f, -0.53f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
             0.53f,  0.1f,  0.53f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
             0.53f,  0.1f,  0.53f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
            -0.53f,  0.1f,  0.53f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
            -0.53f,  0.1f, -0.53f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };

    //_______________________________________
    //crtanje kutije
    unsigned int VBO1, kutijaVAO;
    glGenVertexArrays(1, &kutijaVAO);
    glGenBuffers(1, &VBO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glBindVertexArray(kutijaVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //_______________________________________
    // crtanje kutije izvora svetlosti
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    //_______________________________________
    //crtanje poklopca
    unsigned int VBO2, poklopacVAO;
    glGenVertexArrays(1, &poklopacVAO);
    glGenBuffers(1, &VBO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glBindVertexArray(poklopacVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //_______________________________________

    unsigned int texture1diffuse = loadTexture("resources/textures/gift.jpg"); //diffuse 1
    unsigned int texture2diffuse = loadTexture("resources/textures/glue.jpg"); //diffuse 2

    lightingShader.use();
    lightingShader.setInt("material.texture1diffuse",0);
    lightingShader.setInt("material.texture2diffuse", 1);



    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
//    shader.use();
//    shader.setInt("texture1", 0);
//    shader.setInt("texture2", 1);

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
        glClearColor(0.5f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
        float time = glfwGetTime();
        //racunanje vrednosti vremena da bi stisak tastera P mogao da stimulira pauziranje lampe
        if (flag == 1)
            currentTime = time - timeDiff;
        else
            timeDiff = time - currentTime;

        lightingShader.use();
//        lightingShader.setVec3("light.position", lightPos);

        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 4.0f);
        lightingShader.setVec3("dirlight.direction", glm::vec3(0.1f, -0.25, -0.4f));

// dirlight properties
        lightingShader.setVec3("dirlight.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("dirlight.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("dirlight.specular", 1.0f, 1.0f, 1.0f);

// pointlight properties
        lightingShader.setVec3("pointlight.position", flashlightPos+glm::vec3(1.3*cos(currentTime)-0.2, 0.0f, 1.3*sin(currentTime)-0.131));
        lightingShader.setVec3("pointlight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointlight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointlight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointlight.constant", 1.0f);
        lightingShader.setFloat("pointlight.linear", 0.09);
        lightingShader.setFloat("pointlight.quadratic", 0.032);

        // material properties
        //lightingShader.setFloat("material.shininess", 64.0f);


        // bind textures on corresponding texture units
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, texture1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
//        shader.use();
//        shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
//        shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
//        shader.setVec3("lightPos", lightPos);

//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        shader.setMat4("projection", projection);
//
//        // camera/view transformation
//        glm::mat4 view = camera.GetViewMatrix();
//        shader.setMat4("view", view);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2diffuse);

        // render kutije
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)1, glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);

        glBindVertexArray(kutijaVAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // render poklopca


        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::rotate(model2, (float)1, glm::vec3(0.0f, 1.0f, 0.0f)); //open model coords
        model2 = glm::translate(model2, poklopacPos);
        //model = glm::rotate(model, (float)1, glm::vec3(0.0f, 0.0f, 3.0f)); //open model coords
        //model = glm::translate(model, poklopacOpenPos); // open model coords
        lightingShader.setMat4("model", model2);

        glBindVertexArray(poklopacVAO);
//        glDrawArrays(GL_TRIANGLES, 0, 30);

        //render svetlosne kutije
        flashlightShader.use();
        flashlightShader.setMat4("projection", projection);
        flashlightShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, flashlightPos);
        model = glm::translate(model, glm::vec3(1.3*cos(currentTime)-0.2, 0.0f, 1.3*sin(currentTime)-0.131));
        model = glm::rotate(model, (float)5.7, glm::vec3(0.0f, 1.0f, 0.0));
        model = glm::scale(model, glm::vec3(0.12f)); // a smaller cube
        flashlightShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 30);
        //        glBindVertexArray(poklopacVAO);
        //        glDrawArrays(GL_TRIANGLES, 0, 36);


//        flashlightShader.use();
//        flashlightShader.setMat4("projection", projection);
//        flashlightShader.setMat4("view", view);
//        glDisable(GL_TEXTURE_2D);
        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, flashlightPos);
        model = glm::translate(model, glm::vec3(1.3*cos(currentTime), 0.0f, 1.3*sin(currentTime)));
        model = glm::rotate(model, (float)5.7, glm::vec3(0.0f, 1.0f, 0.0));
        model = glm::scale(model, glm::vec3(.1f));
        lightingShader.setMat4("model", model);

        //glDrawArrays(GL_TRIANGLES, 0, 36);
        ourModel.Draw(lightingShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &kutijaVAO);
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
void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // pomeranje kamere pomocu tastature
    //float speed = 2.5 * deltaTime;
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse){
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

unsigned int loadTexture(char const * path){
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data){
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else{
            cerr << "Error!" << endl;
            exit(EXIT_FAILURE);
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else{
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        flag = -flag;
}