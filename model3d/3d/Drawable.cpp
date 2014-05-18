//
//  Drawable.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-18.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "Drawable.h"
#include "Scene.h"
#include "Stage.h"
#include "NormalMapShader.h"
#include "NormalColorShader.h"
#include "MapShader.h"
#include "VaryShader.h"

void Drawable::draw(){
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
		const Matrix4& projectionMatrix = Stage::sharedInstance()->currentCamera()->get();

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
		modelViewMatrix.transpose();

		unsigned long nSize = m_iMaterialArray.size();
		glBindVertexArray(m_nVertexArraysID);
		for (unsigned long i=0; i<nSize; i++) {
			MtlInfo &minf = m_iMaterialArray[i];
			Mtl*pmtl=m_iMaterial.find(minf.name);

			if (m_bHasNormal){
				VaryShader *pinms = VaryShader::sharedInstance();
				pinms->use();
				pinms->setModelViewProjectionMatrix(_modelViewProjectionMatrix.get());
				pinms->setModelViewMatrix(modelViewMatrix.get());
				pinms->setNormalMatrix(_normalMatrix.get());
				int lc = Stage::sharedInstance()->runningScene()->lightCount();
				pinms->lightReset();
				while(lc--){
					pinms->addLight(Stage::sharedInstance()->runningScene()->getLight(lc));
				}
				pinms->setMaterial(pmtl);
			}else{
				if (pmtl->map_kd){
					MapShader *pims = MapShader::sharedInstance();
					pims->use();
					pims->setModelViewProjectionMatrix(_modelViewProjectionMatrix.get());
					Texture *pctex=pmtl->map_kd;
					pims->setTexture(pctex);
				}
			}
			glDrawArrays( GL_TRIANGLES,minf.first,minf.size);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
}

void Drawable::clear(){
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

Drawable::Drawable():m_nVerticesID(0)
	,m_nNormalsID(0)
	,m_nTexcoordsID(0)
	,m_nVertexCount(0)
	,m_pVertices(nullptr)
	,m_pNormals(nullptr)
	,m_pTexCoords(nullptr)
	,m_piShader(nullptr)
	,rax(0),ray(0),raz(1),radian(0),x(0),y(0),z(0),scalex(1),scaley(1),scalez(1)
	,m_bUseShader(false),m_bHasNormal(false),m_bHasTexCoord(false){
}

Drawable::~Drawable(){
}
