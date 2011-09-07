#include <maapi.h>
#include <mastdlib.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <MAUtil/GLMoblet.h>
#include <GLES/gl.h>

using namespace MAUtil;

/**
 * Moblet to be used as a template for an Open GL application.
 * The template program draws a rotating quad. Touch the
 * screen to change the depth coordinate.
 */
class MyGLMoblet : public GLMoblet
{
public:

	// ================== Constructor ==================

	/**
	 * Initialize rendering parameters.
	 */
	MyGLMoblet() :
		GLMoblet(GLMoblet::GL1),
		mDepth(5.0f),
		mStartTime(maGetMilliSecondCount())
	{
	}

	// ================== Event methods ==================

	/**
	 * Called when a fullscreen window with an OpenGL context
	 * has been created and is ready to be used.
	 */
	void init()
	{
		// Set the GL viewport to be the entire MoSync screen.
		setViewport(
			EXTENT_X(maGetScrSize()),
			EXTENT_Y(maGetScrSize()));

		// Initialize OpenGL.
		initGL();

		// Call draw 60 times per second.
		setPreferredFramesPerSecond(60);
	}

	/**
	 * Called when the application should render the model.
	 * Use method setPreferredFramesPerSecond to set the
	 * desired frame rate.
	 */
	void draw()
	{
		// Compute rotation.
		GLfloat rotation = (maGetMilliSecondCount() - mStartTime) * -0.05f;

		// Render scene.
		draw(mDepth, rotation);
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}

	/**
	 * Called on a touch down event.
	 */
	void pointerPressEvent(MAPoint2d point)
	{
		computeZoomFactor(point);
	}

	/**
	 * Called on a touch drag event.
	 */
	void pointerMoveEvent(MAPoint2d point)
	{
		computeZoomFactor(point);
	}

private:

	// ================== OpenGL/rendering methods ==================

	/**
	 * Standard OpenGL initialization.
	 */
	void initGL()
	{
		glShadeModel(GL_SMOOTH);
		glClearDepthf(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	}

	/**
	 * Setup the projection matrix.
	 */
	void setViewport(int width, int height)
	{
		// Protect against divide by zero.
		if (0 == height)
		{
			height = 1;
		}

		// Set viewport and perspective.
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		GLfloat ratio = (GLfloat)width / (GLfloat)height;
		gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	}

	/**
	 * Render the model (draws a quad).
	 */
	void draw(GLfloat z, GLfloat rotation)
	{
		// Define quad vertices.
	    GLfloat vertices[4][3];
	    // Top right.
	    vertices[0][0] = 1.0f; vertices[0][1] = 1.0f; vertices[0][2] = 0.0f;
	    // Top left.
	    vertices[1][0] = -1.0f; vertices[1][1] = 1.0f; vertices[1][2] = 0.0f;
	    // Bottom left.
	    vertices[2][0] = 1.0f; vertices[2][1] = -1.0f; vertices[2][2] = 0.0f;
	    // Bottom right.
	    vertices[3][0] = -1.0f; vertices[3][1] = -1.0f; vertices[3][2] = 0.0f;

	    // Draw quad.
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(rotation, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, 0.0f, -z);
		glClearColor(0.8, 0.9, 0.6, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4f(0.9, 0.0, 0.0, 1.0);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
		glFinish();
	}

	/**
	 * Compute the visible size of the quad by
	 * setting the depth coordinate.
	 */
	void computeZoomFactor(MAPoint2d point)
	{
		float screenHeight = EXTENT_Y(maGetScrSize());
		mDepth = 20.0f / screenHeight * point.y;
	}

	/**
	 * Standard OpenGL utility function for setting up the
	 * perspective projection matrix.
	 */
	void gluPerspective(
		GLfloat fovy,
		GLfloat aspect,
		GLfloat zNear,
		GLfloat zFar)
	{
		const float M_PI = 3.14159;

		GLfloat ymax = zNear * tan(fovy * M_PI / 360.0);
		GLfloat ymin = -ymax;
		GLfloat xmin = ymin * aspect;
		GLfloat xmax = ymax * aspect;

		glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
	}

private:

	// ================== Instance variables ==================

	/** Time stamp used to calculate rotation. */
	GLfloat mStartTime;

	/** Z-coordinate. */
	GLfloat mDepth;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new MyGLMoblet());
	return 0;
}
