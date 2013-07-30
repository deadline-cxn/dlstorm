/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

    OpenGL Map class

****************************************************************/

#include "c_map_new.h"

CMesh :: CMesh() {
    Initialize();
    bMadeLog=true;
    pLog=0;
}

CMesh::CMesh(CLog *pInLog, CGAF *pInGAF) {
    pLog=pInLog;
    pGAF=pInGAF;
    bMadeLog=false;
    Initialize();
    pTexture=0;
}

CMesh::CMesh(CLog *pInLog, CGAF *pInGAF, CGLTexture *pInTexture) {
    pLog=pInLog;
    pGAF=pInGAF;
    bMadeLog=false;
    Initialize();
    pTexture=pInTexture;
}

CMesh :: ~CMesh() {
    if(bMadeLog) DEL(pLog);
    if( m_pVertices ) delete [] m_pVertices; // Deallocate Vertex Data
    m_pVertices = NULL;
    if( m_pTexCoords ) delete [] m_pTexCoords; // Deallocate Texture Coord Data
    m_pTexCoords = NULL;
    // if( m_pTex ) delete [] m_pTex;     m_pTex = NULL;
}

void CMesh::Initialize(void) {

    pTexture=0;
    m_pVertices = NULL;
    m_pTexCoords = NULL;
    m_nVertexCount = 0;

    x=512;
    y=512;

    pOffset.x=0;
    pOffset.y=0;
    pOffset.z=0;

    m_nVertexCount = (int) (x*y)*6;

    m_pVertices     = new CVector3[m_nVertexCount];						// Allocate Vertex Data
    m_pTexCoords    = new CTexCoord[m_nVertexCount];				// Allocate Tex Coord Data
    //m_pTex          = new CTex[m_nVertexCount];

    int nX,nZ,nTri,nIndex;
    nIndex=0;
    nTri=0;
    float flX, flZ;

    for( nZ = 0; nZ < y; nZ++) { // += (int) res )
        for( nX = 0; nX < x; nX++) { // += (int) res )
            flX = (float) nX; // + ( ( nTri == 1 || nTri == 2 || nTri == 5 ) ? res : 0.0f ); // Using This Quick Hack, Figure The X,Z Position Of The Point
            flZ = (float) nZ;// + ( ( nTri == 2 || nTri == 4 || nTri == 5 ) ? res : 0.0f );

            m_pVertices[nIndex].x = flX ;  //flX - ( x / 2 );
            m_pVertices[nIndex].y = 0.0f;  //PtHeight( (int) flX, (int) flZ ) *  flHeightScale;
            m_pVertices[nIndex].z = flZ;   //flZ - ( y / 2 );
            m_pTexCoords[nIndex].u = 0.0f; //flX / x ;
            m_pTexCoords[nIndex].v = 1.0f; //flZ / y ;
            nIndex++; // Increment Our Index

            m_pVertices[nIndex].x = flX+1;  //flX - ( x / 2 );
            m_pVertices[nIndex].y = 0.0f; //PtHeight( (int) flX, (int) flZ ) *  flHeightScale;
            m_pVertices[nIndex].z = flZ;   //flZ - ( y / 2 );
            m_pTexCoords[nIndex].u = 0.0f;// flX / x ;
            m_pTexCoords[nIndex].v = 0.0f;//flZ / y ;
            nIndex++; // Increment Our Index

            m_pVertices[nIndex].x = flX+1;  //flX - ( x / 2 );
            m_pVertices[nIndex].y = 0.0f; //PtHeight( (int) flX, (int) flZ ) *  flHeightScale;
            m_pVertices[nIndex].z = flZ+1;   //flZ - ( y / 2 );
            m_pTexCoords[nIndex].u = 1.0f;//flX / x ;
            m_pTexCoords[nIndex].v = 0.0f;//flZ / y ;
            nIndex++; // Increment Our Index\

            m_pVertices[nIndex].x = flX+1;  //flX - ( x / 2 );
            m_pVertices[nIndex].y = 0.0f; //PtHeight( (int) flX, (int) flZ ) *  flHeightScale;
            m_pVertices[nIndex].z = flZ+1;   //flZ - ( y / 2 );
            m_pTexCoords[nIndex].u = 0.0f;// flX / x ;
            m_pTexCoords[nIndex].v = 1.0f;//flZ / y ;
            nIndex++; // Increment Our Index

            m_pVertices[nIndex].x = flX;  //flX - ( x / 2 );
            m_pVertices[nIndex].y = 0.0f; //PtHeight( (int) flX, (int) flZ ) *  flHeightScale;
            m_pVertices[nIndex].z = flZ+1;   //flZ - ( y / 2 );
            m_pTexCoords[nIndex].u = 1.0f;//flX / x ;
            m_pTexCoords[nIndex].v = 1.0f;//flZ / y ;
            nIndex++; // Increment Our Index

            m_pVertices[nIndex].x = flX;  //flX - ( x / 2 );
            m_pVertices[nIndex].y = 0.0f; //PtHeight( (int) flX, (int) flZ ) *  flHeightScale;
            m_pVertices[nIndex].z = flZ;   //flZ - ( y / 2 );
            m_pTexCoords[nIndex].u = 1.0f;//flX / x ;
            m_pTexCoords[nIndex].v = 0.0f;//flZ / y ;
            nIndex++; // Increment Our Index
        }
    }

    Terraform();
}

void CMesh::Terraform(void) {
    srand(GetTickCount());
    int nZ,vx,vy;
    float vz;
    vx=50;
    vy=50;
    for(nZ=0; nZ<35000; nZ++) {
        vx = vx + (rand()%3)-1;
        vy = vy + (rand()%3)-1;
        vz =
        (float)rand()/((float)RAND_MAX/5.0f);
        //((float)rand()/(float)RAND_MAX) * 5.0f;
        if(vx < 3)      vx=3;
        if(vx > (x-3))  vx=x-3;
        if(vy < 3)      vy=3;
        if(vy > (y-3))  vy=y-3;
        SetPointHeight(vx,vy,(float)vz);
    }
    // int j;  int wx;
    // vz=5.0f;
    // wx=(rand()%50);
    // j++; if(j>wx) { wx = (rand()%50); =0; }
}

void CMesh::SetPointTexture(int nX, int nZ, CGLTexture* pTex) {
    int nIndex;
    nIndex=((nX % x) + ((nZ % y) * x)) * 6;
    if(nIndex > ( x * y * 6 / ( res * res ))) return;
    if(nIndex < 0) return;
    //m_pTex[nIndex].t =pTex->bmap;
}

void CMesh::SetPointHeight(int nX, int nZ, float fHeight) {
    int nIndex;
    int tex=105;
    if(fHeight<0) tex=112;
    if(fHeight>1) tex=107;
    if(fHeight>2) tex=104;

    nIndex=((nX % x) + ((nZ % y) * x)) * 6;

    if(nIndex > ( x * y * 6 / ( res * res ))) return;
    if(nIndex < 0) return;

    m_pVertices[nIndex].y=fHeight;
    m_pVertices[nIndex+5].y=fHeight;
    nZ--;

    nIndex=((nX % x) + ((nZ % y) * x)) * 6;
    if(nIndex > ( x * y * 6 / ( res * res ))) return;
    if(nIndex < 0) return;

    m_pVertices[nIndex+4].y=fHeight;
    nX--;

    nIndex=((nX % x) + ((nZ % y) * x)) * 6;
    if(nIndex > ( x * y * 6 / ( res * res ))) return;
    if(nIndex < 0) return;

    m_pVertices[nIndex+2].y=fHeight;
    m_pVertices[nIndex+3].y=fHeight;
    nZ++;

    nIndex=((nX % x) + ((nZ % y) * x)) * 6;
    if(nIndex > ( x * y * 6 / ( res * res ))) return;
    if(nIndex < 0) return;

    m_pVertices[nIndex+1].y=fHeight;
}


void CMesh::Draw(void) {

    glPushMatrix();

    glColor3f(1.0f,1.0f,1.0f);

    if(pTexture) glBindTexture(GL_TEXTURE_2D, pTexture->bmap);

    glEnableClientState( GL_VERTEX_ARRAY );						// Enable Vertex Arrays
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );				// Enable Texture Coord Arrays

    glTranslatef(pOffset.x,pOffset.y,pOffset.z);

    glVertexPointer(    3, GL_FLOAT, 0, m_pVertices ); // Set The Vertex Pointer To Our Vertex Data
    glTexCoordPointer(  2, GL_FLOAT, 0, m_pTexCoords ); // Set The Vertex Pointer To Our TexCoord Data
    glDrawArrays( GL_TRIANGLES, 0, m_nVertexCount );	// Draw All Of The Triangles At Once // Disable Pointers
    glDisableClientState( GL_VERTEX_ARRAY );					// Disable Vertex Arrays
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );				// Disable Texture Coord Arrays

    glPopMatrix();
}

/* bool CMesh :: LoadHeightmap( char* filename, float flHeightScale, float flResolution ) {
	long size1;
	unsigned char *fb1;
	GAF_FileBuffer nfbuf1;
	nfbuf1  = pGAF->GetFile((LPSTR)filename);
	size1	= nfbuf1.Size;
	fb1		= nfbuf1.fb;
	if(!fb1) return 0;
	himage=new CxImage((BYTE*)fb1,size1,CXIMAGE_FORMAT_BMP);
	free(fb1);
    x=himage->GetWidth();
    y=himage->GetHeight();
    res=flResolution;
	m_nVertexCount = (int) ( x * y * 6 / ( res * res )); // Generate Vertex Field
	pLog->_Add("vertex count = %d",m_nVertexCount);
	m_pVertices     = new CVec[m_nVertexCount];						// Allocate Vertex Data
	m_pTexCoords    = new CTexCoord[m_nVertexCount];				// Allocate Tex Coord Data
	int nX, nZ, nTri, nIndex=0;									// Create Variables
	float flX, flZ;
	for( nZ = 0; nZ < y; nZ += (int) res ) 	{
		for( nX = 0; nX < x; nX += (int) res ) 		{
			for( nTri = 0; nTri < 6; nTri++ ) 			{
				flX = (float) nX + ( ( nTri == 1 || nTri == 2 || nTri == 5 ) ? res : 0.0f ); // Using This Quick Hack, Figure The X,Z Position Of The Point
				flZ = (float) nZ + ( ( nTri == 2 || nTri == 4 || nTri == 5 ) ? res : 0.0f );

				m_pVertices[nIndex].x = flX - ( x / 2 ); // Set The Data, Using PtHeight To Obtain The Y Value
				m_pVertices[nIndex].y = 0.0f;//PtHeight( (int) flX, (int) flZ ) *  flHeightScale;
				m_pVertices[nIndex].z = flZ - ( y / 2 );
				m_pTexCoords[nIndex].u = flX / x ;
				m_pTexCoords[nIndex].v = flZ / y ;
				nIndex++; // Increment Our Index

            }
		}
	}
	himage->Destroy();
	DEL(himage);
	return true;
}
float CMesh :: PtHeight( int nX, int nY ) {
    //int x; //x=himage->GetWidth();
    //i/nt y; //y=himage->GetHeight();
    RGBQUAD hrgb = himage->GetPixelColor(nX,nY,0);
	int nPos = ( ( nX %	x )  + ( ( nY %	y ) * x ) ) * 3; // Calculate The Position In The Texture, Careful Not To Overflow
	float flR = (float) hrgb.rgbRed;
	float flG = (float) hrgb.rgbGreen;
	float flB = (float) hrgb.rgbBlue;
	return ( 0.299f * flR + 0.587f * flG + 0.114f * flB );		// Calculate The Height Using The Luminance Algorithm
} */
