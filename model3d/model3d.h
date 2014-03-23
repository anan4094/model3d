#ifndef MODEL3D
#define MODEL3D
#include <windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#include <vector>

typedef union vector3{
	struct{
		GLfloat x,y,z;
	};
	GLfloat d[3];
}Vector3;

typedef union texCoord{
	struct{
		GLfloat s,t,r;
	};
	struct{
		GLfloat u,v,w;
	};
	GLfloat d[3];
}TexCoord;

typedef struct face{
	int pi[3];
	int ti[3];
	int ni[3];
}Face;

// ������Ϣ�ṹ�� 
typedef struct tag3DObject  
{ 
	int                         nMaterialID;       // ����ID 
	bool                        bHasTexture;       // �Ƿ��������ӳ�� 
	bool                        bHasNormal;        // �Ƿ���з��� 
	std::vector<Vector3>        PosVerts;          // ����Ķ��� 
	std::vector<Vector3>        Normals;           // ����ķ����� 
	std::vector<TexCoord>       Texcoords;         // ����UV���� 
	std::vector<unsigned short> Indexes;           // ����Ķ������� 
	unsigned int                nNumIndexes;       // ������Ŀ 
	GLuint                      nPosVBO; 
	GLuint                      nNormVBO; 
	GLuint                      nTexcoordVBO; 
	GLuint                      nIndexVBO; 
}t3DObject;

class CMesh{
private:
	GLuint  m_nVBOVertices;
	GLfloat *m_pVertices;
	int     m_nVertexCount;
public:
	bool load(const char* filename);
	void draw();
	void log();
	CMesh();
	~CMesh();
};

#endif // !MODEL3D
