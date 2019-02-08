#pragma once

struct vec2;
struct vec3;
struct vec4;
struct mat3;
struct mat4;
struct quat;

#define PI 3.14159265358979323846f

float ToRadians(float degrees);// { return degrees * PI / 180.f;  };
float ToDegrees(float radians); //{ return radians * 180.f / PI; };

template<class T>
// Clamps data to the range [min, max]
T Clamp(const T &data, const T &min, const T &max)
{
	if (data > max)
	{
		return max;
	}
	else if (data < min)
	{
		return min;
	}
	else
	{
		return data;
	}
}

// Linear interpolation
template<class T>
T LERP(const T &data1, const T &data2, float u)
{
	return data1 * (1.0f - u) + data2 * u;
}


struct mat3
{
	enum // Indexes
	{
		RightX = 0, UpX = 3, ForwardX = 6,
		RightY = 1, UpY = 4, ForwardY = 7,
		RightZ = 2, UpZ = 5, ForwardZ = 8
	};

	mat3();
	//explicit mat3(const quat& rotation);
	// Extracts the rotational component from the 4x4 matrix.
	explicit mat3(const mat4& mat);
	mat3(const quat& rotation, const vec3& scale);
	mat3(const vec3& right, const vec3& up, const vec3& forward);
	mat3(float f0, float f3, float f6,
		float f1, float f4, float f7,
		float f2, float f5, float f8);

	bool operator==(const mat3& m) const;
	bool operator!=(const mat3& m) const;

	mat3& operator*=(const mat3& m);
	mat3& operator*=(float scalar);
	mat3& operator/=(float divisor);
	mat3& operator+=(const mat3& m);
	mat3& operator-=(const mat3& m);
	mat3 operator*(const mat3& m) const;
	mat3 operator+(const mat3& m) const;
	mat3 operator-(const mat3& m) const;
	vec3 operator*(const vec3& v) const;
	mat3 operator*(float scalar) const;
	mat3 operator/(float divisor) const;
	mat3 operator-() const;

	float operator[](unsigned index) const;
	float& operator[](unsigned index);

	void Transpose();
	void Inverse();
	mat3 GetTranspose() const;
	mat3 GetInverse() const;
	float GetDeterminant() const;

	void Scale(const vec3& scale);
	void Scale(float scale);

	void Rotate(const vec3& axis, float degrees);
	void RotateX(float degrees);
	void RotateY(float degrees);
	void RotateZ(float degrees);

	void SetRight(const vec3& V);
	void SetUp(const vec3& V);
	void SetForward(const vec3& V);

	vec3 GetRight() const;
	vec3 GetUp() const;
	vec3 GetForward() const;

	static const mat3 Identity;

	float data[9];
};

struct mat4
{
	enum // Indexes
	{
		RightX = 0, UpX = 4, ForwardX = 8, TransX = 12,
		RightY = 1, UpY = 5, ForwardY = 9, TransY = 13,
		RightZ = 2, UpZ = 6, ForwardZ = 10, TransZ = 14,
		W0 = 3, W1 = 7, W2 = 11, W3 = 15
	};

	mat4();
	//explicit mat4(const quat& rotation);
	explicit mat4(const mat3& rotation);
	//mat4(const quat& rotation, const vec3& translation);
	mat4(const mat3& rotation, const vec3& translation);
	//mat4(const quat& rotation, const vec3& translation, const vec3& scale);
	mat4(const mat3& rotation, const vec3& translation, const vec3& scale);
	mat4(const vec3& right, const vec3& up, const vec3& forward, const vec4& translation);
	mat4(float f0, float f4, float f8, float f12,
		float f1, float f5, float f9, float f13,
		float f2, float f6, float f10, float f14,
		float f3, float f7, float f11, float f15);

	bool operator==(const mat4&) const;
	bool operator!=(const mat4&) const;

	mat4& operator*=(const mat4&);
	mat4& operator*=(float scalar);
	mat4& operator/=(float divisor);
	mat4& operator+=(const mat4&);
	mat4& operator-=(const mat4&);
	mat4 operator*(const mat4&) const;
	mat4 operator+(const mat4&) const;
	mat4 operator-(const mat4&) const;
	vec4 operator*(const vec4&) const;
	mat4 operator*(float scalar) const;
	mat4 operator/(float divisor) const;
	mat4 operator-() const;

	float operator[](unsigned index) const;
	float& operator[](unsigned index);

	void Transpose();
	void Inverse();
	// Computes the inverse assuming standard homogeneous matrix format.
	// [ R     T ]
	// [ 0 0 0 1 ]
	void FastInverse();
	mat4 GetTranspose() const;
	mat4 GetInverse() const;
	// Computes the inverse assuming standard homogeneous matrix format.
	// [ R     T ]
	// [ 0 0 0 1 ]
	mat4 GetFastInverse() const;
	float GetDeterminant() const;

	void Scale(const vec3& scale);
	void Scale(float scale);

	void Rotate(const vec3& axis, float degrees);
	void RotateX(float degrees);
	void RotateY(float degrees);
	void RotateZ(float degrees);

	void Translate(const vec3& translation);

	void SetRight(const vec3& v);
	void SetUp(const vec3& v);
	void SetForward(const vec3& v);
	void SetTranslation(const vec3& v);

	vec3 GetRight() const;
	vec3 GetUp() const;
	vec3 GetForward() const;
	vec3 GetTranslation() const;

	static const mat4 Identity;

	static mat4 PerspectiveProjection(float fovyDegrees, float aspect, float zNear, float zFar);
	static mat4 InversePerspectiveProjection(float fovyDegrees, float aspect, float zNear, float zFar);

	static mat4 OrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar);
	static mat4 InverseOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar);

	static mat4 LookAt(const vec3& position, const vec3& target, const vec3& upVector);

	float data[16];
};

struct vec2
{
	vec2() = default;
	vec2(float x, float y);
	explicit vec2(float val);

	bool operator==(const vec2&) const;
	bool operator!=(const vec2&) const;

	vec2& operator-=(const vec2&);
	vec2& operator+=(const vec2&);
	vec2& operator*=(const vec2&);
	vec2& operator/=(const vec2&);
	vec2& operator*=(float scalar);
	vec2& operator/=(float divisor);

	vec2 operator-() const;
	vec2 operator-(const vec2&) const;
	vec2 operator+(const vec2&) const;
	vec2 operator*(const vec2&) const;
	vec2 operator/(const vec2&) const;
	vec2 operator*(float scalar) const;
	vec2 operator/(float divisor) const;

	float operator[](unsigned index) const;
	float& operator[](unsigned index);

	float Length() const;
	float LengthSquared() const;
	void ClampLength(float length);
	void Normalize();
	vec2 GetNormalized() const;

	static const vec2 Zero;
	static const vec2 One;
	static const vec2 Right;
	static const vec2 Up;

	float x = 0.0f;
	float y = 0.0f;
};

struct vec3
{
	vec3() = default;
	vec3(const vec2& xy, float z);
	vec3(float x, float y, float z);
	explicit vec3(float val);

	bool operator==(const vec3&) const;
	bool operator!=(const vec3&) const;

	vec3& operator-=(const vec3&);
	vec3& operator+=(const vec3&);
	vec3& operator*=(const vec3&);
	vec3& operator/=(const vec3&);
	vec3& operator*=(float scalar);
	vec3& operator/=(float divisor);

	vec3 operator-() const;
	vec3 operator-(const vec3&) const;
	vec3 operator+(const vec3&) const;
	vec3 operator*(const vec3&) const;
	vec3 operator/(const vec3&) const;
	vec3 operator*(float scalar) const;
	vec3 operator/(float divisor) const;

	float operator[](unsigned index) const;
	float& operator[](unsigned index);

	explicit operator vec2() const;

	float Length() const;
	float LengthSquared() const;
	void ClampLength(float length);
	void Normalize();
	vec3 GetNormalized() const;

	static const vec3 Zero;
	static const vec3 One;
	static const vec3 Right;
	static const vec3 Up;
	static const vec3 Forward;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct vec4
{
	vec4() = default;
	vec4(const vec3& xyz, float w);
	vec4(const vec2& xy, float y, float w);
	vec4(const vec2& xy, const vec2& zw);
	vec4(float x, float y, float z, float w);
	explicit vec4(float val);

	bool operator==(const vec4&) const;
	bool operator!=(const vec4&) const;

	vec4& operator-=(const vec4&);
	vec4& operator+=(const vec4&);
	vec4& operator*=(const vec4&);
	vec4& operator/=(const vec4&);
	vec4& operator*=(float scalar);
	vec4& operator/=(float divisor);

	vec4 operator-() const;
	vec4 operator-(const vec4&) const;
	vec4 operator+(const vec4&) const;
	vec4 operator*(const vec4&) const;
	vec4 operator/(const vec4&) const;
	vec4 operator*(float scalar) const;
	vec4 operator/(float divisor) const;

	float operator[](unsigned index) const;
	float& operator[](unsigned index);

	explicit operator vec2() const;
	explicit operator vec3() const;

	float Length() const;
	float LengthSquared() const;
	void ClampLength(float length);
	void Normalize();
	vec4 GetNormalized() const;

	static const vec4 Zero;
	static const vec4 One;
	static const vec4 Right;
	static const vec4 Up;
	static const vec4 Forward;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

float Distance(const vec2& v1, const vec2& v2); //{ return (v1 - v2).Length(); };
float Distance(const vec3& v1, const vec3& v2); //{ return (v1 - v2).Length(); };
float Distance(const vec4& v1, const vec4& v2); //{ return (v1 - v2).Length();};
float Dot(const vec2& v1, const vec2& v2); //{ return v1.x * v2.x + v1.y * v2.y; };
float Dot(const vec3& v1, const vec3& v2); //{ return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; };
float Dot(const vec4& v1, const vec4& v2); //{ return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; };
vec3 Cross(const vec3& v1, const vec3& v2); //{ return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x); };
vec2 Reflect(const vec2& incident, const vec2& normal);
vec3 Reflect(const vec3& incident, const vec3& normal);
vec4 Reflect(const vec4& incident, const vec4& normal);
vec2 Refract(const vec2& incident, const vec2& normal, float index);
vec3 Refract(const vec3& incident, const vec3& normal, float index);
vec4 Refract(const vec4& incident, const vec4& normal, float index);

vec2 operator*(float scalar, const vec2&);
vec2 operator/(float divisor, const vec2&);
vec3 operator*(float scalar, const vec3&);
vec3 operator/(float divisor, const vec3&);
vec4 operator*(float scalar, const vec4&);
vec4 operator/(float divisor, const vec4&);

//struct quat
//{
//	quat() = default;
//	quat(float X, float Y, float Z, float W);
//	quat(const vec3& right, const vec3& up, const vec3& forward);
//	quat(const mat3& rotation);
//
//	bool operator==(const quat&) const;
//	bool operator!=(const quat&) const;
//
//	quat operator*(const quat&) const;
//	vec3 operator*(const vec3&) const;
//	quat& operator*=(const quat&);
//
//	float operator[](unsigned index) const;
//	float& operator[](unsigned index);
//
//	void SetIdentity();
//	void Conjugate();
//	quat GetConjugate() const;
//	void Normalize();
//	quat GetNormalized() const;
//
//	vec3 GetRight() const;
//	vec3 GetUp() const;
//	vec3 GetForward() const;
//
//	void Rotate(const vec3& axis, float degrees);
//	void Rotate(float X, float Y, float Z, float degrees);
//	void RotateX(float degrees);
//	void RotateY(float degrees);
//	void RotateZ(float degrees);
//
//	static const quat Identity;
//
//	float x = 0.0f;
//	float y = 0.0f;
//	float z = 0.0f;
//	float w = 1.0f;
//};
//
//float Dot(const quat& p0, const quat& p1);
//quat Slerp(const quat& p0, const quat& p1, float percent);
