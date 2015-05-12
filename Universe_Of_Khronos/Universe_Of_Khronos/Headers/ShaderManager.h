#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

#include <string>
#include <fstream>
#include "GL\glew.h"

// Shader class used to load and turn on/off shaders
class ShaderManager {
public:

	// Create an empty constructor and have the deconstructor release our memory.
	ShaderManager() {}
	~ShaderManager() { Destroy(); }

	// Loads text file for shader and returns string
	std::string LoadShaderFile(std::string File) {
		// Open the file passed
		std::ifstream fin(File.c_str());

		// Make sure file is open
		if (!fin)
			return "";

		std::string Line = "";
		std::string Text = "";

		// Store each line in the text file within a string object
		while (getline(fin, Line)) {
			Text += "\n" + Line;
		}

		// Close the file
		fin.close();

		// Return the text data
		return Text;
	}

	// Loads vertex/fragment shader from text file
	void Initialize(std::string VertFile, std::string FragFile) {
		// These will hold the shader's text file data
		std::string strVShader, strFShader;

		// Make sure the user passed in a vertex and fragment shader file
		if (!VertFile.length() || !FragFile.length())
			return;

		// If any of our shader pointers are set, let's free them first
		if (VertexShaderID || FragmentShaderID || ShaderProgramID)
			Destroy();

		// Reset the last OpenGL error so we can check if down below
		GLenum ErrorCheckValue = glGetError();

		// Get an Id to our vertex and fragment shaders
		VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Load the shaders from the respective files and store it in a string
		strVShader = LoadShaderFile(VertFile.c_str());
		strFShader = LoadShaderFile(FragFile.c_str());

		// Do a quick switch so we can do a double pointer below
		const char *szVShader = strVShader.c_str();
		const char *szFShader = strFShader.c_str();

		// Assigns the shader text file to each shader pointer
		glShaderSource(VertexShaderID, 1, &szVShader, nullptr);
		glShaderSource(FragmentShaderID, 1, &szFShader, nullptr);

		// Compile the shader code
		glCompileShader(VertexShaderID);
		glCompileShader(FragmentShaderID);

		// Create a program object to represent our shaders
		ShaderProgramID = glCreateProgram();

		// Attach each shader we just loaded to our program object
		glAttachShader(ShaderProgramID, VertexShaderID);
		glAttachShader(ShaderProgramID, FragmentShaderID);

		// Link our program object with OpenGL
		glLinkProgram(ShaderProgramID);

		// Check to see if any errors happened in this function
		ErrorCheckValue = glGetError();

		if (ErrorCheckValue != GL_NO_ERROR)
		{
			fprintf(stderr, "ERROR: Could not create the shader program with error Id: %d\n", ErrorCheckValue);
			exit(-1);
		}
	}

	// Returns ID for a variable in the shader
	GLint GetVariable(std::string Variable) {
		if (!ShaderProgramID)
			return -1;
		return glGetUniformLocation(ShaderProgramID, Variable.c_str());
	}

	// Functions to set an integer
	void SetInt(GLint id, int newvalue)											{ glUniform1i(id, newvalue);		}
	void SetFloat(GLint id, GLfloat newvalue)									{ glUniform1f(id, newvalue);		}
	void SetFloat2(GLint id, GLfloat v0, GLfloat v1)							{ glUniform2f(id, v0, v1);			}
	void SetFloat3(GLint id, GLfloat v0, GLfloat v1, GLfloat v2)				{ glUniform3f(id, v0, v1, v2);		}
	void SetFloat4(GLint id, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)	{ glUniform4f(id, v0, v1, v2, v3);	}

	// Allows us to pass a 4x4 matrix (array of 16 floats) to a shader using the ID receieved from GetVariable()
	void SetMatrix4(GLint id, GLsizei count, GLboolean transpose, const GLfloat *value) {
		glUniformMatrix4fv(id, count, transpose, value);
	}


	void TurnOn()	{ glUseProgram(ShaderProgramID); }
	void TurnOff()	{ glUseProgram(0); glBindVertexArray(0); }

	// Releases memory for the shader
	void Destroy() {
		if (VertexShaderID) {
			glDetachShader(ShaderProgramID, VertexShaderID);
			glDeleteShader(VertexShaderID);
			VertexShaderID = 0;
		}

		if (FragmentShaderID) {
			glDetachShader(ShaderProgramID, FragmentShaderID);
			glDeleteShader(FragmentShaderID);
			FragmentShaderID = 0;
		}

		if (ShaderProgramID) {
			glDeleteProgram(ShaderProgramID);
			ShaderProgramID = 0;
		}
	}

private:

	// Stores vertex shader information
	GLuint VertexShaderID;

	// Stores fragment shader information
	GLuint FragmentShaderID;

	// Stores program information for all shaders
	GLuint ShaderProgramID;	
};

#endif