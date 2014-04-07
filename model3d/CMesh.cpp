#include "CMesh.h"
#include<fstream>
#include "Timer.h"
#ifdef __APPLE__
#define GEN_VERTEX_ARRAYS glGenVertexArraysAPPLE
#define BIND_VERTEX_ARRAY glBindVertexArrayAPPLE
#else
#define GEN_VERTEX_ARRAYS glGenVertexArrays
#define BIND_VERTEX_ARRAY glBindVertexArray
#endif


using namespace std;
bool CMesh::load(const char* filename){
	char *p = const_cast<char*>(filename);
	int len = 0;
	for (;*p;p++);
	for (;*p!='.'&&p!=filename;p--,len++);
	if (len==4){
		if ((p[1]=='o'||p[1]=='O')&&(p[2]=='b'||p[2]=='B')&&(p[3]=='j'||p[3]=='J')){
			return loadObj(filename);
		}
	}
	return false;
}

bool CMesh::loadObj_bak(const char* filename){
	ifstream fs(filename);
	if (fs.good()){
		printf("open file[%s] success\n",filename);
	}else{
		printf("open file[%s] failed\n",filename);
		return false;
	}
	Timer timer;
	timer.start();
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
	bool fsection = false;
	while (c!=EOF){
		if (!fsection&&c=='f'){
			fsection = true;
		}else if (fsection&&c!='f'){
			fsection = false;
			//break;
		}
		switch (c){	
            case 'v':{
                int ntmp=fs.peek();
                if (ntmp==32) {
                    fs>>v.x>>v.y>>v.z;
                    //printf("v %f,%f,%f\n",v.x,v.y,v.z);
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
					//printf("ignore:%c%s\n",c,tmp);
				}
			}break;
            case '#':{
				fs.getline(tmp, 128,'\n');
				//printf("#%s\n",tmp);
			}break;
            case 'f':{
				fs.getline(tmp,128,'\n');
				memset(&fa,0,sizeof(Face));
				int nowv=0,nowt=0;
				for (char*p=tmp;*p;p++){
					if(*p==32){
						if (nowt!=0){
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
					fs.getline(tmp,128,'\n');
					for (char*pch=tmp;*pch;pch++){if(*pch=='\r'&&*(pch+1)==0)*pch=0;}
                    if (mtlinf.name[0]) {
                        m_iMaterialArray.push_back(mtlinf);
                        mtlinf.first+=mtlinf.size;
                        mtlinf.size=0;
                    }
                    strcpy(mtlinf.name, tmp);
                    //printf("usemtl %s\n",mtlinf.name);
                }else{
                    printf("ignore:u%s ",tmp);
                    fs.getline(tmp, 128,'\n');
                    //printf("%s",tmp);
                }
            }break;
            case 'm':{
                fs.getline(tmp, 128,' ');
                if (tmp[0]=='t'&&tmp[1]=='l'&&tmp[2]=='l'&&tmp[3]=='i'&&tmp[4]=='b'&&tmp[5]==0) {
					fs.getline(tmp,128,'\n');
					for (char*pch=tmp;*pch;pch++){if(*pch=='\r'&&*(pch+1)==0)*pch=0;}
                    //printf("mtllib %s\n",tmp);
                    getFilePath(filename, tmp);
                    m_iMaterial.load(tmp);
                }else{
                    //printf("ignore:m%s ",tmp);
                    fs.getline(tmp, 128,'\n');
                    printf("%s",tmp);
                }
            }break;
		default:
			fs.getline(tmp, 128,'\n');
			//printf("ignore:%c%s\n",c,tmp);
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
	timer.stop();
	printf("read file time consuming:%lfms\n",timer.getElapsedTimeInMilliSec());
	timer.start();
	m_nVertexCount = (GLsizei)(3*m_faces.size());
	m_pVertices = new GLfloat[3*m_nVertexCount];
	if(hasnormal){
		m_pNormals = new GLfloat[3*m_nVertexCount];
	}
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

		if (hasnormal){
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
		
	}
	GEN_VERTEX_ARRAYS(1, &m_vertexArray);
	BIND_VERTEX_ARRAY(m_vertexArray);

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
	BIND_VERTEX_ARRAY(0);
    
    delete []m_pVertices;
    m_pVertices=nullptr;
    delete []m_pNormals;
    m_pNormals=nullptr;
    
	m_positions.clear();
	m_normals.clear();
	m_texcoords.clear();
	m_faces.clear();
	timer.stop();
	printf("new vertexs time consuming:%lfms\n",timer.getElapsedTimeInMilliSec());
	return true;
}
bool CMesh::loadObj(const char* filename){
	ifstream fs(filename);
	if (fs.good()){
		printf("open file[%s] success\n",filename);
	}else{
		printf("open file[%s] failed\n",filename);
		return false;
	}
	Timer timer;
	timer.start();
	std::vector<Vector3>        m_positions;
	std::vector<Vector3>        m_normals;
	std::vector<Vector3>       m_texcoords;
	std::vector<Face>           m_faces;

	char tmp[2048];
	char *p,*end;
	Vector3 v;
	bool hasnormal=false,hastexcood=false;
	Face fa;
	MtlInfo mtlinf;
	memset(&mtlinf, 0, sizeof(MtlInfo));
	bool fsection = false;
	fs.read(tmp,2048-128);
	p = tmp;
	end = p+fs.gcount();
	fs.getline(end,128);
	for (;*end;end++);
	GLfloat vec3[3];
	unsigned long long linenum=0;
	while (true){
		if(*p=='v'){
			p++;
			int type = 0;
			if (*p==' '){
				type = 1;
			}else if(*p=='n'){
				p++;
				if (*p==' '){
					hasnormal = true;
					type = 2;
				}
			}else if(*p=='t'){
				p++;
				if (*p==' '){
					hastexcood = true;
					type = 3;
				}
			}
			if (type!=0){
				int flag = 0,channel = 0;
				GLfloat ftmp=0,dot=0;
				do{
					if (*p=='-'){
						if (flag==0) {
							flag=-1;
							ftmp=0;
						}
					}else if (*p>='0' && *p<='9') {
						if (flag==0) {
							flag=1;
							ftmp=*p-'0';
						}else if(flag==1||flag==-1){
							ftmp=ftmp*10+(*p-'0');
						}else if(flag==2||flag==-2){
							ftmp=ftmp+dot*(*p-'0');
							dot/=10;
						}
					}else if(*p=='.'){
						if (flag==1) {
							flag=2;
							dot=0.1f;
						}else if (flag==-1) {
							flag=-2;
							dot=0.1f;
						}
					}else if(*p==' '||*p==0||*p=='\r'||*p=='\n'){
						if (flag!=0) {
							vec3[channel]=flag>0?ftmp:-ftmp;
							channel++;
						}
						flag = 0;
					}
					if (*p=='\r'||*p=='\n'||p==end||*p==0){
						break;
					}else{
						p++;
					}
				}while (true);
				v.x=vec3[0];
				v.y=vec3[1];
				v.z=vec3[2];
				if (type==1){
					m_positions.push_back(v);
				}else if (type==2){
					m_normals.push_back(v);
				}else if(type==3){
					m_texcoords.push_back(v);
				}
			}
		}else if (*p=='f'){
			p++;
			if (*p==' '){
				//like:f 1/1/1 2/2/1 3/3/1 f 1/1 2/2 3/3 f 1//1 2//1 3//1 f 1 2 3
				memset(&fa,0,sizeof(Face));
				int nowv=0,nowt=0;
				for (p++;*p!='\n'&&*p!='\r'&&p!=end;p++){
					if(*p==32){
						if (nowt!=0){
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
			}
		}else if(*p=='#'){
			//like:# 3ds Max Wavefront OBJ Exporter v0.97b - (c)2007 guruware
			for (p++;*p!='\n'&&*p!='\r'&&p!=end;p++);
		}else if (*p=='m'){
			if (p[1]=='t'&&p[2]=='l'&&p[3]=='l'&&p[4]=='i'&&p[5]=='b'&&p[6]==' ') {
				p=p+7;
				char name[64];
				int ind = 0;
				for (;*p!='\n'&&*p!='\r'&&p!=end;p++,ind++){
					name[ind]=*p;
				}
				name[ind]=0;
				getFilePath(filename, name);
				m_iMaterial.load(name);
			}
		}else if (*p=='u'){
			if (p[1]=='s'&&p[2]=='e'&&p[3]=='m'&&p[4]=='t'&&p[5]=='l'&&p[6]==' ') {
				p=p+7;
				char name[64];
				int ind = 0;
				for (;*p!='\n'&&*p!='\r'&&p!=end;p++,ind++){
					name[ind]=*p;
				}
				name[ind]=0;
				if (mtlinf.name[0]) {
					m_iMaterialArray.push_back(mtlinf);
					mtlinf.first+=mtlinf.size;
					mtlinf.size=0;
				}
				strcpy(mtlinf.name, name);
			}
		}
		if (p==end){
			linenum++;
			if (linenum==3429){
				linenum++;
			}
			fs.read(tmp,2048-128);
			p = tmp;
			end = p+fs.gcount();
			if (p==end){
				break;
			}
			fs.getline(end,128,'\n');
			for (;*end;end++);
		}else{
			p++;
		}
	}
	if (mtlinf.name[0]) {
		m_iMaterialArray.push_back(mtlinf);
	}
	fs.close();
	timer.stop();
	printf("read file time consuming:%lfms\n",timer.getElapsedTimeInMilliSec());
	timer.start();
	m_nVertexCount = (GLsizei)(3*m_faces.size());
	m_pVertices = new GLfloat[3*m_nVertexCount];
	if(hasnormal){
		m_pNormals = new GLfloat[3*m_nVertexCount];
	}
	unsigned long fnum = m_faces.size();
	GLfloat *pv=m_pVertices,*qv=m_pNormals;
	for (unsigned long i=0;i<fnum;i++){
		Face &fctmp=(Face&)m_faces[i];
		(*pv++)=m_positions[fctmp.pi[0]-1].x;
		(*pv++)=m_positions[fctmp.pi[0]-1].y;
		(*pv++)=m_positions[fctmp.pi[0]-1].z;
		(*pv++)=m_positions[fctmp.pi[1]-1].x;
		(*pv++)=m_positions[fctmp.pi[1]-1].y;
		(*pv++)=m_positions[fctmp.pi[1]-1].z;
		(*pv++)=m_positions[fctmp.pi[2]-1].x;
		(*pv++)=m_positions[fctmp.pi[2]-1].y;
		(*pv++)=m_positions[fctmp.pi[2]-1].z;

		if (hasnormal){
			(*qv++)=m_normals[fctmp.ni[0]-1].x;
			(*qv++)=m_normals[fctmp.ni[0]-1].y;
			(*qv++)=m_normals[fctmp.ni[0]-1].z;
			(*qv++)=m_normals[fctmp.ni[1]-1].x;
			(*qv++)=m_normals[fctmp.ni[1]-1].y;
			(*qv++)=m_normals[fctmp.ni[1]-1].z;
			(*qv++)=m_normals[fctmp.ni[2]-1].x;
			(*qv++)=m_normals[fctmp.ni[2]-1].y;
			(*qv++)=m_normals[fctmp.ni[2]-1].z;
		}

	}
	GEN_VERTEX_ARRAYS(1, &m_vertexArray);
	BIND_VERTEX_ARRAY(m_vertexArray);

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
	BIND_VERTEX_ARRAY(0);

	delete []m_pVertices;
	m_pVertices=nullptr;
	delete []m_pNormals;
	m_pNormals=nullptr;

	m_positions.clear();
	m_normals.clear();
	m_texcoords.clear();
	m_faces.clear();
	timer.stop();
	printf("new vertexs time consuming:%lfms\n",timer.getElapsedTimeInMilliSec());
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
        
        GLfloat globel_ambient[] = { 0.1f , 0.1f , 0.1f , 1.0f };
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
		BIND_VERTEX_ARRAY(m_vertexArray);
        for (int i=0; i<m_iMaterialArray.size(); i++) {
            Mtl*pmtl=m_iMaterial.find(m_iMaterialArray[i].name);
            normalShader->setDiffuseColor(pmtl->kd.d);
            glDrawArrays( GL_TRIANGLES, m_iMaterialArray[i].first, m_iMaterialArray[i].size);
        }
		BIND_VERTEX_ARRAY(0);
        glUseProgram(0);
    }
}

CMesh::CMesh():m_nVBOVertices(0),m_nVertexCount(0),m_pVertices(nullptr),m_pNormals(nullptr),normalShader(nullptr),rax(0),ray(0),raz(1),x(0),y(0),z(0),scalex(1),scaley(1),scalez(1){
}
CMesh::CMesh(const char* filename){
	CMesh();
    load(filename);
}

CMesh::~CMesh(){
	if (m_pVertices){
		delete[] m_pVertices;
		m_pVertices = nullptr;
	}
	if (m_pNormals){
		delete[] m_pNormals;
	}
	if (m_vertexArray) {
		//glDeleteVertexArrays(1,&m_vertexArray);
	}
	if (m_nVBOVertices){
		//glDeleteBuffers(1,&m_nVBOVertices);
	}
    if (m_nVBONormals){
		//glDeleteBuffers(1,&m_nVBONormals);
	}
	m_iMaterialArray.clear();
}

void getFilePath(const char *filename,char *childfilename){
    if (childfilename[1]!=':') {
        int nlen=0;
        char*p=const_cast<char*>(filename);
        for (; *p; p++,nlen++);
        for (; nlen>=0&&*p!='/'; p--,nlen--);
        if (nlen>=0) {
            memmove(&childfilename[nlen+1], childfilename, strlen(childfilename)+1);
            memcpy(childfilename, filename, nlen+1);
        }
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