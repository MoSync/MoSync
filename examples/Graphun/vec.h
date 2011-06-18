#ifndef _VEC_H_
#define _VEC_H_

#include <madmath.h>

class vec2 {
public:
	vec2(float x=0.0f, float y=0.0f) : x(x), y(y) {
	}

	inline float dot(const vec2& v) const {
		return x*v.x+y*v.y;
	}

	inline float length() const {
		return sqrt(dot(*this));
	}

	inline void normalize() {
		*this /= length();
	}

	inline vec2 operator*(float num) const {
		return vec2(x*num, y*num);
	}

	inline vec2 operator/(float num) const {
		return vec2(x/num, y/num);
	}

	inline vec2 operator+(const vec2& v) const {
		return vec2(x+v.x, y+v.y);
	}

	inline vec2 operator-(const vec2& v) const {
		return vec2(x-v.x, y-v.y);
	}

	inline const vec2& operator *=(float s)
	{
		x*=s;
		y*=s;
		return *this;
	}

	inline const vec2& operator /=(float s)
	{
		x/=s;
		y/=s;
		return *this;
	}

	inline const vec2& operator +=(const vec2 &v)
	{
		x+=v.x;
		y+=v.y;
		return *this;
	}

	inline const vec2& operator -=(const vec2 &v)
	{
		x-=v.x;
		y-=v.y;
		return *this;
	}

	//! projects this vector onto v
	inline void project(const vec2 &v)
	{	
		*this = projected(v);	
	}

	//! returns this vector projected onto v
	inline vec2 projected(const vec2 &v) const
	{   
		return v * (dot(v))/(v.dot(v));	
	}

	//! computes the angle between 2 arbitrary vectors
	inline float angle(const vec2 &v) const
	{   
		return acos((dot(v)) / (length()*v.length()));
	}

	void rotate(float angle) {
		x = x*cos(angle) - y*sin(angle);
		y = x*sin(angle) + y*cos(angle);
	}

	float x, y;
};

class vec3 {
public:
	vec3(float x=0.0f, float y=0.0f, float z=0.0f) : x(x), y(y), z(z) {
	}

	inline float dot(const vec3& v) const {
		return x*v.x+y*v.y+z*v.z;
	}

	inline vec3 cross(const vec3& v) const {
		return vec3(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x
				);
	}

	inline float length() const {
		return sqrt(dot(*this));
	}

	inline void normalize() {
		*this /= length();
	}

	inline vec3 operator*(float num) const {
		return vec3(x*num, y*num, z*num);
	}

	inline vec3 operator/(float num) const {
		return vec3(x/num, y/num, z/num);
	}

	inline vec3 operator+(const vec3& v) const {
		return vec3(x+v.x, y+v.y, z+v.z);
	}

	inline vec3 operator-(const vec3& v) const {
		return vec3(x-v.x, y-v.y, z-v.z);
	}

	inline const vec3& operator *=(float s)
	{
		x*=s;
		y*=s;
		z*=s;
		return *this;
	}

	inline const vec3& operator /=(float s)
	{
		x/=s;
		y/=s;
		z/=s;
		return *this;
	}

	inline const vec3& operator +=(const vec3 &v)
	{
		x+=v.x;
		y+=v.y;
		z+=v.z;
		return *this;
	}

	inline const vec3& operator -=(const vec3 &v)
	{
		x-=v.x;
		y-=v.y;
		z-=v.z;
		return *this;
	}

	//! projects this vector onto v
	inline void project(const vec3 &v)
	{	
		*this = projected(v);	
	}

	//! returns this vector projected onto v
	inline vec3 projected(const vec3 &v) const
	{   
		return v * (dot(v))/(v.dot(v));	
	}

	//! computes the angle between 2 arbitrary vectors
	inline float angle(const vec3 &v) const
	{   
		return acos((dot(v)) / (length()*v.length()));
	}

	void rotate(float ax, float ay, float az) {
			float cx=cos(ax);
			float sx=sin(ax);
			float cy=cos(ay);
			float sy=sin(ay);
			float cz=cos(az);
			float sz=sin(az);
		float xx, xy, xz, yx, yy, yz, zx, zy, zz;
		xx=(cy*cz);
		xy=(cy*sz);
		xz=-sy;
		yx=(sx*sy*cz)-(cx*sz);
		yy=(sx*sy*sz)+(cx*cz);
		yz=(sx*cy);
		zx=(cx*sy*cz)+(sx*sz);
		zy=(cx*sy*sz)-(sx*cz);
		zz=(cx*cy);

		float nx =(xx * x + xy * y + xz * z);
		float ny =(yx * x + yy * y + yz * z);
		float nz =(zx * x + zy * y + zz * z);
		x = nx;
		y = ny;
		z = nz;
	}

	// axis needs to be normalized;
	void rotateAroundAxis(const vec3& axis, float angle) {
		float c = cos(angle);
		float s = sin(angle);
		float t = 1.0 - c;
		//  if axis is not already normalised then uncomment this
		// double magnitude = Math.sqrt(a1.x*a1.x + a1.y*a1.y + a1.z*a1.z);
		// if (magnitude==0) throw error;
		// a1.x /= magnitude;
		// a1.y /= magnitude;
		// a1.z /= magnitude;

		float m00 = c + axis.x*axis.x*t;
		float m11 = c + axis.y*axis.y*t;
		float m22 = c + axis.z*axis.z*t;


		float tmp1 = axis.x*axis.y*t;
		float tmp2 = axis.z*s;
		float m10 = tmp1 + tmp2;
		float m01 = tmp1 - tmp2;
		tmp1 = axis.x*axis.z*t;
		tmp2 = axis.y*s;
		float m20 = tmp1 - tmp2;
		float m02 = tmp1 + tmp2;    
		tmp1 = axis.y*axis.z*t;
		tmp2 = axis.x*s;
		float m21 = tmp1 + tmp2;
		float m12 = tmp1 - tmp2;

		x = m00*x + m10*y + m20*z;
		y = m01*x + m11*y + m21*z;
		z = m02*x + m12*y + m22*z;
	}

	float x, y, z;
};

#endif // _VEC3_H_
