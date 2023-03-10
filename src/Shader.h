#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
private:
    // the program ID
    unsigned int ID{0};

public:
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    auto use() const -> void;
    // utility uniform functions
    auto setBool(const std::string& name, bool value) const -> void;
    auto setInt(const std::string& name, int value) const -> void;
    auto setFloat(const std::string& name, float value) const -> void;
    auto setMat4(const std::string& name, const glm::mat4 &mat) const -> void;
};
  
#endif