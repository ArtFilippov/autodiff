#include "differentiable.h"

#include <algorithm>
#include <cmath>

Differentiable::Differentiable(double value) : value_(value), derivative_(0) {}


double Differentiable::get_value()
{
    return value_;
}

double Differentiable::get_derivative()
{
    return derivative_;
}

std::shared_ptr<Differentiable> operator +(std::shared_ptr<Differentiable> a, std::shared_ptr<Differentiable> b)
{
    return std::make_shared<Plus>(a, b);
}

std::shared_ptr<Differentiable> operator -(std::shared_ptr<Differentiable> a, std::shared_ptr<Differentiable> b)
{
    return std::make_shared<Sub>(a, b);
}

std::shared_ptr<Differentiable> operator *(std::shared_ptr<Differentiable> a, std::shared_ptr<Differentiable> b)
{
    return std::make_shared<Mul>(a, b);
}

std::shared_ptr<Differentiable> operator /(std::shared_ptr<Differentiable> a, std::shared_ptr<Differentiable> b)
{
    return std::make_shared<Dev>(a, b);
}

std::shared_ptr<Differentiable> operator -(std::shared_ptr<Differentiable> a)
{
    return std::make_shared<Neg>(a);
}

std::shared_ptr<Differentiable> d_sin(std::shared_ptr<Differentiable> a)
{
    return std::make_shared<Sin>(a);
}

std::shared_ptr<Differentiable> d_cos(std::shared_ptr<Differentiable> a)
{
    return std::make_shared<Cos>(a);
}

std::shared_ptr<Differentiable> d_pow(std::shared_ptr<Differentiable> a, std::shared_ptr<Differentiable> n)
{
    return std::make_shared<Pow>(a, n);
}


//Const
Const::Const(double c) : Differentiable(c) {}

void Const::get_equation(std::string &eq)
{
    eq += std::to_string(value_);
}

MultipleMutexGuard Const::lock_all_mutaxes()
{
    return MultipleMutexGuard(std::vector<std::mutex*>{});
}

Grad<double> Const::make_grad()
{
    return Grad<double>();
}
double Const::operator()()
{
    return value_;
}


//Var
Var::Var(std::string name) : Differentiable(0), parameter_(std::make_shared<Parameter>(0, 1, name)) {}

Var::Var(std::shared_ptr<Parameter> parameter) : Differentiable(parameter->get_value()), parameter_(parameter)
{
    derivative_ = parameter_->is_diff();
}

void Var::get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters)
{
    for (auto x : parameters) {
        if (x.get() == parameter_.get()) {
            return;
        }
    }
    parameters.push_back(parameter_);
}

void Var::get_equation(std::string &eq)
{
    eq += parameter_->get_name();
}

MultipleMutexGuard Var::lock_all_mutaxes()
{
    std::vector<std::shared_ptr<Parameter>> parameters;
    get_all_parameters(parameters);
    std::sort(parameters.begin(), parameters.end(), [] (std::shared_ptr<Parameter> a, std::shared_ptr<Parameter> b)
        {
            return a->get_name() < b->get_name();
        });

    std::vector<std::mutex*> mutexes;
    for (auto x : parameters) {
        mutexes.push_back(&(x->mut));
    }

    return MultipleMutexGuard(mutexes);
}

Grad<double> Var::make_grad()
{
    std::vector<double> gradient;
    std::vector<std::shared_ptr<Parameter>> parameters;
    get_all_parameters(parameters);

    MultipleMutexGuard m = lock_all_mutaxes();

    for (auto x : parameters) {
        x->make_const();
    }

    for (auto x : parameters) {
        x->make_var();
        operator()();
        gradient.push_back(derivative_);
        x->make_const();
    }

    return Grad(gradient);
}

double Var::operator()()
{
    value_ = parameter_->get_value();
    derivative_ = parameter_->is_diff();

    return value_;
}


//Pow
Pow::Pow(std::shared_ptr<Differentiable> x, std::shared_ptr<Differentiable> n) : Var("^"), x_(x), n_(n)
{
    value_ = pow(x_->get_value(), n_->get_value());
    derivative_ = n_->get_value() * pow(x_->get_value(), n_->get_value() - 1) * x_->get_derivative();
}

void Pow::get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters)
{
    x_->get_all_parameters(parameters);
    n_->get_all_parameters(parameters);
}

void Pow::get_equation(std::string &eq)
{
    eq += "(";
    x_->get_equation(eq);
    eq += ")";
    Var::get_equation(eq);
    eq += "(";
    n_->get_equation(eq);
    eq += ")";
}
double Pow::operator()()
{
    x_->operator()();
    n_->operator()();

    value_ = pow(x_->get_value(), n_->get_value());
    derivative_ = n_->get_value() * pow(x_->get_value(), n_->get_value() - 1) * x_->get_derivative();

    parameter_->set_value(value_);

    return value_;
}


//Plus
Plus::Plus(std::shared_ptr<Differentiable> x, std::shared_ptr<Differentiable> y) : Var("+"), x_(x), y_(y)
{
    value_ = x_->get_value() + y_->get_value();
    derivative_ = x_->get_derivative() + y_->get_derivative();
}

void Plus::get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters)
{
    x_->get_all_parameters(parameters);
    y_->get_all_parameters(parameters);
}

void Plus::get_equation(std::string &eq)
{
    x_->get_equation(eq);
    Var::get_equation(eq);
    y_->get_equation(eq);
}
double Plus::operator()()
{
    x_->operator()();
    y_->operator()();

    value_ = x_->get_value() + y_->get_value();
    derivative_ = x_->get_derivative() + y_->get_derivative();

    parameter_->set_value(value_);

    return value_;
}


//Sub
Sub::Sub(std::shared_ptr<Differentiable> x, std::shared_ptr<Differentiable> y) : Var("-"), x_(x), y_(y)
{
    value_ = x_->get_value() - y_->get_value();
    derivative_ = x_->get_derivative() - y_->get_derivative();
}

void Sub::get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters)
{
    x_->get_all_parameters(parameters);
    y_->get_all_parameters(parameters);
}

void Sub::get_equation(std::string &eq)
{
    x_->get_equation(eq);
    Var::get_equation(eq);
    y_->get_equation(eq);
}
double Sub::operator()()
{
    x_->operator()();
    y_->operator()();

    value_ = x_->get_value() - y_->get_value();
    derivative_ = x_->get_derivative() - y_->get_derivative();

    parameter_->set_value(value_);

    return value_;
}


//Mul
Mul::Mul(std::shared_ptr<Differentiable> x, std::shared_ptr<Differentiable> y) : Var("*"), x_(x), y_(y)
{
    value_ = x_->get_value() * y_->get_value();
    derivative_ = x_->get_value() * y_->get_derivative() + x_->get_derivative() * y_->get_value();
}

void Mul::get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters)
{
    x_->get_all_parameters(parameters);
    y_->get_all_parameters(parameters);
}

void Mul::get_equation(std::string &eq)
{
    eq += "(";
    x_->get_equation(eq);
    eq += ")";
    Var::get_equation(eq);
    eq += "(";
    y_->get_equation(eq);
    eq += ")";
}
double Mul::operator()()
{
    x_->operator()();
    y_->operator()();

    value_ = x_->get_value() * y_->get_value();
    derivative_ = x_->get_value() * y_->get_derivative() + x_->get_derivative() * y_->get_value();

    parameter_->set_value(value_);

    return value_;
}


//Dev
Dev::Dev(std::shared_ptr<Differentiable> x, std::shared_ptr<Differentiable> y) : Var("/"), x_(x), y_(y)
{
    value_ = x_->get_value() / y_->get_value();
    derivative_ = (x_->get_derivative() * y_->get_value() - x_->get_value() * y_->get_derivative()) / (y_->get_value() * y_->get_value());
}

void Dev::get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters)
{
    x_->get_all_parameters(parameters);
    y_->get_all_parameters(parameters);
}

void Dev::get_equation(std::string &eq)
{
    eq += "(";
    x_->get_equation(eq);
    eq += ")";
    Var::get_equation(eq);
    eq += "(";
    y_->get_equation(eq);
    eq += ")";
}
double Dev::operator()()
{
    x_->operator()();
    y_->operator()();

    value_ = x_->get_value() / y_->get_value();
    derivative_ = (x_->get_derivative() * y_->get_value() - x_->get_value() * y_->get_derivative()) / (y_->get_value() * y_->get_value());

    parameter_->set_value(value_);

    return value_;
}


//Cos
Cos::Cos(std::shared_ptr<Differentiable> x) : Var("cos"), x_(x)
{
    value_ = std::cos(x_->get_value());
    derivative_ = -std::sin(x_->get_value()) * x_->get_derivative();
}

void Cos::get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters)
{
    x_->get_all_parameters(parameters);
}

void Cos::get_equation(std::string &eq)
{

    Var::get_equation(eq);
    eq += "(";
    x_->get_equation(eq);
    eq += ")";
}
double Cos::operator()()
{
    x_->operator()();

    value_ = std::cos(x_->get_value());
    derivative_ = -std::sin(x_->get_value()) * x_->get_derivative();

    parameter_->set_value(value_);

    return value_;
}


//Sin
Sin::Sin(std::shared_ptr<Differentiable> x) : Var("sin"), x_(x)
{
    value_ = std::sin(x_->get_value());
    derivative_ = std::cos(x_->get_value()) * x_->get_derivative();
}

void Sin::get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters)
{
    x_->get_all_parameters(parameters);
}

void Sin::get_equation(std::string &eq)
{

    Var::get_equation(eq);
    eq += "(";
    x_->get_equation(eq);
    eq += ")";
}
double Sin::operator()()
{
    x_->operator()();

    value_ = std::sin(x_->get_value());
    derivative_ = std::cos(x_->get_value()) * x_->get_derivative();

    parameter_->set_value(value_);

    return value_;
}


//Neg
Neg::Neg(std::shared_ptr<Differentiable> x) : Var("-"), x_(x)
{
    value_ = -x_->get_value();
    derivative_ = -x_->get_derivative();
}

void Neg::get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters)
{
    x_->get_all_parameters(parameters);
}

void Neg::get_equation(std::string &eq)
{
    eq += "(";
    Var::get_equation(eq);
    x_->get_equation(eq);
    eq += ")";
}
double Neg::operator()()
{
    x_->operator()();

    value_ = -x_->get_value();
    derivative_ = -x_->get_derivative();

    parameter_->set_value(value_);

    return value_;
}
