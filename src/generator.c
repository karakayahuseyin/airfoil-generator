#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "generator.h"

static double a0 = 0.2969;
static double a1 = -0.126;
static double a2 = -0.3516;
static double a3 = 0.2843;
static double a4 = -0.1015;

static double camber_line(double x, double M, double P) {
    if (x <= P) {
        return (M / (P * P)) * (2 * P * x - x * x);
    } else {
        return (M / ((1 - P) * (1 - P))) * ((1 - 2 * P) + 2 * P * x - x * x);
    }
}

static double camber_gradient(double x, double M, double P) {
    if (x <= P) {
        return (2 * M / (P * P)) * (P - x);
    } else {
        return (2 * M / ((1 - P) * (1 - P))) * (P - x);
    }
}

static double thickness_distribution(double x, double T) {
    return (T / 20.0) * (a0 * sqrt(x) + a1 * x + a2 * x * x + a3 * x * x * x + a4 * x * x * x * x);
}

void calculate_airfoil_coordinates(Airfoil *airfoil) {
    for (int i = 0; i < airfoil->num_points; i++) {
        double beta = M_PI * i / (airfoil->num_points - 1);
        airfoil->x[i] = (1 - cos(beta)) / 2;

        double yc = camber_line(airfoil->x[i], airfoil->M, airfoil->P);
        double dyc_dx = camber_gradient(airfoil->x[i], airfoil->M, airfoil->P);
        double theta = atan(dyc_dx);
        double yt = thickness_distribution(airfoil->x[i], airfoil->T);

        airfoil->xu[i] = airfoil->x[i] - yt * sin(theta);
        airfoil->yu[i] = yc + yt * cos(theta);
        airfoil->xl[i] = airfoil->x[i] + yt * sin(theta);
        airfoil->yl[i] = yc - yt * cos(theta);
    }
}

Airfoil *create_airfoil(int num_points, double M, double P, double T) {
    Airfoil *airfoil = (Airfoil *)malloc(sizeof(Airfoil));
    if (!airfoil) return NULL;

    airfoil->num_points = num_points;
    airfoil->T = T / 100.0;
    airfoil->P = P / 10.0;
    airfoil->M = M / 100.0;

    airfoil->x = (double *)malloc(num_points * sizeof(double));
    airfoil->xu = (double *)malloc(num_points * sizeof(double));
    airfoil->yu = (double *)malloc(num_points * sizeof(double));
    airfoil->xl = (double *)malloc(num_points * sizeof(double));
    airfoil->yl = (double *)malloc(num_points * sizeof(double));

    if (!airfoil->x || !airfoil->xu || !airfoil->yu || !airfoil->xl || !airfoil->yl) {
        free(airfoil->x);
        free(airfoil->xu);
        free(airfoil->yu);
        free(airfoil->xl);
        free(airfoil->yl);
        free(airfoil);
        return NULL;
    }

    airfoil->calculate_coordinates = calculate_airfoil_coordinates;
    return airfoil;
}

void free_airfoil(Airfoil *airfoil) {
    if (!airfoil) return;

    free(airfoil->x);
    free(airfoil->xu);
    free(airfoil->yu);
    free(airfoil->xl);
    free(airfoil->yl);
    free(airfoil);
}

