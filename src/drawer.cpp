#include "drawer.h"
#include <iostream>

double BasisFunction(int i, int k, double t, const std::vector<double>& knots)
{
    if (k == 0)
    {
        if (t >= knots[i] && t < knots[i+1])
            return 1.0;
        else
            return 0.0;
    }
    else
    {
        double denom1 = (knots[i + k]   - knots[i]);
        double denom2 = (knots[i + k+1] - knots[i+1]);
        
        double term1 = 0.0;
        double term2 = 0.0;
        
        if (denom1 != 0.0)
            term1 = (t - knots[i]) / denom1 * BasisFunction(i, k - 1, t, knots);
        
        if (denom2 != 0.0)
            term2 = (knots[i + k+1] - t) / denom2 * BasisFunction(i + 1, k - 1, t, knots);
        
        return term1 + term2;
    }
}

std::pair<double,double> EvaluateBSpline(
    double t,
    int degree,
    const std::vector<double>& ctrlX,
    const std::vector<double>& ctrlY,
    const std::vector<double>& knots)
{
    double x = 0.0;
    double y = 0.0;
    int nCtrl = (int)ctrlX.size();
    for (int i = 0; i < nCtrl; i++)
    {
        double b = BasisFunction(i, degree, t, knots);
        x += b * ctrlX[i];
        y += b * ctrlY[i];
    }
    return std::make_pair(x,y);
}

std::vector<double> CreateUniformKnotVector(int nCtrl, int degree)
{
    int nKnots = nCtrl + degree + 1;
    std::vector<double> knots(nKnots);
    for(int i = 0; i < nKnots; i++)
    {
        if(i <= degree)
            knots[i] = 0.0;
        else if(i >= nCtrl)
            knots[i] = 1.0;
        else
        {
            double tmp = (double)(i - degree) / (double)(nKnots - 2*degree - 1);
            knots[i] = tmp; 
        }
    }
    return knots;
}
void draw(const std::vector<AirfoilPoint>& airfoilPoints) {
    if (airfoilPoints.size() < 100) {
        std::cerr << "Error: airfoilPoints vector must have at least 100 elements!" << std::endl;
        return;
    }

    std::vector<double> closedCtrlX;
    std::vector<double> closedCtrlY;

    // trailing edge = index 99
    double xTEu = airfoilPoints[99].Xu;  
    double yTEu = airfoilPoints[99].Yu;  
    closedCtrlX.push_back(xTEu);
    closedCtrlY.push_back(yTEu);

    //  -- Üst tarafa ait noktalar (99..0)
    for (int i = 99; i >= 0; i--) {
        closedCtrlX.push_back(airfoilPoints[i].Xu);
        closedCtrlY.push_back(airfoilPoints[i].Yu);
    }

    //  -- Alt tarafa ait noktalar (0..99)
    for (int i = 0; i <= 99; i++) {
        closedCtrlX.push_back(airfoilPoints[i].Xl);
        closedCtrlY.push_back(airfoilPoints[i].Yl);
    }

    //  -- Tekrar trailing edge noktası
    closedCtrlX.push_back(xTEu);
    closedCtrlY.push_back(yTEu);

    // 2) Knot vektörü
    auto knotClosed = CreateUniformKnotVector(static_cast<int>(closedCtrlX.size()), SPLINE_DEGREE);

    // 3) Spline noktalarını önceden hesaplayalım (dolgu ve çizgi için)
    const int sampleCount = 500;
    for (int s = 0; s <= sampleCount; s++) {
        double t = static_cast<double>(s) / static_cast<double>(sampleCount); // 0..1
        auto pt = EvaluateBSpline(t, SPLINE_DEGREE, closedCtrlX, closedCtrlY, knotClosed);
        polygonPoints.push_back(pt);
    }
}