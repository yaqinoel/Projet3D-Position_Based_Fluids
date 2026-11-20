#pragma once

#include <filesystem>
#include <string>
#include <glad/gl.h>


class Shader {
public:
    // constructor reads and builds the shader
    Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragPath);
    ~Shader();

    // use/activate the shader
    void use() const;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    // the program ID
    GLuint programId = 0;
};
