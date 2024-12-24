#include <iostream>
#include <vector>
#include <cmath>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "airfoil.h"
#include "drawer.h"

// Ekran boyutu
const int WINDOW_WIDTH  = 1280;
const int WINDOW_HEIGHT = 720;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void RenderGUI(float& r, float& g, float& b)
{
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
    Airfoil *airfoil;

    // Generate butonu
    if (ImGui::Button("Generate")) {
        std::cout << "Generate button clicked!" << std::endl;
        std::cout << "4 digit number: " << fourDigitInput << std::endl;
        std::cout << "Points: " << points << std::endl;
        std::cout << "Generating airfoil..." << std::endl;

        // NACA değerlerini çıkar
        double M = fourDigitInput / 1000;
        double P = (fourDigitInput / 100) % 10;
        double T = fourDigitInput % 100;

        // Yeni Airfoil nesnesi oluştur
        airfoil = new Airfoil(points, M, P, T);
        airfoil->calculateCoordinates();
        airfoil->saveCoordinates();
    }

    // Draw butonu
    if (ImGui::Button("Draw")) {
        std::cout << "Draw button clicked!" << std::endl;
        draw(airfoil->getAirfoilData());
    }

    // UI bitişi
    ImGui::End();

    ImGui::Render();
}

void RenderScene(const std::vector<std::pair<double, double>>& polygonPoints, float r, float g, float b)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.05, 1.05, -0.1, 0.1, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(r, g, b); // Dolgu rengi
    glBegin(GL_POLYGON);  
    for (auto &p : polygonPoints)
    {
        glVertex2d(p.first, p.second);
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f); 
    glBegin(GL_LINE_STRIP);
    for (auto &p : polygonPoints)
    {
        glVertex2d(p.first, p.second);
    }
    glEnd();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
    // GLFW başlat
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Airfoil with ImGui", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ImGui bağdaştırıcılarını başlat
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGui::StyleColorsDark();

    // -------------------------------------------------------
    // 1) Üst ve alt noktaları tek dizide (kapalı) birleştirelim
    // -------------------------------------------------------

    float fillColor[3] = {0.8f, 0.5f, 0.2f};
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Yeni bir çerçeve başlat
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // GUI ve sahneyi render edin
        RenderGUI(fillColor[0], fillColor[1], fillColor[2]);
        RenderScene(polygonPoints, fillColor[0], fillColor[1], fillColor[2]);

        // ImGui çizim verilerini işleyin
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Temizlik
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
