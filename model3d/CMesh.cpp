#include "CMesh.h"
#include<fstream>
#ifndef __APPLE__
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4
typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);
// VBO Extension Function Pointers
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;                  // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;                  // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;                  // VBO Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;                // VBO Deletion Procedure
#endif
using namespace std;
bool CMesh::load(const char* filename){
	ifstream fs(filename);
	if (fs.good()){
		printf("open file[%s] success\n",filename);
	}else{
		printf("open file[%s] failed\n",filename);
		return false;
	}
	std::vector<Vector3>        m_positions;
	std::vector<Vector3>        m_normals;
	std::vector<TexCoord>       m_texcoords;
	std::vector<Face>           m_faces;
    
	char tmp[128];
	Vector3 v;
	Vector3 n;
	texCoord t;
	bool hasnormal=false,hastexcood=false;
	Face fa;
    MtlInfo mtlinf;
    memset(&mtlinf, 0, sizeof(MtlInfo));
	char c = fs.get();
	while (c!=EOF){
		switch (c){	
            case 'v':{
                int ntmp=fs.peek();
                if (ntmp==32) {
                    fs>>v.x>>v.y>>v.z;
                    printf("v %f,%f,%f\n",v.x,v.y,v.z);
                    m_positions.push_back(v);
                }else if (ntmp=='n'){
                    fs>>c;
					hasnormal=true;
					fs>>n.x>>n.y>>n.z;
					m_normals.push_back(n);
				}else if(ntmp=='t'){
                    fs>>c;
					hastexcood=true;
					fs>>t.s>>t.t>>t.r;
					m_texcoords.push_back(t);
				}else{
                    fs.getline(tmp, 128,'\n');
					printf("ignore:%c%s\n",c,tmp);
				}
			}break;
            case '#':{
				fs.getline(tmp, 128,'\n');
				printf("#%s\n",tmp);
			}break;
            case 'f':{
				fs.getline(tmp,128,'\n');
				memset(&fa,0,sizeof(Face));
				int nowv=0,nowt=0;
				for (char*p=tmp;*p;p++){
					if(*p==32){
						if (nowt==2){
							nowt=0;
							nowv++;
						}
					}else if (*p>='0'&&*p<='9'){
						if (nowt==0){
							fa.pi[nowv]=fa.pi[nowv]*10+(*p-'0');
						}else if (nowt==1){
							fa.ti[nowv]=fa.ti[nowv]*10+(*p-'0');
						}else if (nowt==2){
							fa.ni[nowv]=fa.ni[nowv]*10+(*p-'0');
						}
					}else if (*p=='/'){
						nowt++;
					}
				}
                mtlinf.size+=3;
				m_faces.push_back(fa);
			}break;
            case 'u':{
                fs.getline(tmp, 128,' ');
                if (tmp[0]=='s'&&tmp[1]=='e'&&tmp[2]=='m'&&tmp[3]=='t'&&tmp[4]=='l'&&tmp[5]==0) {
                    fs.getline(tmp, 128,'\r');
                    if (mtlinf.name[0]) {
                        m_iMaterialArray.push_back(mtlinf);
                        mtlinf.first+=mtlinf.size;
                        mtlinf.size=0;
                    }
                    strcpy(mtlinf.name, tmp);
                    printf("usemtl %s\n",mtlinf.name);
                }else{
                    printf("ignore:u%s ",tmp);
                    fs.getline(tmp, 128,'\n');
                    printf("%s",tmp);
                }
            }break;
            case 'm':{
                fs.getline(tmp, 128,' ');
                if (tmp[0]=='t'&&tmp[1]=='l'&&tmp[2]=='l'&&tmp[3]=='i'&&tmp[4]=='b'&&tmp[5]==0) {
                    fs.getline(tmp, 128,'\r');
                    printf("mtllib %s\n",tmp);
                    if (tmp[1]!=':') {
                        int nlen=0;
                        char*p=const_cast<char*>(filename);
                        for (; *p; p++,nlen++);
                        for (; nlen>=0&&*p!='/'; p--,nlen--);
                        if (nlen>=0) {
                            memmove(&tmp[nlen+1], tmp, strlen(tmp)+1);
                            memcpy(tmp, filename, nlen+1);
                        }
                    }
                    m_iMaterial.load(tmp);
                }else{
                    printf("ignore:m%s ",tmp);
                    fs.getline(tmp, 128,'\n');
                    printf("%s",tmp);
                }
            }break;
		default:
			fs.getline(tmp, 128,'\n');
			printf("ignore:%c%s\n",c,tmp);
			break;
		}
		while (!fs.eof()&&fs.peek()==10){
			fs.getline(tmp, 128,'\n');
		}
		if (!fs.eof()){
			fs>>c;
		}else{
			break;
		}
	}
    if (mtlinf.name[0]) {
        m_iMaterialArray.push_back(mtlinf);
    }
	fs.close();
	m_nVertexCount = (GLsizei)(3*m_faces.size());
	m_pVertices = new GLfloat[3*m_nVertexCount];
	m_pNormals = new GLfloat[3*m_nVertexCount];
    unsigned long fnum = m_faces.size();
    GLfloat *p=m_pVertices,*q=m_pNormals;
	for (unsigned long i=0;i<fnum;i++){
        Face &fctmp=(Face&)m_faces[i];
		(*p++)=m_positions[fctmp.pi[0]-1].x;
		(*p++)=m_positions[fctmp.pi[0]-1].y;
		(*p++)=m_positions[fctmp.pi[0]-1].z;
		(*p++)=m_positions[fctmp.pi[1]-1].x;
		(*p++)=m_positions[fctmp.pi[1]-1].y;
		(*p++)=m_positions[fctmp.pi[1]-1].z;
		(*p++)=m_positions[fctmp.pi[2]-1].x;
		(*p++)=m_positions[fctmp.pi[2]-1].y;
		(*p++)=m_positions[fctmp.pi[2]-1].z;

		(*q++)=m_normals[fctmp.ni[0]-1].x;
		(*q++)=m_normals[fctmp.ni[0]-1].y;
		(*q++)=m_normals[fctmp.ni[0]-1].z;
		(*q++)=m_normals[fctmp.ni[1]-1].x;
		(*q++)=m_normals[fctmp.ni[1]-1].y;
		(*q++)=m_normals[fctmp.ni[1]-1].z;
		(*q++)=m_normals[fctmp.ni[2]-1].x;
		(*q++)=m_normals[fctmp.ni[2]-1].y;
		(*q++)=m_normals[fctmp.ni[2]-1].z;
	}
    
    glGenVertexArraysAPPLE(1, &m_vertexArray);
    glBindVertexArrayAPPLE(m_vertexArray);
    
    glGenBuffers(1, &m_nVBOVertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_nVBOVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*m_nVertexCount, m_pVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(attrib_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrib_position);
    
    glGenBuffers(1, &m_nVBONormals);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBONormals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*m_nVertexCount, m_pNormals, GL_STATIC_DRAW);
    
    glVertexAttribPointer(attrib_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrib_normal);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArrayAPPLE(0);
    
    delete []m_pVertices;
    m_pVertices=nullptr;
    delete []m_pNormals;
    m_pNormals=nullptr;
    
	m_positions.clear();
	m_normals.clear();
	m_texcoords.clear();
	m_faces.clear();
	return true;
}

int CMesh::initShader(){
    normalShader = NormalShader::sharedInstance();
    return normalShader?1:0;
}

void CMesh::draw(){
    if (!normalShader) {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x,y,z);
        glScalef(scalex, scaley, scalez);
        glRotatef(radian, rax, ray, raz);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        
        GLfloat globel_ambient[] = { 0.1 , 0.1 , 0.1 , 1.0 };
        //打开全局光照
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT , globel_ambient);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_nVBOVertices);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, m_nVBONormals);
        glNormalPointer(GL_FLOAT, 0, 0);
        glDrawArrays( GL_TRIANGLES, 0, m_nVertexCount);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }else{
        normalShader->use();
        
        Matrix4 projectionMatrix = setFrustum(-1.0f, 1.0f, -1.0f, 1.0f,1.0f, 40.0f);
        
        // Compute the model view matrix for the object rendered with GL
        Matrix4 modelViewMatrix;
        modelViewMatrix.identity();
        modelViewMatrix.rotate(radian, rax, ray, raz);
        modelViewMatrix.scale(scalex, scaley, scalez);
        modelViewMatrix.translate(x, y, z);
        
        
        
        Matrix4 _normalMatrix = modelViewMatrix;
        _normalMatrix.invert();
        
        Matrix4 _modelViewProjectionMatrix =  projectionMatrix * modelViewMatrix;
        
        normalShader->setModelViewProjectionMatrix(_modelViewProjectionMatrix.transpose().get());
        normalShader->setNormalMatrix(_normalMatrix.get());
        
        glBindVertexArrayAPPLE(m_vertexArray);
        for (int i=0; i<m_iMaterialArray.size(); i++) {
            Mtl*pmtl=m_iMaterial.find(m_iMaterialArray[i].name);
            normalShader->setDiffuseColor(pmtl->kd.d);
            glDrawArrays( GL_TRIANGLES, m_iMaterialArray[i].first, m_iMaterialArray[i].size);
        }
        glBindVertexArrayAPPLE(0);
        glUseProgram(0);
    }
}

CMesh::CMesh():m_nVBOVertices(0),m_nVertexCount(0),m_pVertices(nullptr),m_pNormals(nullptr),normalShader(nullptr),rax(0),ray(0),raz(1),x(0),y(0),z(0),scalex(1),scaley(1),scalez(1){
#ifndef __APPLE__
	glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
	glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
	glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
#endif
}
CMesh::CMesh(const char* filename):CMesh(){
    load(filename);
}

CMesh::~CMesh(){
	if (m_pVertices){
		delete[] m_pVertices;
	}
	if (m_pNormals){
		delete[] m_pNormals;
	}
	if (m_nVBOVertices){
		glDeleteBuffers(1,&m_nVBOVertices);
	}
    if (m_nVBONormals){
		glDeleteBuffers(1,&m_nVBONormals);
	}
    if (m_vertexArray) {
        glDeleteVertexArrays(1,&m_vertexArray);
    }
}

///////////////////////////////////////////////////////////////////////////////
// glFrustum()
///////////////////////////////////////////////////////////////////////////////
Matrix4 setFrustum(float l, float r, float b, float t, float n, float f)
{
    Matrix4 mat;
    mat[0]  = 2 * n / (r - l);
    mat[2]  = (r + l) / (r - l);
    mat[5]  = 2 * n / (t - b);
    mat[6]  = (t + b) / (t - b);
    mat[10] = -(f + n) / (f - n);
    mat[11] = -(2 * f * n) / (f - n);
    mat[14] = -1;
    mat[15] = 0;
    return mat;
}