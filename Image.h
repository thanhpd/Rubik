#ifndef RUBIK_IMAGE_H
#define RUBIK_IMAGE_H

/**
 * This class represents for an image.
 * An image is specified by an array of pixels values,
 * image width and image height
 *
 **/

class Image {
	public:
	    char *pixels;
	    int width;
	    int height;
	    
	    Image(char*, int, int);
	    ~Image();
};

/*
 * This function describe how to load and image from storage and assign it
 * to a pointer of Image
 *
 */
Image *loadBMP(const char*);


#endif
