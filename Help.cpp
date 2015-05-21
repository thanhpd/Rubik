#include "Help.h"

Help::Help() { }

Help::Help(int width, int height) {
	this->textureId = loadTexture("help.bmp");
	this->width = width;
	this->height = height;
}

void Help::set(int width, int height) {
	this->textureId = loadTexture("help.bmp");
	this->width = width;
	this->height = height;
}

GLuint Help::getTextureId() {
	return textureId;
}

void Help::draw() {
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0, 0.0);
    glTexCoord2f(800.0f, 0.0f);
    glVertex2f(800.0, 0.0);
    glTexCoord2f(800.0f, 600.0f);
    glVertex2f(800.0, 600.0);
    glTexCoord2f(0.0f, 600.0f);
    glVertex2f(0.0, 600.0);
    glEnd();
}

GLuint Help::loadTexture(Image *image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    return textureId;
}

GLuint Help::loadTexture(string imageName){
	Image *image = loadBMP(imageName.c_str());
	GLuint id = loadTexture(image);
	delete image;
	return id;
}
