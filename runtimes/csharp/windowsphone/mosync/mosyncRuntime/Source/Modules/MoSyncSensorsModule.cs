using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;
using System.Collections.Generic;
using Microsoft.Devices.Sensors;
using System.Device.Location;

namespace MoSync
{
    public class SensorsModule : IIoctlModule
    {
        private Accelerometer mAccelerometer = null;
        private Compass mCompass = null;
        private Gyroscope mGyroscope = null;
        private Motion mMotion = null;
		private GeoCoordinateWatcher mGeoWatcher = null;

        private void SendSensorEventVector(Runtime runtime, int type, Vector3 data)
        {
            Memory eventData = new Memory(4 * 4);
            eventData.WriteInt32(MoSync.Struct.MASensor.type, type);
			eventData.WriteInt32(MoSync.Struct.MASensor.values + 0, MoSync.Util.ConvertToInt(data.X));
			eventData.WriteInt32(MoSync.Struct.MASensor.values + 4, MoSync.Util.ConvertToInt(data.Y));
			eventData.WriteInt32(MoSync.Struct.MASensor.values + 8, MoSync.Util.ConvertToInt(data.Z));
            runtime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_SENSOR, eventData);
        }

		private void SendGeoEvent(Runtime runtime)
		{
		}

		private int GetSensorIntervalDefaults(int interval)
		{
			switch(interval)
			{
				case MoSync.Constants.SENSOR_RATE_FASTEST: return 50;
				case MoSync.Constants.SENSOR_RATE_GAME: return 80;
				case MoSync.Constants.SENSOR_RATE_NORMAL: return 140;
				case MoSync.Constants.SENSOR_RATE_UI: return 160;
				default: return interval;
			}

		}

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {

            ioctls.maSensorStart = delegate(int _sensor, int _interval)
            {
				_interval = GetSensorIntervalDefaults(_interval);

				TimeSpan time = TimeSpan.FromMilliseconds((double)_interval);

                if (_sensor == MoSync.Constants.SENSOR_TYPE_ACCELEROMETER &&
                    Accelerometer.IsSupported)
                {
                    mAccelerometer = new Accelerometer();
					mAccelerometer.TimeBetweenUpdates = time;
                    mAccelerometer.CurrentValueChanged +=
                        delegate(object sender, SensorReadingEventArgs<AccelerometerReading> args)
                        {
                            Vector3 acc = args.SensorReading.Acceleration;
                            SendSensorEventVector(runtime, MoSync.Constants.SENSOR_TYPE_ACCELEROMETER, acc);
                        };

					mAccelerometer.Start();
                }
                else if (_sensor == MoSync.Constants.SENSOR_TYPE_GYROSCOPE &&
                    Gyroscope.IsSupported)
                {
                    mGyroscope = new Gyroscope();
					mGyroscope.TimeBetweenUpdates = time;
                    mGyroscope.CurrentValueChanged +=
                        delegate(object sender, SensorReadingEventArgs<GyroscopeReading> args)
                        {
                            Vector3 rot = args.SensorReading.RotationRate;
                            SendSensorEventVector(runtime, MoSync.Constants.SENSOR_TYPE_GYROSCOPE, rot);
                        };

					mGyroscope.Start();
                }
                else if (_sensor == MoSync.Constants.SENSOR_TYPE_MAGNETIC_FIELD &&
                    Compass.IsSupported)
                {
                    mCompass = new Compass();
					mCompass.TimeBetweenUpdates = time;
                    mCompass.CurrentValueChanged +=
                        delegate(object sender, SensorReadingEventArgs<CompassReading> args)
                        {
                            Vector3 rot = args.SensorReading.MagnetometerReading;
                            SendSensorEventVector(runtime, MoSync.Constants.SENSOR_TYPE_MAGNETIC_FIELD, rot);
                        };

					mCompass.Start();
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
				if (mGeoWatcher == null)
				{
					mGeoWatcher = new GeoCoordinateWatcher();
					mGeoWatcher.MovementThreshold = 20;

					mGeoWatcher.StatusChanged += delegate(object sender,
						GeoPositionStatusChangedEventArgs args)
					{

					};

					mGeoWatcher.PositionChanged += delegate(object sender,
						GeoPositionChangedEventArgs<GeoCoordinate> args)
					{


					};

					mGeoWatcher.Start();
				}

                return 0;
            };

            ioctls.maLocationStop = delegate()
            {
				if (mGeoWatcher != null)
				{
					mGeoWatcher.Stop();
					mGeoWatcher = null;
				}

                return 0;
            };
        }
    }
}
