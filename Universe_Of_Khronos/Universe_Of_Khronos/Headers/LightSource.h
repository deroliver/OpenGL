#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_
#include <iostream>

#include "../Headers/GraphicsObject.h"

class LightSource : public GraphicsObject {
public:

	LightSource() : GraphicsObject() {}

	void Initialize(Vertex3 vertices[], int vLength, std::string VertexShader, std::string FragmentShader) {
		Light.Initialize(VertexShader, FragmentShader);

		Vertices = vertices;
		VerticesLength = vLength;

		glGenVertexArrays(1, &LightVAOID);
		glGenBuffers(1, &VBOID);
			glBindVertexArray(LightVAOID);

			glBindBuffer(GL_ARRAY_BUFFER, VBOID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * VerticesLength, Vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(VertexIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]), (GLvoid*)0);
			glEnableVertexAttribArray(VertexIndex);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}


	void Render() {
		Light.TurnOn();

		glm::mat4 Projection = Camera->GetProjMat();
		glm::mat4 View		 = Camera->GetViewMat();

		glm::mat4 Model = glm::translate(glm::mat4(1.0f), lightPos);
		Model = glm::scale(Model, glm::vec3(0.2f));

		Model = glm::rotate(Model, Rotation.x, glm::vec3(1, 0, 0));
		Model = glm::rotate(Model, Rotation.y, glm::vec3(0, 1, 0));
		Model = glm::rotate(Model, Rotation.z, glm::vec3(0, 0, 1));
	
		GLuint ModelID		= Light.GetVariable("Model");
		GLuint ViewID		= Light.GetVariable("View");
		GLuint ProjectionID = Light.GetVariable("Projection");

		Light.SetMatrix4(ViewID,		1, GL_FALSE, &View[0][0]);
		Light.SetMatrix4(ProjectionID,	1, GL_FALSE, &Projection[0][0]);
		Light.SetMatrix4(ModelID,		1, GL_FALSE, &Model[0][0]);

		glBindVertexArray(LightVAOID);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		Light.TurnOff();
	}
};


#endif