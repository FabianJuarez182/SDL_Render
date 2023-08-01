#include <iostream>
#include <SDL2/SDL.h>
#include "color.h"
#include "face.h"
#include "triangle.h"
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

SDL_Renderer* renderer = nullptr;

Color clearColor(0, 0, 0, 255); // Black
Color currentColor(255, 255, 255, 255); // White

void line(const glm::vec3& start, const glm::vec3& end) {
    SDL_RenderDrawLine(renderer, static_cast<int>(start.x), static_cast<int>(start.y),
                       static_cast<int>(end.x), static_cast<int>(end.y));
}

void drawTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
    line(v1, v2);
    line(v2, v3);
    line(v3, v1);
}

void drawModel(const std::vector<glm::vec3>& vertices) {
    SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);

    for (size_t i = 0; i < vertices.size(); i += 3) {
        if (i + 2 < vertices.size()) {
            glm::vec3 v1 = vertices[i];
            glm::vec3 v2 = vertices[i + 1];
            glm::vec3 v3 = vertices[i + 2];
            int offsetX = WINDOW_WIDTH / 2;
            int offsetY = WINDOW_HEIGHT / 2;

            drawTriangle(v1 + glm::vec3(offsetX, offsetY, 0), v2 + glm::vec3(offsetX, offsetY, 0), v3 + glm::vec3(offsetX, offsetY, 0));
        }
    }
}

bool loadOBJ(const std::string& path, std::vector<glm::vec3>& out_vertices, std::vector<Face>& out_faces) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << path << std::endl;
        return false;
    }

    std::vector<glm::vec3> temp_vertices;
    std::vector<std::array<int, 3>> temp_faces;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } else if (type == "f") {
            std::array<int, 3> face_indices{};
            for (int i = 0; i < 3; i++) {
                std::string faceIndexStr;
                iss >> faceIndexStr;
                face_indices[i] = std::stoi(faceIndexStr) - 1;
            }
            temp_faces.push_back(face_indices);
        }
    }

    out_vertices = temp_vertices;

    // Convert std::array<int, 3> to std::vector<Face>
    out_faces.reserve(temp_faces.size());
    for (const auto& face : temp_faces) {
        out_faces.push_back({ face });
    }

    return true;
}

std::vector<glm::vec3> setupVertexArray(const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces)
{
    std::vector<glm::vec3> vertexArray;
    float scale = 50.0f; // Increase the scale factor for a larger model

    // For each face
    for (const auto& face : faces)
    {
        // Get the vertex positions from the input arrays using the indices from the face
        glm::vec3 vertexPosition1 = vertices[face.vertexIndices[0]];
        glm::vec3 vertexPosition2 = vertices[face.vertexIndices[1]];
        glm::vec3 vertexPosition3 = vertices[face.vertexIndices[2]];

        // Scale and flip the y-coordinate of the vertex positions
        glm::vec3 vertexScaled1 = glm::vec3(vertexPosition1.x * scale, -vertexPosition1.y * scale, vertexPosition1.z * scale);
        glm::vec3 vertexScaled2 = glm::vec3(vertexPosition2.x * scale, -vertexPosition2.y * scale, vertexPosition2.z * scale);
        glm::vec3 vertexScaled3 = glm::vec3(vertexPosition3.x * scale, -vertexPosition3.y * scale, vertexPosition3.z * scale);

        // Add the vertex positions to the vertex array
        vertexArray.push_back(vertexScaled1);
        vertexArray.push_back(vertexScaled2);
        vertexArray.push_back(vertexScaled3);
    }

    return vertexArray;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error: SDL_Init failed." << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL_SpaceShip", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Error: Could not create SDL window." << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Error: Could not create SDL renderer." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::string filePath = "src/SpaceshipJUA21440.obj";
    std::cout << "Loading OBJ file: " << filePath << std::endl;

    std::vector<glm::vec3> vertices;
    std::vector<Face> faces;

    if (!loadOBJ(filePath, vertices, faces)) {
        std::cout << "Error: Could not load OBJ file." << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::vector<glm::vec3> vertexArray = setupVertexArray(vertices, faces);

    // Clear the framebuffer with the clear color
    SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    drawModel(vertexArray);
    SDL_RenderPresent(renderer);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
