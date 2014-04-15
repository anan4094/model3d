#ifndef MODEL3D
#define MODEL3D
#include "platform.h"

#include <vector>
#include "Shader.h"
#include "Matrices.h"
#include "Vectors.h"
#include "Material.h"
#include "Node.h"

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

class Mesh:public Node{
private:
	GLuint    m_nVerticesID;
    GLuint    m_nNormalsID;
	GLuint    m_nTexcoordsID;

    GLuint    m_nVertexArraysID;

	GLfloat   *m_pVertices;
	GLfloat   *m_pNormals;
	GLfloat   *m_pTexCoords;
	GLsizei   m_nVertexCount;

	GLfloat   m_fMaxX;
	GLfloat   m_fMaxY;
	GLfloat   m_fMaxZ;
	GLfloat   m_fMinX;
	GLfloat   m_fMinY;
	GLfloat   m_fMinZ;
    
    GLfloat   rax,ray,raz,radian;
    GLfloat   x,y,z;
    GLfloat   scalex,scaley,scalez;
	bool      m_bHasNormal;
	bool      m_bHasTexCoord;
    bool      m_bForceGenerateNormal;
    bool      m_bSmoothSurface;
    bool      m_bUseShader;
    Material  m_iMaterial;
    std::vector<MtlInfo> m_iMaterialArray;
    Shader    *m_piShader;
public:
    //加载模型
	bool load(const char* filename);
    //加载obj模型
	bool loadObj(const char* filename);
    //使用glsl着色器 不调用则不使用glsl着色
    int initShader();
    
    //绘制模型
	virtual void draw();
    
    
    inline void setRotationAxis(GLfloat rx,GLfloat ry,GLfloat rz){GLfloat len=sqrt(rx*rx+ry*ry+rz*rz); rax=rx/len;ray=ry/len;raz=rz/len;}
    inline void setRadian(GLfloat rad){radian=rad;}
    inline void setDegree(GLfloat deg){radian=deg*M_PI/180;}
    inline void setPosition(GLfloat x,GLfloat y,GLfloat z){this->x=x;this->y=y;this->z=z;};
    inline void setScale(GLfloat x,GLfloat y,GLfloat z){scalex=x;scaley=y;scalez=z;}
    //设置当模型没有法线时是否自动生成法线
    inline void setForceGenerateNormal(bool fgn){m_bForceGenerateNormal = fgn;}
    //设置当自己生成法线时是否光滑表面
    inline void setSmoothSurface(bool ss){m_bSmoothSurface = ss;}
	inline void setUseShader(bool us){m_bUseShader = us;}
	inline GLfloat getMaxX(){return m_fMaxX;}
	inline GLfloat getMinX(){return m_fMinX;}
	inline GLfloat getMaxY(){return m_fMaxY;}
	inline GLfloat getMinY(){return m_fMinY;}
	inline GLfloat getMaxZ(){return m_fMaxZ;}
	inline GLfloat getMinZ(){return m_fMinZ;}
	Mesh();
    Mesh(const char *name);
	~Mesh();
	void clear();
};
void getFilePath(const char *filename,char*childfilename);

#endif // !MODEL3D
