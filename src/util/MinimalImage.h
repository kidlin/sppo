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
#include "algorithm"

namespace dso
{

template<typename T>
class MinimalImage
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	int w;
	int h;
	T* data;

	/*
	 * creates minimal image with own memory
	 */
	inline MinimalImage(int w_, int h_) : w(w_), h(h_)
	{
		data = new T[w*h];
		ownData=true;
	}

	/*
	 * creates minimal image wrapping around existing memory
	 */
	inline MinimalImage(int w_, int h_, T* data_) : w(w_), h(h_)
	{
		data = data_;
		ownData=false;
	}

	inline ~MinimalImage()
	{
		if(ownData) delete [] data;
	}

	inline MinimalImage* getClone()
	{
		MinimalImage* clone = new MinimalImage(w,h);
		memcpy(clone->data, data, sizeof(T)*w*h);
		return clone;
	}


	inline T& at(int x, int y) {return data[(int)x+((int)y)*w];}
	inline T& at(int i) {return data[i];}

	inline void setBlack()
	{
		memset(data, 0, sizeof(T)*w*h);
	}

	inline void setConst(T val)
	{
		for(int i=0;i<w*h;i++) data[i] = val;
	}

	inline void setPixel1(const float &u, const float &v, T val)
	{
		at(u+0.5f,v+0.5f) = val;
	}

	inline void setPixel4(const float &u, const float &v, T val)
	{
		at(u+1.0f,v+1.0f) = val;
		at(u+1.0f,v) = val;
		at(u,v+1.0f) = val;
		at(u,v) = val;
	}

	inline void setPixel9(const int &u, const int &v, T val)
	{
		at(u+1,v-1) = val;
		at(u+1,v) = val;
		at(u+1,v+1) = val;
		at(u,v-1) = val;
		at(u,v) = val;
		at(u,v+1) = val;
		at(u-1,v-1) = val;
		at(u-1,v) = val;
		at(u-1,v+1) = val;
	}

	inline void setPixelCirc(const int &u, const int &v, T val)
	{
		for(int i=-3;i<=3;i++)
		{
			at(u+3,v+i) = val;
			at(u-3,v+i) = val;
			at(u+2,v+i) = val;
			at(u-2,v+i) = val;

			at(u+i,v-3) = val;
			at(u+i,v+3) = val;
			at(u+i,v-2) = val;
			at(u+i,v+2) = val;
		}
	}

    inline void setPixelVerLine(const int &u, T val){
	    for(int i=0;i<h;i++)
	        at(u,i)=val;
	}

	inline void setPixelHorLine(const int &v, T val){
	    for(int i=0;i<w;i++)
	        at(i,v)=val;
	}

    inline void setPixelLine(float u1, float v1, float u2, float v2, T val){
	    if(u1>u2){
	        float u3=u1,v3=v1;
	        u1=u2;v1=v2;
	        u2=u3;v2=v3;
	    }
	    if(abs(u1-u2)<1){
	        int low=std::min(v1,v2);
	        int high=std::max(v1,v2);
	        for(int i=low;i<high;i++)
	            at(i,u1)=val;
	        return;
	    }
	    float k=(v2-v1)/(u2-u1);
	    for(int u=u+0.5f;u<u2+0.5f;u++){
	        float v=k*u+v1+0.5f;
	        at(u,v)=val;
	    }
	}

	inline void setPixelRect(int u1, int u2, int v1, int v2, T val){
	    for(int u=u1;u<=u2&&u<w&&v2<h;u++){
	        at(u,v1)=val;
	        at(u,v2)=val;
            at(u,v1+1)=val;
            at(u,v2-1)=val;
	    }
	    for(int v=v1;v<=v2&&v<h&&u2<w;v++){
	        at(u1,v)=val;
	        at(u2,v)=val;
	        at(u1+1,v)=val;
	        at(u2-1,v)=val;
	    }
	}

private:
	bool ownData;
};

typedef Eigen::Matrix<unsigned char,3,1> Vec3b;
typedef MinimalImage<float> MinimalImageF;
typedef MinimalImage<Vec3f> MinimalImageF3;
typedef MinimalImage<unsigned char> MinimalImageB;
typedef MinimalImage<Vec3b> MinimalImageB3;
typedef MinimalImage<unsigned short> MinimalImageB16;

}

