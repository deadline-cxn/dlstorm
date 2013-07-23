/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

    OpenGL Map Model class

****************************************************************/

#include "c_mapmodel.h"
/*
void readstr(FILE *f,char *string) {
    do {
        fgets(string, 255, f);
    } while ((string[0] == '/') || (string[0] == '\n'));
    return;
}*/

C_MapModel::C_MapModel() {

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
