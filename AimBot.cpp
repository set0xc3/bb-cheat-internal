#include "AimBot.h"
//#include "AimBot.h"
//
//Vector3 nClamp(Vector3 angle)
//{
//	if (angle.x > 90.0f && angle.x <= 180.0f)
//		angle.x = 90.0f;
//	if (angle.x > 180.0f)
//		angle.x = angle.x - 360.0f;
//	if (angle.x < -90.0f)
//		angle.x = -90.0f;
//	if (angle.y > 180.0f)
//		angle.y = angle.y - 360.0f;
//	if (angle.y < -180.0f)
//		angle.y = angle.y + 360.0f;
//	return angle;
//}
//
//Vector3 nNorm(Vector3 angle)
//{
//	if (angle.x > 180)
//		angle.x -= 360.0f;
//	if (angle.x < 180)
//		angle.x += 360.0f;
//	if (angle.y > 180)
//		angle.y -= 360.0f;
//	if (angle.y < 180)
//		angle.y += 360.0f;
//	return angle;
//}
//
//Vector3 nSubtract(Vector3 src, Vector3 dst)
//{
//	Vector3 diff;
//	diff.x = src.x - dst.x;
//	diff.y = src.y - dst.y;
//	diff.z = src.z - dst.z;
//	return diff;
//}
//
//float nMagnitude(Vector3 vec)
//{
//	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
//}
//
//float nDistance(Vector3 src, Vector3 dst)
//{
//	Vector3 diff = nSubtract(src, dst);
//	diff = nClamp(nNorm(diff));
//	return nMagnitude(diff);
//}
//
//Vector3 nCalcAngle(Vector3 src, Vector3 dst)
//{
//	Vector3 delta = nSubtract(src, dst);
//	Vector3 angle;
//
//	angle.y = atan2f(delta.x, delta.z) * (180.0f / PI);//Pith
//	angle.x = -asin(delta.y / nMagnitude(delta)) * (180.0f / PI);//Yaw
//	angle.z = 0;
//
//	return angle;
//}
//
//

