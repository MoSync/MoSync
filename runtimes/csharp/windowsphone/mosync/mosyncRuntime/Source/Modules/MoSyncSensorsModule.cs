using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;
using System.Collections.Generic;
using Microsoft.Devices.Sensors;

namespace MoSync
{
    public class SensorsModule : IIoctlModule
    {
        private Accelerometer mAccelerometer = null;
        private Compass mCompass = null;
        private Gyroscope mGyroscope = null;
        private Motion mMotion = null;

        private void SendSensorEventVector(Runtime runtime, int type, Vector3 data)
        {
            Memory eventData = new Memory(4 * 4);
            eventData.WriteInt32(MoSync.Struct.MASensor.type, type);
			eventData.WriteInt32(MoSync.Struct.MASensor.values + 0, MoSync.Util.ConvertToInt(data.X));
			eventData.WriteInt32(MoSync.Struct.MASensor.values + 4, MoSync.Util.ConvertToInt(data.Y));
			eventData.WriteInt32(MoSync.Struct.MASensor.values + 8, MoSync.Util.ConvertToInt(data.Z));
            runtime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_SENSOR, eventData);
        }

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {

            ioctls.maSensorStart = delegate(int _sensor, int _interval)
            {
                long intervalIn100Nanoseconds = (long)_interval * 10000;
                if (_sensor == MoSync.Constants.SENSOR_TYPE_ACCELEROMETER &&
                    Accelerometer.IsSupported)
                {
                    mAccelerometer = new Accelerometer();
                    mAccelerometer.TimeBetweenUpdates = new TimeSpan(intervalIn100Nanoseconds);
                    mAccelerometer.CurrentValueChanged +=
                        delegate(object sender, SensorReadingEventArgs<AccelerometerReading> args)
                        {
                            Vector3 acc = args.SensorReading.Acceleration;
                            SendSensorEventVector(runtime, MoSync.Constants.SENSOR_TYPE_ACCELEROMETER, acc);
                        };
                }
                else if (_sensor == MoSync.Constants.SENSOR_TYPE_GYROSCOPE &&
                    Gyroscope.IsSupported)
                {
                    mGyroscope = new Gyroscope();
                    mGyroscope.TimeBetweenUpdates = new TimeSpan(intervalIn100Nanoseconds);
                    mGyroscope.CurrentValueChanged +=
                        delegate(object sender, SensorReadingEventArgs<GyroscopeReading> args)
                        {
                            Vector3 rot = args.SensorReading.RotationRate;
                            SendSensorEventVector(runtime, MoSync.Constants.SENSOR_TYPE_GYROSCOPE, rot);
                        };
                }
                else if (_sensor == MoSync.Constants.SENSOR_TYPE_MAGNETIC_FIELD &&
                    Compass.IsSupported)
                {
                    mCompass = new Compass();
                    mCompass.TimeBetweenUpdates = new TimeSpan(intervalIn100Nanoseconds);
                    mCompass.CurrentValueChanged +=
                        delegate(object sender, SensorReadingEventArgs<CompassReading> args)
                        {
                            Vector3 rot = args.SensorReading.MagnetometerReading;
                            SendSensorEventVector(runtime, MoSync.Constants.SENSOR_TYPE_MAGNETIC_FIELD, rot);
                        };
                }
#if false
                else if (_sensor == MoSync.Constants.SENSOR_TYPE_ORIENTATION &&
                    Motion.IsSupported)
                {
                    mMotion = new Motion();
                    mMotion.TimeBetweenUpdates = new TimeSpan(intervalIn100Nanoseconds);
                    mMotion.CurrentValueChanged +=
                               delegate(object sender, SensorReadingEventArgs<MotionReading> args)
                               {
                               };
                }
#endif
                else
                    return MoSync.Constants.SENSOR_ERROR_NOT_AVAILABLE;

                return MoSync.Constants.SENSOR_ERROR_NONE;
            };

            ioctls.maSensorStop = delegate(int _sensor)
            {
                switch (_sensor)
                {
                    case MoSync.Constants.SENSOR_TYPE_ACCELEROMETER:
                        if (mAccelerometer != null)
                        {
                            mAccelerometer.Stop();
                            mAccelerometer = null;
                        }
                        break;
                    case MoSync.Constants.SENSOR_TYPE_GYROSCOPE:
                        if (mGyroscope != null)
                        {
                            mGyroscope.Stop();
                            mGyroscope = null;
                        }
                        break;
                    case MoSync.Constants.SENSOR_TYPE_MAGNETIC_FIELD:
                        if (mCompass != null)
                        {
                            mCompass.Stop();
                            mCompass = null;
                        }
                        break;
                    case MoSync.Constants.SENSOR_TYPE_ORIENTATION:
                        if (mMotion != null)
                        {
                            mMotion.Stop();
                            mMotion = null;
                        }
                        break;
                }
                return MoSync.Constants.SENSOR_ERROR_NONE;
            };

            ioctls.maLocationStart = delegate()
            {

                return 0;
            };

            ioctls.maLocationStop = delegate()
            {
                return 0;
            };
        }
    }
}
