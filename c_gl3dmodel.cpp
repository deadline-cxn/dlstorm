/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson
****************************************************************/
#include "c_gl3dmodel.h"
CGLModel::CGLModel() {
    memset(name,0,1024);
    nextMesh=0;
    pNext=0;
    pPrev=0;
    pLog=0;
    bMadeLog=1;
    pLog=new CLog("CGLModel.log");
    pLog->Restart();
    pLog->_DebugAdd("CGLModel::CGLModel()");
}
CGLModel::CGLModel(CLog *pInLog) {
    memset(name,0,1024);
    nextMesh=0;
    pNext=0;
    pPrev=0;
    pLog=0;
    bMadeLog=0;
    pLog=pInLog;
    pLog->_DebugAdd("CGLModel::CGLModel(CLog *pInLog)");
}
CGLModel::~CGLModel() {
    DEL(nextMesh);
    if(vertexArray) delete [] vertexArray;
    if(normalArray) delete [] normalArray;
    if(uvArray) delete [] uvArray;
    if(bMadeLog) DEL(pLog);

}
bool CGLModel::Load(char* filename) {


    CGLModel* whichModel;
    whichModel=this;
    memset(texturename,0,1024);
    strcpy(name,filename);

    const aiScene *scene = aiImportFile(filename,aiProcessPreset_TargetRealtime_MaxQuality); // importer.ReadFile(filename,aiProcessPreset_TargetRealtime_Fast);

    if(!scene) return false;

    numMaterials=scene->mNumMaterials;
    numMeshes=scene->mNumMeshes;

    pLog->_Add(" MESHES[%d] MATERIALS[%d]\n",numMeshes,numMaterials);


    for(int i=0;i<numMaterials;i++) {

        aiMaterial *material = scene->mMaterials[i];


        aiString name;
        material->Get(AI_MATKEY_NAME ,name);


        aiString l_difTexName;
        aiString l_nmlTexName;
        aiString l_hgtTexName;
        aiString l_opaTexName;
        aiString l_shnTexName;
        aiString l_spcTexName;

        material->Get( AI_MATKEY_TEXTURE( aiTextureType_DIFFUSE,	0 ), l_difTexName	);
		material->Get( AI_MATKEY_TEXTURE( aiTextureType_NORMALS,	0 ), l_nmlTexName	);
		material->Get( AI_MATKEY_TEXTURE( aiTextureType_HEIGHT,	    0 ), l_hgtTexName	);
		material->Get( AI_MATKEY_TEXTURE( aiTextureType_OPACITY,	0 ), l_opaTexName	);
		material->Get( AI_MATKEY_TEXTURE( aiTextureType_SHININESS,	0 ), l_shnTexName	);
		material->Get( AI_MATKEY_TEXTURE( aiTextureType_SPECULAR,	0 ), l_spcTexName	);

        pLog->_Add(" MATERIALS[%d] [%s]\nDIFFUSE  [%s]\nNORMALS  [%s]\nHEIGHT   [%s]\nOPACITY  [%s]\nSHININESS[%s]\nSPECULAR [%s] \n",i,
        name.C_Str(),
        l_difTexName.C_Str(),
        l_nmlTexName.C_Str(),
        l_hgtTexName.C_Str(),
        l_opaTexName.C_Str(),
        l_shnTexName.C_Str(),
        l_spcTexName.C_Str() );


        if(texturename[0]==0) {
            strcpy(texturename,l_difTexName.C_Str()+3);
            for(int i=0;i<strlen(texturename);i++)
                if(texturename[i]=='\\')
                texturename[i]='/';
            pLog->_Add(" TEXTURE: [%s]",texturename);
        }


    }


    for(int i=0;i<numMeshes;i++) {

        aiMesh *mesh = scene->mMeshes[i];

        //mesh->mFaces
        //mesh->mVertices

        pLog->_Add(" MESH[%s][%d] MATERIAL INDEX[%d]",mesh->mName.C_Str(),i,mesh->mMaterialIndex);


        if(whichModel) {

            whichModel->numTriangles = mesh->mNumFaces*3;
            int index=0;
            whichModel->numUvCoords = mesh->GetNumUVChannels();

            whichModel->vertexArray = new float[mesh->mNumFaces*3*3];
            whichModel->normalArray = new float[mesh->mNumFaces*3*3];
            whichModel->uvArray     = new float[mesh->mNumFaces*3*2];

            for(unsigned int i=0;i<mesh->mNumFaces;i++) {
                const aiFace& face = mesh->mFaces[i];
                for(int j=0;j<3;j++) {
                    aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
                    memcpy(whichModel->uvArray,&uv,sizeof(float)*2);
                    whichModel->uvArray+=2;

                    aiVector3D normal = mesh->mNormals[face.mIndices[j]];
                    memcpy(whichModel->normalArray,&normal,sizeof(float)*3);
                    whichModel->normalArray+=3;

                    aiVector3D pos = mesh->mVertices[face.mIndices[j]];
                    memcpy(whichModel->vertexArray,&pos,sizeof(float)*3);
                    whichModel->vertexArray+=3;
                }
            }

            whichModel->uvArray     -= mesh->mNumFaces*3*2;
            whichModel->normalArray -= mesh->mNumFaces*3*3;
            whichModel->vertexArray -= mesh->mNumFaces*3*3;

            if(i<numMeshes) {
                whichModel->nextMesh=new CGLModel;
                whichModel=whichModel->nextMesh;
            }
        }
    }
}

bool CGLModel::RenderSceneDraw(void) {
    pLog->_DebugAdd("CGLModel::RenderSceneDraw(void)");
    return true;
}
bool CGLModel::Draw(void) {

    CGLModel* wModel;
    wModel=this;

    while(wModel){

        //  glClientActiveTexture(GL_TEXTURE0_ARB);
        if(pGFX) {
            //pLog->_Add("pGFX=[%d]",pGFX);
            if(pGFX->GetTexture(texturename)) {
                //pLog->_Add("texturename=[%s] pGFX->GetTexture(texturename)=[%d]",texturename,pGFX->GetTexture(texturename));
                if(pGFX->GetTexture(texturename)->bmap) {
                    //pLog->_Add("TEXTURE BIND [%s] OPENGL[%d]",texturename,pGFX->GetTexture(texturename)->bmap);
                    glBindTexture(GL_TEXTURE_2D,pGFX->GetTexture(texturename)->bmap);
                }
            }
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3,GL_FLOAT,0,wModel->vertexArray);
        glNormalPointer(GL_FLOAT,0,wModel->normalArray);


        glTexCoordPointer(2,GL_FLOAT,0,wModel->uvArray);

        glDrawArrays(GL_TRIANGLES,0,wModel->numTriangles);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        wModel=wModel->nextMesh;
    }
    return true;
}


