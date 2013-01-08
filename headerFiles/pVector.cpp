#include "pVector.h"
#include <cmath>

void pVector::resetFlags()
{   //this function needs to be called any time a comp is changed.
    unitflag[0] = unitflag[1] = unitflag[2] = magflag = 0;
    return;
}

pVector::pVector()
{
    //ctor
    comp[0]=0;
    comp[1]=0;
    comp[2]=0;
	resetFlags();
    //flags are already initialized to 0
}

pVector::pVector(double given[3])    {
    for (int i =0; i< 3; i++)   {
        comp[i] = given[i];
    }
    resetFlags();
}

pVector::pVector(double comp0, double comp1, double comp2)
{
    //ctor
    comp[0]=comp0;
    comp[1]=comp1;
    comp[2]=comp2;
	resetFlags();
    //flags are already initialized to 0
}

pVector::~pVector()
{
    //dtor
}



double pVector::component(pDirection dir)
{
    return comp[dir];
}

double pVector::magnitude()
{
    if(!magflag)
    {
        mag = sqrt(comp[0]*comp[0]+comp[1]*comp[1]+comp[2]*comp[2]);
        magflag = 1;
    }
    return mag;
}

double pVector::unitComponent(pDirection dir)
{
    if(!unitflag[dir])
       {
           //component by magnitude, but make sure magnitude has been found.
           unitcomp[dir] = comp[dir]/magnitude();
           unitflag[dir] = 1;
       }
    return unitcomp[dir];
}

void pVector::set(pDirection dir, double val)
{
    comp[dir] = val;
    resetFlags();
}

void pVector::set(double val[3])    {
    for(int i =0; i< 3; i++)    {
        comp[i]=val[i];
    }
}
/*
const pVector pVector::operator=(const pVector& rhs)
{
    if (this != &rhs) // if not self assignment
    {
        //assignment operator
        comp[0]=rhs.comp[0];
        comp[1]=rhs.comp[1];
        comp[2]=rhs.comp[2];
        resetFlags();
    }
    return *this;
}
*/
pVector pVector::operator+=(const pVector& other)
{
    if (other.comp[0]!=0 && other.comp[1]!=0 && other.comp[2]!=0)
    {
        comp[0]+=other.comp[0];
        comp[1]+=other.comp[1];
        comp[2]+=other.comp[2];
        resetFlags();
    }
    return *this;
}

const pVector pVector::operator+(const pVector& other) const
{
    return pVector(*this) += other;
}

pVector pVector::operator-=(const pVector& other)
{
    if (other.comp[0]!=0 && other.comp[1]!=0 && other.comp[2]!=0)
    {
        comp[0]-=other.comp[0];
        comp[1]-=other.comp[1];
        comp[2]-=other.comp[2];
        resetFlags();
    }
    return *this;
}

const pVector pVector::operator-(const pVector& other) const
{
    return pVector(*this) -= other;
}

double& pVector::operator[] (const int nIndex)   {
    return comp[nIndex];
}
