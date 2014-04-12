//
//  Material.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-4.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "Timer.h"
#include<fstream>
#include<string.h>

#define BUFF_LEN (1024)
#define LINE_LEN (128)

using namespace std;

Material::Material(){
}

Material::Material(const char *filename){
	//loadMtl(filename);
}

Material::~Material(){
	clear();
}

bool Material::loadMtl(const char*filename){
	Timer timer;
	timer.start();
	ifstream fs(filename);
	if (fs.good()){
		printf("open file[%s] success\n",filename);
	}else{
		printf("open file[%s] failed\n",filename);
		return false;
	}
	char tmp[BUFF_LEN];
	char *p,*end;
	Mtl mtl;
	memset(&mtl, 0, sizeof(Mtl));
	fs.read(tmp,BUFF_LEN-LINE_LEN);
	p = tmp;
	end = p+fs.gcount();
	fs.getline(end,LINE_LEN);
	for (;*end;end++);
	while (true){
		if (*p=='\t'){
			p++;
		}
		if (*p=='n'){
			if (p[1]=='e'&&p[2]=='w'&&p[3]=='m'&&p[4]=='t'&&p[5]=='l'&&p[6]==' ') {
				p=p+7;
				char name[64];
				int ind = 0;
				for (;*p!='\n'&&*p!='\r'&&p!=end;p++,ind++){
					name[ind]=*p;
				}
				name[ind]=0;
				if (mtl.name[0]) {
					m_imtls.push_back(mtl);
				}
				memset(&mtl,0,sizeof(Mtl));
				strcpy(mtl.name, name);
			}
		}else if(*p=='N'){
			p++;
			GLfloat dot = 0;
			if (*p=='s'){
				for (p++;*p!='\n'&&*p!='\r'&&p!=end;p++){
					if (*p>='0'&&*p<='9'){
						if (dot==0){
							mtl.ns=mtl.ns*10+(GLfloat)(*p-'0');
						}else{
							mtl.ns += dot*(GLfloat)(*p-'0');
							dot/=10;
						}
					}else if (*p=='.'){
						dot = 0.1f;
					}
				}
			}else if (*p=='i'){
				for (p++;*p!='\n'&&*p!='\r'&&p!=end;p++){
					if (*p>='0'&&*p<='9'){
						if (dot==0){
							mtl.ni=mtl.ns*10+(GLfloat)(*p-'0');
						}else{
							mtl.ni += dot*(GLfloat)(*p-'0');
							dot/=10;
						}
					}else if (*p=='.'){
						dot = 0.1f;
					}
				}
			}else{
				for (p++;*p!='\n'&&*p!='\r'&&p!=end;p++);
			}
		}else if(*p=='K'){
			p++;
			if (*p=='d'){
				p++;
				readColor(KD,mtl,p);
			}else{
				for (p++;*p!='\n'&&*p!='\r'&&p!=end;p++);
			}
		}else if(*p=='m'){
			if (p[1]=='a'&&p[2]=='p'&&p[3]=='_'&&p[4]=='K'&&p[5]=='d'&&p[6]==' ') {
				p=p+7;
				char name[64];
				int ind = 0;
				for (;*p!='\n'&&*p!='\r'&&p!=end;p++,ind++){
					name[ind]=*p;
				}
				name[ind]=0;
				getFilePath(filename,name);
				Texture *ctex = new Texture(name);
				mtl.map_kd = (void*)ctex;
			}
		}else if(*p=='#'){
			for (p++;*p!='\n'&&*p!='\r'&&p!=end;p++);
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
			//printf("%s",tmp);
		}else{
			p++;
		}
	}
	fs.close();
	timer.stop();
	printf("read mtl file time consuming:%lfms\n",timer.getElapsedTimeInMilliSec());
	if (mtl.name[0]) {
		m_imtls.push_back(mtl);
	}
	return true;
}

void Material::readColor(AttribMaterial am,Mtl &mtl,char* &p){
    int flag = 0,channel = 0;
    GLfloat tmp=0,dot=0;
    do{
        if (*p>='0' && *p<='9') {
            if (flag==0) {
                flag=1;
                tmp=(GLfloat)(*p-'0');
            }else if(flag==1){
                tmp=tmp*10+(*p-'0');
            }else if(flag==2){
                tmp=tmp+dot*(*p-'0');
                dot/=10;
            }
        }else if(*p=='.'){
            if (flag==1) {
                flag=2;
                dot=0.1f;
            }
        }else if(*p==' '||*p==0||*p=='\r'||*p=='\n'){
            if (flag==1||flag==2) {
                if (am==KD) {
                    mtl.kd.d[channel]=tmp;
                }
                channel++;
            }
            flag = 0;
			if (*p!=' '){
				break;
			}
        }
		p++;
    }while (true);
}

Mtl* Material::find(const char* name){
    size_t mc = m_imtls.size();
    for (unsigned int i=0; i<mc; i++) {
        if (strcmp(name,m_imtls[i].name)==0) {
            return &m_imtls[i];
        }
    }
    return nullptr;
}

void Material::clear(){
	std::vector<Mtl>::iterator itr=m_imtls.begin(),end=m_imtls.end();
	for (;itr!=end;itr++){
		Mtl &mtl = *itr;
		if (mtl.map_kd){
			delete (Texture*)mtl.map_kd;
		}
	}
	m_imtls.clear();
}
