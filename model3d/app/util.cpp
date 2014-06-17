//
//  util.cpp
//  model3d
//
//  Created by 魏裕群 on 14-6-17.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "util.h"

static int h(char c){
    if (c>='0'&&c<='9') {
        return c-'0';
    }
    if (c>='a'&&c<='f') {
        return c-'a'+10;
    }
    if (c>='A'&&c<='F') {
        return c-'A'+10;
    }
    return -1;
}

bool readColor(const char*color, float cf[3]){
    if(color[0]=='#'){
        int tmp=0,tmp1=0;
        if (color[4]==0) {
            for (int i=0; i<3; i++) {
                tmp=h(color[1+i]);
                if (tmp==-1) {
                    return false;
                }
                cf[i]=1.0*tmp/15;
            }
            return true;
        }
        if (color[7]==0) {
            for (int i=0; i<3; i++) {
                tmp=h(color[1+i*2]);
                if (tmp==-1) {
                    return false;
                }
                tmp1=h(color[2+i*2]);
                if (tmp1==-1) {
                    return false;
                }
                cf[i]=(16.0*tmp+1.0*tmp1)/255;
            }
            return true;
        }
    }
    return false;
}
