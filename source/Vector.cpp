//
//  Vector.cpp
//  FRE6883Spring2018TeamProject
//
//  Created by Ji Yang on 4/27/18.
//  Copyright © 2018 Ji Yang. All rights reserved.
//

#include "Vector.h"

Vector operator+(const Vector& V,const Vector& W)
{
    unsigned long d = V.size();
    Vector U(d);
    for (int j=0; j<d; j++) U[j] = V[j] + W[j];
    return U;
}

Vector operator-(const Vector& V,const Vector& W)
{
    unsigned long d = V.size();
    Vector U(d);
    for (int j=0; j<d; j++) U[j] = V[j] - W[j];
    return U;
}

Vector operator/(const Vector& V,const Vector& W)
{
    unsigned long d = V.size();
    Vector U(d);
    for (int j=0; j<d; j++) U[j] = V[j] / W[j];
    return U;
}

Vector operator/(const Vector& V, const int x)
{
    double y = double(x);
    unsigned long d = V.size();
    Vector U(d);
    for (int j=0; j<d; j++) U[j] = V[j] / y;
    return U;
}


ostream & operator<<(ostream & out, Vector & V)
{
    for (Vector::iterator itr= V.begin(); itr != V.end(); itr++)
        out << *itr << " ";
    out << endl;
    return out;
}

ostream & operator<<(ostream & out, vector<int> & V)
{
    for (vector<int>::iterator itr= V.begin(); itr != V.end(); itr++)
        out << *itr << " ";
    out << endl;
    return out;
}
