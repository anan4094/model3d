#include "Scene.h"
#include "Stage.h"
#include "Mesh.h"
#include "Texture.h"
#include <fstream>
#include "Timer.h"
#include "NormalMapShader.h"
#include "NormalColorShader.h"
#include "MapShader.h"

#define BUFF_LEN (2048)
#define LINE_LEN (128)

void normalize(float v[3],float out[3]){
	GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if(d == 0.0){
		return;
	}
	out[0] = v[0]/d;
	out[1] = v[1]/d;
	out[2] = v[2]/d;
}

void normcrossprod(float v1[3],float v2[3],float out[3]){
	out[0] = v1[1]*v2[2] - v1[2]*v2[1];
	out[1] = v1[2]*v2[0] - v1[0]*v2[2];
	out[2] = v1[0]*v2[1] - v1[1]*v2[0]; 
	normalize(out,out);
}

void normcrossprodadd(float v1[3],float v2[3],float cache[3]){
    float tmp[3];
	tmp[0] = v1[1]*v2[2] - v1[2]*v2[1];
	tmp[1] = v1[2]*v2[0] - v1[0]*v2[2];
	tmp[2] = v1[0]*v2[1] - v1[1]*v2[0];
	normalize(tmp,tmp);
    cache[0]+=tmp[0];
    cache[1]+=tmp[1];
    cache[2]+=tmp[2];
}

using namespace std;
bool Mesh::load(const char* filename){
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

bool Mesh::loadObj(const char* filename){
	ifstream fs(filename);
	if (fs.good()){
		printf("open file[%s] success\n",filename);
	}else{
		printf("open file[%s] failed\n",filename);
		return false;
	}
	Timer timer;
	timer.start();
	std::vector<Vector3>        ipositions;
	std::vector<Vector3>        inormals;
	std::vector<Vector3>        itexcoords;
	std::vector<Face>           ifaces;

	char tmp[BUFF_LEN];
	char *p,*end;
	Vector3 v;
	m_bHasNormal=false;
	m_bHasTexCoord=false;
	Face fa;
	MtlInfo mtlinf;
	memset(&mtlinf, 0, sizeof(MtlInfo));
	fs.read(tmp,BUFF_LEN-LINE_LEN);
	p = tmp;
	end = p+fs.gcount();
	fs.getline(end,LINE_LEN);
	for (;*end;end++);
	GLfloat gf[3];
//	int repp=0,rept=0,repn=0;
	bool bMeetVertex = false;
	while (true){
		if(*p=='v'){
			p++;
			int type = 0;
			if (*p==' '){
				type = 1;
//				repp++;
			}else if(*p=='n'){
				p++;
				if (*p==' '){
//					repn++;
					m_bHasNormal = true;
					type = 2;
				}
			}else if(*p=='t'){
				p++;
				if (*p==' '){
//					rept++;
					m_bHasTexCoord = true;
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
							ftmp=(GLfloat)(*p-'0');
						}else if(flag==1||flag==-1){
							ftmp=ftmp*10+(*p-'0');
						}else if(flag==2||flag==-2){
							ftmp=ftmp+dot*(*p-'0');
							dot/=10;
						}
					}else if(*p=='.'){
						if (flag==1) {
							flag=2;
						}else if (flag==-1) {
							flag=-2;
						}
						dot=0.1f;
					}else if(*p==' '||*p==0||*p=='\r'||*p=='\n'){
						if (flag!=0) {
							gf[channel]=flag>0?ftmp:-ftmp;
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
				v.x=gf[0];
				v.y=gf[1];
				v.z=gf[2];
				if (type==1){
					ipositions.push_back(v);
					if (bMeetVertex){
						if(v.x>m_fMaxX)m_fMaxX=v.x;
						else if(v.x<m_fMinX)m_fMinX=v.x;
						if(v.y>m_fMaxY)m_fMaxY=v.y;
						else if(v.y<m_fMinY)m_fMinY=v.y;
						if(v.z>m_fMaxZ)m_fMaxZ=v.z;
						else if(v.z<m_fMinZ)m_fMinZ=v.z;
					}else{
						bMeetVertex = true;
						m_fMaxX = m_fMinX = v.x;
						m_fMaxY = m_fMinY = v.y;
						m_fMaxZ = m_fMinZ = v.z;
					}
				}else if (type==2){
					inormals.push_back(v);
				}else if(type==3){
					itexcoords.push_back(v);
				}
			}
		}else if (*p=='f'){
			p++;
			if (*p==' '){
// 				if (repp){
// 					printf("%d vertices,%d normal,%d texture coord\n",repp,repn,rept);
// 					repp=0;
// 					rept=0;
// 					repn=0;
// 				}
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
				ifaces.push_back(fa);
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
				m_iMaterial.setName(name);
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
			fs.read(tmp,BUFF_LEN-LINE_LEN);
			p = tmp;
			end = p+fs.gcount();
			if (p==end){
				break;
			}
			fs.getline(end,LINE_LEN,'\n');
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
	printf("read obj file time consuming:%lfms\n",timer.getElapsedTimeInMilliSec());

	m_iMaterial.load();

	timer.start();
	m_nVertexCount = (GLsizei)(3*ifaces.size());
	m_pVertices = new GLfloat[3*m_nVertexCount];
	
    GLfloat *normal4smooth=nullptr;
	if(m_bHasNormal||m_bForceGenerateNormal){
		m_pNormals = new GLfloat[3*m_nVertexCount];
        if(m_bSmoothSurface){
            normal4smooth = new GLfloat[3*ipositions.size()];
            memset(normal4smooth, 0, 3*ipositions.size());
        }
	}
	if (m_bHasTexCoord){
		m_pTexCoords = new GLfloat[2*m_nVertexCount];
	}
	unsigned long fnum = ifaces.size();
	GLfloat *pv=m_pVertices,*qv=m_pNormals,*rv=m_pTexCoords;
    GLfloat edge1[3],edge2[3];
	for (unsigned long i=0;i<fnum;i++){
		Face &fctmp=(Face&)ifaces[i];
        int ni1= fctmp.pi[0]-1,ni2= fctmp.pi[1]-1,ni3= fctmp.pi[2]-1;
        Vector3 &v1=ipositions[ni1];
        Vector3 &v2=ipositions[ni2];
        Vector3 &v3=ipositions[ni3];
		(*pv++)=v1.x;
		(*pv++)=v1.y;
		(*pv++)=v1.z;
		(*pv++)=v2.x;
		(*pv++)=v2.y;
		(*pv++)=v2.z;
		(*pv++)=v3.x;
		(*pv++)=v3.y;
		(*pv++)=v3.z;

		if (m_bHasNormal){
            Vector3 &n1=inormals[fctmp.ni[0]-1];
            Vector3 &n2=inormals[fctmp.ni[1]-1];
            Vector3 &n3=inormals[fctmp.ni[2]-1];
			(*qv++)=n1.x;
			(*qv++)=n1.y;
			(*qv++)=n1.z;
			(*qv++)=n2.x;
			(*qv++)=n2.y;
			(*qv++)=n2.z;
			(*qv++)=n3.x;
			(*qv++)=n3.y;
			(*qv++)=n3.z;
		}else if (m_bForceGenerateNormal){
            edge1[0]=v1.x-v2.x;
            edge1[1]=v1.y-v2.y;
            edge1[2]=v1.z-v2.z;
            edge2[0]=v2.x-v3.x;
            edge2[1]=v2.y-v3.y;
            edge2[2]=v2.z-v3.z;
            if (m_bSmoothSurface) {
                normcrossprodadd(edge1, edge2, normal4smooth+3*ni1);
                normcrossprodadd(edge1, edge2, normal4smooth+3*ni2);
                normcrossprodadd(edge1, edge2, normal4smooth+3*ni3);
            }else{
                normcrossprod(edge1,edge2,qv);
                *(qv+3)=*(qv+6)=*qv;
                *(qv+4)=*(qv+7)=*(qv+1);
                *(qv+5)=*(qv+8)=*(qv+2);
            }
            qv+=9;
        }

		if (m_bHasTexCoord){
			Vector3 &t1=itexcoords[fctmp.ti[0]-1];
			Vector3 &t2=itexcoords[fctmp.ti[1]-1];
			Vector3 &t3=itexcoords[fctmp.ti[2]-1];
			(*rv++)=t1.x;
			(*rv++)=t1.y;
			(*rv++)=t2.x;
			(*rv++)=t2.y;
			(*rv++)=t3.x;
			(*rv++)=t3.y;
		}

	}
    if (!m_bHasNormal && m_bForceGenerateNormal && m_bSmoothSurface) {
        qv=m_pNormals;
        for (unsigned long i=0;i<fnum;i++){
            Face &fctmp=(Face&)ifaces[i];
            int ni1= fctmp.pi[0]-1,ni2= fctmp.pi[1]-1,ni3= fctmp.pi[2]-1;
            *qv++=*(normal4smooth+3*ni1);
            *qv++=*(normal4smooth+3*ni1+1);
            *qv++=*(normal4smooth+3*ni1+2);
            *qv++=*(normal4smooth+3*ni2);
            *qv++=*(normal4smooth+3*ni2+1);
            *qv++=*(normal4smooth+3*ni2+2);
            *qv++=*(normal4smooth+3*ni3);
            *qv++=*(normal4smooth+3*ni3+1);
            *qv++=*(normal4smooth+3*ni3+2);
        }
    }
	if (!m_bHasNormal&&m_bForceGenerateNormal){
		m_bHasNormal=true;
	}
	if (normal4smooth){
		delete [] normal4smooth;
		normal4smooth=nullptr;
	}
    
    //test
    /*m_pVertices[0]=-4;
    m_pVertices[1]=-4;
    m_pVertices[2]=0;
    m_pVertices[3]=-4;
    m_pVertices[4]=4;
    m_pVertices[5]=0;
    m_pVertices[6]=4;
    m_pVertices[7]=4;
    m_pVertices[8]=0;
    
    m_pVertices[9]=-4;
    m_pVertices[10]=-4;
    m_pVertices[11]=0;
    m_pVertices[12]=4;
    m_pVertices[13]=4;
    m_pVertices[14]=0;
    m_pVertices[15]=4;
    m_pVertices[16]=-4;
    m_pVertices[17]=0;
    
    m_pTexCoords[0]=-1;
    m_pTexCoords[1]=-1;
    m_pTexCoords[2]=-1;
    m_pTexCoords[3]=1;
    m_pTexCoords[4]=1;
    m_pTexCoords[5]=1;
    m_pTexCoords[6]=-1;
    m_pTexCoords[7]=-1;
    m_pTexCoords[8]=1;
    m_pTexCoords[9]=1;
    m_pTexCoords[10]=1;
    m_pTexCoords[11]=-1;
    
    m_pNormals[0]=0;
    m_pNormals[1]=0;
    m_pNormals[2]=1;
    m_pNormals[3]=0;
    m_pNormals[4]=0;
    m_pNormals[5]=1;
    m_pNormals[6]=0;
    m_pNormals[7]=0;
    m_pNormals[8]=1;
    m_pNormals[9]=0;
    m_pNormals[10]=0;
    m_pNormals[11]=1;
    m_pNormals[12]=0;
    m_pNormals[13]=0;
    m_pNormals[14]=1;
    m_pNormals[15]=0;
    m_pNormals[16]=0;
    m_pNormals[17]=1;*/
    
	glGenVertexArrays(1, &m_nVertexArraysID);
	glBindVertexArray(m_nVertexArraysID);

	glGenBuffers(1, &m_nVerticesID);
	glBindBuffer(GL_ARRAY_BUFFER, m_nVerticesID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*m_nVertexCount, m_pVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(attrib_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrib_position);

	if (m_bHasNormal){
		glGenBuffers(1, &m_nNormalsID);
		glBindBuffer(GL_ARRAY_BUFFER, m_nNormalsID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*m_nVertexCount, m_pNormals, GL_STATIC_DRAW);

		glVertexAttribPointer(attrib_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(attrib_normal);
	}
	
	if (m_bHasTexCoord){
		glGenBuffers(1, &m_nTexcoordsID);
		glBindBuffer(GL_ARRAY_BUFFER, m_nTexcoordsID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2*m_nVertexCount, m_pTexCoords, GL_STATIC_DRAW);

		glVertexAttribPointer(attrib_texcoord0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(attrib_texcoord0);
	}

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	delete []m_pVertices;
	m_pVertices=nullptr;
	delete []m_pNormals;
	m_pNormals=nullptr;
	delete []m_pTexCoords;
	m_pTexCoords=nullptr;

	ipositions.clear();
	inormals.clear();
	itexcoords.clear();
	ifaces.clear();
	timer.stop();
	printf("generate vertexs time consuming:%lfms\n",timer.getElapsedTimeInMilliSec());
	return true;
}

int Mesh::initShader(){
    return 0;
}

void Mesh::draw(){
    if (!m_bUseShader) {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x,y,z);
        glScalef(scalex, scaley, scalez);
        glRotatef(radian, rax, ray, raz);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        GLfloat globel_ambient[] = { 0.1f , 0.1f , 0.1f , 1.0f };
        //打开全局光照
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT , globel_ambient);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_nVerticesID);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, m_nNormalsID);
        glNormalPointer(GL_FLOAT, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_nTexcoordsID);
		glTexCoordPointer(2,GL_FLOAT, 0, 0);
		unsigned long nSize = m_iMaterialArray.size();
		for (unsigned long i=0; i<nSize; i++) {
			Mtl*pmtl=m_iMaterial.find(m_iMaterialArray[i].name);

			if (pmtl->map_kd){
				glEnable(GL_TEXTURE_2D);
				Texture *pctex=(Texture*)(pmtl->map_kd);
				glBindTexture(GL_TEXTURE_2D,pctex->get());
			}else{
				glDisable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,0);
			}
			glDrawArrays( GL_TRIANGLES, m_iMaterialArray[i].first, m_iMaterialArray[i].size);
		}
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }else{
		const Matrix4& projectionMatrix = Stage::sharedInstance()->runningScene()->getProjection();

		// Compute the model view matrix for the object rendered with GL
		Matrix4 modelViewMatrix;
		modelViewMatrix.identity();
		modelViewMatrix.rotate(radian, rax, ray, raz);
		modelViewMatrix.scale(scalex, scaley, scalez);
		modelViewMatrix.translate(x, y, z);

		Matrix4 _normalMatrix = modelViewMatrix;
		_normalMatrix.invert();

		Matrix4 _modelViewProjectionMatrix =  projectionMatrix * modelViewMatrix;
		_modelViewProjectionMatrix.transpose();

		unsigned long nSize = m_iMaterialArray.size();
		glBindVertexArray(m_nVertexArraysID);
		for (unsigned long i=0; i<nSize; i++) {
			MtlInfo &minf = m_iMaterialArray[i];
			Mtl*pmtl=m_iMaterial.find(minf.name);

			if (m_bHasNormal){
				if (pmtl->map_kd){
					NormalMapShader *pinms = NormalMapShader::sharedInstance();
					pinms->use();
					pinms->setModelViewProjectionMatrix(_modelViewProjectionMatrix.get());
					pinms->setNormalMatrix(_normalMatrix.get());
					Texture *pctex=pmtl->map_kd;
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D,pctex->get());
					pinms->setTexture(0);
				}else{
					NormalColorShader *pincs = NormalColorShader::sharedInstance();
					pincs->use();
					pincs->setModelViewProjectionMatrix(_modelViewProjectionMatrix.get());
					pincs->setNormalMatrix(_normalMatrix.get());
					pincs->setDiffuseColor(pmtl->kd.d);
					//glBindTexture(GL_TEXTURE_2D,0);
				}
			}else{
				if (pmtl->map_kd){
					MapShader *pims = MapShader::sharedInstance();
					pims->use();
					pims->setModelViewProjectionMatrix(_modelViewProjectionMatrix.get());
					Texture *pctex=pmtl->map_kd;
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D,pctex->get());
					pims->setTexture(0);
				}
			}
			glDrawArrays( GL_TRIANGLES,minf.first,minf.size);
		}
		glBindVertexArray(0);
		glUseProgram(0);
    }
}

bool Mesh::screenSizeChange(int width, int height){
    return false;
}
bool Mesh::dispatcherTouchEvent(TouchEventType event, double x, double y){
    return false;
}

Mesh::Mesh():m_nVerticesID(0)
	,m_nNormalsID(0)
	,m_nTexcoordsID(0)
	,m_nVertexCount(0)
	,m_pVertices(nullptr)
	,m_pNormals(nullptr)
	,m_pTexCoords(nullptr)
	,m_piShader(nullptr)
	,rax(0),ray(0),raz(1),x(0),y(0),z(0),scalex(1),scaley(1),scalez(1)
	,m_bForceGenerateNormal(false),m_bSmoothSurface(false),m_bUseShader(false){
}
Mesh::Mesh(const char* filename):m_nVerticesID(0)
	,m_nNormalsID(0)
	,m_nTexcoordsID(0)
	,m_nVertexCount(0)
	,m_pVertices(nullptr)
	,m_pNormals(nullptr)
	,m_pTexCoords(nullptr)
	,m_piShader(nullptr)
	,rax(0),ray(0),raz(1),x(0),y(0),z(0),scalex(1),scaley(1),scalez(1)
	,m_bForceGenerateNormal(false),m_bSmoothSurface(false),m_bUseShader(false){
    load(filename);
}

Mesh::~Mesh(){
	clear();
}

void Mesh::clear(){
	if (m_pVertices){
		delete[] m_pVertices;
		m_pVertices = nullptr;
	}
	if (m_pNormals){
		delete[] m_pNormals;
		m_pNormals = nullptr;
	}
	if (m_pTexCoords){
		delete[] m_pTexCoords;
		m_pTexCoords = nullptr;
	}
	if (m_nVertexArraysID) {
		glDeleteVertexArrays(1,&m_nVertexArraysID);
	}
	if (m_nVerticesID){
		glDeleteBuffers(1,&m_nVerticesID);
	}
    if (m_nNormalsID){
		glDeleteBuffers(1,&m_nNormalsID);
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