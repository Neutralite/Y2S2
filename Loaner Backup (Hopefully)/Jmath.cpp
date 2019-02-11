#include "Jmath.h"
#include <cmath>
//#include <iostream>

const mat4 mat4::Identity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
const mat3 mat3::Identity(1, 0, 0, 0, 1, 0, 0, 0, 1);
const quat quat::Identity(0, 0, 0, 1);

mat3::mat3()
{
	data[0] = 1;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 1;
	data[5] = 0;
	data[6] = 0;
	data[7] = 0;
	data[8] = 1;
}

//mat3::mat3(const quat & rotation)
//{
//}

mat3::mat3(const mat4 & mat)
{
	for (int i = 0; i < 9; i++)
	{
		data[i] = mat.data[i + i / 3];
	}
}

mat3::mat3(const quat & rotation, const vec3 & scale)
{
}

mat3::mat3(const vec3 & right, const vec3 & up, const vec3 & forward)
{
}

mat3::mat3(float f0, float f3, float f6, float f1, float f4, float f7, float f2, float f5, float f8)
{
	data[0] = f0;
	data[1] = f3;
	data[2] = f6;
	data[3] = f1;
	data[4] = f4;
	data[5] = f7;
	data[6] = f2;
	data[7] = f5;
	data[8] = f8;
}

bool mat3::operator==(const mat3 & m) const
{
	for (int i = 0; i < 9; i++)
		if (m.data[i] != this->data[i])
			return false;
	return true;
}

bool mat3::operator!=(const mat3 & m) const
{
	return !(*this == m);
}

mat3 & mat3::operator*=(const mat3 & m)
{
	mat3 temp;
	temp.data[0] = data[0] * m.data[0] + data[1] * m.data[3] + data[2] * m.data[6];
	temp.data[1] = data[0] * m.data[1] + data[1] * m.data[4] + data[2] * m.data[7];
	temp.data[2] = data[0] * m.data[2] + data[1] * m.data[5] + data[2] * m.data[8];
	temp.data[3] = data[3] * m.data[0] + data[4] * m.data[3] + data[5] * m.data[6];
	temp.data[4] = data[3] * m.data[1] + data[4] * m.data[4] + data[5] * m.data[7];
	temp.data[5] = data[3] * m.data[2] + data[4] * m.data[5] + data[5] * m.data[7];
	temp.data[6] = data[6] * m.data[0] + data[7] * m.data[3] + data[8] * m.data[6];
	temp.data[7] = data[6] * m.data[1] + data[7] * m.data[4] + data[8] * m.data[7];
	temp.data[8] = data[6] * m.data[2] + data[7] * m.data[5] + data[8] * m.data[8];
	*this = temp;
	return *this;
}

mat3 & mat3::operator*=(float scalar)
{
	for (int i = 0; i < 9; i++)
		this->data[i] *= scalar;
	return *this;
}

mat3 & mat3::operator/=(float divisor)
{
	float scalar = 1.0f / divisor;
	for (int i = 0; i < 9; i++)
		this->data[i] *= scalar;
	return *this;
}

mat3 & mat3::operator+=(const mat3 & m)
{
	for (int i = 0; i < 9; i++)
		this->data[i] += m.data[i];
	return *this;
}

mat3 & mat3::operator-=(const mat3 & m)
{
	for (int i = 0; i < 9; i++)
		this->data[i] += m.data[i];
	return *this;
}

mat3 mat3::operator*(const mat3 & m) const
{
	mat3 temp;
	temp.data[0] = data[0] * m.data[0] + data[1] * m.data[3] + data[2] * m.data[6];
	temp.data[1] = data[0] * m.data[1] + data[1] * m.data[4] + data[2] * m.data[7];
	temp.data[2] = data[0] * m.data[2] + data[1] * m.data[5] + data[2] * m.data[8];
	temp.data[3] = data[3] * m.data[0] + data[4] * m.data[3] + data[5] * m.data[6];
	temp.data[4] = data[3] * m.data[1] + data[4] * m.data[4] + data[5] * m.data[7];
	temp.data[5] = data[3] * m.data[2] + data[4] * m.data[5] + data[5] * m.data[7];
	temp.data[6] = data[6] * m.data[0] + data[7] * m.data[3] + data[8] * m.data[6];
	temp.data[7] = data[6] * m.data[1] + data[7] * m.data[4] + data[8] * m.data[7];
	temp.data[8] = data[6] * m.data[2] + data[7] * m.data[5] + data[8] * m.data[8];
	return temp;
}

mat3 mat3::operator+(const mat3 & m) const
{
	mat3 temp;
	for (int i = 0; i < 9; i++)
		temp.data[i] = data[i] + m.data[i];
	return temp;
}

mat3 mat3::operator-(const mat3 & m) const
{
	mat3 temp;
	for (int i = 0; i < 9; i++)
		temp.data[i] = data[i] - m.data[i];
	return temp;;
}

vec3 mat3::operator*(const vec3 & v) const
{
	vec3 temp;
	temp.x = data[0] * v.x + temp.y * data[1] + temp.z * data[2];
	temp.y = data[3] * v.x + temp.y * data[4] + temp.z * data[5];
	temp.z = data[6] * v.x + temp.y * data[7] + temp.z * data[8];

	return temp;
}

mat3 mat3::operator*(float scalar) const
{
	mat3 temp;
	for (int i = 0; i < 9; i++)
		temp.data[i] = data[i] * scalar;
	return temp;
}

mat3 mat3::operator/(float divisor) const
{
	mat3 temp;
	float scalar = 1.0f / divisor;
	for (int i = 0; i < 9; i++)
		temp.data[i] = data[i] * scalar;
	return temp;
}

mat3 mat3::operator-() const
{
	mat3 temp;
	for (int i = 0; i < 9; i++)
		temp.data[i] = 0 - data[i];
	return temp;
}

float mat3::operator[](unsigned index) const
{
	return data[index];
}

float & mat3::operator[](unsigned index)
{
	return data[index];
}

void mat3::Transpose()
{
	mat3 temp;
	temp.data[0] = data[0];
	temp.data[1] = data[3];
	temp.data[2] = data[6];
	temp.data[3] = data[1];
	temp.data[5] = data[5];
	temp.data[5] = data[7];
	temp.data[6] = data[3];
	temp.data[7] = data[5];
	temp.data[8] = data[8];
	*this = temp;
}

void mat3::Inverse()
{
	mat3 temp;
	float det = GetDeterminant();
	if (det == 0)
	{

	}
	else
	{
		temp.data[0] = data[4] * data[8] - data[5] * data[7];
		temp.data[1] = 0.f - data[3] * data[8] + data[5] * data[6];
		temp.data[2] = data[3] * data[7] - data[4] * data[6];
		temp.data[3] = 0.f - data[1] * data[8] + data[2] * data[7];
		temp.data[4] = data[0] * data[8] - data[2] * data[6];
		temp.data[5] = 0.f - data[0] * data[7] + data[1] * data[6];
		temp.data[6] = data[1] * data[5] - data[2] * data[4];
		temp.data[7] = 0.f - data[0] * data[5] + data[2] * data[3];
		temp.data[8] = data[0] * data[5] - data[1] * data[3];
	}
	temp.Transpose();
	temp /= det;
	*this = temp;
}

mat3 mat3::GetTranspose() const
{
	mat3 temp = *this;
	temp.Transpose();
	return temp;
}

mat3 mat3::GetInverse() const
{
	mat3 temp = *this;
	temp.Inverse();
	return temp;
}

float mat3::GetDeterminant() const
{
	return data[0] * (data[4] * data[8] - data[5] * data[7]) - data[1] * (data[3] * data[8] - data[5] * data[6]) + data[2] * (data[3] * data[7] - data[4] * data[6]);
}

void mat3::Scale(const vec3 & scale)
{
}

void mat3::Scale(float scale)
{
}

void mat3::Rotate(const vec3 & axis, float degrees)
{
	vec3 v = axis;
	v.Normalize();

	float rad = ToRadians(degrees);
	float cr = cos(rad);
	float sr = sin(rad);
	mat3 temp = mat3(
		cr + v.x * v.x * (1.f - cr), v.y * v.x * (1.f - cr) + v.z * sr, v.z * v.x * (1.f - cr) - v.y * sr, 
		v.x * v.y * (1.f - cr) - v.z * sr, cr + v.y * v.y * (1.f - cr), v.z * v.y * (1.f - cr) + v.x * sr, 
		v.x * v.z * (1.f - cr) + v.y * sr, v.y * v.z * (1.f - cr) - v.x * sr, cr + v.z * v.z * (1.f - cr)
		);
	temp.Transpose();
	*this *= temp;
}

void mat3::RotateX(float degrees)
{
	float rad = ToRadians(degrees);
	mat3 temp = mat3(1 ,0 ,0, 0, cos(rad), -sin(rad), 0, sin(rad), cos(rad));
	*this *= temp;
}

void mat3::RotateY(float degrees)
{
	float rad = ToRadians(degrees);
	mat3 temp = mat3(cos(rad), 0, sin(rad), 0, 1, 0, -sin(rad), 0, cos(rad));
	*this *= temp;
}

void mat3::RotateZ(float degrees)
{
	float rad = ToRadians(degrees);
	mat3 temp = mat3(cos(rad), -sin(rad), 0, sin(rad), cos(rad), 0, 0, 0, 1);
	*this *= temp;
}

void mat3::SetRight(const vec3 & V)
{
}

void mat3::SetUp(const vec3 & V)
{
}

void mat3::SetForward(const vec3 & V)
{
}

vec3 mat3::GetRight() const
{
	return vec3();
}

vec3 mat3::GetUp() const
{
	return vec3();
}

vec3 mat3::GetForward() const
{
	return vec3();
}

mat4::mat4()
{
	data[0] = 1;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	data[5] = 1;
	data[6] = 0;
	data[7] = 0;
	data[8] = 0;
	data[9] = 0;
	data[10] = 1;
	data[11] = 0;
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 1;
}

//mat4::mat4(const quat & rotation)
//{
//}

mat4::mat4(const mat3 & rotation)
{
	data[0] = rotation.data[0];
	data[1] = rotation.data[1];
	data[2] = rotation.data[2];
	data[3] = 0;
	data[4] = rotation.data[3];
	data[5] = rotation.data[4];
	data[6] = rotation.data[5];
	data[7] = 0;
	data[8] = rotation.data[6];
	data[9] = rotation.data[7];
	data[10] = rotation.data[8];
	data[11] = 0;
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 1;
}

//mat4::mat4(const quat & rotation, const vec3 & translation)
//{
//}

mat4::mat4(const mat3 & rotation, const vec3 & translation)
{
	data[0] = rotation.data[0];
	data[1] = rotation.data[1];
	data[2] = rotation.data[2];
	data[3] = translation.x;
	data[4] = rotation.data[3];
	data[5] = rotation.data[4];
	data[6] = rotation.data[5];
	data[7] = translation.y;
	data[8] = rotation.data[6];
	data[9] = rotation.data[7];
	data[10] = rotation.data[8];
	data[11] = translation.z;
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 1;
}

//mat4::mat4(const quat & rotation, const vec3 & translation, const vec3 & scale)
//{
//}

mat4::mat4(const mat3 & rotation, const vec3 & translation, const vec3 & scale)
{
	data[0] = rotation.data[0];
	data[1] = rotation.data[1];
	data[2] = rotation.data[2];
	data[3] = translation.x;
	data[4] = rotation.data[3];
	data[5] = rotation.data[4];
	data[6] = rotation.data[5];
	data[7] = translation.y;
	data[8] = rotation.data[6];
	data[9] = rotation.data[7];
	data[10] = rotation.data[8];
	data[11] = translation.z;
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 1;

	Scale(scale);
}

mat4::mat4(const vec3 & right, const vec3 & up, const vec3 & forward, const vec4 & translation)
{
}

mat4::mat4(float f0, float f4, float f8, float f12, float f1, float f5, float f9, float f13, float f2, float f6, float f10, float f14, float f3, float f7, float f11, float f15)
{
	data[0] = f0;
	data[1] = f4;
	data[2] = f8;
	data[3] = f12;
	data[4] = f1;
	data[5] = f5;
	data[6] = f9;
	data[7] = f13;
	data[8] = f2;
	data[9] = f6;
	data[10] = f10;
	data[11] = f14;
	data[12] = f3;
	data[13] = f7;
	data[14] = f11;
	data[15] = f15;
}

bool mat4::operator==(const mat4 & m) const
{
	for (int i = 0; i < 16; i++)
		if (data[i] != m.data[i])
			return false;
	return true;
}

bool mat4::operator!=(const mat4 & m) const
{
	return !(*this == m);
}

mat4 & mat4::operator*=(const mat4 & m)
{
	mat4 temp;
	temp[0] = data[0] * m.data[0] + data[1] * m.data[4] + data[2] * m.data[8] + data[3] * m.data[12];
	temp[1] = data[0] * m.data[1] + data[1] * m.data[5] + data[2] * m.data[9] + data[3] * m.data[13];
	temp[2] = data[0] * m.data[2] + data[1] * m.data[6] + data[2] * m.data[10] + data[3] * m.data[14];
	temp[3] = data[0] * m.data[3] + data[1] * m.data[7] + data[2] * m.data[11] + data[3] * m.data[15];
	temp[4] = data[4] * m.data[0] + data[5] * m.data[4] + data[6] * m.data[8] + data[7] * m.data[12];
	temp[5] = data[4] * m.data[1] + data[5] * m.data[5] + data[6] * m.data[9] + data[7] * m.data[13];
	temp[6] = data[4] * m.data[2] + data[5] * m.data[6] + data[6] * m.data[10] + data[7] * m.data[14];
	temp[7] = data[4] * m.data[3] + data[5] * m.data[7] + data[6] * m.data[11] + data[7] * m.data[15];
	temp[8] = data[8] * m.data[0] + data[9] * m.data[4] + data[10] * m.data[8] + data[11] * m.data[12];
	temp[9] = data[8] * m.data[1] + data[9] * m.data[5] + data[10] * m.data[9] + data[11] * m.data[13];
	temp[10] = data[8] * m.data[2] + data[9] * m.data[6] + data[10] * m.data[10] + data[11] * m.data[14];
	temp[11] = data[8] * m.data[3] + data[9] * m.data[7] + data[10] * m.data[11] + data[11] * m.data[15];
	temp[12] = data[12] * m.data[0] + data[13] * m.data[4] + data[14] * m.data[8] + data[15] * m.data[12];
	temp[13] = data[12] * m.data[1] + data[13] * m.data[5] + data[14] * m.data[9] + data[15] * m.data[13];
	temp[14] = data[12] * m.data[2] + data[13] * m.data[6] + data[14] * m.data[10] + data[15] * m.data[14];
	temp[15] = data[12] * m.data[3] + data[13] * m.data[7] + data[14] * m.data[11] + data[15] * m.data[15];
	*this = temp;
	return *this;
}

mat4 & mat4::operator*=(float scalar)
{
	for (int i = 0; i < 16; i++)
		this->data[i] *= scalar;
	return *this;
}

mat4 & mat4::operator/=(float divisor)
{
	float scalar = 1.f / divisor;
	for (int i = 0; i < 16; i++)
		this->data[i] *= scalar;
	return *this;
}

mat4 & mat4::operator+=(const mat4 & m)
{
	for (int i = 0; i < 16; i++)
		this->data[i] += m.data[i];
	return *this;
}

mat4 & mat4::operator-=(const mat4 & m)
{
	for (int i = 0; i < 16; i++)
		this->data[i] -= m.data[i];
	return *this;
}

mat4 mat4::operator*(const mat4 & m) const
{
	mat4 temp;
	temp[0] = data[0] * m.data[0] + data[1] * m.data[4] + data[2] * m.data[8] + data[3] * m.data[12];
	temp[1] = data[0] * m.data[1] + data[1] * m.data[5] + data[2] * m.data[9] + data[3] * m.data[13];
	temp[2] = data[0] * m.data[2] + data[1] * m.data[6] + data[2] * m.data[10] + data[3] * m.data[14];
	temp[3] = data[0] * m.data[3] + data[1] * m.data[7] + data[2] * m.data[11] + data[3] * m.data[15];
	temp[4] = data[4] * m.data[0] + data[5] * m.data[4] + data[6] * m.data[8] + data[7] * m.data[12];
	temp[5] = data[4] * m.data[1] + data[5] * m.data[5] + data[6] * m.data[9] + data[7] * m.data[13];
	temp[6] = data[4] * m.data[2] + data[5] * m.data[6] + data[6] * m.data[10] + data[7] * m.data[14];
	temp[7] = data[4] * m.data[3] + data[5] * m.data[7] + data[6] * m.data[11] + data[7] * m.data[15];
	temp[8] = data[8] * m.data[0] + data[9] * m.data[4] + data[10] * m.data[8] + data[11] * m.data[12];
	temp[9] = data[8] * m.data[1] + data[9] * m.data[5] + data[10] * m.data[9] + data[11] * m.data[13];
	temp[10] = data[8] * m.data[2] + data[9] * m.data[6] + data[10] * m.data[10] + data[11] * m.data[14];
	temp[11] = data[8] * m.data[3] + data[9] * m.data[7] + data[10] * m.data[11] + data[11] * m.data[15];
	temp[12] = data[12] * m.data[0] + data[13] * m.data[4] + data[14] * m.data[8] + data[15] * m.data[12];
	temp[13] = data[12] * m.data[1] + data[13] * m.data[5] + data[14] * m.data[9] + data[15] * m.data[13];
	temp[14] = data[12] * m.data[2] + data[13] * m.data[6] + data[14] * m.data[10] + data[15] * m.data[14];
	temp[15] = data[12] * m.data[3] + data[13] * m.data[7] + data[14] * m.data[11] + data[15] * m.data[15];
	return temp;
}

mat4 mat4::operator+(const mat4 & m) const
{
	mat4 temp;
	for (int i = 0; i < 16; i++)
		temp.data[i] = data[i] + m.data[i];
	return temp;
}

mat4 mat4::operator-(const mat4 & m) const
{
	mat4 temp;
	for (int i = 0; i < 16; i++)
		temp.data[i] = data[i] - m.data[i];
	return temp;
}

vec4 mat4::operator*(const vec4 & v) const
{
	vec4 temp;
	temp.x = v.x * data[0] + v.y * data[1] + v.z * data[2] + v.w * data[3];
	temp.y = v.x * data[4] + v.y * data[5] + v.z * data[6] + v.w * data[7];
	temp.z = v.x * data[8] + v.y * data[9] + v.z * data[10] + v.w * data[11];
	temp.w = v.x * data[12] + v.y * data[13] + v.z * data[14] + v.w * data[15];
	return temp;
}

mat4 mat4::operator*(float scalar) const
{
	mat4 temp;
	for (int i = 0; i < 16; i++)
		temp.data[i] = data[i] * scalar;
	return temp;
}

mat4 mat4::operator/(float divisor) const
{
	mat4 temp;
	float scalar = 1.f / divisor;
	for (int i = 0; i < 16; i++)
		temp.data[i] = data[i] * scalar;
	return temp;
}

mat4 mat4::operator-() const
{
	mat4 temp;
	for (int i = 0; i < 16; i++)
		temp.data[i] = 0 - data[i];
	return temp;
}

float mat4::operator[](unsigned index) const
{
	return data[index];
}

float & mat4::operator[](unsigned index)
{
	return data[index];
}

void mat4::Transpose()
{
	mat4 temp;
	temp.data[0] = data[0];
	temp.data[1] = data[4];
	temp.data[2] = data[8];
	temp.data[3] = data[12];
	temp.data[4] = data[1];
	temp.data[5] = data[5];
	temp.data[6] = data[9];
	temp.data[7] = data[13];
	temp.data[8] = data[2];
	temp.data[9] = data[6];
	temp.data[10] = data[10];
	temp.data[11] = data[14];
	temp.data[12] = data[3];
	temp.data[13] = data[7];
	temp.data[14] = data[11];
	temp.data[15] = data[15];
	*this = temp;
}

void mat4::Inverse()
{
	mat4 temp;
	float det = GetDeterminant();
	if (det == 0)
	{

	}
	else
	{
		temp.data[0] = data[5] * (data[10] * data[15] - data[11] * data[14]) - data[6] * (data[9] * data[15] - data[11] * data[13]) + data[7] * (data[9] * data[14] - data[10] * data[13]);
		temp.data[1] = 0.f - (data[4] * (data[10] * data[15] - data[11] * data[14]) - data[6] * (data[8] * data[15] - data[11] * data[12]) + data[7] * (data[8] * data[14] - data[10] * data[12]));
		temp.data[2] = data[4] * (data[9] * data[15] - data[11] * data[13]) - data[5] * (data[8] * data[15] - data[11] * data[12]) + data[7] * (data[8] * data[13] - data[9] * data[12]);
		temp.data[3] = 0.f - (data[4] * (data[9] * data[14] - data[10] * data[13]) - data[5] * (data[8] * data[14] - data[10] * data[12]) + data[6] * (data[8] * data[13] - data[9] * data[12]));
		temp.data[4] = 0.f - (data[1] * (data[10] * data[15] - data[11] * data[14]) - data[2] * (data[9] * data[15] - data[11] * data[13]) + data[3] * (data[9] * data[14] - data[10] * data[13]));
		temp.data[5] = data[0] * (data[10] * data[15] - data[11] * data[14]) - data[2] * (data[8] * data[15] - data[11] * data[12]) + data[3] * (data[8] * data[14] - data[10] * data[12]);
		temp.data[6] = 0.f - (data[0] * (data[9] * data[15] - data[11] * data[13]) - data[1] * (data[8] * data[15] - data[11] * data[12]) + data[3] * (data[8] * data[13] - data[9] * data[12]));
		temp.data[7] = data[0] * (data[9] * data[14] - data[10] * data[13]) - data[1] * (data[8] * data[14] - data[10] * data[12]) + data[2] * (data[8] * data[13] - data[9] * data[12]);
		temp.data[8] = data[1] * (data[6] * data[15] - data[7] * data[14]) - data[2] * (data[5] * data[15] - data[7] * data[13]) + data[3] * (data[5] * data[14] - data[6] * data[13]);
		temp.data[9] = 0.f - (data[0] * (data[6] * data[15] - data[7] * data[14]) - data[2] * (data[4] * data[15] - data[7] * data[12]) + data[3] * (data[4] * data[14] - data[6] * data[12]));
		temp.data[10] = data[0] * (data[5] * data[15] - data[7] * data[13]) - data[1] * (data[4] * data[15] - data[7] * data[12]) + data[3] * (data[4] * data[13] - data[5] * data[12]);
		temp.data[11] = 0.f - (data[0] * (data[5] * data[14] - data[6] * data[13]) - data[1] * (data[4] * data[14] - data[6] * data[12]) + data[2] * (data[4] * data[13] - data[5] * data[12]));
		temp.data[12] = 0.f - (data[1] * (data[6] * data[11] - data[7] * data[10]) - data[2] * (data[5] * data[11] - data[7] * data[9]) + data[3] * (data[5] * data[10] - data[6] * data[9]));
		temp.data[13] = data[0] * (data[6] * data[11] - data[7] * data[10]) - data[2] * (data[4] * data[11] - data[7] * data[8]) + data[3] * (data[4] * data[10] - data[6] * data[8]);
		temp.data[14] = 0.f - (data[0] * (data[5] * data[11] - data[7] * data[9]) - data[1] * (data[4] * data[11] - data[7] * data[8]) + data[3] * (data[4] * data[9] - data[5] * data[8]));
		temp.data[15] = data[0] * (data[5] * data[10] - data[6] * data[9]) - data[1] * (data[4] * data[10] - data[6] * data[8]) + data[2] * (data[4] * data[9] - data[5] * data[8]);
	}
	temp.Transpose();
	temp /= det;
	*this = temp;
}

void mat4::FastInverse()
{
}

mat4 mat4::GetTranspose() const
{
	mat4 temp;
	temp[0] = data[0];
	temp[1] = data[4];
	temp[2] = data[8];
	temp[3] = data[12];
	temp[4] = data[1];
	temp[5] = data[5];
	temp[6] = data[9];
	temp[7] = data[13];
	temp[8] = data[2];
	temp[9] = data[6];
	temp[10] = data[10];
	temp[11] = data[14];
	temp[12] = data[3];
	temp[13] = data[7];
	temp[14] = data[11];
	temp[15] = data[15];
	return temp;
}

mat4 mat4::GetInverse() const
{
	mat4 temp = *this;
	temp.Inverse();
	return temp;
}

mat4 mat4::GetFastInverse() const
{
	return mat4();
}

float mat4::GetDeterminant() const
{
	return 
		data[0] * (data[5] * (data[10] * data[15] - data[11] * data[14]) - data[6] * (data[9] * data[15] - data[11] * data[13]) + data[7] * (data[9] * data[14] - data[10] * data[13])) - 
		data[1] * (data[4] * (data[10] * data[15] - data[11] * data[14]) - data[6] * (data[8] * data[15] - data[11] * data[12]) + data[7] * (data[8] * data[14] - data[10] * data[12])) +
		data[2] * (data[4] * (data[9] * data[15] - data[11] * data[13]) - data[5] * (data[8] * data[15] - data[11] * data[12]) + data[7] * (data[8] * data[13] - data[9] * data[12])) -
		data[3] * (data[4] * (data[9] * data[14] - data[10] * data[13]) - data[5] * (data[8] * data[14] - data[10] * data[12]) + data[6] * (data[8] * data[13] - data[9] * data[12]));
}

void mat4::Scale(const vec3 & scale)
{
	mat4 temp = mat4(scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1);
	*this = temp * *this;
}

void mat4::Scale(float scale)
{
	mat4 temp = mat4(scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, 1);
	*this = temp * *this;
}

void mat4::Rotate(const vec3 & axis, float degrees)
{
	vec3 v = axis;
	v.Normalize();

	float rad = ToRadians(degrees);
	float cr = cos(rad);
	float sr = sin(rad);
	mat4 temp = mat4(
		cr + v.x * v.x * (1.f - cr), v.y * v.x * (1.f - cr) + v.z * sr, v.z * v.x * (1.f - cr) - v.y * sr, 0,
		v.x * v.y * (1.f - cr) - v.z * sr, cr + v.y * v.y * (1.f - cr), v.z * v.y * (1.f - cr) + v.x * sr, 0,
		v.x * v.z * (1.f - cr) + v.y * sr, v.y * v.z * (1.f - cr) - v.x * sr, cr + v.z * v.z * (1.f - cr), 0,
		0, 0, 0, 1
	);
	temp.Transpose();
	*this = temp * *this;
}

void mat4::RotateX(float degrees)
{
	float rad = ToRadians(degrees);
	mat4 temp = mat4(1, 0, 0, 0, 0, cos(rad), -sin(rad), 0, 0, sin(rad), cos(rad), 0, 0, 0, 0, 1);
	*this = temp * *this;
}

void mat4::RotateY(float degrees)
{
	float rad = ToRadians(degrees);
	mat4 temp = mat4(cos(rad), 0, sin(rad), 0, 0, 1, 0, 0, -sin(rad), 0, cos(rad), 0, 0, 0, 0, 1);
	*this = temp * *this;
}

void mat4::RotateZ(float degrees)
{
	float rad = ToRadians(degrees);
	mat4 temp = mat4(cos(rad), -sin(rad), 0, 0, sin(rad), cos(rad), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	*this = temp * *this;
}

void mat4::Translate(const vec3 & translation)
{
	mat4 temp = mat4(1, 0, 0, translation.x, 0, 1, 0, translation.y, 0, 0, 1, translation.z, 0, 0, 0, 1);
	*this = temp * *this;
}

void mat4::SetRight(const vec3 & v)
{
	data[0] = v.x;
	data[4] = v.y;
	data[8] = v.z;
}

void mat4::SetUp(const vec3 & v)
{
	data[1] = v.x;
	data[5] = v.y;
	data[9] = v.z;
}

void mat4::SetForward(const vec3 & v)
{
	data[2] = v.x;
	data[6] = v.y;
	data[10] = v.z;
}

void mat4::SetTranslation(const vec3 & v)
{
	data[3] = v.x;
	data[7] = v.y;
	data[11] = v.z;
}

vec3 mat4::GetRight() const
{
	return vec3(data[0], data[4], data[8]);
}

vec3 mat4::GetUp() const
{
	return vec3(data[1], data[5], data[9]);
}

vec3 mat4::GetForward() const
{
	return vec3(data[2], data[6], data[10]);
}

vec3 mat4::GetTranslation() const
{
	return vec3(data[3], data[7], data[11]);
}

mat4 mat4::PerspectiveProjection(float fovyDegrees, float aspect, float zNear, float zFar)
{
	fovyDegrees = ToRadians(fovyDegrees);
	return mat4(1.f / (aspect * tan(fovyDegrees / 2.f)), 0, 0, 0, 0, 1.f / tan(fovyDegrees / 2.f), 0, 0, 0, 0, -(zFar + zNear) / (zFar - zNear), -2.f * zFar * zNear / (zFar - zNear), 0, 0, -1.f, 0);
}

mat4 mat4::InversePerspectiveProjection(float fovyDegrees, float aspect, float zNear, float zFar)
{
	fovyDegrees = ToRadians(fovyDegrees);
	return mat4(1.f / (aspect * tan(fovyDegrees / 2.f)), 0, 0, 0, 0, 1.f/tan(fovyDegrees / 2.f), 0, 0, 0, 0, -(zFar + zNear)/(zFar - zNear), -2.f * zFar * zNear/(zFar - zNear), 0, 0, -1.f, 0).GetInverse();
}

mat4 mat4::OrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar)
{
	return mat4(2.f / (right - left), 0, 0, -(right + left) / (right - left), 0, 2.f / (top - bottom), 0, -(top + bottom) / (top - bottom), 0, 0, -2.f / (zFar - zNear), -(zFar + zNear) / (zFar - zNear), 0, 0, 0, 1.f);
}

mat4 mat4::InverseOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar)
{
	return mat4(2.f/(right - left), 0, 0, -(right + left)/(right - left), 0, 2.f/(top - bottom), 0, -(top + bottom)/(top - bottom), 0, 0, -2.f/(zFar - zNear), -(zFar + zNear)/(zFar - zNear), 0, 0, 0, 1.f).GetInverse();
}

mat4 mat4::LookAt(const vec3 & position, const vec3 & target, const vec3 & upVector)
{
	return mat4();
}

vec2::vec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

vec2::vec2(float val)
{
	this->x = val;
	this->y = val;
}

bool vec2::operator==(const vec2 & v) const
{
	return x == v.x && y == v.y;
}

bool vec2::operator!=(const vec2 & v) const
{
	return !(*this == v);
}

vec2 & vec2::operator-=(const vec2 & v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

vec2 & vec2::operator+=(const vec2 & v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

vec2 & vec2::operator*=(const vec2 & v)
{
	this->x *= v.x;
	this->y *= v.y;
	return *this;
}

vec2 & vec2::operator/=(const vec2 & v)
{
	this->x /= v.x;
	this->y /= v.y;
	return *this;
}

vec2 & vec2::operator*=(float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	return *this;
}

vec2 & vec2::operator/=(float divisor)
{
	this->x /= divisor;
	this->y /= divisor;
	return *this;
}

vec2 vec2::operator-() const
{
	return vec2(-x, -y);
}

vec2 vec2::operator-(const vec2 & v) const
{
	return vec2(x - v.x, y - v.y);
}

vec2 vec2::operator+(const vec2 & v) const
{
	return vec2(x + v.x, y + v.y);
}

vec2 vec2::operator*(const vec2 & v) const
{
	return vec2(x * v.x, y * v.y);
}

vec2 vec2::operator/(const vec2 & v) const
{
	return vec2(x / v.x, y / v.y);
}

vec2 vec2::operator*(float scalar) const
{
	return vec2(x * scalar, y * scalar);
}

vec2 vec2::operator/(float divisor) const
{
	return vec2(x / divisor, y / divisor);
}

//float vec2::operator[](unsigned index) const
//{
//	return 0.0f;
//}
//
//float & vec2::operator[](unsigned index)
//{
//	// TODO: insert return statement here
//}

float vec2::Length() const
{
	return sqrt(LengthSquared());
}

float vec2::LengthSquared() const
{
	return x * x + y * y;
}

void vec2::ClampLength(float length)
{
	Normalize();
	*this *= length;
}

void vec2::Normalize()
{
	*this /= Length();
}

vec2 vec2::GetNormalized() const
{
	return *this / Length();
}

vec3::vec3(const vec2 & xy, float z)
{
	this->x = xy.x;
	this->y = xy.y;
	this->z = z;
}

vec3::vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3::vec3(float val)
{
	this->x = val;
	this->y = val;
	this->z = val;
}

bool vec3::operator==(const vec3 & v) const
{
	return x == v.x && y == v.y && z == v.z;
}

bool vec3::operator!=(const vec3 & v) const
{
	return !(*this == v);
}

vec3 & vec3::operator-=(const vec3 & v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

vec3 & vec3::operator+=(const vec3 & v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

vec3 & vec3::operator*=(const vec3 & v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	return *this;
}

vec3 & vec3::operator/=(const vec3 & v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	return *this;
}

vec3 & vec3::operator*=(float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	return *this;
}

vec3 & vec3::operator/=(float divisor)
{
	this->x /= divisor;
	this->y /= divisor;
	this->z /= divisor;
	return *this;
}

vec3 vec3::operator-() const
{
	return vec3(-x, -y, -z);
}

vec3 vec3::operator-(const vec3 & v) const
{
	return vec3(x - v.x, y - v.y, z - v.z);
}

vec3 vec3::operator+(const vec3 & v) const
{
	return vec3(x + v.x, y + v.y, z + v.z);
}

vec3 vec3::operator*(const vec3 & v) const
{
	return vec3(x * v.x, y * v.y, z * v.z);
}

vec3 vec3::operator/(const vec3 & v) const
{
	return vec3(x / v.x, y / v.y, z / v.z);
}

vec3 vec3::operator*(float scalar) const
{
	return vec3(x * scalar, y * scalar, z * scalar);
}

vec3 vec3::operator/(float divisor) const
{
	return vec3(x / divisor, y / divisor, z / divisor);
}

//float vec3::operator[](unsigned index) const
//{
//	return 0.0f;
//}
//
//float & vec3::operator[](unsigned index)
//{
//	// TODO: insert return statement here
//}

vec3::operator vec2() const
{
	return vec2(x, y);
}

float vec3::Length() const
{
	return sqrt(LengthSquared());
}

float vec3::LengthSquared() const
{
	return x * x + y * y + z * z;
}

void vec3::ClampLength(float length)
{
	this->Normalize();
	*this *= length;
}

void vec3::Normalize()
{
	*this /= Length();
}

vec3 vec3::GetNormalized() const
{
	return *this / Length();
}

vec4::vec4(const vec3 & xyz, float w)
{
	this->x = xyz.x;
	this->y = xyz.y;
	this->z = xyz.z;
	this->w = w;
}

vec4::vec4(const vec2 & xy, float y, float w)
{
	this->x = xy.x;
	this->y = xy.y;
	this->z = z;
	this->w = w;
}

vec4::vec4(const vec2 & xy, const vec2 & zw)
{
	this->x = xy.x;
	this->y = xy.y;
	this->z = zw.x;
	this->w = zw.y;
}

vec4::vec4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

vec4::vec4(float val)
{
	this->x = val;
	this->y = val;
	this->z = val;
	this->w = val;
}

bool vec4::operator==(const vec4 & v) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool vec4::operator!=(const vec4 & v) const
{
	return !(*this == v);
}

vec4 & vec4::operator-=(const vec4 & v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;
	return *this;
}

vec4 & vec4::operator+=(const vec4 & v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;
	return *this;
}

vec4 & vec4::operator*=(const vec4 & v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	this->w *= v.w;
	return *this;
}

vec4 & vec4::operator/=(const vec4 & v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	this->w /= v.w;
	return *this;
}

vec4 & vec4::operator*=(float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	this->w *= scalar;
	return *this;
}

vec4 & vec4::operator/=(float divisor)
{
	this->x /= divisor;
	this->y /= divisor;
	this->z /= divisor;
	this->w /= divisor;
	return *this;
}

vec4 vec4::operator-() const
{
	return vec4(-x, -y, -z, -w);
}

vec4 vec4::operator-(const vec4 & v) const
{
	return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

vec4 vec4::operator+(const vec4 & v) const
{
	return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

vec4 vec4::operator*(const vec4 & v) const
{
	return vec4(x * v.x, y * v.y, z * v.z, w * v.w);
}

vec4 vec4::operator/(const vec4 & v) const
{
	return vec4(x / v.x, y / v.y, z / v.z, w / v.w);
}

vec4 vec4::operator*(float scalar) const
{
	return vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}

vec4 vec4::operator/(float divisor) const
{
	return vec4(x / divisor, y / divisor, z / divisor, w / divisor);
}

//float vec4::operator[](unsigned index) const
//{
//	return 0.0f;
//}
//
//float & vec4::operator[](unsigned index)
//{
//	// TODO: insert return statement here
//}

vec4::operator vec2() const
{
	return vec2(x, y);
}

vec4::operator vec3() const
{
	return vec3(x, y, z);
}

float vec4::Length() const
{
	return sqrt(LengthSquared());
}

float vec4::LengthSquared() const
{
	return x * x + y * y + z * z + w * w;
}

void vec4::ClampLength(float length)
{
	Normalize();
	*this *= length;
}

void vec4::Normalize()
{
	*this /= Length();
}

vec4 vec4::GetNormalized() const
{
	return *this / Length();
}

float ToRadians(float degrees)
{
	return degrees * PI / 180.f;
}

float ToDegrees(float radians)
{
	return radians * 180.f / PI;
}

vec2 Reflect(const vec2 & incident, const vec2 & normal)
{
	return incident - 2.f * normal * Dot(incident, normal) / normal.LengthSquared();
}

vec3 Reflect(const vec3 & incident, const vec3 & normal)
{
	return incident - 2.f * normal * Dot(incident, normal) / normal.LengthSquared();
}

vec4 Reflect(const vec4 & incident, const vec4 & normal)
{
	return incident - 2.f * normal * Dot(incident, normal) / normal.LengthSquared();
}

vec2 Refract(const vec2 & incident, const vec2 & normal, float index)
{
	return vec2();
}

vec3 Refract(const vec3 & incident, const vec3 & normal, float index)
{
	return vec3();
}

vec4 Refract(const vec4 & incident, const vec4 & normal, float index)
{
	return vec4();
}

vec2 operator*(float scalar, const vec2 & v)
{
	return v * scalar;
}

vec2 operator/(float divisor, const vec2 & v)
{
	return vec2(divisor / v.x, divisor / v.y);
}

vec3 operator*(float scalar, const vec3 & v)
{
	return v * scalar;
}

vec3 operator/(float divisor, const vec3 & v)
{
	return vec3(divisor / v.x, divisor / v.y, divisor / v.z);
}

vec4 operator*(float scalar, const vec4 & v)
{
	return v * scalar;
}

vec4 operator/(float divisor, const vec4 & v)
{
	return vec4(divisor / v.x, divisor / v.y, divisor / v.z, divisor / v.w);
}

float Distance(const vec2 & v1, const vec2 & v2)
{
	return (v1 - v2).Length();
}

float Distance(const vec3 & v1, const vec3 & v2)
{
	return (v1 - v2).Length();
}

float Distance(const vec4 & v1, const vec4 & v2)
{
	return (v1 - v2).Length();
}

float Dot(const vec2 & v1, const vec2 & v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float Dot(const vec3 & v1, const vec3 & v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Dot(const vec4 & v1, const vec4 & v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

vec3 Cross(const vec3 & v1, const vec3 & v2)
{
	return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

quat::quat(float X, float Y, float Z, float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}
