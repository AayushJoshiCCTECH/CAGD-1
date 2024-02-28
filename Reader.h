#pragma once

#include "Triangulation.h"


class Reader
{

public:
    Reader();
    ~Reader();

    void readSTL(string &filepath ,Triangulation& triangulation);
private:
};