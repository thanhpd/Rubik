#ifndef RUBIK_POINT3D_H
#define RUBIK_POINT3D_H

class Point3D {
private:
    float x, y, z;

public:
    Point3D();

    Point3D(float, float, float);

    Point3D(const Point3D &other);

    ~Point3D();

    void set(float, float, float);

    void set(const Point3D &other);

    float getX();

    float getY();

    float getZ();

    void rotate(float ux, float uy, float uz, float angle);
};

#endif //RUBIK_POINT3D_H
