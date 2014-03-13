#include "Transform.h"

namespace xihad { namespace ngn
{
	
	Transform::Transform()
	{
		resetScale();
		resetRotate();
		resetTranslate();
	}

	void Transform::setFromMatrix( const Matrix& mat )
	{
		mTranslate = mat.getTranslation();
		mRotation = mat.getRotationDegrees();
		mScale = mat.getScale();
	}

}}

