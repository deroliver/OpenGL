#ifndef GRAPHICSOBJECT_H_
#define GRAPHICSOBJECT_H_

#define GLEW_STATIC
#define GLM_FORCE_RADIANS	

#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL\glew.h"			
#include <SOIL.h>
#include "../Headers/ShaderManager.h"
#include "../Headers/Camera.h"

const glm::vec3 lightPos(-1.7f, 100.0f, -4.0f);

class Vertex3 {
public:

	Vertex3() {}
	Vertex3(glm::vec3 v1) : xyz(v1) {}
	glm::vec3 xyz;
	glm::vec4 rgba;
	glm::vec2 tex;
	glm::vec3 norm;													
};


// Handles initializing, rendering, and destroying a 3D object
class GraphicsObject {

public:

	// Used for handling index properties for vertex properties
	static const GLuint VertexIndex = 0;		
	static const GLuint ColorIndex = 1;			
	static const GLuint TextureIndex = 2;	
	static const GLuint NormalIndex = 3;		

	GraphicsObject() { Scale = glm::vec3(1.0f, 1.0f, 1.0f); };
	~GraphicsObject() { Destroy(); }

	void Initialize(Vertex3 vertices[], int vLength, std::string VertexShader, std::string FragmentShader) {
		Shader.Initialize(VertexShader, FragmentShader);
	
		Vertices = vertices;
		VerticesLength = vLength;

		glGenVertexArrays(1, &VAOID);
		glGenBuffers(1, &VBOID);

		glBindVertexArray(VAOID);
			glBindBuffer(GL_ARRAY_BUFFER, VBOID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * vLength, Vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(VertexIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]), (GLvoid*)0);
			glEnableVertexAttribArray(VertexIndex);
			
			glVertexAttribPointer(ColorIndex, 4, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]), (GLvoid*)sizeof(Vertices[0].xyz));
			glEnableVertexAttribArray(ColorIndex);

			glVertexAttribPointer(TextureIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]), (GLvoid*)(sizeof(Vertices[0].xyz) + sizeof(Vertices[0].rgba)));
			glEnableVertexAttribArray(TextureIndex);
			
			glVertexAttribPointer(NormalIndex,  3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]), (GLvoid*)(sizeof(Vertices[0].xyz) + sizeof(Vertices[0].rgba) + sizeof(Vertices[0].tex)));
			glEnableVertexAttribArray(NormalIndex);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void InitLight() {
		GLuint ObjectColorID	= Shader.GetVariable("ObjectColor");
		GLuint LightColorID		= Shader.GetVariable("LightColor");
		GLuint LightPosID		= Shader.GetVariable("LightPos");
		GLuint ViewPosID		= Shader.GetVariable("ViewPos");

		GLuint MatAmbientID		= Shader.GetVariable("material.Ambient");
		GLuint MatDiffuseID		= Shader.GetVariable("material.Diffuse");
		GLuint MatSpecularID	= Shader.GetVariable("material.Specular");
		GLuint MatShineID		= Shader.GetVariable("material.Shininess");

		GLuint LightAmbientID	= Shader.GetVariable("light.Ambient");
		GLuint LightDiffuseID	= Shader.GetVariable("light.Diffuse");
		GLuint LightSpecularID	= Shader.GetVariable("light.Specular");

		Shader.SetFloat3(LightAmbientID,  0.2f,  0.2f,  0.2f);
		Shader.SetFloat3(LightDiffuseID, 0.75f, 0.75f, 0.75f);
		Shader.SetFloat3(LightSpecularID, 1.0f,  1.0f,  1.0f);

		Shader.SetFloat3(MatAmbientID,  1.0f, 0.5f, 0.31f);
		Shader.SetFloat3(MatDiffuseID,  1.0f, 0.5f, 0.31f);
		Shader.SetFloat3(MatSpecularID, 0.5f, 0.5f, 0.50f);
		Shader.SetFloat(MatShineID, 32.0f);

		Shader.SetFloat3(ViewPosID, Camera->Position.x, Camera->Position.y, Camera->Position.z);
		Shader.SetFloat3(LightPosID, lightPos.x, lightPos.y, lightPos.z);
	}

	void Render() {
		Shader.TurnOn();
		
		//InitLight();

		glm::mat4 Projection = Camera->GetProjMat();
		glm::mat4 View		 = Camera->GetViewMat();
		
		glm::mat4 Model = glm::translate(glm::mat4(1.0f), Position);
		Model = glm::scale(Model, Scale);
		Model = glm::rotate(Model, Rotation.x, glm::vec3(1, 0, 0));
		Model = glm::rotate(Model, Rotation.y, glm::vec3(0, 1, 0));
		Model = glm::rotate(Model, Rotation.z, glm::vec3(0, 0, 1));

		//glm::mat4 NormalMatrix = glm::transpose(glm::inverse(Model));

		GLuint ModelID		= Shader.GetVariable("Proj_Mat");
		GLuint ViewID		= Shader.GetVariable("View_Mat");
		GLuint ProjectionID = Shader.GetVariable("Mode_Mat");
		//GLuint NormalID		= Shader.GetVariable("NormalMatrix");

		Shader.SetMatrix4(ViewID,		1, GL_FALSE, &View[0][0]);
		Shader.SetMatrix4(ProjectionID, 1, GL_FALSE, &Projection[0][0]);
		Shader.SetMatrix4(ModelID,		1, GL_FALSE, &Model[0][0]);
		//Shader.SetMatrix4(NormalID,		1, GL_FALSE, &NormalMatrix[0][0]);

		glBindVertexArray(VAOID);

		glDrawArrays(GL_TRIANGLES, 0, VerticesLength);

		glBindVertexArray(0);

		Shader.TurnOff();
	}

	glm::vec3 GetPosition() { return Position; }
	void SetPosition(glm::vec3 pos) { Position = pos; }

	glm::vec3 GetRotation() { return Rotation; }
	void SetRotation(glm::vec3 rot) { Rotation = rot; }

	glm::vec3 GetScale() { return Scale; }
	void SetScale(glm::vec3 scal) { Scale = scal; }

	Camera *GetCamera() { return Camera; }
	void SetCamera(Camera *cam) { Camera = cam; }

	void Destroy() {
		if (VBOID) {

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &VBOID);
			VBOID = 0;
		}

		if (VAOID)
		{
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &VAOID);
			VAOID = 0;
		}

		Camera = nullptr;
	}

protected:

	Vertex3 *Vertices;					
	Vertex3 *Normal;

	GLint VerticesLength;				

	GLuint VBOID;				
	GLuint VAOID;	
	GLuint LightVAOID;
	GLuint TexBufferID;					
	GLuint Texture;

	glm::vec3 Position;					
	glm::vec3 Rotation;					
	glm::vec3 Scale;					

	ShaderManager Shader, Light;

	Camera *Camera;						
};


#endif