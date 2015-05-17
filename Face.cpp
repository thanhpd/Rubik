#include "Face.h"

Face::Face() {
    for (int i = 0; i < NUM_POINTS; ++i) {
        vertex[i].set(0.0, 0.0, 0.0);
    }
    center.set(0.0, 0.0, 0.0);
}

Face::Face(Point3D v0, Point3D v1, Point3D v2, Point3D v3, GLuint textureId) {
    set(v0, v1, v2, v3, textureId);
}

void Face::set(const Face &f) {
    set(f.vertex[0], f.vertex[1], f.vertex[2], f.vertex[3], f.textureId);
}


void Face::set(Point3D v0, Point3D v1, Point3D v2, Point3D v3, GLuint textureId) {
    vertex[0].set(v0);
    vertex[1].set(v1);
    vertex[2].set(v2);
    vertex[3].set(v3);

    float cenX = (vertex[0].getX() + vertex[1].getX() + vertex[2].getX() + vertex[3].getX()) / 4;
    float cenY = (vertex[0].getY() + vertex[1].getY() + vertex[2].getY() + vertex[3].getY()) / 4;
    float cenZ = (vertex[0].getZ() + vertex[1].getZ() + vertex[2].getZ() + vertex[3].getZ()) / 4;

    center.set(cenX, cenY, cenZ);

    this->textureId = textureId;
}

void Face::draw() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(vertex[0].getX(), vertex[0].getY(), vertex[0].getZ());
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(vertex[1].getX(), vertex[1].getY(), vertex[1].getZ());
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(vertex[2].getX(), vertex[2].getY(), vertex[2].getZ());
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(vertex[3].getX(), vertex[3].getY(), vertex[3].getZ());
    glEnd();
}

void Face::rotate(Vector3D u, float angle) {
	for (int i = 0; i < NUM_POINTS; i++ ) {
		vertex[i].rotate(u.getX(), u.getY(), u.getZ(), angle);
	}
    center.rotate(u.getX(), u.getY(), u.getZ(), angle);
}

Point3D Face::getCenter() {
    return center;
}

