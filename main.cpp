#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "LadeBMP.h"
#include "LadeShader.h"
#include "libs/glfw-3.3.8/deps/linmath.h"
#include "linmath.h"

#include <stdlib.h>

#define PI 3.1415926536

struct myVertexType // selbst definierter Datentyp für die Shaderprogramme
{
  float x, y, z;
  float r, g, b;
  float nx, ny, nz;
  float u, v;

  myVertexType(myVertexType other, vec3 norm, vec2 uv) {
    x = other.x;
    y = other.y;
    z = other.z;
    r = other.r;
    g = other.g;
    b = other.b;

    nx = norm[0];
    ny = norm[1];
    nz = norm[2];

    u = uv[0];
    v = uv[1];
  }
  myVertexType(float x, float y, float z, float r, float g, float b) {
    this->x = x;
    this->y = y;
    this->z = z;

    this->r = r;
    this->g = g;
    this->b = b;

    this->nx = 0;
    this->ny = 0;
    this->nz = 0;

    this->u = 0;
    this->v = 0;
  }
  myVertexType() { x = y = z = r = g = b = nx = ny = nz = u = v = 0; }
};

myVertexType vertices[36];

float ambfac = 0.3;
float diffac = 0.7;

float specfac = 0.6;
int shinefac = 16;

// Key-Callback
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (action != 0) {
    if (key == GLFW_KEY_A) {
      if (mods == 1)
        ambfac += .1f;
      else
        ambfac -= .1f;
    }
    if (key == GLFW_KEY_D) {
      if (mods == 1)
        diffac += .1f;
      else
        diffac -= .1f;
    }

    if (key == GLFW_KEY_W) {
      if (mods == 1)
        specfac += .1f;
      else
        specfac -= .1f;
    }
    if (key == GLFW_KEY_S) {
      if (mods == 1)
        shinefac += 1;
      else
        shinefac -= 1;
    }
  }
}

int main(void) {
  GLFWwindow *window;
  GLuint vertex_buffer, vertex_shader, fragment_shader, complete_shader_program;
  GLint matrix_access, ambient, diffuse, shininess, specular, position_access,
      color_access, normal_access, uv_access;

  myVertexType cubeEdges[8]{
      // x = y = z = r = g = b = nx = ny = nz = u = v = 0;
      /*0*/ myVertexType(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f,
                         0.0f), // Koordinate 1,1,1 mit roter Farbe
      /*1*/ myVertexType(+1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f), // Koordinate
                                                                 // 1,1,-1 mit
                                                                 // grüner Farbe
      /*2*/ myVertexType(+1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f), // blau
      /*3*/ myVertexType(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 1.0f), // magenta
      /*4*/ myVertexType(-1.0f, +1.0f, +1.0f, 0.0f, 1.0f, 1.0f), // cyan
      /*5*/ myVertexType(-1.0f, -1.0f, +1.0f, 1.0f, 1.0f, 0.0f), // gelb
      /*6*/ myVertexType(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f), // weiß
      /*7*/ myVertexType(-1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f)  // schwarz
  };

  float bearZoom = 0.4;
  float planeZoom = 0.17;

  // verkleinere Würfelkoordinaten
  for (int i = 0; i < 8; i++) {
    cubeEdges[i].x *= bearZoom;
    cubeEdges[i].y *= bearZoom;
    cubeEdges[i].z *= bearZoom;
  }

  // linke Seite -- verde -- acabado
  vec3 v3_0 = {1, 0, 0};
  vec3 v3_1 = {1, 0, 0};
  vec3 v3_2 = {1, 0, 0};
  vec3 v3_3 = {1, 0, 0};
  vec3 v3_4 = {1, 0, 0};
  vec3 v3_5 = {1, 0, 0};
  vec2 v2_0 = {0, 0.25};
  vec2 v2_1 = {0, 0.5};
  vec2 v2_2 = {0.25, 0.5};
  vec2 v2_3 = {0.25, 0.25};
  vec2 v2_4 = {0, 0.25};
  vec2 v2_5 = {0.25, 0.5};
  vertices[0] = myVertexType(cubeEdges[2], v3_0, v2_0);
  vertices[1] = myVertexType(cubeEdges[1], v3_1, v2_1);
  vertices[2] = myVertexType(cubeEdges[0], v3_2, v2_2);
  vertices[3] = myVertexType(cubeEdges[3], v3_3, v2_3);
  vertices[4] = myVertexType(cubeEdges[2], v3_4, v2_4);
  vertices[5] = myVertexType(cubeEdges[0], v3_5, v2_5);

  // Rechte Seite -- rojo -- acabado
  vec3 v3_6 = {-1, 0, 0};
  vec3 v3_7 = {-1, 0, 0};
  vec3 v3_8 = {-1, 0, 0};
  vec3 v3_9 = {-1, 0, 0};
  vec3 v3_10 = {-1, 0, 0};
  vec3 v3_11 = {-1, 0, 0};
  vec2 v2_6 = {0.75, 0.5};
  vec2 v2_7 = {0.5, 0.25};
  vec2 v2_8 = {0.75, 0.25};
  vec2 v2_9 = {0.75, 0.5};
  vec2 v2_10 = {0.5, 0.5};
  vec2 v2_11 = {0.5, 0.25};
  vertices[6] = myVertexType(cubeEdges[4], v3_6, v2_6);
  vertices[7] = myVertexType(cubeEdges[6], v3_7, v2_7);
  vertices[8] = myVertexType(cubeEdges[7], v3_8, v2_8);
  vertices[9] = myVertexType(cubeEdges[4], v3_9, v2_9);
  vertices[10] = myVertexType(cubeEdges[5], v3_10, v2_10);
  vertices[11] = myVertexType(cubeEdges[6], v3_11, v2_11);

  // Unten -- azul claro -- acabado
  vec3 v3_12 = {0, -1, 0};
  vec3 v3_13 = {0, -1, 0};
  vec3 v3_14 = {0, -1, 0};
  vec3 v3_15 = {0, -1, 0};
  vec3 v3_16 = {0, -1, 0};
  vec3 v3_17 = {0, -1, 0};
  vec2 v2_12 = {0.25, 1};
  vec2 v2_13 = {0.25, 0.75};
  vec2 v2_14 = {0.5, 1};
  vec2 v2_15 = {0.25, 0.75};
  vec2 v2_16 = {0.5, 0.75};
  vec2 v2_17 = {0.5, 1};
  // vec2 v2_12 = {0.25, 0.5};
  // vec2 v2_13 = {0.25, 0.75};
  // vec2 v2_14 = {0.5, 0.75};
  // vec2 v2_15 = {0.25, 0.5};
  // vec2 v2_16 = {0.5, 0.5};
  // vec2 v2_17 = {0.5, 0.75};
  vertices[12] = myVertexType(cubeEdges[2], v3_12, v2_12);
  vertices[13] = myVertexType(cubeEdges[3], v3_13, v2_13);
  vertices[14] = myVertexType(cubeEdges[6], v3_14, v2_14);
  vertices[15] = myVertexType(cubeEdges[3], v3_15, v2_15);
  vertices[16] = myVertexType(cubeEdges[5], v3_16, v2_16);
  vertices[17] = myVertexType(cubeEdges[6], v3_17, v2_17);

  // Oben -- rosa -- acabado
  vec3 v3_18 = {0, 1, 0};
  vec3 v3_19 = {0, 1, 0};
  vec3 v3_20 = {0, 1, 0};
  vec3 v3_21 = {0, 1, 0};
  vec3 v3_22 = {0, 1, 0};
  vec3 v3_23 = {0, 1, 0};
  vec2 v2_18 = {0.25, 0.5};
  vec2 v2_19 = {0.25, 0.25};
  vec2 v2_20 = {0.5, 0.5};
  vec2 v2_21 = {0.25, 0.25};
  vec2 v2_22 = {0.5, 0.5};
  vec2 v2_23 = {0.5, 0.25};
  vertices[18] = myVertexType(cubeEdges[0], v3_18, v2_18);
  vertices[19] = myVertexType(cubeEdges[1], v3_19, v2_19);
  vertices[20] = myVertexType(cubeEdges[4], v3_20, v2_20);
  vertices[21] = myVertexType(cubeEdges[1], v3_21, v2_21);
  vertices[22] = myVertexType(cubeEdges[4], v3_22, v2_22);
  vertices[23] = myVertexType(cubeEdges[7], v3_23, v2_23);

  // Hinten -- naranja -- acabado
  vec3 v3_24 = {0, 0, -1};
  vec3 v3_25 = {0, 0, -1};
  vec3 v3_26 = {0, 0, -1};
  vec3 v3_27 = {0, 0, -1};
  vec3 v3_28 = {0, 0, -1};
  vec3 v3_29 = {0, 0, -1};
  vec2 v2_24 = {0.25, 0};
  vec2 v2_25 = {0.5, 0};
  vec2 v2_26 = {0.5, 0.25};

  vec2 v2_27 = {0.25, 0};
  vec2 v2_28 = {0.5, 0.25};
  vec2 v2_29 = {0.25, 0.25};

  vertices[24] = myVertexType(cubeEdges[1], v3_24, v2_24);
  vertices[25] = myVertexType(cubeEdges[2], v3_25, v2_25);
  vertices[26] = myVertexType(cubeEdges[6], v3_26, v2_26);
  vertices[27] = myVertexType(cubeEdges[1], v3_27, v2_27);
  vertices[28] = myVertexType(cubeEdges[6], v3_28, v2_28);
  vertices[29] = myVertexType(cubeEdges[7], v3_29, v2_29);
  //
  // Vorne -- azul oscuro/morado -- acabado
  vec3 v3_30 = {0, 0, 1};
  vec3 v3_31 = {0, 0, 1};
  vec3 v3_32 = {0, 0, 1};
  vec3 v3_33 = {0, 0, 1};
  vec3 v3_34 = {0, 0, 1};
  vec3 v3_35 = {0, 0, 1};
  vec2 v2_30 = {0.25, 0.5};
  vec2 v2_31 = {0.25, 0.75};
  vec2 v2_32 = {0.5, 0.75};
  vec2 v2_33 = {0.25, 0.5};
  vec2 v2_34 = {0.5, 0.5};
  vec2 v2_35 = {0.5, 0.75};
  vertices[30] = myVertexType(cubeEdges[0], v3_30, v2_30);
  vertices[31] = myVertexType(cubeEdges[3], v3_31, v2_31);
  vertices[32] = myVertexType(cubeEdges[5], v3_32, v2_32);
  vertices[33] = myVertexType(cubeEdges[0], v3_33, v2_33);
  vertices[34] = myVertexType(cubeEdges[4], v3_34, v2_34);
  vertices[35] = myVertexType(cubeEdges[5], v3_35, v2_35);

  if (!glfwInit()) // GLFW initialisieren
    exit(EXIT_FAILURE);

  window = glfwCreateWindow(1280, 1024, "Happy Cube", NULL,
                            NULL); // Fenster mit 1280x1024 Pixel erstellen
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window,
                     key_callback); // Key-Callback setzten (Funktion oben)
  glfwMakeContextCurrent(window);   // Verbindung mit dem Fenster
  gladLoadGL(glfwGetProcAddress);   // Laden der Fuktionszeiger

  // Load Bear
  int bearNumber;
  // myVertexType *bear = loadModel("../Textures/Bear.bmp", &bearNumber);

  // Load bitmap
  int width, height;
  unsigned char *pictureData =
      loadBMP24("../Textures/Cube.bmp", &width, &height);

  // Erzeugen des Vertex-Shaders
  char *vertex_shader_code = readTextFileIntoString(
      "../Shaders/VertexShaderLabor4.glsl"); // Shader aus Datei laden
                                             // (LadeShader.h)
  if (vertex_shader_code == 0)
    exit(EXIT_FAILURE); // Sicherstellen, dass die Datei erfolgreich geladen
                        // wurde!
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
  glCompileShader(vertex_shader);
  char info[500];
  int num;
  glGetShaderInfoLog(vertex_shader, 500, &num, info);
  printf("%s", info);

  // Erzeugen des Fragment-Shaders
  char *fragment_shader_code = readTextFileIntoString(
      "../Shaders/FragmentShaderLabor4.glsl"); // Shader aus Datei laden
                                               // (LadeShader.h)
  if (fragment_shader_code == 0)
    exit(EXIT_FAILURE); // Sicherstellen, dass die Datei erfolgreich geladen
                        // wurde!
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
  glCompileShader(fragment_shader);
  glGetShaderInfoLog(fragment_shader, 500, &num, info);
  printf("%s", info);

  // Linken des Shader-Progamms
  complete_shader_program = glCreateProgram();
  glAttachShader(complete_shader_program, vertex_shader);
  glAttachShader(complete_shader_program, fragment_shader);
  glLinkProgram(complete_shader_program);

  // eine Texture mit width/height und den Daten in pictureData
  GLuint texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR,
               GL_UNSIGNED_BYTE,
               pictureData); // in pictureData befinden sich die Texelteldaten
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // Speicherfreigabe im Arbeitsspeicher jetzt möglich, da die Textur jetzt
  // auf der Grafikkarte ist
  delete[] pictureData;
  glGenerateMipmap(GL_TEXTURE_2D); // ist in jedem Fall nötig
  GLuint TextureID =
      glGetUniformLocation(complete_shader_program,
                           "myTextureSampler"); // Name des Shader-Prg und der
                                                // Variablenname ggf. anpassen

  // Uniforme Positionen für den Zugriff während des Renderns
  matrix_access = glGetUniformLocation(complete_shader_program, "matrix");
  ambient = glGetUniformLocation(complete_shader_program, "ambientFactor");
  diffuse = glGetUniformLocation(complete_shader_program, "diffuseFactor");
  shininess = glGetUniformLocation(complete_shader_program, "shininess");
  specular = glGetUniformLocation(complete_shader_program, "specFactor");

  // Zugriff auf Position und Farbe innerhalb des Vertex-Buffers
  position_access = glGetAttribLocation(complete_shader_program, "position");
  color_access = glGetAttribLocation(complete_shader_program, "color");
  normal_access = glGetAttribLocation(complete_shader_program, "normal");
  uv_access = glGetAttribLocation(complete_shader_program, "uv");

  // Aufbau und Übermittlung des Vertex-Buffers
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  // change bear <- -> vertices
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Assuming uv is at index 8 and 9 in your myVertexType struct
  // Enable and define the UV attribute
  glEnableVertexAttribArray(uv_access); // Enable the UV attribute
  glVertexAttribPointer(uv_access, 2, GL_FLOAT, GL_FALSE, sizeof(myVertexType),
                        (void *)(sizeof(float) * 9));

  // Die Daten für Position und Farbe werden dem Shader mitgeteilt
  glEnableVertexAttribArray(position_access);
  glVertexAttribPointer(position_access, 3, GL_FLOAT, GL_FALSE,
                        sizeof(myVertexType), (void *)0);

  glEnableVertexAttribArray(color_access);
  glVertexAttribPointer(color_access, 3, GL_FLOAT, GL_FALSE,
                        sizeof(myVertexType), (void *)(sizeof(float) * 3));

  glEnableVertexAttribArray(normal_access);
  glVertexAttribPointer(normal_access, 3, GL_FLOAT, GL_FALSE,
                        sizeof(myVertexType), (void *)(sizeof(float) * 6));

  // Z-Achse positiv defineren
  glClearDepth(0);
  glDepthFunc(GL_GREATER); // größere Z-Werte überschreiben kleinere
  glEnable(GL_DEPTH_TEST); // Verdeckung der Dreiecke untereinander aktivieren

  while (!glfwWindowShouldClose(window)) // Hauptscheife
  {
    mat4x4 m;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(
        complete_shader_program); // Ab jetzt dieses Shader-Programm nutzen

    // Rotationsmatrix erstellen
    mat4x4_identity(
        m); // Über die Linmath.h wird eine 4x4 Einheitsmatrix erzeugt
    mat4x4_rotate_Z(m, m,
                    (float)glfwGetTime()); // Auf die Matrix m wird eine
                                           // Rotation um Z aufmultipliziert
    mat4x4_rotate_X(m, m, 10 * 2 * PI / 360 * (float)glfwGetTime());
    // // Auf
    //   die Matrix m wird eine Rotation um Y aufmultipliziert
    mat4x4_rotate_Y(m, m, 2 * PI * 10 / 360 * sin((float)glfwGetTime()));

    glUniformMatrix4fv(
        matrix_access, 1, GL_FALSE,
        (const GLfloat *)
            m); // Die Matrix wird in das Shader-Programm übertragen

    glUniform1f(ambient, ambfac);
    // Der ambiente Faktor wird in das Shader-Programm übertragen
    glUniform1f(diffuse, diffac);
    glUniform1f(specular, specfac);
    glUniform1i(shininess, shinefac);

    // printf("ambfac=%.2f\n", ambfac);
    // printf("diffac=%.2f\n", diffac);
    // printf("specfac=%.2f\n", specfac);
    // printf("shinefac=%.2d\n", shinefac);
    glDrawArrays(GL_TRIANGLES, 0, 36); // Ein Dreieck wird gezeichnet

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
