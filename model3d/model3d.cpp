#include "model3d.h"
#include<fstream>
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
				fs>>c;
				if (c=='n'){
					hasnormal=true;
					fs>>n.x>>n.y>>n.z;
					m_normals.push_back(n);
				}else if(c=='t'){
					hastexcood=true;
					fs>>t.s>>t.t>>t.r;
					m_texcoords.push_back(t);
				}else{
					fs>>v.x>>v.y>>v.z;
					m_positions.push_back(v);
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
	m_nVertexCount= 3*m_faces.size();
	m_pVertices=new GLfloat[3*m_nVertexCount];
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
	glEnableClientState(GL_VERTEX_ARRAY); //开始使用vbo
	//glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices); //选择当前使用的vbo
	glVertexPointer(3, GL_FLOAT, 0, m_pVertices); //指定vbo顶点格式
	glDrawArrays( GL_TRIANGLES, 0, m_nVertexCount/3); //画吧
	glDisableClientState(GL_VERTEX_ARRAY); //停止使用vbo
}

void CMesh::log(){
	for(int i=0;i<m_nVertexCount;i++){
		printf("p:%f,%f,%f\n",m_pVertices[3*i],m_pVertices[3*i+1],m_pVertices[3*i+2]);
	}
}
CMesh::CMesh():m_nVBOVertices(0),m_nVertexCount(0),m_pVertices(nullptr){
	glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
	glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
	glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
}
CMesh::~CMesh(){
	if (m_pVertices){
		delete[] m_pVertices;
	}
	if (m_nVBOVertices){
		glDeleteBuffersARB(1,&m_nVBOVertices);
	}
}