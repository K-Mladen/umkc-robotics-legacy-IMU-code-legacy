/******************************************************************************
*  despite weird naming, this is a part of an eigenvalue based error checking *
*   algorithm for the quaternion derived rotation matrix                      *
******************************************************************************/

#include <cmath>
#include "pVector.h"

pVector rotatePOV(pVector vec, pVector rot)
{

  double s = sin(rot.magnitude()),
         c = cos(rot.magnitude());
  double qxx, qxy, qxz, qyx, qyy, qyz, qzx, qzy, qzz;
  double m11, m12, m13, m14, 
         m21, m22, m23, m24,
		 m31, m32, m33, m34,
		 m41, m42, m43, m44;

    qxx=(c+rot.unitComponent(X)*(1-c))*vec.component(X);
	qxy=(rot.unitComponent(X)*rot.unitComponent(Y)*(1-c)
	  - rot.unitComponent(Z)*s)*vec.component(Y);
	qxz=(rot.unitComponent(X)*rot.unitComponent(Z)*(1-c)
		+ rot.unitComponent(Y)*s)*vec.component(Z);
	qyx=(rot.unitComponent(Y)*rot.unitComponent(X)*(1-c)
	  + rot.unitComponent(Z)*s)*vec.component(X);
	qyy=(c+rot.unitComponent(Y)*(1-c))*vec.component(Y);
	qyz=(rot.unitComponent(Y)*rot.unitComponent(Z)*(1-c)
	  - rot.unitComponent(X)*s)*vec.component(Z);
	qzx=(rot.unitComponent(Z)*rot.unitComponent(X)*(1-c)
	  - rot.unitComponent(Y)*s)*vec.component(X);
	qzy=(rot.unitComponent(Z)*rot.unitComponent(Y)*(1-c)
	  + rot.unitComponent(X)*s)*vec.component(Y);
	qzz=(c+rot.unitComponent(Z)*(1-c))*vec.component(Z);

	m11 = qxx - qyy - qzz;
	m12 = qyx + qxy;
	m13 = qzx + qxz;
	m14 = qyz - qzy;

	m21 = qyx + qxy;
	m22 = qyy - qxx - qzz;
	m23 = qzy + qyz;
	m24 = qzx - qxz;

	m31 = qzx + qxz;
	m32 = qzy + qyz;
	m33 = qzz - qxx -qyy;
	m34 = qxy - qyx;

	m41 = qyz - qzy;
	m42 = qzx - qxz;
	m43 = qxy - qyx;
	m44 = qxx + qyy + qzz;

  return newVec;
}
