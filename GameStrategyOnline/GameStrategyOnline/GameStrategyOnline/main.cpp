#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    /* tell OpenGL we want to display in a recangle that is the
     same size as the window*/
    glViewport(0,0,width ,height);
    
    /* switch to the projection matrix */
    glMatrixMode(GL_PROJECTION);
    
    /* clear the projection matrix */
    glLoadIdentity();
    
    /* set the camera view, orthographic projection in 2D */
    gluOrtho2D(0,width,0,height);
    
    /* switch back to the model view matrix */
    glMatrixMode(GL_MODELVIEW);
}

void StartGlut()
{
    int argc = 0;
    char** argv = new(char*);
    glutInit(&argc, argv);
    
    /* set the window size to 512 x 512 */
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(300, 300);
    
    /* set the display mode to Red, Green, Blue and Alpha
     allocate a depth buffer
     enable double buffering
     */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    /* create the window (and call it Lab 1) */
    glutCreateWindow("Game");
    
    /* set the glut display callback function
     this is the function GLUT will call every time
     the window needs to be drawn
     */
    glutDisplayFunc(display);
    
    /* set the glut reshape callback function
     this is the function GLUT will call whenever
     the window is resized, including when it is
     first created
     */
    glutReshapeFunc(reshape);
    
    /* set the default background color to black */
    glClearColor(1,1,1,1);
    
    /* enter the main event loop so that GLUT can process
     all of the window event messages
     */
    glutMainLoop();
}

int main()
{
    StartGlut();
    return 0;
}
