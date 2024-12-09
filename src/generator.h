#ifndef NACA_AIRFOIL_GENERATOR_H
#define NACA_AIRFOIL_GENERATOR_H

// Airfoil struct tanımı
typedef struct Airfoil{
    int num_points;
    double *x; // X noktaları
    double *xu; // Kanadın x eksenine göre üstünde olan noktaların X eksenindeki konumları
    double *yu; // Kanadın x eksenine göre üstünde olan noktaların Y eksenindeki konumları
    double *xl; // Kanadın x eksenine göre altında olan noktaların X eksenindeki konumları
    double *yl; // Kanadın x eksenine göre altında olan noktaların Y eksenindeki konumları
    double M, P, T; // Max Camber, Camber Position, Thickness

    void (*calculate_coordinates)(struct Airfoil *self);
} Airfoil;

Airfoil *create_airfoil(int num_points, double M, double P, double T);  // CONSTRUCTOR HAHAHA :D
void free_airfoil(Airfoil *airfoil);                                    // Airfoil ekrandan kaldırılacağı zaman bellek sızıntısı olmasın diye kullanılacak fonksiyon
void calculate_airfoil_coordinates(Airfoil *airfoil);                   // Airfoil noktalarını oluşturan fonksiyon

#endif // NACA_AIRFOIL_GENERATOR_H

