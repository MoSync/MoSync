package com.mosync.nativeui.ui.egl;

import com.mosync.internal.generated.IX_OPENGL_ES;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;

public class EGLConfigFactory
{
    /**
     * An interface for choosing an EGLConfig configuration from a list of
     * potential configurations.
     * <p>
     * This interface must be implemented by clients wishing to call
     * {@link GLSurfaceView#setEGLConfigChooser(EGLConfigChooser)}
     */
    public interface EGLConfigChooser {
        /**
         * Choose a configuration from the list. Implementors typically
         * implement this method by calling
         * {@link EGL10#eglChooseConfig} and iterating through the results. Please consult the
         * EGL specification available from The Khronos Group to learn how to call eglChooseConfig.
         * @param egl the EGL10 for the current display.
         * @param display the current display.
         * @return the chosen configuration.
         */
        EGLConfig chooseConfig(EGL10 egl, EGLDisplay display);
    }

    private static abstract class BaseConfigChooser
            implements EGLConfigChooser {
        public BaseConfigChooser(int[] configSpec) {
            mConfigSpec = configSpec;
        }
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
            int[] num_config = new int[1];
            egl.eglChooseConfig(display, mConfigSpec, null, 0, num_config);

            int numConfigs = num_config[0];

            if (numConfigs <= 0) {
                throw new IllegalArgumentException(
                        "No configs match configSpec");
            }

            EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, mConfigSpec, configs, numConfigs,
                    num_config);
            EGLConfig config = chooseConfig(egl, display, configs);
            if (config == null) {
                throw new IllegalArgumentException("No config chosen");
            }
            return config;
        }

        abstract EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
                EGLConfig[] configs);

        protected int[] mConfigSpec;
    }

    private static class ComponentSizeChooser extends BaseConfigChooser {
        public ComponentSizeChooser(int redSize, int greenSize, int blueSize,
                int alphaSize, int depthSize, int stencilSize, int glApi) {

            super(null);

            if(glApi == IX_OPENGL_ES.MA_GL_API_GL1) {
				glApi = 0;
				mConfigSpec = new int[] {
					EGL10.EGL_RED_SIZE, redSize,
					EGL10.EGL_GREEN_SIZE, greenSize,
					EGL10.EGL_BLUE_SIZE, blueSize,
					EGL10.EGL_ALPHA_SIZE, alphaSize,
					EGL10.EGL_DEPTH_SIZE, depthSize,
					EGL10.EGL_STENCIL_SIZE, stencilSize,
					EGL10.EGL_NONE};
            } else if(glApi == IX_OPENGL_ES.MA_GL_API_GL2) {
				int EGL_OPENGL_ES2_BIT = 4;
				glApi = 4;
				mConfigSpec = new int[] {
					EGL10.EGL_RED_SIZE, redSize,
					EGL10.EGL_GREEN_SIZE, greenSize,
					EGL10.EGL_BLUE_SIZE, blueSize,
					EGL10.EGL_ALPHA_SIZE, alphaSize,
					EGL10.EGL_DEPTH_SIZE, depthSize,
					EGL10.EGL_STENCIL_SIZE, stencilSize,
					EGL10.EGL_RENDERABLE_TYPE, glApi,
					EGL10.EGL_NONE};
			}

			mGLApi = glApi;
			mValue = new int[1];
			mRedSize = redSize;
			mGreenSize = greenSize;
			mBlueSize = blueSize;
			mAlphaSize = alphaSize;
            mDepthSize = depthSize;
			mStencilSize = stencilSize;
		}

        @Override
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
                EGLConfig[] configs) {
            EGLConfig closestConfig = null;
            int closestDistance = 1000;
            for(EGLConfig config : configs) {
                int r = findConfigAttrib(egl, display, config,
                        EGL10.EGL_RED_SIZE, 0);
                int g = findConfigAttrib(egl, display, config,
                         EGL10.EGL_GREEN_SIZE, 0);
                int b = findConfigAttrib(egl, display, config,
                          EGL10.EGL_BLUE_SIZE, 0);
                int a = findConfigAttrib(egl, display, config,
                        EGL10.EGL_ALPHA_SIZE, 0);
                int d = findConfigAttrib(egl, display, config,
                        EGL10.EGL_DEPTH_SIZE, 0);
                int s = findConfigAttrib(egl, display, config,
                        EGL10.EGL_STENCIL_SIZE, 0);
               // int api = findConfigAttrib(egl, display, config,
               //         EGL10.EGL_RENDERABLE_TYPE, 0);

                int distance = Math.abs(r - mRedSize)
                    + Math.abs(g - mGreenSize)
                    + Math.abs(b - mBlueSize) + Math.abs(a - mAlphaSize)
                    + Math.abs(d - mDepthSize) + Math.abs(s - mStencilSize)
                 ;//   + Math.abs(api - mGLApi);

                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestConfig = config;
                }
            }
            return closestConfig;
        }

        private int findConfigAttrib(EGL10 egl, EGLDisplay display,
                EGLConfig config, int attribute, int defaultValue) {

            if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
                return mValue[0];
            }
            return defaultValue;
        }

        private int[] mValue;
        // Subclasses can adjust these values:

        protected int mRedSize;
        protected int mGreenSize;
        protected int mBlueSize;
        protected int mAlphaSize;
        protected int mDepthSize;
        protected int mStencilSize;
        protected int mGLApi;
        }

    /**
     * This class will choose the ARGB8888 config.
     */
    public static class SimpleEGLConfigChooser extends ComponentSizeChooser {
        public SimpleEGLConfigChooser(boolean withDepthBuffer, int glApi) {
            super(8, 8, 8, 8, withDepthBuffer ? 16 : 0, 0, glApi);

            mRedSize = 8;
            mGreenSize = 8;
            mBlueSize = 8;
        }
    }

    public static EGLConfig findConfig(EGL10 egl, EGLDisplay display, int glApi)
    {
		return new SimpleEGLConfigChooser( true, glApi ).chooseConfig( egl, display);
    }
}
