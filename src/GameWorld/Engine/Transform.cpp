#include "Transform.h"

namespace xihad { namespace ngn
{
	
	Transform::Transform()
	{
		resetScale();
		resetRotate();
		resetTranslate();
	}

	void Transform::resetScale(float sx, float sy, float sz)
	{
		mScale[X_AXIS_INDEX] = sx;
		mScale[Y_AXIS_INDEX] = sy;
		mScale[Z_AXIS_INDEX] = sz;
	}

	void Transform::resetRotate(float rx, float ry, float rz)
	{
		mRotation[X_AXIS_INDEX] = rx;
		mRotation[Y_AXIS_INDEX] = ry;
		mRotation[Z_AXIS_INDEX] = rz;
	}

	void Transform::resetTranslate(float tx, float ty, float tz)
	{
		mTranslate[X_AXIS_INDEX] = tx;
		mTranslate[Y_AXIS_INDEX] = ty;
		mTranslate[Z_AXIS_INDEX] = tz;
	}

}}

