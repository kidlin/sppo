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
#include "vector"
#include <math.h>
#include <FullSystem/HessianBlocks.h>
#include "OptimizationBackend/RawResidualJacobian.h"

namespace dso
{

class PointFrameResidual;
class PlaneFrameResidual;
class CalibHessian;
class FrameHessian;
class PointHessian;
class PlaneHessian;

class EFResidual;
class EFPResidual;
class EFPoint;
class EFFrame;
class EFPlane;
class EnergyFunctional;






class EFResidual
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

	inline EFResidual(PointFrameResidual* org, EFPoint* point_, EFFrame* host_, EFFrame* target_) :
		data(org), point(point_), host(host_), target(target_)
	{
		isLinearized=false;
		isActiveAndIsGoodNEW=false;
		J = new RawResidualJacobian();
		assert(((long)this)%16==0);
		assert(((long)J)%16==0);
	}
	inline ~EFResidual()
	{
		delete J;
	}


	void takeDataF();


	void fixLinearizationF(EnergyFunctional* ef);


	// structural pointers
	PointFrameResidual* data;
	int hostIDX, targetIDX;
	EFPoint* point;
	EFFrame* host;
	EFFrame* target;
	int idxInAll;

	RawResidualJacobian* J;

	VecNRf res_toZeroF;
	Vec8f JpJdF;


	// status.
	bool isLinearized;

	// if residual is not OOB & not OUTLIER & should be used during accumulations
	bool isActiveAndIsGoodNEW;
	inline const bool &isActive() const {return isActiveAndIsGoodNEW;}
};

class EFPResidual
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    EFPResidual(PlaneFrameResidual* org, EFPlane* plane_, EFFrame* host_, EFFrame* target_);

    inline ~EFPResidual()
    {
        delete J;
    }


    void takeDataF();


    void fixLinearizationF(EnergyFunctional* ef);


    // structural pointers
    PlaneFrameResidual* data;
    int hostIDX, targetIDX;
    EFPlane* plane;
    EFFrame* host;
    EFFrame* target;
    int idxInAll;

    PlaneResidualJacobian* J;

    std::vector<float> res_toZeroF;
    Mat83f JpJnF;

    // static number, not changed
    int num;

    // status.
    bool isLinearized;

    // if residual is not OOB & not OUTLIER & should be used during accumulations
    bool isActiveAndIsGoodNEW;
    inline const bool &isActive() const {return isActiveAndIsGoodNEW;}
};


enum EFPointStatus {PS_GOOD=0, PS_MARGINALIZE, PS_DROP};

class EFPoint
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	EFPoint(PointHessian* d, EFFrame* host_) : data(d),host(host_)
	{
		takeData();
		stateFlag=EFPointStatus::PS_GOOD;
	}
	void takeData();

	PointHessian* data;



	float priorF;
	float deltaF;


	// constant info (never changes in-between).
	int idxInPoints;
	EFFrame* host;

	// contains all residuals.
	std::vector<EFResidual*> residualsAll;

	float bdSumF;
	float HdiF;
	float Hdd_accLF;
	VecCf Hcd_accLF;
	float bd_accLF;
	float Hdd_accAF;
	VecCf Hcd_accAF;
	float bd_accAF;


	EFPointStatus stateFlag;
};

enum EFPlaneStatus {PL_GOOD=0, PL_MARGINALIZE, PL_DROP};

class EFPlane
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    EFPlane(PlaneHessian* d, EFFrame* host_) : data(d),host(host_)
    {
        takeData();
        stateFlag=EFPlaneStatus::PL_GOOD;
    }

    void takeData();

    PlaneHessian* data;

    Mat33f priorF;
    Vec3f deltaF;

    int num;

    // constant info (never changes in-between).
    int idxInPlanes;
    EFFrame* host;

    // contains all residuals.
    std::vector<EFPResidual*> residualsAll;

    Vec3f bnSumF;
    Mat33f HniF;
    Mat33f Hdn_accLF;
    MatC3f Hcn_accLF;
    Vec3f bn_accLF;
    Mat33f Hdn_accAF;
    MatC3f Hcn_accAF;
    Vec3f bn_accAF;

    EFPlaneStatus stateFlag;

};

class EFFrame
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	EFFrame(FrameHessian* d) : data(d)
	{
		takeData();
	}
	void takeData();


	Vec8 prior;				// prior hessian (diagonal)
	Vec8 delta_prior;		// = state-state_prior (E_prior = (delta_prior)' * diag(prior) * (delta_prior)
	Vec8 delta;				// state - state_zero.



	std::vector<EFPoint*> points;
	std::vector<EFPlane*> planes;
	FrameHessian* data;
	int idx;	// idx in frames.

	int frameID;
};

}

