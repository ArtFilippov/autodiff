#include "optimizer.h"

#include <cmath>
#include <iostream>

Optimizer::Optimizer(std::shared_ptr<Differentiable> cond_to_min, double lr, double beta_1, double beta_2)
    : cond_to_min_(cond_to_min), lr_(lr), beta_1_(beta_1), beta_2_(beta_2)
{
    cond_to_min_->get_all_parameters(parameters);
}

Optimizer::~Optimizer() = default;


void Optimizer::step_for_parameters(Grad<double> grad)
{
    for (int i = 0; i < parameters.size(); ++i) {
        parameters[i]->set_value(parameters[i]->get_value() + grad[i]);
    }
}

bool isEqual(double a, double b)
{
    constexpr double epsilon = 1e-30;
    return std::abs(a - b) <= epsilon;
}

void Optimizer::operator()()
{
    std::vector<double> tmp(parameters.size());
    for (auto x : tmp) {
        x = 0;
    }
    Grad<double> moment = Grad(tmp);
    double v = 0;
    double t_beta_1 = beta_1_;
    double t_beta_2 = beta_2_;

    int t = 0;
    constexpr int num_of_iterations = 50000;

    while(1) {
        Grad<double> g = cond_to_min_->make_grad();
        loss = cond_to_min_->get_value();
        if (loss <= max_loss || t >= num_of_iterations) {
            return;
        }

        moment = moment * beta_1_ + g * (1 - beta_1_);
        v = beta_2_ * v + (1 - beta_2_) * (g * g);
        auto moment_hat = moment / (1 - t_beta_1);
        t_beta_1 *= beta_1_;
        auto v_hat = v / (1 - t_beta_2);
        t_beta_2 *= t_beta_2;

        step_for_parameters(moment_hat * (-lr_) / (std::sqrt(v_hat) + eps));
        ++t;
    }
}

// x ^ 2 + y ^ 2 - 10 ^ 2
