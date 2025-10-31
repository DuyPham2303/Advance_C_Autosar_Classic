#ifndef CALCULATION_H
#define CALCULATION_H

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

typedef enum{
    VO_NGHIEM,
    NGHIEM_KEP,
    HAI_NGHIEM
}Type;

// Nhập hệ số
void input_coefficients(double* a,double* b,double* c);

// Tính delta
//double calculate_delta(double a, double b, double c);

// Giải phương trình
//Type solve(double a,double b,double c,double delta,double* x1,double* x2);

// Hiển thị kết quả
void display_result(double a,double b,double c,double delta);

#endif

