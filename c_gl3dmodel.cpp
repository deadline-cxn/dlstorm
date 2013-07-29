/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson
****************************************************************/
#include "c_gl3dmodel.h"
CGLModel::CGLModel() {
    memset(name,0,1024);
    pNext=0;
    pPrev=0;
    bMadeLog=1;
    pLog=new CLog("CGLModel.log");
    pLog->Restart();
    pLog->_DebugAdd("CGLModel::CGLModel()");
}
CGLModel::CGLModel(CLog *pInLog) {
    memset(name,0,1024);
    pNext=0;
    pPrev=0;
    bMadeLog=0;
    pLog=pInLog;
    pLog->_DebugAdd("CGLModel::CGLModel(CLog *pInLog)");
}
CGLModel::~CGLModel() {

    delete [] vertexArray;
    delete [] normalArray;
    delete [] uvArray;
    if(bMadeLog) DEL(pLog);
}
bool CGLModel::Load(char* filename) {

    strcpy(name,filename);

    const aiScene *scene = aiImportFile(filename,aiProcessPreset_TargetRealtime_MaxQuality);
    // importer.ReadFile(filename,aiProcessPreset_TargetRealtime_Fast);

    if(!scene) return false;

    aiMesh *mesh=scene->mMeshes[0];
    numTriangles = mesh->mNumFaces*3;
    int index=0;
    numUvCoords = mesh->GetNumUVChannels();

    vertexArray = new float[mesh->mNumFaces*3*3];
    normalArray = new float[mesh->mNumFaces*3*3];
    uvArray = new float[mesh->mNumFaces*3*2];

    for(unsigned int i=0;i<mesh->mNumFaces;i++) {
        const aiFace& face = mesh->mFaces[i];
        for(int j=0;j<3;j++) {
            aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
            memcpy(uvArray,&uv,sizeof(float)*2);
            uvArray+=2;

            aiVector3D normal = mesh->mNormals[face.mIndices[j]];
            memcpy(normalArray,&normal,sizeof(float)*3);
            normalArray+=3;

            aiVector3D pos = mesh->mVertices[face.mIndices[j]];
            memcpy(vertexArray,&pos,sizeof(float)*3);
            vertexArray+=3;
        }
    }

    uvArray-=mesh->mNumFaces*3*2;
    normalArray-=mesh->mNumFaces*3*3;
    vertexArray-=mesh->mNumFaces*3*3;
}

bool CGLModel::RenderSceneDraw(void) {
    pLog->_DebugAdd("CGLModel::RenderSceneDraw(void)");
    return true;
}
bool CGLModel::Draw(void) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3,GL_FLOAT,0,vertexArray);
    glNormalPointer(GL_FLOAT,0,normalArray);

//     glClientActiveTexture(GL_TEXTURE0_ARB);
    glTexCoordPointer(2,GL_FLOAT,0,uvArray);

    glDrawArrays(GL_TRIANGLES,0,numTriangles);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    return true;
}


