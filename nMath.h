#pragma once
#include <Vectors.h>

Vector3 Clamp(Vector3 angle)
{
	if (angle.x > 90.0f && angle.x <= 180.0f)
		angle.x = 90.0f;
	if (angle.x > 180.0f)
		angle.x = angle.x - 360.0f;
	if (angle.x < -90.0f)
		angle.x = -90.0f;
	if (angle.y > 180.0f)
		angle.y = angle.y - 360.0f;
	if (angle.y < -180.0f)
		angle.y = angle.y + 360.0f;
	return angle;
}

Vector3 Norm(Vector3 angle)
{
	if (angle.x > 180)
		angle.x -= 360.0f;
	if (angle.x < 180)
		angle.x += 360.0f;
	if (angle.y > 180)
		angle.y -= 360.0f;
	if (angle.y < 180)
		angle.y += 360.0f;
	return angle;
}

Vector3 Subtract(Vector3 src, Vector3 dst)
{
	Vector3 diff;
	diff.x = src.x - dst.x;
	diff.y = src.y - dst.y;
	diff.z = src.z - dst.z;
	return diff;
}

float Magnitude(Vector3 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float Distance(Vector3 src, Vector3 dst)
{
	Vector3 diff = Subtract(src, dst);
	diff = Clamp(Norm(diff));
	return Magnitude(diff);
}

Vector3 CalcAngle(Vector3 src, Vector3 dst)
{
	//dst.y = (dst.y += 2.379999924f);

	Vector3 delta = Subtract(src, dst);
	Vector3 angle;

	angle.x = atan2f(delta.x, delta.z) * (180.0f / PI);//Pith
	angle.y = -asin(delta.y / Magnitude(delta)) * (180.0f / PI);//Yaw
	angle.z = 0;

	return angle;
}
