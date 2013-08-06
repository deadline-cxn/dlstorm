/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson
****************************************************************/
#include "c_gl3dmodel.h"


CGLMaterial::CGLMaterial() { Initialize(); }
CGLMaterial::~CGLMaterial() {}
void CGLMaterial::Initialize() {
    iMaterialIndex=0;
    pNext=0;
    memset(DiffuseTexture,0,1024);
    memset(NormalTexture,0,1024);
    memset(HeightTexture,0,1024);
    memset(OpacityTexture,0,1024);
    memset(ShininessTexture,0,1024);
    memset(SpecularTexture,0,1024);
}

CGLMesh::CGLMesh() { Initialize(); }
CGLMesh::~CGLMesh() {
    if(vertexArray) delete [] vertexArray;
    if(normalArray) delete [] normalArray;
    if(uvArray)     delete [] uvArray;
}
void CGLMesh::Initialize(){
    pNext=0;
    iMeshIndex=0;
    numTriangles=0;
    numUvCoords=0;
    vertexArray=0;
    normalArray=0;
    uvArray=0;
}

CGLModel::CGLModel() {
    Initialize();
    bMadeLog=1;
    pLog=new CLog("CGLModel.log");
    pLog->Restart();
    pLog->_DebugAdd("CGLModel::CGLModel()");
}
CGLModel::CGLModel(C_GFX* pinGFX, CLog *pInLog) {
    Initialize();
    pGFX=pinGFX;
    bMadeLog=0;
    pLog=pInLog;
    pLog->_DebugAdd("CGLModel::CGLModel(CLog *pInLog)");
}
CGLModel::~CGLModel() {
    CGLMesh* pMesh;
    CGLMaterial* pMat;
    pMesh=pFirstMesh;
    while(pMesh) {
        pFirstMesh=pMesh;
        pMesh=pMesh->pNext;
        DEL(pFirstMesh);
    }
    pMat=pFirstMaterial;
    while(pMat) {
        pFirstMaterial=pMat;
        pMat=pMat->pNext;
        DEL(pFirstMaterial);
    }
    if(bMadeLog)
        DEL(pLog);
}
void CGLModel::Initialize() {
    memset(name,0,1024);
    pNext=0;
    pPrev=0;
    pLog=0;
    pGAF=0;
    bMadeLog=0;
    pFirstMesh=0;
    pFirstMaterial=0;
    numMeshes=0;
    numMaterials=0;

}
bool CGLModel::Load(char* filename) {

    pLog->_Add("============================================================");
    CGLMaterial* pMat;
    CGLMesh*     pMesh;
    strcpy(name,filename);
    const aiScene *scene = aiImportFile(filename,aiProcessPreset_TargetRealtime_MaxQuality); // importer.ReadFile(filename,aiProcessPreset_TargetRealtime_Fast);
    if(!scene) {
        pLog->_Add("Model load error %s",filename);
        pLog->_Add("============================================================");
        return false;
    }

    numMaterials=scene->mNumMaterials;
    numMeshes=scene->mNumMeshes;
    pLog->_Add("MODEL[%s] MESHES[%d] MATERIALS[%d]",name,numMeshes,numMaterials);

    for(int i=0;i<numMaterials;i++) {
        pMat=pFirstMaterial;
        if(pMat) {
            while(pMat->pNext) {
                pMat=pMat->pNext;
            }
            pMat->pNext=new CGLMaterial;
            pMat=pMat->pNext;

        } else {
            pFirstMaterial=new CGLMaterial;
            pMat=pFirstMaterial;
        }
        pMat->iMaterialIndex=i;
        aiMaterial *material = scene->mMaterials[i];
        aiString mname;
        material->Get(AI_MATKEY_NAME , mname);
        strcpy(pMat->name,mname.C_Str());
        mname.Clear();
        material->Get( AI_MATKEY_TEXTURE( aiTextureType_DIFFUSE,	0 ), mname	);
        strcpy(pMat->DiffuseTexture,mname.C_Str());
        while(  (pMat->DiffuseTexture[0]=='.')  ||
                (pMat->DiffuseTexture[0]=='\\') )
            for(int z=0;z<strlen(pMat->DiffuseTexture);z++)
                pMat->DiffuseTexture[z]=pMat->DiffuseTexture[z+1];
        dlcs_charreplace(pMat->DiffuseTexture,'\\','/');
        mname.Clear();
/*
        pLog->_Add(" MATERIAL[%s][%d] DIFFUSETEXTURE:[%s] ",pMat->name,i,pMat->DiffuseTexture);
        material->Get( AI_MATKEY_TEXTURE( aiTextureType_NORMALS,	0 ), mname	);
        strcpy(pMat->NormalTexture,mname.C_Str()+3);
        mname.Clear();
        pLog->_Add(" MATERIAL[%s][%d] NORMALTEXTURE:[%s] ",pMat->name,i,pMat->NormalTexture);
        material->Get( AI_MATKEY_TEXTURE( aiTextureType_HEIGHT,	    0 ), mname	);
        strcpy(pMat->HeightTexture,mname.C_Str()+3);
        mname.Clear();
        pLog->_Add(" MATERIAL[%s][%d] HEIGHTTEXTURE:[%s] ",pMat->name,i,pMat->HeightTexture);
        material->Get( AI_MATKEY_TEXTURE( aiTextureType_OPACITY,	0 ), mname	);
        strcpy(pMat->OpacityTexture,mname.C_Str()+3);
        pLog->_Add(" MATERIAL[%s][%d] OPACITYTEXTURE:[%s] ",pMat->name,i,pMat->OpacityTexture);
        material->Get( AI_MATKEY_TEXTURE( aiTextureType_SHININESS,	0 ), mname	);
        strcpy(pMat->ShininessTexture,mname.C_Str()+3);
        pLog->_Add(" MATERIAL[%s][%d] SHININESSTEXTURE:[%s] ",pMat->name,i,pMat->ShininessTexture);
        material->Get( AI_MATKEY_TEXTURE( aiTextureType_SPECULAR,	0 ), mname	);
        strcpy(pMat->SpecularTexture,mname.C_Str()+3);
        pLog->_Add(" MATERIAL[%s][%d] SPECULARTEXTURE:[%s] ",pMat->name,i,pMat->SpecularTexture);
        */
    }



    for(int i=0;i<numMeshes;i++) {

        pMesh=pFirstMesh;
        if(pMesh) {
            while(pMesh->pNext) {
                pMesh=pMesh->pNext;
            }
            pMesh->pNext=new CGLMesh;
            pMesh=pMesh->pNext;

        } else {
            pFirstMesh=new CGLMesh;
            pMesh=pFirstMesh;
        }

        aiMesh *inMesh = scene->mMeshes[i];

        pMesh->iMeshIndex       = i;
        pMesh->iMaterialIndex   = inMesh->mMaterialIndex;
        pMesh->numTriangles     = inMesh->mNumFaces*3;
        pMesh->numUvCoords      = inMesh->GetNumUVChannels();


        pLog->_Add("Meshindex[%d] MaterialIndex[%d] NumTri[%d] NumUV[%d]",
                        pMesh->iMeshIndex,
                        pMesh->iMaterialIndex,
                        pMesh->numTriangles,
                        pMesh->numUvCoords
                   );

        pMesh->vertexArray  = new float[inMesh->mNumFaces*3*3];
        pMesh->normalArray  = new float[inMesh->mNumFaces*3*3];
        pMesh->uvArray      = new float[inMesh->mNumFaces*3*2];

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

    pLog->_Add("Model loaded: %s (%d)",name,this);
    pLog->_Add("============================================================");
    return true;
}

CGLMesh* CGLModel::GetMesh(int x){
    CGLMesh* pMesh;
    pMesh=pFirstMesh;
    while(pMesh) {
        if(pMesh->iMeshIndex==x) return pMesh;
        pMesh=pMesh->pNext;
    }
    return 0;
}
CGLMaterial* CGLModel::GetMaterial(int x){
    CGLMaterial* pMat;
    pMat=pFirstMaterial;
    while(pMat) {
        if(pMat->iMaterialIndex==x) return pMat;
        pMat=pMat->pNext;
    }
    return 0;
}
CGLMaterial* CGLModel::GetMaterial(char* inDiffuseTex){
    CGLMaterial* pMat;
    pMat=pFirstMaterial;
    while(pMat) {
        if(dlcs_strcasecmp(pMat->DiffuseTexture,inDiffuseTex)) return pMat;
        pMat=pMat->pNext;
    }
    return 0;
}


bool CGLModel::Draw(CGLTexture* pTexture) {
    if(!pGFX) return 0;
    bool bMatFound;
    CGLMesh* pMesh;
    CGLMaterial* pMat;

    pMesh=pFirstMesh;
    while(pMesh) {
        bMatFound=false;
        pMat=GetMaterial(pMesh->iMaterialIndex);
        if(pGFX->GetTexture(pMat->DiffuseTexture)) {
           if(pGFX->GetTexture(pMat->DiffuseTexture)->bmap) {
                glBindTexture(GL_TEXTURE_2D,pGFX->GetTexture(pMat->DiffuseTexture)->bmap);
                bMatFound=true;
           }
        }
        if(pTexture) {
            if(pTexture->bmap) {
                glBindTexture(GL_TEXTURE_2D,pTexture->bmap);
                bMatFound=true;
            }
        }
        if(!bMatFound) {
            glBindTexture(GL_TEXTURE_2D,pGFX->pDefaultTexture->bmap);
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);   //  glClientActiveTexture(GL_TEXTURE0_ARB);

        glVertexPointer(3,GL_FLOAT,0,   pMesh->vertexArray);
        glNormalPointer(GL_FLOAT,0,     pMesh->normalArray);
        glTexCoordPointer(2,GL_FLOAT,0, pMesh->uvArray);
        glDrawArrays(GL_TRIANGLES,0,    pMesh->numTriangles);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        pMesh=pMesh->pNext;
    }
    return true;
}


