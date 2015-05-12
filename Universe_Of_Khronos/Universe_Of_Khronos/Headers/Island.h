#ifndef ISLAND_H_
#define ISLAND_H_

#include "../Headers/GraphicsObject.h"

glm::vec3 Normals[] = {
	glm::vec3( 0.0f,  0.0f,  1.0f), 
	glm::vec3( 0.0f,  0.0f, -1.0f), 
	glm::vec3(-1.0f,  0.0f,  0.0f), 
	glm::vec3( 1.0f,  0.0f,  0.0f), 
	glm::vec3( 0.0f, -1.0f,  0.0f), 
	glm::vec3( 0.0f,  1.0f,  0.0f), 
};


glm::vec3 Quad[8] {
		glm::vec3(-2.0, -2.0, 2.0),
		glm::vec3( 2.0, -2.0, 2.0),
		glm::vec3( 2.0,  2.0, 2.0),
		glm::vec3(-2.0,  2.0, 2.0),

		glm::vec3(-2.0, -2.0, -2.0),
		glm::vec3( 2.0, -2.0, -2.0),
		glm::vec3( 2.0,  2.0, -2.0),
		glm::vec3(-2.0,  2.0, -2.0),
};

class Island : public GraphicsObject {
public:

	struct Block {
		GLfloat Width = 4.0f;
		GLboolean Draw = true;
	};


	Island() : GraphicsObject() {}

	void InitIslandData() {
		for (int i = 0; i < 25; i++) {
			for (int j = 0; j < 25; j++) {
				for (int k = 0; k < 25; k++){
					if (true) {
						CreateBlock(i, j, k);
					}
				}
			}
		}
	}

	void CreateBlock(int x, int z, int y) {
		CreateFace(0, 1, 2, 3, x, z, y); 
		CreateFace(4, 5, 6, 7, x, z, y);
		CreateFace(4, 0, 3, 7, x, z, y);
		CreateFace(1, 2, 6, 5, x, z, y);
		CreateFace(4, 0, 1, 5, x, z, y);
		CreateFace(3, 7, 6, 2, x, z, y);
	}


	void CreateFace(int A, int B, int C, int D, int x, int z, int y) {
		vertices[Index].xyz = Quad[A] + glm::vec3(4.0f * x, 4.0f * z, 4.0f * y); vertices[Index].norm = Normals[NormIndex]; Index++;
		vertices[Index].xyz = Quad[B] + glm::vec3(4.0f * x, 4.0f * z, 4.0f * y); vertices[Index].norm = Normals[NormIndex]; Index++;
		vertices[Index].xyz = Quad[C] + glm::vec3(4.0f * x, 4.0f * z, 4.0f * y); vertices[Index].norm = Normals[NormIndex]; Index++;

		vertices[Index].xyz = Quad[C] + glm::vec3(4.0f * x, 4.0f * z, 4.0f * y); vertices[Index].norm = Normals[NormIndex]; Index++;
		vertices[Index].xyz = Quad[D] + glm::vec3(4.0f * x, 4.0f * z, 4.0f * y); vertices[Index].norm = Normals[NormIndex]; Index++;
		vertices[Index].xyz = Quad[A] + glm::vec3(4.0f * x, 4.0f * z, 4.0f * y); vertices[Index].norm = Normals[NormIndex]; Index++;

		NormIndex++;
	}

	void Initialize( std::string VertexShader, std::string FragmentShader) {
		Shader.Initialize(VertexShader, FragmentShader);

		Vertices = vertices;
		VerticesLength = 562500;

		glGenVertexArrays(1, &VAOID);
		glGenBuffers(1, &VBOID);

		glBindVertexArray(VAOID);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * VerticesLength, Vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(VertexIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]), (GLvoid*)0);
		glEnableVertexAttribArray(VertexIndex);

		glVertexAttribPointer(ColorIndex, 4, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]), (GLvoid*)sizeof(Vertices[0].xyz));
		glEnableVertexAttribArray(ColorIndex);

		glVertexAttribPointer(TextureIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]), (GLvoid*)(sizeof(Vertices[0].xyz) + sizeof(Vertices[0].rgba)));
		glEnableVertexAttribArray(TextureIndex);

		glVertexAttribPointer(NormalIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]), (GLvoid*)(sizeof(Vertices[0].xyz) + sizeof(Vertices[0].rgba) + sizeof(Vertices[0].tex)));
		glEnableVertexAttribArray(NormalIndex);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


private:
	
	int Index = 0;
	int NormIndex = 0;
	Block Blocks[25][25][25];
	Vertex3 vertices[562500];
};



#endif