#include <iostream>
#include <string.h>
#include "Mesh.h"
#include "Object.h"
#include "Rasteriser.h"



Rasteriser r;

Mesh *m = Mesh::Triangle();

Object o = Object(m);
Object o2 = Object(m);

//Moving mouse callback
void handleMouse(int x, int y) {
	static int lastx = x;	//Static so retain value between func calls.
	static int lasty = y;

	//o.transform *= Matrix4::translate(float(lastx-x)/100, -float(lasty-y)/100, 0);

	lastx = x;
	lasty = y;

}

//Called by the window once per frame.
void displayCallBack() {
	


}

int main(int argc, char** argv) {

	r.init(argc, argv);

	o.transform *= Matrix4::translate(-0.5, 0, 0) * Matrix4::scale(0.5, 1, 1) * Matrix4::rotation(0, 1, 0);
	o2.transform *= Matrix4::translate(0.5, 0, 0) * Matrix4::rotation(0, 1, 0) * Matrix4::scale(0.5, 1, 1);

	r.addObject(&o);
	

	while (1) {
		o.transform *= Matrix4::rotation(0, 0.01f, 0);
		r.update();
	}
	



	glutPassiveMotionFunc(&handleMouse);
	glutMotionFunc(&handleMouse);
	glutDisplayFunc(&displayCallBack);
	glutIdleFunc(&displayCallBack);
	
	glutMainLoop();

	return 0;
}

