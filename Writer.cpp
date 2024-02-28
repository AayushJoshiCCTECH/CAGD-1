
#include <stdexcept>

#include <iostream>

#include <QOpenGLShaderProgram>

#include "Writer.h"
#include "Triangle.h"

vector<double>Writer::write(Triangulation& triangulationObj)

{

    vector<Point3D>& points = triangulationObj.uniquePoints();

    vector<Triangle>& triangles = triangulationObj.triangles();

    vector<double> vertices;

    for (const Triangle& triangle : triangles)

    {

        vertices.push_back(points[triangle.v1()].x());
        vertices.push_back(points[triangle.v1()].y());
        vertices.push_back(points[triangle.v1()].z());

        vertices.push_back(points[triangle.v2()].x());
        vertices.push_back(points[triangle.v2()].y());
        vertices.push_back(points[triangle.v2()].z());

        vertices.push_back(points[triangle.v3()].x());
        vertices.push_back(points[triangle.v3()].y());
        vertices.push_back(points[triangle.v3()].z());

    }

    return vertices;

}