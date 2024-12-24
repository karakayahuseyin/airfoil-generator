#pragma once

#ifndef AIRFOIL_GENERATOR_H
#define AIRFOIL_GENERATOR_H

#include <vector>
#include <cmath>

struct AirfoilPoint
{
    double X;   // Orta nokta X? (isteğe bağlı)
    double Xu;  // Üst yüz X
    double Yu;  // Üst yüz Y
    double Xl;  // Alt yüz X
    double Yl;  // Alt yüz Y
};

class Airfoil {
private:
    int numPoints;
    double M, P, T;
    std::vector<double> x, xu, yu, xl, yl;

    double camberLine(double x) const;
    double camberGradient(double x) const;
    double thicknessDistribution(double x) const;
    std::vector<AirfoilPoint> airfoilData;

public:
    Airfoil(int numPoints, double M, double P, double T);
    ~Airfoil() = default;

    void calculateCoordinates();
    void saveCoordinates();
    std::vector<AirfoilPoint> getAirfoilData() { return airfoilData; }

    const std::vector<double>& getX() const { return x; }
    const std::vector<double>& getXu() const { return xu; }
    const std::vector<double>& getYu() const { return yu; }
    const std::vector<double>& getXl() const { return xl; }
    const std::vector<double>& getYl() const { return yl; }
};

#endif // AIRFOIL_GENERATOR_H

