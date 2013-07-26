/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson
****************************************************************/
#include "c_gl3dmodel.h"
CGLModel::CGLModel() {
    memset(name,0,1024);
    memset(skin,0,1024);
    pNext=0;
    pPrev=0;

    numOfObjects=0;
    numOfMaterials=0;
    numOfAnimations=0;
    pAnimations.clear();
    pMaterials.clear();
    pObject.clear();

    bMadeLog=1;
    pLog=new CLog("CGLModel.log");
    pLog->Restart();
    pLog->_DebugAdd("CGLModel::CGLModel()");
}
CGLModel::CGLModel(CLog *pInLog) {
    memset(name,0,1024);
    memset(skin,0,1024);
    pNext=0;
    pPrev=0;

    numOfObjects=0;
    numOfMaterials=0;
    numOfAnimations=0;
    pAnimations.clear();
    pMaterials.clear();
    pObject.clear();

    bMadeLog=0;
    pLog=pInLog;
    pLog->_DebugAdd("CGLModel::CGLModel(CLog *pInLog)");
}
CGLModel::~CGLModel() {
    pLog->_DebugAdd("CGLModel::~CGLModel()");
    if(bMadeLog) DEL(pLog);

}
bool CGLModel::Load(char* filename) {
    strcpy(name,va("%s/tris.md2",filename));
    strcpy(skin,va("%s/skin.png",filename));
    return true;
}

bool CGLModel::Load(char* filename,char* texture) {
    pLog->_DebugAdd("CGLModel::Load(char *filename, char *texture)");
    strcpy(skin,texture);
    return true;
}
bool CGLModel::RenderSceneDraw(void) {
    pLog->_DebugAdd("CGLModel::RenderSceneDraw(void)");
    return true;
}
bool CGLModel::Draw(void) {
    return true;
}
t3DObject::t3DObject() {
    numOfVerts=0;
    numOfFaces=0;
    numTexVertex=0;
    materialID=0;
    bHasTexture=0;
    memset(strName,0,255);
    pVerts=0;
    pNormals=0;
    pTexVerts=0;
    pFaces=0;
}
t3DObject::~t3DObject() {}

