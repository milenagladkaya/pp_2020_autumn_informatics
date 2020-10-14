//Copyright 2020 Zoreev Mikhail
#include "rectangle_method.h"

double integralSeqential(double a, double b, size_t count)
{
    if (count == 0) {
        throw std::runtime_error("Zero rectangles count");
    }
    double result = 0, delta = (b - a) / count;
    for (size_t i = 0; i < count; i++) {
        result += cos(a + delta * i);
    }
    result *= delta;
    return result;
}

double integralParallel(double a, double b, size_t count)
{
    int rank, process_count;
    double delta = (b - a) / count;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    size_t part = count / static_cast<size_t>(process_count);
    double result = 0, local_result = 0;
    if (rank != process_count - 1) {
        local_result = integralSeqential(a + rank * (part * delta), a + (rank + 1) * (part * delta), part);
    }
    else {
        local_result = integralSeqential(a + rank * (part * delta), b, count - part * (process_count - 1));
    }
    MPI_Reduce(&local_result, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return result;
}
