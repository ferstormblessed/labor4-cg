#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "LadeBMP.h"
#include "LadeObj.h"
#include "LadeShader.h"

#include <stdlib.h>

#define PI 3.1415926536

float ambfac = 0.3;
float diffac = 0.7;

float specfac = 0.6;
int shinefac = 16;

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

int numVertices = 36;
myVertexType *vertices = loadModel("../Textures/Bear.bmp", &numVertices);

int main(void) {
  GLFWwindow *window;
  GLuint vertex_buffer, vertex_shader, fragment_shader, complete_shader_program;
  GLint matrix_access, ambient, diffuse, shininess, specular, position_access,
      color_access, normal_access, uv_access;

  float bearZoom = 0.4;
  float planeZoom = 0.17;

  // verkleinere Würfelkoordinaten
  for (int i = 0; i < numVertices; i++) {
    vertices[i].x *= bearZoom;
    vertices[i].y *= bearZoom;
    vertices[i].z *= bearZoom;
  }

  if (!glfwInit()) // GLFW initialisieren
    exit(EXIT_FAILURE);

  window = glfwCreateWindow(1280, 1024, "Bear texture", NULL,
                            NULL); // Fenster mit 1280x1024 Pixel erstellen
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window); // Verbindung mit dem Fenster
  gladLoadGL(glfwGetProcAddress); // Laden der Fuktionszeiger

  // Erzeugen des Vertex-Shaders
  char *vertex_shader_code = readTextFileIntoString(
      "../Shaders/VertexShaderBear.glsl"); // Shader aus Datei laden
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
      "../Shaders/FragmentShaderBear.glsl"); // Shader aus Datei laden
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

  matrix_access = glGetUniformLocation(complete_shader_program, "matrix");
  // Zugriff auf Position und Farbe innerhalb des Vertex-Buffers
  position_access = glGetAttribLocation(complete_shader_program, "position");
  color_access = glGetAttribLocation(complete_shader_program, "color");
  normal_access = glGetAttribLocation(complete_shader_program, "normal");
  uv_access = glGetAttribLocation(complete_shader_program, "uv");

  // Aufbau und Übermittlung des Vertex-Buffers
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices), vertices,
               GL_STATIC_DRAW);

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

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Ab jetzt dieses Shader-Programm nutzen
    glUseProgram(complete_shader_program);

    glDrawArrays(GL_TRIANGLES, 0, numVertices); // Ein Dreieck wird gezeichnet

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
