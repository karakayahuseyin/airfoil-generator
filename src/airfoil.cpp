#include <iostream>
#include <cmath>

#include "airfoil.h"

// Sabit katsayılar
const double a0 = 0.2969;
const double a1 = -0.126;
const double a2 = -0.3516;
const double a3 = 0.2843;
const double a4 = -0.1015;

Airfoil::Airfoil(int numPoints, double M, double P, double T)
    : numPoints(numPoints), M(M / 100.0), P(P / 10.0), T(T / 100.0),
      x(numPoints), xu(numPoints), yu(numPoints), xl(numPoints), yl(numPoints) {}

double Airfoil::camberLine(double x) const {
    if (x <= P) {
        return (M / (P * P)) * (2 * P * x - x * x);
    } else {
        return (M / ((1 - P) * (1 - P))) * ((1 - 2 * P) + 2 * P * x - x * x);
    }
}

double Airfoil::camberGradient(double x) const {
    if (x <= P) {
        return (2 * M / (P * P)) * (P - x);
    } else {
        return (2 * M / ((1 - P) * (1 - P))) * (P - x);
    }
}

double Airfoil::thicknessDistribution(double x) const {
    return 5 * T * (a0 * std::sqrt(x) + a1 * x + a2 * x * x + a3 * x * x * x + a4 * x * x * x * x);
}

void Airfoil::calculateCoordinates() {
    for (int i = 0; i < numPoints; ++i) {
        double beta = M_PI * i / (numPoints - 1);
        x[i] = (1 - std::cos(beta)) / 2;

        double yc = camberLine(x[i]);
        double dyc_dx = camberGradient(x[i]);
        double theta = std::atan(dyc_dx);
        double yt = thicknessDistribution(x[i]);

        xu[i] = x[i] - yt * std::sin(theta);
        yu[i] = yc + yt * std::cos(theta);
        xl[i] = x[i] + yt * std::sin(theta);
        yl[i] = yc - yt * std::cos(theta);
    }
}

void Airfoil::printCoordinates() const {
    for (int i = 0; i < numPoints; ++i) {
        std::cout << "Point " << i << ": "
                  << "X: " << x[i] 
                  << ", Xu: " << xu[i] 
                  << ", Yu: " << yu[i] 
                  << ", Xl: " << xl[i] 
                  << ", Yl: " << yl[i] 
                  << std::endl;
    }
}

