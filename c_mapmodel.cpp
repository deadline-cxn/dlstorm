/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

    OpenGL Map Model class

****************************************************************/

#include "c_mapmodel.h"

C_MapModelList::C_MapModelList() {
    pMapModel=0;
    loc.x=0.0f;
    loc.y=0.0f;
    loc.z=0.0f;
    rot.x=0.0f;
    rot.y=0.0f;
    rot.z=0.0f;
    scale.x=1.0f;
    scale.y=1.0f;
    scale.z=1.0f;
    color.r=1.0f;
    color.g=1.0f;
    color.b=1.0f;
    transparency=1.0f;
    pNext=0;
}
C_MapModelList::~C_MapModelList(){
}

void C_MapModelList::Draw(void) {
    glTranslatef(loc.x, loc.y, loc.z);  // location
    glRotatef(rot.x,1.0f,0,0);
    glRotatef(rot.y,0,1.0f,0);
    glRotatef(rot.z,0,0,1.0f);       // rotation
    glScalef(scale.x,scale.y,scale.z);  // scale
    glColor3f(color.r,color.g,color.b); // color

    pMapModel->Draw();
}

C_MapModel::C_MapModel() {
    pNext=0;
}

C_MapModel::~C_MapModel() {
}

void C_MapModel::Load(char* filename) {
    float x, y, z, u, v;
    int numtriangles;
    int loop,vert;
    loop=0;
    vert=0;
    FILE *filein;
    char oneline[255];
    memset(name,0,255);
    memset(author,0,255);
    filein = fopen(filename, "rt");
    if(filein) {
        fgets(oneline, 255, filein);
        sscanf(oneline, "[name] %s\n", &name);
        fgets(oneline, 255, filein);
        sscanf(oneline, "[author] %s\n", &author);
        fgets(oneline, 255, filein);
        sscanf(oneline, "[polycount] %d\n", &numtriangles);
        fgets(oneline, 255, filein);
        sscanf(oneline, "[texture] %s\n", &texture);
        fgets(oneline, 255, filein);
        sscanf(oneline, "[transparency] %f",&transparency);

        sector1.triangle = new TRIANGLE[numtriangles];
        sector1.numtriangles = numtriangles;

        for (int loop = 0; loop < numtriangles; loop++) {
            for (int vert = 0; vert < 3; vert++) {

                // readstr(filein,oneline);//
                fgets(oneline, 255, filein);
                sscanf(oneline, "%f %f %f %f %f",
                       &sector1.triangle[loop].vertex[vert].x,
                       &sector1.triangle[loop].vertex[vert].y,
                       &sector1.triangle[loop].vertex[vert].z,
                       &sector1.triangle[loop].vertex[vert].u,
                       &sector1.triangle[loop].vertex[vert].v);
            /*  sector1.triangle[loop].vertex[vert].x = x;
                sector1.triangle[loop].vertex[vert].y = y;
                sector1.triangle[loop].vertex[vert].z = z;
                sector1.triangle[loop].vertex[vert].u = u;
                sector1.triangle[loop].vertex[vert].v = v;                 */
            }
        }
        fclose(filein);
    }
}

void C_MapModel::Draw(void) {
    GLfloat x_m, y_m, z_m, u_m, v_m;
    int numtriangles = sector1.numtriangles;
    // Process Each Triangle
    for (int loop_m = 0; loop_m < numtriangles; loop_m++) {
        glBegin(GL_TRIANGLES);
        glNormal3f( 0.0f, 0.0f, 1.0f);
        x_m = sector1.triangle[loop_m].vertex[0].x;
        y_m = sector1.triangle[loop_m].vertex[0].y;
        z_m = sector1.triangle[loop_m].vertex[0].z;
        u_m = sector1.triangle[loop_m].vertex[0].u;
        v_m = sector1.triangle[loop_m].vertex[0].v;
        glTexCoord2f(u_m,v_m);
        glVertex3f(x_m,y_m,z_m);

        x_m = sector1.triangle[loop_m].vertex[1].x;
        y_m = sector1.triangle[loop_m].vertex[1].y;
        z_m = sector1.triangle[loop_m].vertex[1].z;
        u_m = sector1.triangle[loop_m].vertex[1].u;
        v_m = sector1.triangle[loop_m].vertex[1].v;
        glTexCoord2f(u_m,v_m);
        glVertex3f(x_m,y_m,z_m);

        x_m = sector1.triangle[loop_m].vertex[2].x;
        y_m = sector1.triangle[loop_m].vertex[2].y;
        z_m = sector1.triangle[loop_m].vertex[2].z;
        u_m = sector1.triangle[loop_m].vertex[2].u;
        v_m = sector1.triangle[loop_m].vertex[2].v;
        glTexCoord2f(u_m,v_m);
        glVertex3f(x_m,y_m,z_m);
        glEnd();
    }
}
