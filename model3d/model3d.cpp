#include "model3d.h"
#include<fstream>
#ifdef WIN32
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
		printf("open file success\n");
	}else{
		printf("open file failed\n");
		return false;
	}
	std::vector<Vector3>        m_positions;
	std::vector<Vector3>        m_normals;
	std::vector<TexCoord>       m_texcoords;
	std::vector<Face>           m_faces;
	m_positions.clear();
	char c;
	char tmp[128];
	Vector3 v;
	Vector3 n;
	texCoord t;
	bool hasnormal=false,hastexcood=false;
	Face fa;
	c = fs.get();
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
				m_faces.push_back(fa);
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
	fs.close();
	m_nVertexCount = (GLsizei)(3*m_faces.size());
	m_pVertices = new GLfloat[3*m_nVertexCount];
	m_pNormals = new GLfloat[3*m_nVertexCount];
	for (int i=0;i<m_faces.size();i++){
		m_pVertices[9*i]=m_positions[m_faces[i].pi[0]-1].x;
		m_pVertices[9*i+1]=m_positions[m_faces[i].pi[0]-1].y;
		m_pVertices[9*i+2]=m_positions[m_faces[i].pi[0]-1].z;
		m_pVertices[9*i+3]=m_positions[m_faces[i].pi[1]-1].x;
		m_pVertices[9*i+4]=m_positions[m_faces[i].pi[1]-1].y;
		m_pVertices[9*i+5]=m_positions[m_faces[i].pi[1]-1].z;
		m_pVertices[9*i+6]=m_positions[m_faces[i].pi[2]-1].x;
		m_pVertices[9*i+7]=m_positions[m_faces[i].pi[2]-1].y;
		m_pVertices[9*i+8]=m_positions[m_faces[i].pi[2]-1].z;

		m_pNormals[9*i]=m_normals[m_faces[i].ni[0]-1].x;
		m_pNormals[9*i+1]=m_normals[m_faces[i].ni[0]-1].y;
		m_pNormals[9*i+2]=m_normals[m_faces[i].ni[0]-1].z;
		m_pNormals[9*i+3]=m_normals[m_faces[i].ni[1]-1].x;
		m_pNormals[9*i+4]=m_normals[m_faces[i].ni[1]-1].y;
		m_pNormals[9*i+5]=m_normals[m_faces[i].ni[1]-1].z;
		m_pNormals[9*i+6]=m_normals[m_faces[i].ni[2]-1].x;
		m_pNormals[9*i+7]=m_normals[m_faces[i].ni[2]-1].y;
		m_pNormals[9*i+8]=m_normals[m_faces[i].ni[2]-1].z;
	}
	//glGenBuffersARB(1, &m_nVBOVertices);
	//glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_nVBOVertices);
	//glBufferDataARB(GL_ARRAY_BUFFER_ARB,3*m_nVertexCount,m_pVertices,GL_STATIC_DRAW_ARB);
	m_positions.clear();
	m_normals.clear();
	m_texcoords.clear();
	m_faces.clear();
	return true;
}

void CMesh::draw(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -20);
    glRotatef(rx, 1, 0, 0);
    glRotatef(ry, 0, 1, 0);
    glRotatef(rz, 0, 0, 1);
    
	glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    GLfloat globel_ambient[] = { 0.1 , 0.1 , 0.1 , 1.0 };
    
    //打开全局光照
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT , globel_ambient);
	//glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices);
	glVertexPointer(3, GL_FLOAT, 0, m_pVertices);
	glNormalPointer(GL_FLOAT, 0, m_pNormals);
	glDrawArrays( GL_TRIANGLES, 0, m_nVertexCount);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void CMesh::setRotation(GLfloat rx, GLfloat ry, GLfloat rz){
    this->rx = rx;
    this->ry = ry;
    this->rz = rz;
}

void CMesh::log(){
	for(int i=0;i<m_nVertexCount;i++){
		printf("p%d:%f,%f,%f\n",i,m_pVertices[3*i],m_pVertices[3*i+1],m_pVertices[3*i+2]);
	}
	for(int i=0;i<m_nVertexCount;i++){
		printf("n%d:%f,%f,%f\n",i,m_pNormals[3*i],m_pNormals[3*i+1],m_pNormals[3*i+2]);
	}
}
CMesh::CMesh():m_nVBOVertices(0),m_nVertexCount(0),m_pVertices(nullptr),m_pNormals(nullptr),rx(0),ry(0),rz(0){
#ifdef WIN32
	glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
	glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
	glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
#endif
}
CMesh::~CMesh(){
	if (m_pVertices){
		delete[] m_pVertices;
	}
	if (m_pNormals){
		delete[] m_pNormals;
	}
	if (m_nVBOVertices){
		glDeleteBuffersARB(1,&m_nVBOVertices);
	}
}