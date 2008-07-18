#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include "platform.h"
#include <assert.h>

namespace pammo
{

struct Vector2
{
		float x, y;
		
		Vector2()
		{
			x = 0;
			y = 0;
		}
		
		Vector2(Vector2 const& rhs)
		{
			*this = rhs;
		}
		
		Vector2(float v)
		{
			this->x = v;
			this->y = v;
		}
		
		Vector2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		
		Vector2& operator=(Vector2 const& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			return *this;
		}
		
		bool operator==(Vector2 const& rhs) const
		{
			return x == rhs.x && y == rhs.y;
		}
		
		bool operator!=(Vector2 const& rhs) const
		{
			return !(x == rhs.x && y == rhs.y);
		}

        float operator[](uint32_t i) const
		{
			switch(i)
			{
				case 0:
					return x;
				case 1:
					return y;
				default:
					assert(0);
			}
            // To make win32 compiler happy.
            return x;
        }
		
		float& operator[](uint32_t i)
		{
			switch(i)
			{
				case 0:
					return x;
				case 1:
					return y;
				default:
					assert(0);
			}
            // To make win32 compiler happy.
            return x;
        }
		
		Vector2 operator-() const
		{
			return Vector2(-x, -y);
		}
		
		Vector2 operator+(Vector2 const& rhs) const
		{
			Vector2 tmp(*this);
			tmp += rhs;
			return tmp;
		}
		
		Vector2& operator+=(Vector2 const& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		
		Vector2 operator-(Vector2 const& rhs) const
		{
			Vector2 tmp(*this);
			tmp -= rhs;
			return tmp;
		}
		
		Vector2& operator-=(Vector2 const& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		
		Vector2 operator*(Vector2 const& rhs) const
		{
			Vector2 tmp(*this);
			tmp *= rhs;
			return tmp;
		}
		
		Vector2& operator*=(Vector2 const& rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			return *this;
		}
		
		Vector2 operator/(Vector2 const& rhs) const
		{
			Vector2 tmp(*this);
			tmp /= rhs;
			return tmp;
		}
		
		Vector2& operator/=(Vector2 const& rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
			return *this;
		}
};

static inline float dot(Vector2 const& lhs, Vector2 const& rhs)
{
	return lhs.x*rhs.x + lhs.y*rhs.y;
}

static inline float magnitude(Vector2 const& rhs)
{
	return sqrt(rhs.x*rhs.x + rhs.y*rhs.y);
}

static inline Vector2 normalize(Vector2 const& rhs)
{
	return rhs / Vector2(magnitude(rhs));
}

}

#endif