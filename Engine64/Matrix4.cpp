#include "Matrix4.h"



Matrix4::Matrix4()
{
}


Matrix4::~Matrix4()
{
}



std::ostream &operator<<(std::ostream &o, Matrix4 m) {

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			o << m.value[i * 4 + j] << " ";
		}
		o << std::endl;
	}
	return o;
}