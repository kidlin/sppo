/**
* This file is part of DSO.
* 
* Copyright 2016 Technical University of Munich and Intel.
* Developed by Jakob Engel <engelj at in dot tum dot de>,
* for more information see <http://vision.in.tum.de/dso>.
* If you use this code, please cite the respective publications as
* listed on the above website.
*
* DSO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DSO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DSO. If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

 
#include "util/NumType.h"

namespace dso
{
struct RawResidualJacobian
{
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	// ================== new structure: save independently =============.
	VecNRf resF;

	// the two rows of d[x,y]/d[xi].
	Vec6f Jpdxi[2];			// 2x6

	// the two rows of d[x,y]/d[C].
	VecCf Jpdc[2];			// 2x4

	// the two rows of d[x,y]/d[idepth].
	Vec2f Jpdd;				// 2x1

	// the two columns of d[r]/d[x,y].
	VecNRf JIdx[2];			// 9x2

	// = the two columns of d[r] / d[ab]
	VecNRf JabF[2];			// 9x2


	// = JIdx^T * JIdx (inner product). Only as a shorthand.
	Mat22f JIdx2;				// 2x2
	// = Jab^T * JIdx (inner product). Only as a shorthand.
	Mat22f JabJIdx;			// 2x2
	// = Jab^T * Jab (inner product). Only as a shorthand.
	Mat22f Jab2;			// 2x2

};

struct PlaneResidualJacobian
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    inline explicit PlaneResidualJacobian(int num):num(num)
    {
        resF.resize(num);
        Jpdn.resize(num);
        JIdx2.resize(num);
        JabJIdx.resize(num);
        Jab2.resize(num);
        for(int i=0;i<2;i++){
            Jpdxi[i].resize(num);
            Jpdc[i].resize(num);
            JIdx[i].resize(num);
            JabF[i].resize(num);
        }
    }

    int num;

    std::vector<float> resF;

    // the two rows of d[x,y]/d[xi].
    std::vector<Vec6f> Jpdxi[2];		// 2x6

    // the two rows of d[x,y]/d[C].
    std::vector<VecCf> Jpdc[2];			// 2x4

    // the two rows of d[x,y]/d[iNormal]
    std::vector<Mat23f> Jpdn;            //2x3

    // the two columns of d[r]/d[x,y].
    std::vector<float> JIdx[2];			// nx2

    // = the two columns of d[r] / d[ab]
    std::vector<float> JabF[2];			// nx2


    // = JIdx^T * JIdx (inner product). Only as a shorthand.
    std::vector<Mat22f> JIdx2;				// 2x2
    // = Jab^T * JIdx (inner product). Only as a shorthand.
    std::vector<Mat22f> JabJIdx;			// 2x2
    // = Jab^T * Jab (inner product). Only as a shorthand.
    std::vector<Mat22f> Jab2;			// 2x2
};
}

