//
//  Drawable.h
//  model3d
//
//  Created by 魏裕群 on 14-5-18.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Drawable__
#define __model3d__Drawable__
#include "platform.h"
#include "Matrices.h"
#include "Vectors.h"
#include "Node.h"
#include "Shader.h"
#include "Material.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846f
#endif

typedef struct face{
	int pi[3];
	int ti[3];
	int ni[3];
}Face;

typedef struct mtlInfo{
	char name[32];
	GLuint first;
	GLuint size;
}MtlInfo;

class Drawable:public Node{
public:
	Drawable();
	~Drawable();
	virtual void draw();
	virtual void clear();
protected:
	GLuint    m_nVerticesID;
	GLuint    m_nNormalsID;
	GLuint    m_nTexcoordsID;
	GLuint    m_nVertexArraysID;

	bool      m_bUseShader;
	bool      m_bHasNormal;
	bool      m_bHasTexCoord;

	GLfloat   *m_pVertices;
	GLfloat   *m_pNormals;
	GLfloat   *m_pTexCoords;
	GLsizei   m_nVertexCount;

	GLfloat   rax,ray,raz,radian;
	GLfloat   x,y,z;
	GLfloat   scalex,scaley,scalez;

	Material  m_iMaterial;
	std::vector<MtlInfo> m_iMaterialArray;
	Shader    *m_piShader;
};

#endif /* defined(__model3d__Drawable__) */
