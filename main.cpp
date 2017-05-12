/*
 *   Copyright (c) 2007 John Weaver
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

/*
 * Some example code.
 *
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include "HungaryAlgo.h"
#include<cstdint>


int main(int argc, char *argv[])
{
    int nrows = 3;
    int ncols = 3;
    cv::Mat matrix(nrows, ncols, CV_64F);
    double values[] = {1,2,3,2,4,6,3,6,9};

    for ( int row = 0 ; row < nrows ; row++ )
    {
        for ( int col = 0 ; col < ncols ; col++ )
        {
            matrix.at<double>(row,col) = values[row*ncols+col];
        }
    }

    // Display begin matrix state.
    for ( int row = 0 ; row < nrows ; row++ )
    {
        for ( int col = 0 ; col < ncols ; col++ )
        {
            std::cout << matrix.at<double>(row,col) << ",";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Apply Hungary algorithm to matrix.
    HungaryAlgo hung(matrix);
    hung.run();

    for ( int row = 0 ; row < nrows ; row++ )
    {
        for ( int col = 0 ; col < ncols ; col++ )
        {
            std::cout << unsigned(hung.mask.at<uint8_t>(row,col)) << ",";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    return 0;
}