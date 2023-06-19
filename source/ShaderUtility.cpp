#include "ShaderUtility.h"
#include <iostream>
#include <cassert>

GLuint ShaderUtility::CompileShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) {
	GLint succeeded = GL_FALSE;

	// Se crean objetos para los shaders:

	GLuint   vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	// Se carga el código de los shaders:

	const char* vertex_shaders_code[] = { vertexShaderCode.c_str() };
	const char* fragment_shaders_code[] = { fragmentShaderCode.c_str() };
	const GLint    vertex_shaders_size[] = { (GLint)vertexShaderCode.size() };
	const GLint  fragment_shaders_size[] = { (GLint)fragmentShaderCode.size() };

	glShaderSource(vertex_shader_id, 1, vertex_shaders_code, vertex_shaders_size);
	glShaderSource(fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

	// Se compilan los shaders:

	glCompileShader(vertex_shader_id);
	glCompileShader(fragment_shader_id);

	// Se comprueba que si la compilación ha tenido éxito:

	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
	if (!succeeded) ShaderUtility::ShowCompilationError(vertex_shader_id);

	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
	if (!succeeded) ShaderUtility::ShowCompilationError(fragment_shader_id);

	// Se crea un objeto para un programa:

	GLuint program_id = glCreateProgram();

	// Se cargan los shaders compilados en el programa:

	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);

	// Se linkan los shaders:

	glLinkProgram(program_id);

	// Se comprueba si el linkage ha tenido éxito:

	glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
	if (!succeeded) ShaderUtility::ShowLinkageError(program_id);

	// Se liberan los shaders compilados una vez se han linkado:

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return (program_id);
}

void ShaderUtility::ShowCompilationError(GLuint shaderId) {
	std::string info_log;
	GLint  info_log_length;

	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &info_log_length);

	info_log.resize(info_log_length);

	glGetShaderInfoLog(shaderId, info_log_length, NULL, &info_log.front());

	std::cerr << info_log.c_str() << std::endl;

#ifdef _MSC_VER
	//OutputDebugStringA (info_log.c_str ());
#endif

	assert(false);
}

void ShaderUtility::ShowLinkageError(GLuint programId) {
	std::string info_log;
	GLint  info_log_length;

	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &info_log_length);

	info_log.resize(info_log_length);

	glGetProgramInfoLog(programId, info_log_length, NULL, &info_log.front());

	std::cerr << info_log.c_str() << std::endl;

#ifdef _MSC_VER
	//OutputDebugStringA (info_log.c_str ());
#endif

	assert(false);
}
