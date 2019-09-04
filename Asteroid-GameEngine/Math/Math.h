#ifndef MATH_H
#define MATH_H
#include <glm/glm.hpp>

struct EulerAngles
{
	double roll, pitch, yaw;  //Z   X   Y
};
struct Quaternion
{
	double w, x, y, z;
};

#include <glm/gtc/constants.hpp>
class Math
{
public:

	static EulerAngles ToEulerAngles(Quaternion q)
	{
		EulerAngles angles;

		// roll (x-axis rotation)
		double sinr_cosp = +2.0 * (q.w * q.x + q.y * q.z);
		double cosr_cosp = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
		angles.roll = atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		double sinp = +2.0 * (q.w * q.y - q.z * q.x);
		if (fabs(sinp) >= 1)
			angles.pitch = copysign(glm::pi<float>() / 2, sinp); // use 90 degrees if out of range
		else
			angles.pitch = asin(sinp);

		// yaw (z-axis rotation)
		double siny_cosp = +2.0 * (q.w * q.z + q.x * q.y);
		double cosy_cosp = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
		angles.yaw = atan2(siny_cosp, cosy_cosp);

		return angles;
	}


	static Quaternion ToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
	{
		// Abbreviations for the various angular functions
		double cy = cos(yaw * 0.5);
		double sy = sin(yaw * 0.5);
		double cp = cos(pitch * 0.5);
		double sp = sin(pitch * 0.5);
		double cr = cos(roll * 0.5);
		double sr = sin(roll * 0.5);

		Quaternion q;
		q.w = cy * cp * cr + sy * sp * sr;
		q.x = cy * cp * sr - sy * sp * cr;
		q.y = sy * cp * sr + cy * sp * cr;
		q.z = sy * cp * cr - cy * sp * sr;

		return q;
	}
	static glm::vec3 QuaternionToGlmVec3(Quaternion _qu)
	{
		EulerAngles angles= ToEulerAngles(_qu);
		return glm::vec3(angles.roll, -angles.pitch, angles.yaw);
	}

};



#endif // !MATH_H


