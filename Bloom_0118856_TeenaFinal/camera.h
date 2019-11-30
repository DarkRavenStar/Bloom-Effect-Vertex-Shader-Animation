#ifndef _CAMERA_H
#define _CAMERA_H

#include "Angle_Util/Matrix.h"
#include "Angle_Util/Vector.h"

class Camera
{
//! \name Attributes.
//@{
protected:
	//! Camera position.
	Vector3 mPosition;

	//! Rotation matrix.
	Matrix4 mOrient;

	//! Flag to tell if we need to update world matrix.
	mutable bool mWorldMatrixDirty;

	//! World matrix. (Cache)
	mutable Matrix4 mWorld;

	//! View matrix. (Cache)
	mutable Matrix4 mView;

//@}

//! \name Methods.
//@{
public:
	Camera() :
	mPosition(0.0f, 0.0f, 0.0f),
	mOrient(Matrix::makeIdentityMatrix()),
	mWorldMatrixDirty(true)
	{ }

	//! Set camera position.
	void setPosition(float x, float y, float z)
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
		mWorldMatrixDirty = true;
	}

	//! Move camera in world space.
	void translate(float x, float y, float z)
	{
		mPosition.x += x;
		mPosition.y += y;
		mPosition.z += z;
		mWorldMatrixDirty = true;
	}

	//! Move camera in local orient space.
	void translateLocal(float x, float y, float z)
	{
		Vector3 localOrientOffset = mOrient * Vector3(x, y, z);
		mPosition += localOrientOffset;
		mWorldMatrixDirty = true;
	}

	//! Rotate camera in world space.
	void rotate(float angle, const Vector& axis)
	{
		mOrient = Matrix::makeRotateMatrix(angle, axis) * mOrient;
		mWorldMatrixDirty = true;
	}

	//! Rotate camera in local orient space.
	void rotateLocal(float angle, const Vector& axis)
	{
		mOrient = mOrient * Matrix::makeRotateMatrix(angle, axis);
		mWorldMatrixDirty = true;
	}

	//! Return view matrix.
	inline const Matrix& getViewMatrix() const
	{
		if (mWorldMatrixDirty)
		{
			mWorld = Matrix::makeTranslationMatrix(mPosition) * mOrient;
			mView = mWorld.inverse();
			mWorldMatrixDirty = false;
		}

		return mView;
	}

//@}
};

#endif
