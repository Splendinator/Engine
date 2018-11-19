#pragma once
#include <ostream>



template <class E, unsigned int LENGTH>
class Vector
{
public:
	E data[LENGTH];

	Vector() {};
	Vector(const E(&e)[LENGTH]) { memcpy(data, e, sizeof(E[LENGTH])); };
	Vector(const Vector<E, LENGTH> &v) { memcpy(data, v.data, sizeof(E[LENGTH])); };

	Vector operator+(const Vector &v) const;
	Vector &operator+=(const Vector &v);
	Vector operator*(const E x) const;
	Vector operator-(const Vector &v) const;


	E &operator[] (int x) { return data[x]; };
	E operator[] (int x) const { return data[x]; };

	friend std::ostream &operator<<(std::ostream &os, const Vector &v) {

		for (int i = 0; i < LENGTH; ++i) {
			if (v.data[i] > 0) os << "+";
			os << v.data[i];
			if (i != LENGTH - 1) os << ", ";
		}

		return os;
	};

	Vector &normalise();

	~Vector() {};
};



//TYPEDEFS
typedef Vector<float, 2> Vector2;
typedef Vector<float, 3> Vector3;
typedef Vector<float, 4> Vector4;

typedef Vector<int, 2> Vector2i;
typedef Vector<int, 3> Vector3i;



template <class E, unsigned int LENGTH>
inline Vector<E, LENGTH> Vector<E, LENGTH>::operator+(const Vector<E, LENGTH> &v) const {
	Vector<E, LENGTH> ret;
	for (int i = 0; i < LENGTH; ++i)
		ret.data[i] = data[i] + v.data[i];
	return ret;
}

template <class E, unsigned int LENGTH>
inline Vector<E, LENGTH> &Vector<E, LENGTH>::operator+=(const Vector<E, LENGTH> &v) {
	for (int i = 0; i < LENGTH; ++i)
		data[i] += v.data[i];
	return *this;
}


template <class E, unsigned int LENGTH>
inline Vector<E, LENGTH> Vector<E, LENGTH>::operator*(const E x) const {
	Vector<E, LENGTH> v = data;
	for (int i = 0; i < LENGTH; ++i)
		v.data[i] *= x;
	return v;
}

template<class E, unsigned int LENGTH>
inline Vector<E, LENGTH> Vector<E, LENGTH>::operator-(const Vector & v) const
{
	Vector<E, LENGTH> ret = data;
	for (int i = 0; i < LENGTH; ++i)
		ret.data[i] -= v.data[i];
	return ret;
}


template<class E, unsigned int LENGTH>
inline Vector<E,LENGTH> &Vector<E,LENGTH>::normalise()
{
	E tot = 0;
	
	for (int i = 0; i < LENGTH; ++i)
		tot += abs(data[i]);

	for (int i = 0; i < LENGTH; ++i)
		data[i] /= tot;

	return *this;
}

namespace Util {
	inline Vector3 cross(Vector3 a, Vector3 b) {
		return Vector3({ a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0] });
	}
}