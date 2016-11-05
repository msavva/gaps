// Source file for the scene viewer program



// Include files 

#include "R3Graphics/R3Graphics.h"
#include "R3Graphics/p5d.h"
#include "fglut/fglut.h"
#include "debug.h"



// Program variables

static char *input_scene_name = NULL;
static char *output_image_name = NULL;
static char *input_cameras_name = NULL;
static char *input_lights_name = NULL;
static R3Vector initial_camera_towards(-0.57735, -0.57735, -0.57735);
static R3Vector initial_camera_up(0, 0, 1);
static R3Point initial_camera_origin(0,0,0);
static RNBoolean initial_camera = FALSE;
static RNRgb background_color(0,0,0);
static double max_vertex_spacing = 0;
static int print_verbose = 0;



// GLUT variables 

static int GLUTwindow = 0;
static int GLUTwindow_width = 640;
static int GLUTwindow_height = 480;
static int GLUTmouse[2] = { 0, 0 };
static int GLUTbutton[3] = { 0, 0, 0 };
static int GLUTmouse_drag = 0;
static int GLUTmodifiers = 0;



// Application variables

static R3Scene *scene = NULL;
static R3Viewer *viewer = NULL;
static R3SceneNode *selected_node = NULL;
static R3SceneElement *selected_element = NULL;
static RNArray<R3Camera *> *cameras = NULL;
static int selected_camera_index = -1;
static R3Point center(0, 0, 0);
static int headlight = 0;



// Display variables

static int show_faces = 1;
static int show_edges = 0;
static int show_cameras = 0;
static int show_lights = 0;
static int show_materials = 0;
static int show_bboxes = 0;
static int show_axes = 0;
static int show_rays = 0;
static int show_walls = 0;
static int show_closest = 0;
static int show_debug = 0;
static int show_frame_rate = 0;
static int show_backfacing = 0;



////////////////////////////////////////////////////////////////////////
// Draw functions
////////////////////////////////////////////////////////////////////////

static const R3Material *
KthMaterial(int k)
{
  // Make array of colors
  const int nmaterials = 72;
  static const R3Material materials[nmaterials] = {
    R3Material(new R3Brdf(1.0, 1.0, 0.0)), R3Material(new R3Brdf(0.0, 1.0, 0.0)), R3Material(new R3Brdf(0.0, 0.0, 1.0)), 
    R3Material(new R3Brdf(0.3, 0.6, 0.0)), R3Material(new R3Brdf(0.0, 1.0, 1.0)), R3Material(new R3Brdf(1.0, 0.0, 1.0)), 
    R3Material(new R3Brdf(1.0, 0.5, 0.0)), R3Material(new R3Brdf(0.0, 1.0, 0.5)), R3Material(new R3Brdf(0.5, 0.0, 1.0)), 
    R3Material(new R3Brdf(0.5, 1.0, 0.0)), R3Material(new R3Brdf(0.0, 0.5, 1.0)), R3Material(new R3Brdf(1.0, 0.0, 0.5)), 
    R3Material(new R3Brdf(0.5, 0.0, 0.0)), R3Material(new R3Brdf(0.0, 0.5, 0.0)), R3Material(new R3Brdf(0.0, 0.0, 0.5)), 
    R3Material(new R3Brdf(0.5, 0.5, 0.0)), R3Material(new R3Brdf(0.0, 0.5, 0.5)), R3Material(new R3Brdf(0.5, 0.0, 0.5)),
    R3Material(new R3Brdf(0.7, 0.0, 0.0)), R3Material(new R3Brdf(0.0, 0.7, 0.0)), R3Material(new R3Brdf(0.0, 0.0, 0.7)), 
    R3Material(new R3Brdf(0.7, 0.7, 0.0)), R3Material(new R3Brdf(0.0, 0.7, 0.7)), R3Material(new R3Brdf(0.7, 0.0, 0.7)), 
    R3Material(new R3Brdf(0.7, 0.3, 0.0)), R3Material(new R3Brdf(0.0, 0.7, 0.3)), R3Material(new R3Brdf(0.3, 0.0, 0.7)), 
    R3Material(new R3Brdf(0.3, 0.7, 0.0)), R3Material(new R3Brdf(0.0, 0.3, 0.7)), R3Material(new R3Brdf(0.7, 0.0, 0.3)), 
    R3Material(new R3Brdf(0.3, 0.0, 0.0)), R3Material(new R3Brdf(0.0, 0.3, 0.0)), R3Material(new R3Brdf(0.0, 0.0, 0.3)), 
    R3Material(new R3Brdf(0.3, 0.3, 0.0)), R3Material(new R3Brdf(0.0, 0.3, 0.3)), R3Material(new R3Brdf(0.3, 0.0, 0.3)),
    R3Material(new R3Brdf(1.0, 0.3, 0.3)), R3Material(new R3Brdf(0.3, 1.0, 0.3)), R3Material(new R3Brdf(0.3, 0.3, 1.0)), 
    R3Material(new R3Brdf(1.0, 1.0, 0.3)), R3Material(new R3Brdf(0.3, 1.0, 1.0)), R3Material(new R3Brdf(1.0, 0.3, 1.0)), 
    R3Material(new R3Brdf(1.0, 0.5, 0.3)), R3Material(new R3Brdf(0.3, 1.0, 0.5)), R3Material(new R3Brdf(0.5, 0.3, 1.0)), 
    R3Material(new R3Brdf(0.5, 1.0, 0.3)), R3Material(new R3Brdf(0.3, 0.5, 1.0)), R3Material(new R3Brdf(1.0, 0.3, 0.5)), 
    R3Material(new R3Brdf(0.5, 0.3, 0.3)), R3Material(new R3Brdf(0.3, 0.5, 0.3)), R3Material(new R3Brdf(0.3, 0.3, 0.5)), 
    R3Material(new R3Brdf(0.5, 0.5, 0.3)), R3Material(new R3Brdf(0.3, 0.5, 0.5)), R3Material(new R3Brdf(0.5, 0.3, 0.5)),
    R3Material(new R3Brdf(0.3, 0.5, 0.5)), R3Material(new R3Brdf(0.5, 0.3, 0.5)), R3Material(new R3Brdf(0.5, 0.5, 0.3)), 
    R3Material(new R3Brdf(0.3, 0.3, 0.5)), R3Material(new R3Brdf(0.5, 0.3, 0.3)), R3Material(new R3Brdf(0.3, 0.5, 0.3)), 
    R3Material(new R3Brdf(0.3, 0.8, 0.5)), R3Material(new R3Brdf(0.5, 0.3, 0.8)), R3Material(new R3Brdf(0.8, 0.5, 0.3)), 
    R3Material(new R3Brdf(0.8, 0.3, 0.5)), R3Material(new R3Brdf(0.5, 0.8, 0.3)), R3Material(new R3Brdf(0.3, 0.5, 0.8)), 
    R3Material(new R3Brdf(0.8, 0.5, 0.5)), R3Material(new R3Brdf(0.5, 0.8, 0.5)), R3Material(new R3Brdf(0.5, 0.5, 0.8)), 
    R3Material(new R3Brdf(0.8, 0.8, 0.5)), R3Material(new R3Brdf(0.5, 0.8, 0.8)), R3Material(new R3Brdf(0.8, 0.5, 0.8))
  };

  // Return material
  return (k >= 0) ? &materials[k % nmaterials] : &R3default_material;
}



#if 0
static void 
DrawText(const R3Point& p, const char *s)
{
  // Draw text string s and position p
  glRasterPos3d(p[0], p[1], p[2]);
  while (*s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *(s++));
}
#endif



static void 
DrawText(const R2Point& p, const char *s)
{
  // Draw text string s and position p
  R3Ray ray = viewer->WorldRay((int) p[0], (int) p[1]);
  R3Point position = ray.Point(2 * viewer->Camera().Near());
  glRasterPos3d(position[0], position[1], position[2]);
  while (*s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *(s++));
}



static void 
DrawCamera(R3Scene *scene, const R3Camera& camera, RNScalar radius = 0)
{
  // Compute default radius
  if (radius == 0) radius = 0.025 * scene->BBox().DiagonalRadius();
  
  // Draw view frustum
  R3Point eye = camera.Origin();
  R3Vector towards = camera.Towards();
  R3Vector up = camera.Up();
  R3Vector right = camera.Right();
  RNAngle xfov = camera.XFOV();
  RNAngle yfov = camera.YFOV();
  R3Point org = eye + towards * radius;
  R3Vector dx = right * radius * tan(xfov);
  R3Vector dy = up * radius * tan(yfov);
  R3Point ur = org + dx + dy;
  R3Point lr = org + dx - dy;
  R3Point ul = org - dx + dy;
  R3Point ll = org - dx - dy;
  glBegin(GL_LINE_LOOP);
  glVertex3d(ur[0], ur[1], ur[2]);
  glVertex3d(ul[0], ul[1], ul[2]);
  glVertex3d(ll[0], ll[1], ll[2]);
  glVertex3d(lr[0], lr[1], lr[2]);
  glVertex3d(ur[0], ur[1], ur[2]);
  glVertex3d(eye[0], eye[1], eye[2]);
  glVertex3d(lr[0], lr[1], lr[2]);
  glVertex3d(ll[0], ll[1], ll[2]);
  glVertex3d(eye[0], eye[1], eye[2]);
  glVertex3d(ul[0], ul[1], ul[2]);
  glEnd();
}



static void
DrawCameras(R3Scene *scene, const RNArray<R3Camera *> *cameras = NULL)
{
  // Draw cameras
  if (cameras) {
    // Draw all input cameras
    for (int i = 0; i < cameras->NEntries(); i++) {
      R3Camera *camera = cameras->Kth(i);
      DrawCamera(scene, *camera);
    }
  }
  else {
    // Draw default camera
    DrawCamera(scene, scene->Camera());
  }
}


static void 
DrawLights(R3Scene *scene)
{
  // Draw all lights
  double radius = scene->BBox().DiagonalRadius();
  for (int i = 0; i < scene->NLights(); i++) {
    R3Light *light = scene->Light(i);
    RNLoadRgb(light->Color());
    if (light->ClassID() == R3DirectionalLight::CLASS_ID()) {
      R3DirectionalLight *directional_light = (R3DirectionalLight *) light;
      R3Vector direction = directional_light->Direction();

      // Draw direction vector
      glBegin(GL_LINES);
      R3Point centroid = scene->BBox().Centroid();
      R3LoadPoint(centroid - radius * direction);
      R3LoadPoint(centroid - 0.1 * radius * direction);
      glEnd();
    }
    else if (light->ClassID() == R3PointLight::CLASS_ID()) {
      // Draw sphere at point light position
      R3PointLight *point_light = (R3PointLight *) light;
      R3Point position = point_light->Position();

      // Draw sphere at light position 
      R3Sphere(position, 0.01 * radius).Draw();
    }
    else if (light->ClassID() == R3SpotLight::CLASS_ID()) {
      R3SpotLight *spot_light = (R3SpotLight *) light;
      R3Point position = spot_light->Position();
      R3Vector direction = spot_light->Direction();

      // Draw sphere at light position 
      R3Sphere(position, 0.01 * radius).Draw();
  
      // Draw direction vector
      glBegin(GL_LINES);
      R3LoadPoint(position);
      R3LoadPoint(position + 0.025 * radius * direction);
      glEnd();
    }
    else {
      fprintf(stderr, "Unrecognized light type: %d\n", light->ClassID());
      return;
    }
  }
}



static void 
DrawShapes(R3Scene *scene, R3SceneNode *node, const R3Affine& parent_transformation, RNFlags draw_flags = R3_DEFAULT_DRAW_FLAGS)
{
  // Update transformation
  R3Affine cumulative_transformation = parent_transformation;
  cumulative_transformation.Transform(node->Transformation());

  // Draw elements 
  if (node->NElements() > 0) {
    // Load lights 
    int max_lights = 8 - headlight;
    if (scene->NLights() > max_lights) {
      node->LoadLights(headlight, 7);
    }

    // Push transformation
    cumulative_transformation.Push();
    
    // Draw surfaces
    for (int i = 0; i < node->NElements(); i++) {
      R3SceneElement *element = node->Element(i);

      // Set material
      const R3Material *material = element->Material();
      static R3Material selection_material(&R3red_brdf, "Selection");
      if (element == selected_element) selection_material.Draw();
      else if (show_materials) KthMaterial(material->SceneIndex())->Draw();
      else if (node == selected_node) selection_material.Draw();
      else if (material) material->Draw();
      else R3default_material.Draw();

      // Draw shapes
      for (int j = 0; j < element->NShapes(); j++) {
        R3Shape *shape = element->Shape(j);
        shape->Draw(draw_flags);
      }
    }

    // Pop transformation
    cumulative_transformation.Pop();
  }

  // Draw children
  for (int i = 0; i < node->NChildren(); i++) {
    R3SceneNode *child = node->Child(i);
    DrawShapes(scene, child, cumulative_transformation, draw_flags);
  }
}



static void 
DrawShapes(R3Scene *scene, RNFlags draw_flags = R3_DEFAULT_DRAW_FLAGS)
{
  // Draw the scene
  DrawShapes(scene, scene->Root(), R3identity_affine, draw_flags);
}



static void 
DrawBBoxes(R3Scene *scene, R3SceneNode *node)
{
  // Draw node bounding box
  node->BBox().Outline();

  // Push transformation
  node->Transformation().Push();

  // Draw children bboxes
  for (int i = 0; i < node->NChildren(); i++) {
    R3SceneNode *child = node->Child(i);
    DrawBBoxes(scene, child);
  }

  // Pop transformation
  node->Transformation().Pop();
}



static void 
DrawRays(R3Scene *scene)
{
  // Ray intersection variables
  R3SceneNode *node;
  R3SceneElement *element;
  R3Shape *shape;
  R3Point point;
  R3Vector normal;
  RNScalar t;

  // Ray generation variables
  int istep = scene->Viewport().Width() / 10;
  int jstep = scene->Viewport().Height() / 10;
  if (istep == 0) istep = 1;
  if (jstep == 0) jstep = 1;

  // Ray drawing variables
  double radius = 0.005 * scene->BBox().DiagonalRadius();
  // const R3Point& eye = scene->Camera().Origin();
  const R3Point& eye = viewer->Camera().Origin();

  // Draw intersection point and normal for some rays
  for (int i = istep/2; i < scene->Viewport().Width(); i += istep) {
    for (int j = jstep/2; j < scene->Viewport().Height(); j += jstep) {
      // R3Ray ray = scene->Viewer().WorldRay(i, j);
      R3Ray ray = viewer->WorldRay(i, j);
      if (scene->Intersects(ray, &node, &element, &shape, &point, &normal, &t)) {
        // Get intersection information
        const R3Material *material = (element) ? element->Material() : NULL;
        const R3Brdf *brdf = (material) ? material->Brdf() : NULL;

        // Compute color
        RNRgb color = scene->Ambient();
        if (brdf) {
          color += brdf->Emission();
          for (int k = 0; k < scene->NLights(); k++) {
            R3Light *light = scene->Light(k);
            color += light->Reflection(*brdf, eye, point, normal);
          }
        }

        // Draw intersection
        RNLoadRgb(color);
        R3Sphere(point, radius).Draw();
        R3Span(point, point + 2 * radius * normal).Draw();
      }
    }
  }
}



static void 
DrawClosest(R3Scene *scene)
{
  // Closest point variables
  R3SceneNode *node;
  R3SceneElement *element;
  R3Shape *shape;
  R3Point point;
  R3Vector normal;
  RNScalar d;

  // Closest point drawing variables
  double radius = 0.01 * scene->BBox().DiagonalRadius();
  // const R3Point& eye = scene->Camera().Origin();
  const R3Point& eye = viewer->Camera().Origin();

  // Draw closest point
  if (scene->FindClosest(eye, &node, &element, &shape, &point, &normal, &d)) {
    R3Sphere(point, radius).Draw();
    R3Span(point, point + 2 * radius * normal).Draw();
  }
}



static void
DrawPlanner5DWalls(R3Scene *scene)
{
  // Get project
  if (!scene->Root()) return;
  P5DProject *project = (P5DProject *) scene->Root()->Data();
  if (!project) return;

  // Set root transformation -- mirror the world across x to compensate for swap of Y and Z axes 
  R4Matrix xmirror_matrix(-1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
  R3Affine root_transformation(xmirror_matrix, TRUE);
  root_transformation.Push();
  
  // Draw floors
  RNScalar floor_z = 0;
  for (int i = 0; i < project->NFloors(); i++) {
    P5DFloor *floor = project->Floor(i);

    // Set floor transformation
    R3Affine floor_transformation(R3identity_affine);
    floor_transformation.Translate(R3Vector(0, 0, floor_z));
    floor_transformation.Push();
    floor_z += floor->h;

    // Draw rooms
    for (int j = 0; j < floor->NRooms(); j++) {
      P5DRoom *room = floor->Room(j);
      if (strcmp(room->className, "Room")) continue;
      
      // Set room transformation
      R3Affine room_transformation(R3identity_affine);
      room_transformation.Translate(R3Vector(room->x, room->y, 0));
      room_transformation.Push();
      
      // Draw walls
      glColor3d(0,1,1);
      glBegin(GL_QUADS);
      for (int k = 0; k < room->NWalls(); k++) {
        P5DWall *wall = room->Wall(k);
        if (wall->hidden) continue;
        R3Point p1(wall->x1, wall->y1, wall->z1);
        R3Point p2(wall->x2, wall->y2, wall->z2);
        if (R3Contains(p1, p2)) continue;
        R3Vector direction = p2 - p1;
        direction.Normalize();
        R3Vector normal = direction;
        normal.ZRotate(RN_PI_OVER_TWO);
        // for (int side = -1; side <= 1; side += 2) {
        for (int side = 1; side <= 1; side += 2) {
          R3LoadPoint(p1 + 0.5*wall->w*direction + side*0.5*wall->w*normal);
          R3LoadPoint(p2 - 0.5*wall->w*direction + side*0.5*wall->w*normal);
          R3LoadPoint(p2 - 0.5*wall->w*direction + side*0.5*wall->w*normal + room->h*R3posz_vector);
          R3LoadPoint(p1 + 0.5*wall->w*direction + side*0.5*wall->w*normal + room->h*R3posz_vector);
        }
      }
      glEnd();
      
      // Draw floor
      glColor3d(1,0,1);
      glBegin(GL_TRIANGLE_FAN);
      for (int k = 0; k < room->NWalls(); k++) {
        P5DWall *wall = room->Wall(k);
        R3LoadPoint(wall->x1, wall->y1, wall->z1 + room->h);
      }
      glEnd();

      // Draw ceiling
      if (room->h > 2) {
        glColor3d(0,0,1);
        glBegin(GL_TRIANGLE_FAN);
        for (int k = 0; k < room->NWalls(); k++) {
          P5DWall *wall = room->Wall(k);
          R3LoadPoint(wall->x1, wall->y1, wall->z1 + room->h);
        }
        glEnd();
      }

      // Pop the room transformation
      room_transformation.Pop();
    }

    // Pop the floor transformation 
    floor_transformation.Pop();
  }

  // Pop the root transformation
  root_transformation.Pop();
}



////////////////////////////////////////////////////////////////////////
// GLUT user interface functions
////////////////////////////////////////////////////////////////////////

void GLUTStop(void)
{
  // Destroy window 
  glutDestroyWindow(GLUTwindow);

  // Exit
  exit(0);
}



void GLUTRedraw(void)
{
  // Check scene
  if (!scene) return;

  // Clear window 
  // background_color = scene->Background();
  glClearColor(background_color.R(), background_color.G(), background_color.B(), 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set backface culling
  if (show_backfacing) glDisable(GL_CULL_FACE);
  else glEnable(GL_CULL_FACE);

  // Set viewing transformation
  viewer->Camera().Load();

  // Draw camera
  if (show_cameras) {
    glDisable(GL_LIGHTING);
    glLineWidth(5);
    DrawCameras(scene, cameras);
    glLineWidth(1);
  }

  // Draw lights
  if (show_lights) {
    glDisable(GL_LIGHTING);
    glColor3d(1.0, 1.0, 1.0);
    glLineWidth(5);
    DrawLights(scene);
    glLineWidth(1);
  }

  // Draw rays
  if (show_rays) {
    glDisable(GL_LIGHTING);
    glColor3d(0.0, 1.0, 0.0);
    glLineWidth(3);
    DrawRays(scene);
    glLineWidth(1);
  }

  // Draw walls
  if (show_walls) {
    glDisable(GL_LIGHTING);
    glLineWidth(3);
    DrawPlanner5DWalls(scene);
    glLineWidth(1);
  }

  // Draw closest point
  if (show_closest) {
    glDisable(GL_LIGHTING);
    glColor3d(0.75, 0.0, 0.75);
    DrawClosest(scene);
  }

  // Draw scene nodes
  if (show_faces) {
    glEnable(GL_LIGHTING);
    scene->LoadLights(headlight);
    R3null_material.Draw();
    DrawShapes(scene);
    R3null_material.Draw();
  }

  // Draw scene nodes
  if (show_edges) {
    glDisable(GL_LIGHTING);
    R3null_material.Draw();
    glColor3d(0.0, 1.0, 0.0);
    R3null_material.Draw();
    DrawShapes(scene, R3_EDGES_DRAW_FLAG);
    R3null_material.Draw();
  }

  // Draw bboxes
  if (show_bboxes) {
    glDisable(GL_LIGHTING);
    glColor3d(1.0, 0.0, 0.0);
    DrawBBoxes(scene, scene->Root());
  }

  // Draw axes
  if (show_axes) {
    RNScalar d = scene->BBox().DiagonalRadius();
    glDisable(GL_LIGHTING);
    glLineWidth(3);
    R3BeginLine();
    glColor3f(1, 0, 0);
    R3LoadPoint(R3zero_point + 0.5 * d * R3negx_vector);
    R3LoadPoint(R3zero_point + d * R3posx_vector);
    R3EndLine();
    R3BeginLine();
    glColor3f(0, 1, 0);
    R3LoadPoint(R3zero_point + 0.5 * d * R3negy_vector);
    R3LoadPoint(R3zero_point + d * R3posy_vector);
    R3EndLine();
    R3BeginLine();
    glColor3f(0, 0, 1);
    R3LoadPoint(R3zero_point + 0.5 * d * R3negz_vector);
    R3LoadPoint(R3zero_point + d * R3posz_vector);
    R3EndLine();
    glLineWidth(1);
  }

  // Draw frame time
  if (show_frame_rate) {
    char buffer[128];
    static RNTime last_time;
    double frame_time = last_time.Elapsed();
    last_time.Read();
    if ((frame_time > 0) && (frame_time < 10)) {
      glDisable(GL_LIGHTING);
      glColor3d(1.0, 1.0, 1.0);
      sprintf(buffer, "%.1f fps", 1.0 / frame_time);
      DrawText(R2Point(100, 100), buffer);
    }
  }  

  // Draw debug
  if (show_debug) {
    glEnable(GL_LIGHTING);
    R3null_material.Draw();
    DrawDebug(scene, viewer, selected_node);
  }

  // Capture image and exit
  if (output_image_name) {
    R2Image image(GLUTwindow_width, GLUTwindow_height, 3);
    image.Capture();
    image.Write(output_image_name);
    GLUTStop();
  }

  // Swap buffers 
  glutSwapBuffers();
}    



void GLUTResize(int w, int h)
{
  // Resize window
  glViewport(0, 0, w, h);

  // Resize viewer viewport
  viewer->ResizeViewport(0, 0, w, h);

  // Resize scene viewport
  scene->SetViewport(viewer->Viewport());

  // Remember window size 
  GLUTwindow_width = w;
  GLUTwindow_height = h;

  // Redraw
  glutPostRedisplay();
}



void GLUTMotion(int x, int y)
{
  // Invert y coordinate
  y = GLUTwindow_height - y;

  // Compute mouse movement
  int dx = x - GLUTmouse[0];
  int dy = y - GLUTmouse[1];
  
  // Update mouse drag
  GLUTmouse_drag += dx*dx + dy*dy;

  // World in hand navigation 
  if (GLUTbutton[0]) viewer->RotateWorld(1.0, center, x, y, dx, dy);
  else if (GLUTbutton[1]) viewer->ScaleWorld(1.0, center, x, y, dx, dy);
  else if (GLUTbutton[2]) viewer->TranslateWorld(1.0, center, x, y, dx, dy);
  if (GLUTbutton[0] || GLUTbutton[1] || GLUTbutton[2]) glutPostRedisplay();

  // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;
}



void GLUTMouse(int button, int state, int x, int y)
{
  // Invert y coordinate
  y = GLUTwindow_height - y;

  // Mouse is going down
  if (state == GLUT_DOWN) {
    // Reset mouse drag
    GLUTmouse_drag = 0;

    // Process thumbwheel
    if (button == 3) viewer->ScaleWorld(center, 0.9);
    else if (button == 4) viewer->ScaleWorld(center, 1.1);
  }
  else if (button == 0) {
    // Check for double click  
    static RNBoolean double_click = FALSE;
    static RNTime last_mouse_up_time;
    double_click = (!double_click) && (last_mouse_up_time.Elapsed() < 0.4);
    last_mouse_up_time.Read();

    // Check for click (rather than drag)
    if (GLUTmouse_drag < 100) {
      // Check for double click
      if (double_click) {
        // Set viewing center point 
        R3Ray ray = viewer->WorldRay(x, y);
        R3Point intersection_point;
        if (scene->Intersects(ray, NULL, NULL, NULL, &intersection_point)) {
          center = intersection_point;
        }
      }
      else {
        // Select node 
        R3Point position;
        R3Vector normal;
        selected_node = NULL;;
        selected_element = NULL;
        R3Ray ray = viewer->WorldRay(x, y);
        if (scene->Intersects(ray, &selected_node, &selected_element, NULL, &position, &normal)) {
          const R3Material *material = selected_element->Material();
          const char *material_name = (material) ? material->Name() : "-";
          const R3Brdf *brdf = (material) ? material->Brdf() : NULL;
          const char *brdf_name = (brdf) ? brdf->Name() : "-";
          RNRgb diffuse = (brdf) ? brdf->Diffuse() : RNblack_rgb;
          RNRgb specular = (brdf) ? brdf->Specular() : RNblack_rgb;
          RNRgb transmission = (brdf) ? brdf->Transmission() : RNblack_rgb;
          RNRgb emission = (brdf) ? brdf->Emission() : RNblack_rgb;
          const R2Texture *texture = (material) ? material->Texture() : NULL;
          const char *texture_name = (texture) ? texture->Name() : "-";
          printf("Selected %s    %g %g %g    %g %g %g\n", (selected_node->Name()) ? selected_node->Name() : "NoName",
            position.X(), position.Y(), position.Z(), normal.X(), normal.Y(), normal.Z());
          printf("  Material %s : brdf %s  kd=(%g %g %g) ks=(%g %g %g) kt=(%g %g %g) ke=(%g %g %g) ns=%g : texture %s\n", material_name,
            brdf_name, diffuse.R(), diffuse.G(), diffuse.B(), specular.R(), specular.G(), specular.B(),
            transmission.R(), transmission.G(), transmission.B(), emission.R(), emission.G(), emission.B(),
            brdf->Shininess(), texture_name);
        }
      }
    }
  }

  // Remember button state 
  int b = (button == GLUT_LEFT_BUTTON) ? 0 : ((button == GLUT_MIDDLE_BUTTON) ? 1 : 2);
  GLUTbutton[b] = (state == GLUT_DOWN) ? 1 : 0;

  // Remember modifiers 
  GLUTmodifiers = glutGetModifiers();

   // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;

  // Redraw
  glutPostRedisplay();
}



void GLUTSpecial(int key, int x, int y)
{
  // Invert y coordinate
  y = GLUTwindow_height - y;

  // Process keyboard button event 

  // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;

  // Remember modifiers 
  GLUTmodifiers = glutGetModifiers();

  // Redraw
  glutPostRedisplay();
}



void GLUTKeyboard(unsigned char key, int x, int y)
{
  // Process keyboard button event 
  switch (key) {
  case 'A':
  case 'a':
    show_axes = !show_axes;
    break;

  case 'B':
  case 'b':
    show_backfacing = !show_backfacing;
    break;

  case 'C':
  case 'c':
    show_cameras = !show_cameras;
    break;

  case 'E':
  case 'e':
    show_edges = !show_edges;
    break;

  case 'F':
  case 'f':
    show_faces = !show_faces;
    break;

  case 'H':
  case 'h':
    show_bboxes = !show_bboxes;
    break;

  case 'L':
  case 'l':
    show_lights = !show_lights;
    break;

  case 'M':
  case 'm':
    show_materials = !show_materials;
    break;

  case 'P':
  case 'p':
    show_closest = !show_closest;
    break;

  case 'R':
  case 'r':
    show_rays = !show_rays;
    break;

  case 'T':
  case 't':
    show_frame_rate = !show_frame_rate;
    break;

  case 'W':
  case 'w':
    show_walls = !show_walls;
    break;

  case 'V':
  case 'v':
    // Set camera
    if (cameras && (cameras->NEntries() > 0)) {
      if (key == 'v') selected_camera_index++;
      else if (key == 'V') selected_camera_index--;
      if (selected_camera_index < 0) selected_camera_index = 0;
      if (selected_camera_index >= cameras->NEntries()) selected_camera_index = cameras->NEntries()-1;
      viewer->SetCamera(*(cameras->Kth(selected_camera_index)));
    }
    else {
      viewer->SetCamera(scene->Camera());
    }

    // Set center
    if (1) {
      R3Point hit_point;
      R3Ray ray = viewer->WorldRay(GLUTwindow_width/2, GLUTwindow_height/2);
      if (scene->Intersects(ray, NULL, NULL, NULL, &hit_point)) center = hit_point;
    }
    break;

  case ' ': {
    // Print camera
    const R3Camera& camera = viewer->Camera();
    printf("%g %g %g  %g %g %g  %g %g %g  %g %g  1\n",
           camera.Origin().X(), camera.Origin().Y(), camera.Origin().Z(),
           camera.Towards().X(), camera.Towards().Y(), camera.Towards().Z(),
           camera.Up().X(), camera.Up().Y(), camera.Up().Z(),
           camera.XFOV(), camera.YFOV());
    break; }
    
  case '@':
    show_debug = !show_debug;
    break;

  case 27: // ESCAPE
    GLUTStop();
    break;
  }

  // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = GLUTwindow_height - y;

  // Remember modifiers 
  GLUTmodifiers = glutGetModifiers();

  // Redraw
  glutPostRedisplay();  
}




void GLUTInit(int *argc, char **argv)
{
  // Open window 
  glutInit(argc, argv);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(GLUTwindow_width, GLUTwindow_height);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // | GLUT_STENCIL
  GLUTwindow = glutCreateWindow("Scene Viewer");

  // Initialize depth testing
  glEnable(GL_DEPTH_TEST);

  // Initialize lighting
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  static GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING); 

  // Initialize GLUT callback functions 
  glutDisplayFunc(GLUTRedraw);
  glutReshapeFunc(GLUTResize);
  glutKeyboardFunc(GLUTKeyboard);
  glutSpecialFunc(GLUTSpecial);
  glutMouseFunc(GLUTMouse);
  glutMotionFunc(GLUTMotion);
}



void GLUTMainLoop(void)
{
  // Just checking
  if (!viewer || !scene) return;
  
  // Initialize viewing center
  center = scene->BBox().Centroid();

  // Initialize camera
  // viewer->SetCamera(scene->Camera());

  // Remove transformations
  // scene->RemoveTransformations();
  
  // Subdivide triangles for lighting
  if (max_vertex_spacing > 0) {
    scene->SubdivideTriangles(max_vertex_spacing);
  }

  // Define headlight
  if (headlight) {
    static GLfloat light0_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    static GLfloat light0_position[] = { 0.0, 0.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glEnable(GL_LIGHT0);
  }
  
  // Run main loop -- never returns 
  glutMainLoop();
}


 
static R3Viewer *
CreateBirdsEyeViewer(const R3Scene *scene)
{
  // Setup camera view looking down the Z axis
  R3Box bbox = scene->BBox();
  assert(!bbox.IsEmpty());
  RNLength r = bbox.DiagonalRadius();
  assert((r > 0.0) && RNIsFinite(r));
  if (!initial_camera) initial_camera_origin = scene->Centroid() - initial_camera_towards * (2.5 * r);
  R3Camera camera(initial_camera_origin, initial_camera_towards, initial_camera_up, 0.4, 0.4, 0.01 * r, 100.0 * r);
  R2Viewport viewport(0, 0, GLUTwindow_width, GLUTwindow_height);
  return new R3Viewer(camera, viewport);
}



static R3Scene *
ReadScene(char *filename)
{
  // Start statistics
  RNTime start_time;
  start_time.Read();

  // Allocate scene
  R3Scene *scene = new R3Scene();
  if (!scene) {
    fprintf(stderr, "Unable to allocate scene for %s\n", filename);
    return NULL;
  }

  // Read scene from file
  if (!scene->ReadFile(filename)) {
    delete scene;
    return NULL;
  }

  // Print statistics
  if (print_verbose) {
    printf("Read scene from %s ...\n", filename);
    printf("  Time = %.2f seconds\n", start_time.Elapsed());
    printf("  # Nodes = %d\n", scene->NNodes());
    printf("  # Materials = %d\n", scene->NMaterials());
    printf("  # Brdfs = %d\n", scene->NBrdfs());
    printf("  # Textures = %d\n", scene->NTextures());
    printf("  # Lights = %d\n", scene->NLights());
    fflush(stdout);
  }

  // Return scene
  return scene;
}



static RNArray<R3Camera *> *
ReadCameras(const char *filename)
{
  // Start statistics
  RNTime start_time;
  start_time.Read();

  // Get useful variables
  RNScalar neardist = 0.01 * scene->BBox().DiagonalRadius();
  RNScalar fardist = 100 * scene->BBox().DiagonalRadius();

  // Allocate array of cameras
  RNArray<R3Camera *> *cameras = new RNArray<R3Camera *>();
  if (!cameras) {
    fprintf(stderr, "Unable to allocate cameras for %s\n", filename);
    return NULL;
  }

  // Open file
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "Unable to open cameras file %s\n", filename);
    delete cameras;
    return NULL;
  }

  // Read file
  RNScalar vx, vy, vz, tx, ty, tz, ux, uy, uz, xf, yf, value;
  while (fscanf(fp, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf", &vx, &vy, &vz, &tx, &ty, &tz, &ux, &uy, &uz, &xf, &yf, &value) == (unsigned int) 12) {
    R3Point viewpoint(vx, vy, vz);
    R3Vector towards(tx, ty, tz);
    R3Vector up(ux, uy, uz);
    R3Vector right = towards % up;
    towards.Normalize();
    up = right % towards;
    up.Normalize();
    R3Camera *camera = new R3Camera(viewpoint, towards, up, xf, yf, neardist, fardist);
    camera->SetValue(value);
    cameras->Insert(camera);
  }

  // Close file
  fclose(fp);

  // Print statistics
  if (print_verbose) {
    printf("Read cameras from %s ...\n", filename);
    printf("  Time = %.2f seconds\n", start_time.Elapsed());
    printf("  # Cameras = %d\n", cameras->NEntries());
    fflush(stdout);
  }

  // Return success
  return cameras;
}



static int
ReadLights(const char *filename)
{
  // Start statistics
  RNTime start_time;
  start_time.Read();

  // Read lights file
  if (!scene->ReadLightsFile(filename)) return 0;

  // Print statistics
  if (print_verbose) {
    printf("Read lights from %s ...\n", filename);
    printf("  Time = %.2f seconds\n", start_time.Elapsed());
    printf("  # Lights = %d\n", scene->NLights());
    fflush(stdout);
  }

  // Return success
  return 1;
}
  


static int 
ParseArgs(int argc, char **argv)
{
  // Parse arguments
  argc--; argv++;
  while (argc > 0) {
    if ((*argv)[0] == '-') {
      if (!strcmp(*argv, "-v")) print_verbose = 1;
      else if (!strcmp(*argv, "-hide_faces")) show_faces = 0;
      else if (!strcmp(*argv, "-show_faces")) show_faces = 1;
      else if (!strcmp(*argv, "-show_edges")) show_edges = 1;
      else if (!strcmp(*argv, "-show_cameras")) show_cameras = 1;
      else if (!strcmp(*argv, "-show_lights")) show_lights = 1;
      else if (!strcmp(*argv, "-show_materials")) show_materials = 1;
      else if (!strcmp(*argv, "-show_bboxes")) show_bboxes = 1;
      else if (!strcmp(*argv, "-show_axes")) show_axes = 1;
      else if (!strcmp(*argv, "-show_rays")) show_rays = 1;
      else if (!strcmp(*argv, "-show_backfaces")) show_backfacing = 1;
      else if (!strcmp(*argv, "-headlight")) headlight = 1;
      else if (!strcmp(*argv, "-max_vertex_spacing")) {
        argc--; argv++; max_vertex_spacing = atof(*argv);
      }
      else if (!strcmp(*argv, "-image")) {
        argc--; argv++; output_image_name = *argv;
      }
      else if (!strcmp(*argv, "-cameras")) {
        argv++; argc--; input_cameras_name = *argv;
      }
      else if (!strcmp(*argv, "-lights")) {
        argv++; argc--; input_lights_name = *argv;
      }
      else if (!strcmp(*argv, "-background")) {
        argv++; argc--; background_color[0] = atof(*argv);
        argv++; argc--; background_color[1] = atof(*argv);
        argv++; argc--; background_color[2] = atof(*argv);
      }
      else if (!strcmp(*argv, "-camera")) {
        RNCoord x, y, z, tx, ty, tz, ux, uy, uz;
        argv++; argc--; x = atof(*argv);
        argv++; argc--; y = atof(*argv);
        argv++; argc--; z = atof(*argv);
        argv++; argc--; tx = atof(*argv);
        argv++; argc--; ty = atof(*argv);
        argv++; argc--; tz = atof(*argv);
        argv++; argc--; ux = atof(*argv);
        argv++; argc--; uy = atof(*argv);
        argv++; argc--; uz = atof(*argv);
        initial_camera_origin = R3Point(x, y, z);
        initial_camera_towards.Reset(tx, ty, tz);
        initial_camera_up.Reset(ux, uy, uz);
        initial_camera = TRUE;
      }
      else { 
        fprintf(stderr, "Invalid program argument: %s", *argv); 
        exit(1); 
      }
      argv++; argc--;
    }
    else {
      if (!input_scene_name) input_scene_name = *argv;
      else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
      argv++; argc--;
    }
  }

  // Check scene filename
  if (!input_scene_name) {
    fprintf(stderr, "Usage: scnview inputscenefile\n");
    return 0;
  }

  // Return OK status 
  return 1;
}



int main(int argc, char **argv)
{
  // Parse program arguments
  if (!ParseArgs(argc, argv)) exit(-1);

  // Initialize GLUT
  GLUTInit(&argc, argv);

  // Read scene
  scene = ReadScene(input_scene_name);
  if (!scene) exit(-1);

  // Read cameras
  if (input_cameras_name) {
    cameras = ReadCameras(input_cameras_name);
    if (!cameras) exit(-1);
  }

  // Read/create lights
  if (input_lights_name) { if (!ReadLights(input_lights_name)) exit(-1); }
  else { scene->CreateDirectionalLights(); headlight = 1; }

  // Create viewer
  viewer = CreateBirdsEyeViewer(scene);
  if (!viewer) exit(-1);

  // Run GLUT interface
  GLUTMainLoop();

  // Return success 
  return 0;
}



