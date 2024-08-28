#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "differentiable.h"
#include "parameter.h"

#include <memory>
#include <vector>

class Optimizer
{
    std::shared_ptr<Differentiable> cond_to_min_;
    std::vector<std::shared_ptr<Parameter>> parameters{};
    double loss{100.0};
    double lr_;
    double beta_1_;
    double beta_2_;
    double eps{1e-8};
    double max_loss{1e-30};

public:
    Optimizer(std::shared_ptr<Differentiable> cond_to_min, double lr = 1e-3, double beta_1 = 0.9, double beta_2 = 0.999);
    ~Optimizer();
    void operator()();
    double get_loss();
private:
    void step_for_parameters(Grad<double> grad);
};

#endif // OPTIMIZER_H
