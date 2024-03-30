# STL Viewer with QT OpenGL

This project is a simple STL file viewer implemented using QT OpenGL.

## Description

The STL Viewer is a lightweight application designed to provide users with a simple yet effective tool for visualizing 3D models stored in STL files. Whether you're a hobbyist, engineer, or designer, the STL Viewer offers an intuitive interface for exploring intricate 3D geometries.

## Usage

To use the STL Viewer, follow these steps:

1. Build the project.
2. Run the executable.
3. Load an STL file using the file menu.
4. Visualize a solid model.

## Features

- Display 3D models from STL files.
- Load STL files.
- Smooth rendering using OpenGL.

## Installation

To build and run the STL Viewer, you need to have the following dependencies installed:

- QT framework
- OpenGL
- C++ compiler

Clone the repository:

```bash
git clone https://github.com/AayushJoshiCCTECH/CAGD-1.git
```
## Sample Code

### TriangleWindow.cpp
```cpp
#include "TriangleWindow.h"
#include <QScreen>
#include "Triangulation.h"
#include "Point3D.h"
#include "Reader.h"
#include "Writer.h"
#include <random>
//! [3]
static const char* vertexShaderSource =
"attribute highp vec4 posAttr;\n"
"attribute lowp vec4 colAttr;\n"
"varying lowp vec4 col;\n"
"uniform highp mat4 matrix;\n"
"void main() {\n"
"   col = colAttr;\n"
"   gl_Position = matrix * posAttr;\n"
"}\n";

static const char* fragmentShaderSource =
"varying lowp vec4 col;\n"
"void main() {\n"
"   gl_FragColor = col;\n"
"}\n";
//! [3]

//! [4]
void TriangleWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = m_program->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
    glEnable(GL_DEPTH_TEST);
}
//! [4]

//! [5]
void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -102);
    matrix.scale(0.5f);
    matrix.rotate(1500.0f * m_frame / screen()->refreshRate(), 1, 0, 0);
    matrix.rotate(150.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
    matrix.rotate(150.0f * m_frame / screen()->refreshRate(), 0, 0, 1);

    m_program->setUniformValue(m_matrixUniform, matrix);


    Reader stlReader;
    string filePath = "Sphere.stl";

    Triangulation triangulationObj;
    stlReader.readSTL(filePath, triangulationObj);

    
    Writer writer;
    vector<double> vertices = writer.write(triangulationObj);

    /*std::random_device random;
    std::mt19937 generate(random());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);*/
    GLfloat* colors = new GLfloat[vertices.size()];
    
    for (int i = 0; i < vertices.size(); i+=3)
    {
       // colors[i] = distribution(generate);
        if (i < vertices.size() / 2)
        {
            colors[i] = 1.0;
            colors[i + 1] = 0.0;
            colors[i + 2] = 1.0;
        }
        else
        {
            colors[i] = 0.5;
            colors[i + 1] = 1.0;
            colors[i + 2] = 1.0;
        }
    }
    /* static const GLfloat colors[] = {
         1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f

     };*/

    glVertexAttribPointer(m_posAttr, 3, GL_DOUBLE, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);


    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);

    m_program->release();

    ++m_frame;
}
```
### main.cpp

```cpp
// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "openglwindow.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include <QtMath>
#include "TriangleWindow.h"



//! [2]
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    TriangleWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}
//! [2]


//! [3]

```
