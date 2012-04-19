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

		private bool mCompassEnabled = false;
		private bool mMagneticFieldEnabled = false;

		private void SendSensorEventVector(Runtime runtime, int type, Vector3 data)
		{
			Memory evt = new Memory(5 * 4);
			evt.WriteInt32(MoSync.Struct.MAEvent.type, MoSync.Constants.EVENT_TYPE_SENSOR);
			evt.WriteInt32(MoSync.Struct.MAEvent.sensor.type, type);
			evt.WriteInt32(MoSync.Struct.MAEvent.sensor.values + 0, MoSync.Util.ConvertToInt(data.X));
			evt.WriteInt32(MoSync.Struct.MAEvent.sensor.values + 4, MoSync.Util.ConvertToInt(data.Y));
			evt.WriteInt32(MoSync.Struct.MAEvent.sensor.values + 8, MoSync.Util.ConvertToInt(data.Z));
			runtime.PostEvent(new Event(evt));
		}

		private void SendGeoEvent(Runtime runtime)
		{
		}

		private int GetSensorIntervalDefaults(int interval)
		{
			switch (interval)
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
					if (mAccelerometer != null)
						return MoSync.Constants.SENSOR_ERROR_ALREADY_ENABLED;

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
					if (mGyroscope != null)
						return MoSync.Constants.SENSOR_ERROR_ALREADY_ENABLED;

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
				else if ((_sensor == MoSync.Constants.SENSOR_TYPE_MAGNETIC_FIELD || _sensor == MoSync.Constants.SENSOR_TYPE_COMPASS) &&
						Compass.IsSupported)
				{
					if (_sensor == MoSync.Constants.SENSOR_TYPE_MAGNETIC_FIELD &&
						mMagneticFieldEnabled == true)
						return MoSync.Constants.SENSOR_ERROR_ALREADY_ENABLED;

					if (_sensor == MoSync.Constants.SENSOR_TYPE_COMPASS &&
						mCompassEnabled == true)
						return MoSync.Constants.SENSOR_ERROR_ALREADY_ENABLED;

					if (mCompass == null)
					{
						mCompass = new Compass();
						mCompass.TimeBetweenUpdates = time;
					}
					else
					{
						if(time < mCompass.TimeBetweenUpdates)
							mCompass.TimeBetweenUpdates = time;
					}

					if (mCompassEnabled == false && mMagneticFieldEnabled == false)
					{
						mCompass.CurrentValueChanged +=
							delegate(object sender, SensorReadingEventArgs<CompassReading> args)
							{
								if (mMagneticFieldEnabled)
								{
									Vector3 rot = args.SensorReading.MagnetometerReading;
									SendSensorEventVector(runtime, MoSync.Constants.SENSOR_TYPE_MAGNETIC_FIELD, rot);
								}

								if (mCompassEnabled)
								{
									Vector3 heading = new Vector3();
									heading.X = (float)args.SensorReading.MagneticHeading;
									SendSensorEventVector(runtime, MoSync.Constants.SENSOR_TYPE_COMPASS, heading);
								}
							};

						mCompass.Start();
					}

					if (_sensor == MoSync.Constants.SENSOR_TYPE_MAGNETIC_FIELD)
						mMagneticFieldEnabled = true;
					else if (_sensor == MoSync.Constants.SENSOR_TYPE_COMPASS)
						mCompassEnabled = true;
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
						else
						{
							return MoSync.Constants.SENSOR_ERROR_NOT_ENABLED;
						}
						break;
					case MoSync.Constants.SENSOR_TYPE_GYROSCOPE:
						if (mGyroscope != null)
						{
							mGyroscope.Stop();
							mGyroscope = null;
						}
						else
						{
							return MoSync.Constants.SENSOR_ERROR_NOT_ENABLED;
						}
						break;
					case MoSync.Constants.SENSOR_TYPE_MAGNETIC_FIELD:
						if(!mMagneticFieldEnabled)
							return MoSync.Constants.SENSOR_ERROR_NOT_ENABLED;

						if (mCompass != null && !mCompassEnabled)
						{
							mCompass.Stop();
							mCompass = null;
						}

						mMagneticFieldEnabled = false;
						break;
					case MoSync.Constants.SENSOR_TYPE_COMPASS:
						if (!mCompassEnabled)
							return MoSync.Constants.SENSOR_ERROR_NOT_ENABLED;

						if (mCompass != null && !mMagneticFieldEnabled)
						{
							mCompass.Stop();
							mCompass = null;
						}
						mCompassEnabled = false;
						break;
					case MoSync.Constants.SENSOR_TYPE_ORIENTATION:
						if (mMotion != null)
						{
							mMotion.Stop();
							mMotion = null;
						}
						else
						{
							return MoSync.Constants.SENSOR_ERROR_NOT_ENABLED;
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
					//mGeoWatcher.MovementThreshold = 20;

					mGeoWatcher.StatusChanged += delegate(object sender,
						GeoPositionStatusChangedEventArgs args)
					{
						int maState;
						switch (args.Status)
						{
							case GeoPositionStatus.Disabled:
								maState = MoSync.Constants.MA_LPS_OUT_OF_SERVICE;
								break;
							case GeoPositionStatus.NoData:
							case GeoPositionStatus.Initializing:
								maState = MoSync.Constants.MA_LPS_TEMPORARILY_UNAVAILABLE;
								break;
							case GeoPositionStatus.Ready:
								maState = MoSync.Constants.MA_LPS_AVAILABLE;
								break;
							default:
								throw new Exception("invalid GeoPositionStatus");
						}
						Memory evt = new Memory(2 * 4);
						evt.WriteInt32(MoSync.Struct.MAEvent.type, MoSync.Constants.EVENT_TYPE_LOCATION_PROVIDER);
						evt.WriteInt32(MoSync.Struct.MAEvent.state, maState);
						runtime.PostEvent(new Event(evt));
					};

					mGeoWatcher.PositionChanged += delegate(object sender,
						GeoPositionChangedEventArgs<GeoCoordinate> args)
					{
						int maValidity = args.Position.Location.IsUnknown ?
							MoSync.Constants.MA_LOC_INVALID : MoSync.Constants.MA_LOC_QUALIFIED;
						Memory evt = new Memory(4 + 4 * 8 + 4);
						GeoCoordinate l = args.Position.Location;
						evt.WriteInt32(MoSync.Struct.MALocation.state, maValidity);
						evt.WriteDouble(MoSync.Struct.MALocation.lat, l.Latitude);
						evt.WriteDouble(MoSync.Struct.MALocation.lon, l.Longitude);
						evt.WriteDouble(MoSync.Struct.MALocation.horzAcc, l.HorizontalAccuracy);
						evt.WriteDouble(MoSync.Struct.MALocation.vertAcc, l.VerticalAccuracy);
						evt.WriteFloat(MoSync.Struct.MALocation.alt, (float)l.Altitude);
						runtime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_LOCATION, evt);
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
