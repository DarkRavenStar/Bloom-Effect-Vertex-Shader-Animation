#define GLFW_INCLUDE_ES2 1
#define GLFW_DLL 1
//#define GLFW_EXPOSE_NATIVE_WIN32 1
//#define GLFW_EXPOSE_NATIVE_EGL 1

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream> 
#include "Angle_Util/Matrix.h"
#include "Angle_Util/geometry_utils.h"
#include "bitmap.h"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

#define TEXTURE_COUNT 5

GLint GprogramID = -1;
GLuint GtextureID[TEXTURE_COUNT];


GLuint Gframebuffer;
GLuint GdepthRenderbuffer;

GLuint GfullscreenTexture1;
GLuint GfullscreenTexture2;
GLuint GfullscreenTexture3;
GLuint GfullscreenTexture4;
GLuint GfullscreenTexture5;


GLFWwindow* window;

Matrix4 gPerspectiveMatrix;
Matrix4 gViewMatrix;

GLfloat vVertices[10000];
GLfloat vColors[10000];
int v = 0; //vertex count


void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

void UpdateText()
{
	gotoxy(0, 0);
	std::cout << "Bloom & Vertex Animation by 0118856 Teena" << std::endl;
	gotoxy(0, 1);
	std::cout << "Press A or D to rotate left and right" << std::endl;
	gotoxy(0, 2);
	std::cout << "Press W or S to rotate up and down" << std::endl;
	gotoxy(0, 3);
	std::cout << "Press R or F to zoom in and out" << std::endl;
}

void GenPillar(void)
{
	float size = 0.1f;
	float height = 0.02f;
	float startY = -0.3f;
	int segment = 20;

	for (int i = 0; i < segment; i++)
	{
		vVertices[v + 0] = -size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;
		vVertices[v + 0] = -size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;
		vVertices[v + 0] = size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;
		vVertices[v + 0] = size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;
		vVertices[v + 0] = size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;
		vVertices[v + 0] = -size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;

		vVertices[v + 0] = -size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;
		vVertices[v + 0] = -size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;
		vVertices[v + 0] = size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;
		vVertices[v + 0] = size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;
		vVertices[v + 0] = size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;
		vVertices[v + 0] = -size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;

		vVertices[v + 0] = -size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;
		vVertices[v + 0] = -size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;
		vVertices[v + 0] = -size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;
		vVertices[v + 0] = -size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;
		vVertices[v + 0] = -size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;
		vVertices[v + 0] = -size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;

		vVertices[v + 0] = size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;
		vVertices[v + 0] = size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;
		vVertices[v + 0] = size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;
		vVertices[v + 0] = size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = size; v += 3;
		vVertices[v + 0] = size; vVertices[v + 1] = height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;
		vVertices[v + 0] = size; vVertices[v + 1] = -height + startY + i * height*2.0f; vVertices[v + 2] = -size; v += 3;
	}

	//lazily fill random color for each vertex
	/*
	for(int i=0; i<v*4; i+=4)
	{

	vColors[i] = rand() / float(RAND_MAX);
	vColors[i+1] = rand() / float(RAND_MAX);
	vColors[i+2] = rand() / float(RAND_MAX);
	vColors[i+3] = 1.0f;
	}
	*/


	//===========generate colors
	float s = -height + startY;
	float d = -height + startY + (segment - 1)*height*2.0f;
	Vector3 color1(1.0f, 0.0f, 0.0f);
	Vector3 color2(1.0f, 1.0f, 0.0f);
	int c = 0;
	for (int i = 0; i < v * 3; i += 3)
	{
		float h = vVertices[i + 1];
		float r = (h - s) / (d - s);
		Vector3 color3 = color1 * (1.0f - r) + color2 * (r);

		vColors[c] = color3.x;
		vColors[c + 1] = color3.y;
		vColors[c + 2] = color3.z;
		vColors[c + 3] = 1.0f;

		c += 4;
	}
	//============

}

static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

GLuint LoadShader(GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   	return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );
   
   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) 
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
		 char infoLog[4096];
         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         printf ( "Error compiling shader:\n%s\n", infoLog );            
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;
}

GLuint LoadShaderFromFile(GLenum shaderType, std::string path)
{
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile( path.c_str() );

    if( sourceFile )
    {
        shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
        const GLchar* shaderSource = shaderString.c_str();

		return LoadShader(shaderType, shaderSource);
    }
    else
        printf( "Unable to open file %s\n", path.c_str() );

    return shaderID;
}

void loadTexture(const char* path, GLuint textureID)
{
	CBitmap bitmap(path);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

	// bilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());
}

int Init ( void )
{
   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   //load textures
   glGenTextures(TEXTURE_COUNT, GtextureID);
   loadTexture("../media/skybox.bmp", GtextureID[0]);
   loadTexture("../media/background.bmp", GtextureID[1]);
   loadTexture("../media/win8.bmp", GtextureID[2]);
   loadTexture("../media/fury_nano2.bmp", GtextureID[3]);
   loadTexture("../media/rainbow.bmp", GtextureID[4]);
   //====



   //==================== set up frame buffer, render buffer, and create an empty texture for blurring purpose
   // create a new FBO
   glGenFramebuffers(1, &Gframebuffer);
   
   // create a new empty texture for rendering original scene (screenshot)
   glGenTextures(1, &GfullscreenTexture1);
   glBindTexture(GL_TEXTURE_2D, GfullscreenTexture1);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glGenTextures(1, &GfullscreenTexture2);
   glBindTexture(GL_TEXTURE_2D, GfullscreenTexture2);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glGenTextures(1, &GfullscreenTexture3);
   glBindTexture(GL_TEXTURE_2D, GfullscreenTexture3);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glGenTextures(1, &GfullscreenTexture4);
   glBindTexture(GL_TEXTURE_2D, GfullscreenTexture4);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glGenTextures(1, &GfullscreenTexture5);
   glBindTexture(GL_TEXTURE_2D, GfullscreenTexture5);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   // create and bind renderbuffer, and create a 16-bit depth buffer
   glGenRenderbuffers(1, &GdepthRenderbuffer);
   glBindRenderbuffer(GL_RENDERBUFFER, GdepthRenderbuffer);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, WINDOW_WIDTH, WINDOW_HEIGHT);
   //=================================


   fragmentShader = LoadShaderFromFile(GL_VERTEX_SHADER, "../_Shader/vertexShader1.vert");
   vertexShader = LoadShaderFromFile(GL_FRAGMENT_SHADER, "../_Shader/fragmentShader1.frag");

   // Create the program object
   programObject = glCreateProgram ( );
   
   if ( programObject == 0 )
      return 0;

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   glBindAttribLocation ( programObject, 0, "vPosition" );
   glBindAttribLocation ( programObject, 1, "vColor" );
   glBindAttribLocation ( programObject, 2, "vTexCoord" );

   // Link the program
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked ) 
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
		 char infoLog[1024];
         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         printf ( "Error linking program:\n%s\n", infoLog );            
      }

      glDeleteProgram ( programObject );
      return 0;
   }

   // Store the program object
   GprogramID = programObject;


   glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   //initialize matrices
   gPerspectiveMatrix = Matrix4::perspective(60.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.5f, 3000.0f);
   gViewMatrix = Matrix4::translate(Vector3(0.0f, 0.0f, -2.0f));

   GenPillar();

   return 1;
}

void UpdateCamera(void)
{
	static float yaw = 0.0f;
	static float pitch = 0.0f;
	static float distance = 15.5f;

	if (glfwGetKey(window, 'A'))pitch -= 1.0f;
	if (glfwGetKey(window, 'D'))pitch += 1.0f;
	if (glfwGetKey(window, 'W'))yaw -= 1.0f;
	if (glfwGetKey(window, 'S'))yaw += 1.0f;

	if (glfwGetKey(window, 'R'))
	{
		distance -= 0.6f;
		if (distance < 1.0f)
		{
			distance = 1.0f;
		}
	}
	if (glfwGetKey(window, 'F'))distance += 0.6f;

	gViewMatrix = Matrix4::translate(Vector3(0.0f, 0.0f, -distance))*
		Matrix4::rotate(yaw, Vector3(1.0f, 0.0f, 0.0f))*
		Matrix4::rotate(pitch, Vector3(0.0f, 1.0f, 0.0f));
}

void DrawSquare(GLuint texture)
{
    static GLfloat vVertice[] = {-1.0f,  1.0f, 0.0f,
								-1.0f, -1.0f, 0.0f,
								1.0f, -1.0f,  0.0f,
								1.0f,  -1.0f, 0.0f,
								1.0f, 1.0f, 0.0f,
								-1.0f, 1.0f,  0.0f};
					 

   static GLfloat vColor[] = {1.0f,  0.0f, 0.0f, 1.0f,
								0.0f, 1.0f, 0.0f, 1.0f,
								0.0f, 0.0f,  1.0f, 1.0f,
								0.0f,  0.0f, 1.0f, 1.0f,
								1.0f, 1.0f, 0.0f, 1.0f,
								1.0f, 0.0f,  0.0f, 1.0f};

   static GLfloat vTexCoord[] = {0.0f,  1.0f,
									0.0f, 0.0f,
									1.0f, 0.0f,
									1.0f,  0.0f,
									1.0f, 1.0f,
									0.0f, 1.0f};


   glBindTexture(GL_TEXTURE_2D, texture);


   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertice);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vColor);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, vTexCoord);

   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);


   glDrawArrays(GL_TRIANGLES, 0, 6);


   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
   glDisableVertexAttribArray(2);
}

void DrawSkyBox(GLuint texture, float Size)
{
	static GLfloat vVertice[] = {
		//Front Square
		-Size,  Size, Size,			-Size, -Size, Size,			Size, -Size,  Size,

		Size,  -Size, Size,			Size, Size, Size,			-Size, Size,  Size,


		//Back Square
		-Size,  Size, -Size,			-Size, -Size, -Size,			Size, -Size,  -Size,

		Size,  -Size, -Size,			Size, Size, -Size,			-Size, Size,  -Size,


		//Left Square
		-Size,  Size, Size,			-Size, -Size, Size,			-Size, -Size,  -Size,

		-Size,  -Size, -Size,			-Size, Size, -Size,			-Size, Size,  Size,


		//Right Square
		Size,  Size, Size,			Size, -Size, Size,			Size, -Size,  -Size,

		Size,  -Size, -Size,			Size, Size, -Size,			Size, Size,  Size,


		//Top Square
		-Size,  Size, -Size,			-Size, Size, Size,			Size, Size,  Size,

		Size,  Size, Size,			Size, Size, -Size,			-Size, Size,  -Size,


		//Bottom Square
		-Size,  -Size, -Size,			-Size, -Size, Size,			Size, -Size,  Size,

		Size,  -Size, Size,			Size, -Size, -Size,			-Size, -Size,  -Size
	};

	static GLfloat vColor[] = { 1.0f,  0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f,  1.0f, 1.0f,
		0.0f,  0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f,  0.0f, 1.0f };

	//Setup
	float X = (1.0f / 4.0f);
	float Y = (1.0f / 3.0f);

	static GLfloat vTexCoord[] = {

		//Front
		//3.0f*X,	2.0f*Y,			3.0f*X,	1.0f*Y,			4.0f*X,	1.0f*Y,
		4.0f*X,	2.0f*Y,			4.0f*X,	1.0f*Y,			3.0f*X,	1.0f*Y,

		//4.0f*X,	1.0f*Y,			4.0f*X,	2.0f*Y,			3.0f*X,	2.0f*Y,
		3.0f*X,	1.0f*Y,			3.0f*X,	2.0f*Y,			4.0f*X,	2.0f*Y,


		//Back
		1.0f*X,	2.0f*Y,			1.0f*X,	1.0f*Y,			2.0f*X,	1.0f*Y,

		2.0f*X,	1.0f*Y,			2.0f*X,	2.0f*Y,			1.0f*X,	2.0f*Y ,


		//Left
		0.0f*X,	2.0f*Y,			0.0f*X,	1.0f*Y,			1.0f*X,	1.0f*Y,

		1.0f*X,	1.0f*Y,			1.0f*X,	2.0f*Y,			0.0f*X,	2.0f*Y,


		//Right
		//2.0f*X,	2.0f*Y,			2.0f*X,	1.0f*Y,			3.0f*X,	1.0f*Y,
		3.0f*X,	2.0f*Y,			3.0f*X,	1.0f*Y,			2.0f*X,	1.0f*Y,

		//3.0f*X,	1.0f*Y,			3.0f*X,	2.0f*Y,			2.0f*X,	2.0f*Y,
		2.0f*X,	1.0f*Y,			2.0f*X,	2.0f*Y,			3.0f*X,	2.0f*Y,


		//Top
		//1.0f*X,	3.0f*Y,			1.0f*X,	2.0f*Y,			2.0f*X,	2.0f*Y,
		1.0f*X,	2.0f*Y,			1.0f*X,	3.0f*Y,			2.0f*X,	3.0f*Y,

		//2.0f*X,	2.0f*Y,			2.0f*X,	3.0f*Y,			1.0f*X,	3.0f*Y,
		2.0f*X,	3.0f*Y,			2.0f*X,	2.0f*Y,			1.0f*X,	2.0f*Y,


		//Bottom
		1.0f*X,	1.0f*Y,			1.0f*X,	0.0f*Y,			2.0f*X,	0.0f*Y,

		2.0f*X,	0.0f*Y,			2.0f*X,	1.0f*Y,			1.0f*X,	1.0f*Y
	};


	glBindTexture(GL_TEXTURE_2D, texture);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertice);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vColor);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, vTexCoord);

	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	glDrawArrays(GL_TRIANGLES, 0, 36);


	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void DrawColorBox(float Size, float color[4])
{
	static GLfloat vVertice[] = {
		//Front Square
		-Size,  Size, Size,			-Size, -Size, Size,			Size, -Size,  Size,

		Size,  -Size, Size,			Size, Size, Size,			-Size, Size,  Size,


		//Back Square
		-Size,  Size, -Size,			-Size, -Size, -Size,			Size, -Size,  -Size,

		Size,  -Size, -Size,			Size, Size, -Size,			-Size, Size,  -Size,


		//Left Square
		-Size,  Size, Size,			-Size, -Size, Size,			-Size, -Size,  -Size,

		-Size,  -Size, -Size,			-Size, Size, -Size,			-Size, Size,  Size,


		//Right Square
		Size,  Size, Size,			Size, -Size, Size,			Size, -Size,  -Size,

		Size,  -Size, -Size,			Size, Size, -Size,			Size, Size,  Size,


		//Top Square
		-Size,  Size, -Size,			-Size, Size, Size,			Size, Size,  Size,

		Size,  Size, Size,			Size, Size, -Size,			-Size, Size,  -Size,


		//Bottom Square
		-Size,  -Size, -Size,			-Size, -Size, Size,			Size, -Size,  Size,

		Size,  -Size, Size,			Size, -Size, -Size,			-Size, -Size,  -Size
	};

	const int space = 144;

	GLfloat vColor[space];

	for (int i = 0; i < space; i+= 4)
	{
		vColor[i + 0] = color[0];
		vColor[i + 1] = color[1];
		vColor[i + 2] = color[2];
		vColor[i + 3] = color[3];
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertice);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vColor);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void DrawGenPillar(void)
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vColors);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, v);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Draw(void)
{	
	// Use the program object, it's possible that you have multiple shader programs and switch it accordingly
    glUseProgram(GprogramID);

	// set the sampler2D varying variable to the first texture unit(index 0)
	glUniform1i(glGetUniformLocation(GprogramID, "sampler2d"), 0);

	//========pass texture size to shader
    glUniform1f(glGetUniformLocation(GprogramID, "uTextureW"), (GLfloat)WINDOW_WIDTH);
    glUniform1f(glGetUniformLocation(GprogramID, "uTextureH"), (GLfloat)WINDOW_HEIGHT);
    //=========


	//modify Factor 1 varying variable
	static float factor1 = 0.0f;
	factor1 += 0.01f;
	GLint factor1Loc = glGetUniformLocation(GprogramID, "Factor1");
	if (factor1Loc != -1)
	{
		glUniform1f(factor1Loc, factor1);
	}

	//Make model rotate
	static float modelRotation = 0.0f;
	modelRotation += 0.5f;
	if (modelRotation > 360.0f)
	{
		modelRotation -= 360.0f;
	}
	//===================

	UpdateCamera();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//=====================draw 2 rectangles on a texture
	// bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, Gframebuffer);
	// specify texture as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GfullscreenTexture1, 0);
	// specify depth_renderbufer as depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, GdepthRenderbuffer);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status == GL_FRAMEBUFFER_COMPLETE)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniform1i(glGetUniformLocation(GprogramID, "uBlurDirection"), -1); //set to no blur
		
		Matrix4 modelMatrix, mvpMatrix;
		modelMatrix = Matrix4::translate(Vector3(-1.2f, 0.0f, 1.0f)) *
						Matrix4::rotate(0, Vector3(0.0f, 1.0f, 0.0f));
		mvpMatrix = gPerspectiveMatrix * gViewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(GprogramID, "uMvpMatrix"), 1, GL_FALSE, mvpMatrix.data);
		glUniform1i(glGetUniformLocation(GprogramID, "vertexAnimation"), -1);
		DrawSquare(GtextureID[3]); //draw first rectangle

		glBindTexture(GL_TEXTURE_2D,GtextureID[1]);
		modelMatrix = Matrix4::translate(Vector3(1.2f, 0.0f, 1.0f)) *
						Matrix4::rotate(0, Vector3(0.0f, 1.0f, 0.0f));
		mvpMatrix = gPerspectiveMatrix * gViewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(GprogramID, "uMvpMatrix"), 1, GL_FALSE, mvpMatrix.data);
		glUniform1i(glGetUniformLocation(GprogramID, "vertexAnimation"), -1);
		float color[4] = { 1.0f, 0.5f, 1.0f, 1.0f };
		DrawColorBox(0.5, color);

		glBindTexture(GL_TEXTURE_2D, GtextureID[2]);
		modelMatrix =	Matrix4::translate(Vector3(0.0f, 0.0f, 5.0f))*
						Matrix4::rotate(90, Vector3(1.0f, 0.0f, 0.0f)) *
						Matrix4::rotate(-modelRotation, Vector3(0.0f, 1.0f, 0.0f));
		mvpMatrix = gPerspectiveMatrix * gViewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(GprogramID, "uMvpMatrix"), 1, GL_FALSE, mvpMatrix.data);
		glUniform1i(glGetUniformLocation(GprogramID, "vertexAnimation"), 0);
		DrawGenPillar();

		glBindTexture(GL_TEXTURE_2D, GtextureID[0]);
		modelMatrix = Matrix4::translate(Vector3(0.0f, 0.0f, 0.0f))*
			Matrix4::rotate(90, Vector3(1.0f, 0.0f, 0.0f)) *
			Matrix4::rotate(-modelRotation, Vector3(0.0f, 1.0f, 0.0f));
		mvpMatrix = gPerspectiveMatrix * gViewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(GprogramID, "uMvpMatrix"), 1, GL_FALSE, mvpMatrix.data);
		glUniform1i(glGetUniformLocation(GprogramID, "vertexAnimation"), 1);
		DrawGenPillar();

		glBindTexture(GL_TEXTURE_2D, GtextureID[1]);
		modelMatrix = Matrix4::translate(Vector3(0.0f, 0.0f, -5.0f))*
			Matrix4::rotate(90, Vector3(1.0f, 0.0f, 0.0f)) *
			Matrix4::rotate(-modelRotation, Vector3(0.0f, 1.0f, 0.0f));
		mvpMatrix = gPerspectiveMatrix * gViewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(GprogramID, "uMvpMatrix"), 1, GL_FALSE, mvpMatrix.data);
		glUniform1i(glGetUniformLocation(GprogramID, "vertexAnimation"), 2);
		DrawGenPillar();


		modelMatrix = Matrix4::translate(Vector3(1.2f, 0.0f, 1.0f)) *
			Matrix4::rotate(0, Vector3(0.0f, 1.0f, 0.0f));
		mvpMatrix = gPerspectiveMatrix * gViewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(GprogramID, "uMvpMatrix"), 1, GL_FALSE, mvpMatrix.data);
		glUniform1i(glGetUniformLocation(GprogramID, "vertexAnimation"), -1);
		DrawSkyBox(GtextureID[0], 30.0f); //Skybox
	}
	else
	{
		printf("frame buffer is not ready!\n");
	}
	//=============================================


	
	//==============Highpass the texture ================================
	// bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, Gframebuffer);
	// specify texture as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GfullscreenTexture2, 0);
	// specify depth_renderbufer as depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, GdepthRenderbuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// reset the mvpMatrix to identity matrix so that it renders fully on texture in normalized device coordinates
	glUniformMatrix4fv(glGetUniformLocation(GprogramID, "uMvpMatrix"), 1, GL_FALSE, Matrix4::identity().data);

	// draw the texture, apply blurring
	glUniform1i(glGetUniformLocation(GprogramID, "uBlurDirection"), 2);
	DrawSquare(GfullscreenTexture1);
	//======================================================

	

	//==============blur the texture ================================
	// bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, Gframebuffer);
	// specify texture as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GfullscreenTexture3, 0);
	// specify depth_renderbufer as depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, GdepthRenderbuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// reset the mvpMatrix to identity matrix so that it renders fully on texture in normalized device coordinates
	glUniformMatrix4fv(glGetUniformLocation(GprogramID, "uMvpMatrix"), 1, GL_FALSE, Matrix4::identity().data);
	
	// draw the texture, apply blurring
	glUniform1i(glGetUniformLocation(GprogramID, "uBlurDirection"), 0); 
	DrawSquare(GfullscreenTexture2);
	//======================================================



	//==============blur the texture ================================
	// bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, Gframebuffer);
	// specify texture as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GfullscreenTexture4, 0);
	// specify depth_renderbufer as depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, GdepthRenderbuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// reset the mvpMatrix to identity matrix so that it renders fully on texture in normalized device coordinates
	glUniformMatrix4fv(glGetUniformLocation(GprogramID, "uMvpMatrix"), 1, GL_FALSE, Matrix4::identity().data);

	// draw the texture, apply blurring
	glUniform1i(glGetUniformLocation(GprogramID, "uBlurDirection"), 1);
	DrawSquare(GfullscreenTexture3);
	//======================================================

	//==============Render the texture ================================
	// this time, render directly to window system framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// reset the mvpMatrix to identity matrix so that it renders fully on texture in normalized device coordinates
	glUniformMatrix4fv(glGetUniformLocation(GprogramID, "uMvpMatrix"), 1, GL_FALSE, Matrix4::identity().data);
	// draw the texture, apply blurring
	glUniform1i(glGetUniformLocation(GprogramID, "uBlurDirection"), -1);

	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	DrawSquare(GfullscreenTexture1);
	DrawSquare(GfullscreenTexture4);

	glDisable(GL_BLEND);
	glDepthMask(true);
	//======================================================
}

int main(void)
{
  glfwSetErrorCallback(error_callback);

  // Initialize GLFW library
  if (!glfwInit())
    return -1;

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create and open a window
  window = glfwCreateWindow(WINDOW_WIDTH,
                            WINDOW_HEIGHT,
                            "BLOOM & VERTEX SHADER ANIMATION BY TEENA 0118856",
                            NULL,
                            NULL);

  if(!window)
  {
    glfwTerminate();
    printf("glfwCreateWindow Error\n");
    exit(1);
  }

  glfwMakeContextCurrent(window);

  Init();
  UpdateText();

  // Repeat
  while(!glfwWindowShouldClose(window))
  {
	  Draw();
	  glfwSwapBuffers(window);
	  glfwPollEvents();

	  if(glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
