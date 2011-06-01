#include <maapi.h>
#include <mastdlib.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <GLES/gl.h>
#include <IX_WIDGET.h>

using namespace MAUtil;

/**
 * Moblet to be used as a template for an Open GL application.
 * The template program draws a rotating quad. Touch the
 * screen to change the depth coordinate.
 */
class GLMoblet :
	public Moblet,
	public TimerListener
{
public:

	// ================== Constructor ==================

	/**
	 * The user interface is set up in the constructor.
	 */
	GLMoblet() :
		mGLViewInitialized(false),
		mRotation(0.0f),
		mDepth(5.0f)
	{
		// Create a NativeUI screen that will hold the GL view.
		int screen = maWidgetCreate(MAW_SCREEN);

		// Error handling for devices that do not support NativeUI.
		if (-1 == screen)
		{
			maPanic(0,
				"OpenGL is only available on Android and iPhone. "
				"You must run directly on the device or devices emulator.");
		}

		// Create a GL_VIEW widget and add it to the screen.
		mGLView = maWidgetCreate(MAW_GL_VIEW);
		widgetSetPropertyInt(
			mGLView,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			mGLView,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		maWidgetAddChild(screen, mGLView);

		// Show the screen.
		maWidgetScreenShow(screen);

		// Make the Moblet listen to custom events, so that we
		// know when the GLView widget is ready to be drawn.
		Environment::getEnvironment().addCustomEventListener(this);
	}

	// ================== Event methods ==================

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

	/**
	 * Method that implements the custom event listener interface.
	 * Widget events are sent as custom events.
	 */
	void customEvent(const MAEvent& event)
	{
		if (EVENT_TYPE_WIDGET == event.type)
		{
			// Get the widget event data structure.
			MAWidgetEventData* eventData = (MAWidgetEventData*) event.data;

			// MAW_EVENT_GL_VIEW_READY is sent when the GL view is
			// ready for drawing.
			if (MAW_EVENT_GL_VIEW_READY == eventData->eventType)
			{
				// Associate the OpenGL context with the GLView.
				maWidgetSetProperty(mGLView, MAW_GL_VIEW_BIND, "");

				// Set the GL viewport.
				int viewWidth = widgetGetPropertyInt(mGLView, MAW_WIDGET_WIDTH);
				int viewHeight = widgetGetPropertyInt(mGLView, MAW_WIDGET_HEIGHT);
				setViewport(viewWidth, viewHeight);

				// Initialize OpenGL.
				initGL();

				// The GLView has been initialized.
				mGLViewInitialized = true;

				// Draw the initial scene.
				draw(mDepth, mRotation);

				// Start timer that will redraw the scene.
				// This calls runTimerEvent each 20 ms.
				Environment::getEnvironment().addTimer(this, 20, -1);
			}
		}
	}

	/**
	 * Called on a timer event.
	 */
	void runTimerEvent()
	{
		draw(mDepth, mRotation);
		mRotation += 5.0f;
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
	 * Compute the visible size of the quad by
	 * setting the depth coordinate.
	 */
	void computeZoomFactor(MAPoint2d point)
	{
		float screenHeight = EXTENT_Y(maGetScrSize());
		mDepth = 20.0f / screenHeight * point.y;
		// Note: The quad gets redrawn by the timer.
	}

	/**
	 * Render the model (draws a quad).
	 */
	void draw(GLfloat z, GLfloat rotation)
	{
		// The GL_View must be initialized before we can do any drawing.
		if (!mGLViewInitialized)
		{
			return;
		}

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

		// Update the GLView.
	    maWidgetSetProperty(mGLView, MAW_GL_VIEW_INVALIDATE, "");
	}

	// ================== Helper methods ==================

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

	/**
	 * Helper method for setting a widget property interger value.
	 */
	int widgetSetPropertyInt(MAHandle handle, const char *property, int value)
	{
		char buffer[256];
		sprintf(buffer, "%i", value);
		maWidgetSetProperty(handle, property, buffer);
	}

	/**
	 * Helper method for getting a widget property integer value.
	 */
	int widgetGetPropertyInt(MAHandle handle, const char *property)
	{
		char buffer[256];
		maWidgetGetProperty( handle, property, buffer, 256);
		return atoi(buffer);
	}

private:

	// ================== Instance variables ==================

	/** Handle to the GLView widget. */
	MAHandle mGLView;

	/** GLView state (true = initialized and ready to be drawn). */
	bool mGLViewInitialized;

	/** Rotation. */
	GLfloat mRotation;

	/** Z-coordinate. */
	GLfloat mDepth;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new GLMoblet());
	return 0;
}
