#include <MAUtil/Moblet.h>

#include <maapi.h>
#include <conprint.h>

#define SENSOR_TYPES                    6
#define SENSOR_VALUES                   3

#define BUFFER_SIZE                     50
#define BG_COLOR                        0
#define TEXT_COLOR                      0xFF0000
#define OFFSET_Y                        20

#define TXT_SENSOR_NONE                 "None"
#define TXT_SENSOR_ACCELEROMETER        "Accelerometer"
#define TXT_SENSOR_MAG_FIELD            "Magnetic field"
#define TXT_SENSOR_ORIENTATION          "Orientation"
#define TXT_SENSOR_GYROSCOPE            "Gyroscope"
#define TXT_SENSOR_PROXIMITY            "Proximity"

#define TXT_AVAILABLE                   "Available"
#define TXT_NOT_AVAILABLE               "Not available"
#define TXT_INTERVAL                    "Interval not set"
#define TXT_ENABLED                     "Already enabled"

#define TXT_UNKNOWN                     "Unknown"
#define TXT_PORTRAIT                    "Portrait"
#define TXT_UPSIDE_DOWN                 "Portrait upside down"
#define TXT_LANDSCAPE_LEFT              "Landscape left"
#define TXT_LANDSCAPE_RIGHT             "Landscape right"
#define TXT_FACE_UP                     "Face up"
#define TXT_FACE_DOWN                   "Face down"

    char* getErrorText(int errorCode)
    {
        switch (errorCode)
        {
            case SENSOR_ERROR_NOT_AVAILABLE:
                return TXT_NOT_AVAILABLE;
                break;
            case SENSOR_ERROR_INTERVAL_NOT_SET:
                return TXT_INTERVAL;
                break;
            case SENSOR_ERROR_ALREADY_ENABLED:
                return TXT_ENABLED;
                break;
            default:
                return TXT_AVAILABLE;
                break;
        }
    }

    char* getOrientationText(int orientation)
    {
        switch (orientation)
        {
            case UIDEVICE_ORIENTATION_UNKNOWN:
                return TXT_UNKNOWN;
            case UIDEVICE_ORIENTATION_PORTRAIT:
                return TXT_PORTRAIT;
            case UIDEVICE_ORIENTATION_PORTRAIT_UPSIDE_DOWN:
                return TXT_UPSIDE_DOWN;
            case UIDEVICE_ORIENTATION_LANDSCAPE_LEFT:
                return TXT_LANDSCAPE_LEFT;
            case UIDEVICE_ORIENTATION_LANDSCAPE_RIGHT:
                return TXT_LANDSCAPE_RIGHT;
            case UIDEVICE_ORIENTATION_FACE_UP:
                return TXT_FACE_UP;
            case UIDEVICE_ORIENTATION_FACE_DOWN:
                return TXT_FACE_DOWN;
        }
    }

extern "C" int MAMain()
{
    int mSensorList[SENSOR_TYPES];
    float mSensorValues[SENSOR_TYPES][SENSOR_VALUES];
    const char *mSensorNames[SENSOR_TYPES] =
    {
        TXT_SENSOR_NONE,
        TXT_SENSOR_ACCELEROMETER,
        TXT_SENSOR_MAG_FIELD,
        TXT_SENSOR_ORIENTATION,
        TXT_SENSOR_GYROSCOPE,
        TXT_SENSOR_PROXIMITY
    };

    for (int type=0; type<SENSOR_TYPES; type++)
    {
        mSensorList[type] = maSensorStart(type, SENSOR_RATE_NORMAL);
    }

    while (true)
    {
        MAEvent event;
        while(maGetEvent(&event))
        {
            char buffer[BUFFER_SIZE];
            maSetColor(BG_COLOR);
            maFillRect(0, 0, 10000, 10000);
            if(event.type == EVENT_TYPE_SENSOR)
            {
                memcpy( mSensorValues[event.sensor.type], event.sensor.values, SENSOR_VALUES * sizeof(float) );
            }
            maSetColor(TEXT_COLOR);
            int startY = 0;
            for (int i=0; i<SENSOR_TYPES; i++)
            {
                sprintf(buffer, "%s: %s", mSensorNames[i], getErrorText(mSensorList[i]));
                maDrawText(0, startY, buffer);
                startY += OFFSET_Y;
                if (mSensorList[i] != 0)
                    continue;
                switch (i)
                {
                    case SENSOR_TYPE_ACCELEROMETER:
                    case SENSOR_TYPE_MAGNETIC_FIELD:
                    case SENSOR_TYPE_GYROSCOPE:
                        sprintf(buffer, "Values: %f; %f; %f", mSensorValues[i][0], mSensorValues[i][1], mSensorValues[i][2]);
                        break;
                    case SENSOR_TYPE_ORIENTATION:
                        sprintf(buffer, "Value: %s", getOrientationText((int)mSensorValues[i][0]));
                        break;
                    case SENSOR_TYPE_PROXIMITY:
                        if (mSensorValues[i][0] == SENSOR_PROXIMITY_VALUE_FAR)
                            sprintf(buffer, "Value: FAR");
                        else
                            sprintf(buffer, "Value: NEAR");
                        break;
                }
                maDrawText(0, startY, buffer);
                startY += OFFSET_Y;
            }

            maUpdateScreen();
        }
    }

    for (int type=0; type<SENSOR_TYPES; type++)
    {
        mSensorList[type] = maSensorStop(type);
    }
};
