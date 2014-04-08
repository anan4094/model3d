//
//  CMaterial.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-4.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "CMaterial.h"
#include "Timer.h"
#include<fstream>
#include<string.h>
using namespace std;

CMaterial::CMaterial(){

}

CMaterial::CMaterial(const char *filename){
	CMaterial();
	load(filename);
}

CMaterial::~CMaterial(){
	m_imtls.clear();
}

bool CMaterial::load(const char *filename){
    Timer timer;
    timer.start();
    ifstream fs(filename);
	if (fs.good()){
		printf("open file[%s] success\n",filename);
	}else{
		printf("open file[%s] failed\n",filename);
		return false;
	}
    char c = fs.get();
    char tmp[128];
    Mtl mtl;
    memset(&mtl, 0, sizeof(Mtl));
	while (c!=EOF){
		switch (c){
            case 'n':{
                fs.getline(tmp, 128,' ');
                if (tmp[0]=='e'&&tmp[1]=='w'&&tmp[2]=='m'&&tmp[3]=='t'&&tmp[4]=='l'&&tmp[5]==0) {
					fs.getline(tmp,128,'\n');
                    for (char*pch=tmp;*pch;pch++){if(*pch=='\r'&&*(pch+1)==0)*pch=0;}
                    if (mtl.name[0]) {
                        m_imtls.push_back(mtl);
                    }
                    memset(&mtl,0,sizeof(Mtl));
                    strcpy(mtl.name, tmp);
                    printf("new mtl:%s\n",mtl.name);
                }else{
                    printf("ignore:n%s",tmp);
                    fs.getline(tmp, 128,'\n');
                    printf(" %s",tmp);
                }
			}break;
            case 'N':{
                c = fs.get();
                if (c=='s') {
                    fs>>mtl.ns;
                }else if (c=='i'){
                    fs>>mtl.ni;
                }else{
                    fs.getline(tmp, 128,'\n');
                    printf("ignore:N%c%s",c,tmp);
                }
            }break;
            case 'K':{
                c = fs.get();
                if (c=='d') {
                    fs.getline(tmp, 128,'\n');
                    readColor(KD,mtl,tmp);
                }else{
                    fs.getline(tmp, 128,'\n');
                    printf("ignore:K%c%s",c,tmp);
                }
            }break;
            case '#':{
				fs.getline(tmp, 128,'\n');
				printf("#%s\n",tmp);
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
    timer.stop();
	printf("read mtl file time consuming:%lfms\n",timer.getElapsedTimeInMilliSec());
    if (mtl.name[0]) {
        m_imtls.push_back(mtl);
    }
    return true;
}

void CMaterial::readColor(AttribMaterial am,Mtl &mtl,char* bundle){
    int flag = 0,channel = 0;
    GLfloat tmp=0,dot=0;
    char *p=bundle;
    do{
        if (*p>='0' && *p<='9') {
            if (flag==0) {
                flag=1;
                tmp=*p-'0';
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
        }else if(*p==' '||*p==0){
            if (flag==1||flag==2) {
                if (am==KD) {
                    mtl.kd.d[channel]=tmp;
                }
                channel++;
            }
            flag = 0;
        }
    }while (*p++);
}

Mtl* CMaterial::find(const char* name){
    size_t mc = m_imtls.size();
    for (unsigned int i=0; i<mc; i++) {
        if (strcmp(name,m_imtls[i].name)==0) {
            return &m_imtls[i];
        }
    }
    return nullptr;
}
