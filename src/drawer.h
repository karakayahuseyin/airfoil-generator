#pragma once

#ifndef DRAWER_H
#define DRAWER_H

#include <vector>
#include "airfoil.h"

static std::vector<std::pair<double,double>> polygonPoints; // Çizilecek poligon noktaları

static const int SPLINE_DEGREE = 3;

void draw(const std::vector<AirfoilPoint>& airfoilPoints); // 100 adet nokta çizimi

double BasisFunction(int i, int k, double t, const std::vector<double>& knots);

std::pair<double,double> EvaluateBSpline(
    double t,
    int degree,
    const std::vector<double>& ctrlX,
    const std::vector<double>& ctrlY,
    const std::vector<double>& knots);

std::vector<double> CreateUniformKnotVector(int nCtrl, int degree);

#endif // DRAWER_H