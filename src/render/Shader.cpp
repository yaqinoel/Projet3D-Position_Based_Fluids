#include "Shader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

// reader shader source code from a fie
std::string readShaderFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open shader: " + path.string());
    }

    std::ostringstream source;
    source << file.rdbuf();
    if (!source || file.bad()) {
        throw std::runtime_error("Could not read shader source: " + path.string());
    }

    return source.str();
}

// compile shader by type
GLuint compileShader(GLenum type, const std::filesystem::path& path) {
    const std::string source = readShaderFile(path);
    const char* text = source.c_str();

    const GLuint shader = glCreateShader(type);
    if (shader == 0) {
        throw std::runtime_error("Could not create shader: " + path.string());
    }

    glShaderSource(shader, 1, &text, nullptr);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE) {
        return shader;
    }

    char log[2048]{};   // error informations when not compiled
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    glDeleteShader(shader);

    throw std::runtime_error(
        "Could not compile shader " + path.string() + "\n" + log
    );
}

// link vertex shader and frag shader, return program ID
GLuint linkProgram(GLuint vertex, GLuint fragment,
                   const std::filesystem::path& vertexPath,
                   const std::filesystem::path& fragmentPath) {
    const GLuint program = glCreateProgram();
    if (program == 0) {
        throw std::runtime_error("Could not create shader program");
    }

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    // when link failed, print error
    if (linked != GL_TRUE) {
        char log[2048]{};
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        glDeleteProgram(program);

        throw std::runtime_error(
            "Could not link " + vertexPath.string()
            + " and " + fragmentPath.string() + "\n" + log
        );
    }

    glDetachShader(program, vertex);
    glDetachShader(program, fragment);
    return program;
}

// constructor for shader, take shader paths, return Shader instance where keep program id
Shader::Shader(const std::filesystem::path& vertexPath,
               const std::filesystem::path& fragPath) {
    const GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexPath);
    GLuint fragment = 0;

    try {
        fragment = compileShader(GL_FRAGMENT_SHADER, fragPath);
        programId = linkProgram(vertex, fragment, vertexPath, fragPath);
    } catch (...) {
        glDeleteShader(vertex);
        if (fragment != 0) {
            glDeleteShader(fragment);
        }
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


Shader::~Shader() {
    glDeleteProgram(programId);
}

// use/activate the shader
void Shader::use() const {
    glUseProgram(programId);
}


void Shader::setBool(const std::string& name, bool value) const {
    setInt(name, value ? 1 : 0);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    const GLint location = glGetUniformLocation(programId, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}