#ifndef MODEL3D
#define MODEL3D

#include <vector>
#include "Drawable.h"


class Mesh:public Drawable{
private:

	GLfloat   m_fMaxX;
	GLfloat   m_fMaxY;
	GLfloat   m_fMaxZ;
	GLfloat   m_fMinX;
	GLfloat   m_fMinY;
	GLfloat   m_fMinZ;
    
    bool      m_bForceGenerateNormal;
    bool      m_bSmoothSurface;
public:
    //加载模型
	bool load(const char* filename);
    //加载obj模型
	bool loadObj(const char* filename);
    //使用glsl着色器 不调用则不使用glsl着色
    int initShader();
    
    virtual bool screenSizeChange(int width,int height);
    
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
};
void getFilePath(const char *filename,char*childfilename);

#endif // !MODEL3D
