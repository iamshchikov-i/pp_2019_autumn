// Copyright 2019 Iamshchikov Ivan

#include "../../../modules/task_3/iamshchikov_i_minimizer/functions.h"

double f1(double x, double y) {
    return pow(x, 2) + pow(y - 1, 2);
}

double f2(double x, double y) {
    return pow(x, 2) - x * y + pow(y, 2) - 2 * x + y;
}

double f3(double x, double y) {
    return pow(x, 2) + x * y + pow(y, 2) - 4 * log(x) - 10 * log(y);
}
