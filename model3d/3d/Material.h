//
//  Material.h
//  model3d
//
//  Created by 魏裕群 on 14-4-4.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Material__
#define __model3d__Material__

#include "platform.h"

#include <vector>

typedef enum attrib_material{
    KD
}AttribMaterial;

typedef struct mtl{
    char name[32];
    GLfloat ns;
    GLfloat ni;
    GLfloat d;
    GLfloat tr;
    union{
        struct{
            GLfloat r,g,b;
        };
        GLfloat d[3];
    }tf;
    union{
        struct{
            GLfloat r,g,b;
        };
        GLfloat d[3];
    }ka;
    union{
        struct{
            GLfloat r,g,b;
        };
        GLfloat d[3];
    }kd;
    union{
        struct{
            GLfloat r,g,b;
        };
        GLfloat d[3];
    }ks;
    union{
        struct{
            GLfloat r,g,b;
        };
        GLfloat d[3];
    }ke;
    void* map_kd;
}Mtl;

class Material{
private:
    std::vector<Mtl> m_imtls;
    
    void readColor(AttribMaterial am,Mtl &mtl,char*&bundle);
public:
    bool loadMtl(const char*filename);
    mtl* find(const char*name);
	void clear();
	Material();
	Material(const char *filename);
	~Material();
};

#endif /* defined(__model3d__Material__) */
