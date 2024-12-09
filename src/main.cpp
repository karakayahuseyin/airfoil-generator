#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>
#include "generator.h"

// Shader kodları
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Kırmızı renk
}
)";

int generate_airfoil(int num_points, int digit) {
    double M, P, T ;
    M = digit / 1000;
    P = (digit / 100) % 10;
    T = digit % 100;
    Airfoil *airfoil = create_airfoil(num_points, M, P, T);

    if (!airfoil) {
        printf("Airfoil oluşturulamadı.\n");
        return 1;
    }

    airfoil->calculate_coordinates(airfoil);

    printf("x\tXu\tYu\tXl\tYl\n");
    for (int i = 0; i < airfoil->num_points; i++) {
        printf("%.4f\t%.4f\t%.4f\t%.4f\t%.4f\n",
               airfoil->x[i], airfoil->xu[i], airfoil->yu[i], airfoil->xl[i], airfoil->yl[i]);
    }

    free_airfoil(airfoil);
    return 0;
}

// OpenGL sahneyi render etme
void RenderScene(unsigned int VAO) {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

// UI çizme fonksiyonu
void RenderUI() {
    // UI pencere boyutu ve konumu
    ImGui::SetNextWindowSize(ImVec2(300, 200));
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);

    // UI başlangıcı
    ImGui::Begin("Control Panel", NULL, ImGuiWindowFlags_NoResize);

    // 4 haneli sayı girişi
    static int fourDigitInput;
    ImGui::InputInt("4 digit number", &fourDigitInput);

    // Nokta sayısı girişi
    static int points = 100; // Default olarak 100
    ImGui::InputInt("Points", &points);

    // Generate butonu
    if (ImGui::Button("Generate")) {
        std::cout << "Generate button clicked!" << std::endl;
        std::cout << "4 digit number: " << fourDigitInput << std::endl;
        std::cout << "Points: " << points << std::endl;
        std::cout << "Generating airfoil..." << std::endl;
        generate_airfoil(points, fourDigitInput);
    }

    // Draw butonu
    if (ImGui::Button("Draw")) {
        std::cout << "Draw button clicked!" << std::endl;
    }

    // UI bitişi
    ImGui::End();
}

int main() {
    // GLFW başlat
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // OpenGL bağlamı oluştur
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL and ImGui Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD'ı başlat
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ImGui'yi başlat
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsLight();

    // Shader'ları derle
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Üçgen verilerini hazırlama
    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Ana döngü
    glUseProgram(shaderProgram);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // ImGui frame başlat
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI ve sahneyi çiz
        RenderUI();
        glClearColor(0.7f, 0.85f, 1.0f, 1.0f); // Açık mavi arka plan
        glClear(GL_COLOR_BUFFER_BIT);
        RenderScene(VAO);

        // ImGui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Temizlik
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

