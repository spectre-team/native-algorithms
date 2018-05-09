/*
 * Matrix.h
 * Provides temporary implementation of matrix class that stores
 * its contents contiguously in the memory.
 *
Copyright 2017 Michal Gallus

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#pragma once
#include "DataTypes.h"

namespace spectre::unsupervised::gmm
{
/// <summary>
/// Struct serves as a temporary matrix representation.
/// It stores its data contigiuously in the memory.
/// </summary>
template <class Type>
struct Matrix
{
    /// <summary>
    /// Constructor initializing the data based on given dimensions.
    /// </summary>
    /// <param name="height">Height of the array.</param>
    /// <param name="width">Width of the array.</param>
    Matrix(unsigned height, unsigned width)
    {
        data = new Type*[height];
        if (height > 0)
        {
            data[0] = new Type[width * height]; // allocate data once
            for (unsigned i = 1; i < height; ++i)
                data[i] = data[0] + i * width; // assign pointers
        }
        memset(data[0], 0, width * height * sizeof(Type));
    }


    /// <summary>
    /// Deallocate data and pointers created using constructor.
    /// </summary>
    ~Matrix()
    {
        delete[] data[0]; // delete allocated data
        delete[] data; // delete allocated pointers
    }

    /// <summary>
    /// Returns a row indicated by passed index.
    /// </summary>
    /// <param name="index">Index of the row.</param>
    /// <returns>Row indicated by passed index.</returns>
    Type* operator[](unsigned index)
    {
        return data[index];
    }

    /// <summary>
    /// Prints matrix to standard output.
    /// </summary>
    /// <param name="height">Height of the array.</param>
    /// <param name="width">Width of the array.</param>
    void PrintMatrix(unsigned width, unsigned height);

    /// <summary>
    /// Matrix data in a form of 2-dimensional array.
    /// </summary>
    Type **data;
};

template<>
inline void Matrix<Index>::PrintMatrix(unsigned width, unsigned height)
{
    for (unsigned i = 0; i < height; i++)
    {
        for (unsigned j = 0; j < width; j++)
        {
            if (data[i][j] == std::numeric_limits<Index>::infinity())
                printf("INF \t");
            else
                printf("%u\t", data[i][j]);
        }
        printf("\n");
    }
}

template<>
inline void Matrix<DataType>::PrintMatrix(unsigned width, unsigned height)
{
    for (unsigned i = 0; i < height; i++)
    {
        for (unsigned j = 0; j < width; j++)
        {
            if (data[i][j] == std::numeric_limits<DataType>::infinity())
                printf("INF \t");
            else
                printf("%.3g\t", data[i][j]);
        }
        printf("\n");
    }
}
}
