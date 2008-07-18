/*
 *  transform2.h
 *  Flain
 *
 *  Created by James Marr on 4/13/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __TRANSFORM2_H__
#define __TRANSFORM2_H__

#include "vector2.h"

namespace pammo
{

struct Transform2
{
	float mValues[6];
	
	Transform2()
	{
		memset(mValues, 0, sizeof(mValues));
		mValues[0] = 1;
		mValues[3] = 1;
	}
	
	Transform2(Transform2 const& rhs)
	{
		*this = rhs;
	}
	
	Transform2& operator=(Transform2 const& rhs)
	{
		memcpy(mValues, rhs.mValues, sizeof(mValues));
		return *this;
	}
	
	bool operator==(Transform2 const& rhs)
	{
		return memcmp(mValues, rhs.mValues, sizeof(mValues)) == 0;
	}
	
	float operator[](int i) const
	{
		return mValues[i];
	}
	
	float& operator[](int i)
	{
		return mValues[i];
	}
	
	Transform2 operator*(Transform2 const& rhs) const
	{
		Transform2 tmp(*this);
		tmp *= rhs;
		return tmp;
	}
	
	Transform2& operator*=(Transform2 const& rhs)
	{
		Transform2 lhs(*this);
		
		mValues[0] = rhs[0]*lhs[0] + rhs[1]*lhs[2];
		mValues[1] = rhs[0]*lhs[1] + rhs[1]*lhs[3];
		
		mValues[2] = rhs[2]*lhs[0] + rhs[3]*lhs[2];
		mValues[3] = rhs[2]*lhs[1] + rhs[3]*lhs[3];
		
		mValues[4] = rhs[4]*lhs[0] + rhs[5]*lhs[2] + lhs[4];
		mValues[5] = rhs[4]*lhs[1] + rhs[5]*lhs[3] + lhs[5];
		
		return *this;
	}
	
	void getValues(float* values) const
	{
		memcpy(values, mValues, sizeof(mValues));
	}
	
	void setValues(float const* values)
	{
		memcpy(mValues, values, sizeof(mValues));
	}
	
	static Transform2 createRotation(float rads)
	{
		Transform2 tmp;
		float cosa = cos(rads);
		float sina = sin(rads);
		tmp[0] = cosa;
		tmp[1] = sina;
		tmp[2] = -sina;
		tmp[3] = cosa;
		return tmp;
	}
	
	static Transform2 createTranslation(Vector2 const& distance)
	{
		Transform2 tmp;
		tmp[4] = distance[0];
		tmp[5] = distance[1];
		return tmp;
	}
	
	static Transform2 createScale(Vector2 const& scale)
	{
		Transform2 tmp;
		tmp[0] = scale[0];
		tmp[3] = scale[1];
		return tmp;
	}
};

inline static float det(Transform2 const& t)
{
	assert(0);
}

inline static Transform2 inverse(Transform2 const& t)
{
	Transform2 tmp;
	float det = t[0]*t[3] - t[1]*t[2];
	tmp[0] = t[3]/det;
	tmp[1] = -t[1]/det;
	tmp[2] = -t[2]/det;
	tmp[3] = t[0]/det;
	tmp[4] = (t[2]*t[5] - t[4]*t[3])/det;
	tmp[5] = (t[1]*t[4] - t[5]*t[0])/det;
	return tmp;
}

inline static Vector2& operator*=(Vector2& lhs, Transform2 const& rhs)
{
	Vector2 tmp(lhs);
	
	lhs[0] = tmp[0]*rhs[0] + tmp[1]*rhs[2] + rhs[4];
	lhs[1] = tmp[0]*rhs[1] + tmp[1]*rhs[3] + rhs[5];
	
	return lhs;
}

inline static Vector2 operator*(Vector2 const& lhs, Transform2 const& rhs)
{
	Vector2 tmp(lhs);
	tmp *= rhs;
	return tmp;
}

}

#endif
