/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_gl3dmodel.cpp
 **   Class:        CGLModel
 **   Description:  Import 3d models
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_gl3dmodel.h"
/////////////////////////////////// CGLMaterial class
CGLMaterial::CGLMaterial() { Initialize(); }
CGLMaterial::~CGLMaterial() {}
void CGLMaterial::Initialize() {
    iMaterialIndex = 0;
    name.clear();
}
/////////////////////////////////// CGLMesh class
CGLMesh::CGLMesh() { Initialize(); }
CGLMesh::~CGLMesh() {
    if (vertexArray) delete[] vertexArray;
    if (normalArray) delete[] normalArray;
    if (uvArray) delete[] uvArray;
}
void CGLMesh::Initialize() {
    iMeshIndex   = 0;
    numTriangles = 0;
    numUvCoords  = 0;
    vertexArray  = 0;
    normalArray  = 0;
    uvArray      = 0;
    name.clear();
}
/////////////////////////////////// CGLModel class
CGLModel::CGLModel() {
    Initialize();
    bMadeLog = 1;
    pLog     = new CLog("CGLModel.log");
    pLog->Restart();
    pLog->_DebugAdd("CGLModel::CGLModel()");
}
CGLModel::CGLModel(C_GFX* pinGFX, CLog* pInLog) {
    Initialize();
    pGFX     = pinGFX;
    bMadeLog = 0;
    pLog     = pInLog;
    pLog->_DebugAdd("CGLModel::CGLModel(CLog *pInLog)");
}

CGLModel::~CGLModel() {
    dlcsm_delete_vector(CGLMesh*, meshes);
    dlcsm_delete_vector(CGLMaterial*, materials);
    if (bMadeLog) dlcsm_delete(pLog);
}
void CGLModel::Initialize() {
    pLog         = 0;
    pGAF         = 0;
    bMadeLog     = 0;
    numMeshes    = 0;
    numMaterials = 0;
    name.clear();
}
bool CGLModel::Load(string filename) {
    /*
    vector<string> strx;
    pLog->_DebugAdd("============================================================");
    CGLMaterial* pMat;
    CGLMesh*     pMesh;
    name=filename;
    if(name.empty()) name = "EMPTY";
    /*
    // const aiScene *scene = aiImportFile(filename.c_str(),aiProcessPreset_TargetRealtime_MaxQuality); // importer.ReadFile(filename,aiProcessPreset_TargetRealtime_Fast);
    if(!scene) {
        pLog->_Add("Model load error %s",filename.c_str());
        return false;
    }
    numMaterials=scene->mNumMaterials;
    numMeshes=scene->mNumMeshes;
    pLog->_DebugAdd("MODEL[%s] MESHES[%d] MATERIALS[%d]",name.c_str(),numMeshes,numMaterials);
    for(int i=0;i<numMaterials;i++) {
        pMat=new CGLMaterial;
        pMat->iMaterialIndex=i;
        materials.push_back(pMat);
        aiMaterial *material = scene->mMaterials[i];
        aiString str;

        material->Get(AI_MATKEY_NAME,str);
        pMat->name.assign(str.data);
        material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE,0),str);
        pMat->DiffuseTexture.assign(str.data);
        strx=dlcs_explode("\\",pMat->DiffuseTexture);
        pMat->DiffuseTexture=strx.back();
        pLog->_Add("  Diffuse Material = %s",pMat->DiffuseTexture.c_str());
//      pMat->DiffuseTexture.erase(pMat->DiffuseTexture.find("..\\"),3);
//        material->Get( AI_MATKEY_TEXTURE( aiTextureType_NORMALS,	0 ), pMat->NormalTexture);
//        material->Get( AI_MATKEY_TEXTURE( aiTextureType_HEIGHT,	    0 ), pMat->HeightTexture);
//        material->Get( AI_MATKEY_TEXTURE( aiTextureType_OPACITY,	0 ), pMat->OpacityTexture);
//        material->Get( AI_MATKEY_TEXTURE( aiTextureType_SHININESS,	0 ), pMat->ShininessTexture);
//        material->Get( AI_MATKEY_TEXTURE( aiTextureType_SPECULAR,	0 ), pMat->SpecularTexture);
    }
    pLog->_DebugAdd("%d Materials loaded",materials.size());

    for(int i=0;i<numMeshes;i++) {
        pMesh=new CGLMesh;
        pMesh->iMeshIndex       = i;
        meshes.push_back(pMesh);
        aiMesh *inMesh = scene->mMeshes[i];

        pMesh->iMaterialIndex   = inMesh->mMaterialIndex;
        pMesh->numTriangles     = inMesh->mNumFaces*3;
        pMesh->numUvCoords      = inMesh->GetNumUVChannels();
        pLog->_DebugAdd("Meshindex[%d] MaterialIndex[%d] NumTri[%d] NumUV[%d]",
                    pMesh->iMeshIndex, pMesh->iMaterialIndex,  pMesh->numTriangles, pMesh->numUvCoords);
        pMesh->uvArray      = new float[inMesh->mNumFaces*3*2];
        pMesh->vertexArray  = new float[inMesh->mNumFaces*3*3];
        pMesh->normalArray  = new float[inMesh->mNumFaces*3*3];
        for(unsigned int k=0;k<inMesh->mNumFaces;k++) {
            const aiFace& face = inMesh->mFaces[k];
            for(int j=0;j<3;j++) {
                aiVector3D uv = inMesh->mTextureCoords[0][face.mIndices[j]];
                memcpy(pMesh->uvArray,&uv,sizeof(float)*2);
                pMesh->uvArray+=2;
                aiVector3D normal = inMesh->mNormals[face.mIndices[j]];
                memcpy(pMesh->normalArray,&normal,sizeof(float)*3);
                pMesh->normalArray+=3;
                aiVector3D pos = inMesh->mVertices[face.mIndices[j]];
                memcpy(pMesh->vertexArray,&pos,sizeof(float)*3);
                pMesh->vertexArray+=3;
            }
        }
        pMesh->uvArray      -= inMesh->mNumFaces*3*2;
        pMesh->normalArray  -= inMesh->mNumFaces*3*3;
        pMesh->vertexArray  -= inMesh->mNumFaces*3*3;
    }
    pLog->_DebugAdd("Model loaded: %s (%d)",name.c_str(),this);
     * */
    return true;
}
CGLMesh* CGLModel::GetMesh(int x) {
    for (vector<CGLMesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
        if ((*it)->iMeshIndex == x) return &(**it);
    }
    return 0;
}
CGLMaterial* CGLModel::GetMaterial(int x) {
    for (vector<CGLMaterial*>::iterator it = materials.begin(); it != materials.end(); ++it) {
        if ((*it)->iMaterialIndex == x) return &(**it);
    }
    return 0;
}
CGLMaterial* CGLModel::GetMaterial(string name) {
    pLog->_Add("%s", name.c_str());
    for (vector<CGLMaterial*>::iterator it = materials.begin(); it != materials.end(); ++it) {
        if ((*it)->DiffuseTexture == name) return &(**it);
    }
    return 0;
}

bool CGLModel::Draw(CGLTexture* pTexture) {
    if (!pGFX) return 0;
    glEnable(GL_TEXTURE_2D);
    CGLTexture*  pTex  = 0;
    CGLMesh*     pMesh = 0;
    CGLMaterial* pMat  = 0;
    for (vector<CGLMesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
        pMesh = (*it);
        if (pMesh) {
            if (!pTexture) {
                pMat = GetMaterial(pMesh->iMaterialIndex);
                pTex = pGFX->GetTexture("base/" + pMat->DiffuseTexture);
                if (!pTex) pTex = pGFX->pDefaultTexture;
            } else {
                if (pTexture->glBmap)
                    pTex = pTexture;
                else
                    pTex = pGFX->pDefaultTexture;
            }
            if (pTex)
                if (pTex->glBmap) glBindTexture(GL_TEXTURE_2D, pTex->glBmap);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            // glClientActiveTexture(GL_TEXTURE0_ARB);
            glVertexPointer(3, GL_FLOAT, 0, pMesh->vertexArray);
            glNormalPointer(GL_FLOAT, 0, pMesh->normalArray);
            glTexCoordPointer(2, GL_FLOAT, 0, pMesh->uvArray);
            glDrawArrays(GL_TRIANGLES, 0, pMesh->numTriangles);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
    return true;
}
