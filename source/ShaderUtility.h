#ifndef SHADER_UTILITY_H
#define SHADER_UTILITY_H

#include <glad/glad.h>
#include <string>

class ShaderUtility {
public:
    static GLuint CompileShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);

private:
    static void ShowCompilationError(GLuint shaderId);
    static void ShowLinkageError(GLuint programId);
};

#endif // SHADER_UTILITY_H
