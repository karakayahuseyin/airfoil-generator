#ifndef NACA_AIRFOIL_GENERATOR_H
#define NACA_AIRFOIL_GENERATOR_H

#include <vector>
#include <cmath>

class Airfoil {
private:
    int numPoints;
    double M, P, T;
    std::vector<double> x, xu, yu, xl, yl;

    double camberLine(double x) const;
    double camberGradient(double x) const;
    double thicknessDistribution(double x) const;

public:
    Airfoil(int numPoints, double M, double P, double T);
    ~Airfoil() = default;

    void calculateCoordinates();
    void printCoordinates() const;

    const std::vector<double>& getX() const { return x; }
    const std::vector<double>& getXu() const { return xu; }
    const std::vector<double>& getYu() const { return yu; }
    const std::vector<double>& getXl() const { return xl; }
    const std::vector<double>& getYl() const { return yl; }
};

#endif // NACA_AIRFOIL_GENERATOR_H

