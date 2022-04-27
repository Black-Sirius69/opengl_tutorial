#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>

struct ShaderSource {
    std::string vertex;
    std::string fragment;
};

ShaderSource loadShader(const std::string &source) {

    std::ifstream file(source);

    enum class ShaderType {
        kNone = -1,
        kVertexShader = 0,
        kFragmentShader = 1
    };
    ShaderType type = ShaderType::kNone;
    std::stringstream ss[2];
    std::string line;
    while (getline(file, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::kVertexShader;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::kFragmentShader;
        } else {
            ss[(int) type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

static unsigned int compileShader(unsigned int type, const std::string &source) {
    unsigned int shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *) alloca(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);
        std::cout << "Error Failed to compile " << (type == GL_VERTEX_SHADER ? "Vertex " : "Fragment ") << "shader: "
                  << message << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static unsigned int createShader(const std::string &vertex_shader, const std::string &fragment_shader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main() {
    if (!glfwInit())
        return -1;

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error: " << glGetError() << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float vertices[] = {
            // Vertex             color
            -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  1.0f, 1.0f, 0.5f
    };

    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    /* Vertex Buffer */
    unsigned int vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBindVertexArray(vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* Index Buffer */
    unsigned int index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    ShaderSource source = loadShader("./shaders/shader.glsl");
    unsigned int shader = createShader(source.vertex, source.fragment);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,  nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}