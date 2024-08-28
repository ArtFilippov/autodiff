#ifndef DIFFERENTIABLE_H
#define DIFFERENTIABLE_H

#include "parameter.h"
#include "grad.h"
#include "multiplemutex.h"

#include <memory>
#include <vector>
#include <string>

#define CONST(x) std::make_shared<Const>(x)


class Differentiable
{
protected:
    double value_{};
    double derivative_{};
public:
    Differentiable(double value);
    virtual ~Differentiable() = default;

    double get_value();
    double get_derivative();

    virtual void get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters) = 0;
    virtual void get_equation(std::string &eq) = 0;
    virtual MultipleMutexGuard lock_all_mutaxes() = 0;
    virtual Grad<double> make_grad() = 0;
    virtual double operator()() = 0;

    friend std::shared_ptr<Differentiable> operator +(std::shared_ptr<Differentiable> a, std::shared_ptr<Differentiable> b);
    friend std::shared_ptr<Differentiable> operator -(std::shared_ptr<Differentiable> a, std::shared_ptr<Differentiable> b);
    friend std::shared_ptr<Differentiable> operator *(std::shared_ptr<Differentiable> a, std::shared_ptr<Differentiable> b);
    friend std::shared_ptr<Differentiable> operator /(std::shared_ptr<Differentiable> a, std::shared_ptr<Differentiable> b);
    friend std::shared_ptr<Differentiable> operator -(std::shared_ptr<Differentiable> a);
    friend std::shared_ptr<Differentiable> d_sin(std::shared_ptr<Differentiable> a);
    friend std::shared_ptr<Differentiable> d_cos(std::shared_ptr<Differentiable> a);
    friend std::shared_ptr<Differentiable> d_pow(std::shared_ptr<Differentiable> a, std::shared_ptr<Differentiable> n);

};



class Const : public Differentiable
{
public:
    Const(double c);

    void get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters) override {/*Empty*/}
    void get_equation(std::string &eq) override;
    MultipleMutexGuard lock_all_mutaxes() override;
    Grad<double> make_grad() override;
    double operator()() override;
};

class Var : public Differentiable
{
protected:
    std::shared_ptr<Parameter> parameter_;
public:
    Var(std::string name);
    Var(std::shared_ptr<Parameter> parameter);

    virtual void get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters) override;
    virtual void get_equation(std::string &eq) override;
    MultipleMutexGuard lock_all_mutaxes() override;
    Grad<double> make_grad() override;
    virtual double operator()() override;
};


//Functions

class Pow : public Var
{
    std::shared_ptr<Differentiable> x_;
    std::shared_ptr<Differentiable> n_;
public:
    Pow(std::shared_ptr<Differentiable> x, std::shared_ptr<Differentiable> n);

    void get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters) override;
    void get_equation(std::string &eq) override;
    double operator()() override;
};

class Plus : public Var
{
    std::shared_ptr<Differentiable> x_;
    std::shared_ptr<Differentiable> y_;
public:
    Plus(std::shared_ptr<Differentiable> x, std::shared_ptr<Differentiable> y);

    void get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters) override;
    void get_equation(std::string &eq) override;
    double operator()() override;
};

class Sub : public Var
{
    std::shared_ptr<Differentiable> x_;
    std::shared_ptr<Differentiable> y_;
public:
    Sub(std::shared_ptr<Differentiable> x, std::shared_ptr<Differentiable> y);

    void get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters) override;
    void get_equation(std::string &eq) override;
    double operator()() override;
};

class Mul : public Var
{
    std::shared_ptr<Differentiable> x_;
    std::shared_ptr<Differentiable> y_;
public:
    Mul(std::shared_ptr<Differentiable> x, std::shared_ptr<Differentiable> y);

    void get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters) override;
    void get_equation(std::string &eq) override;
    double operator()() override;
};

class Dev : public Var
{
    std::shared_ptr<Differentiable> x_;
    std::shared_ptr<Differentiable> y_;
public:
    Dev(std::shared_ptr<Differentiable> x, std::shared_ptr<Differentiable> y);

    void get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters) override;
    void get_equation(std::string &eq) override;
    double operator()() override;
};

class Cos : public Var
{
    std::shared_ptr<Differentiable> x_;
public:
    Cos(std::shared_ptr<Differentiable> x);

    void get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters) override;
    void get_equation(std::string &eq) override;
    double operator()() override;
};

class Sin : public Var
{
    std::shared_ptr<Differentiable> x_;
public:
    Sin(std::shared_ptr<Differentiable> x);

    void get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters) override;
    void get_equation(std::string &eq) override;
    double operator()() override;
};

class Neg : public Var
{
    std::shared_ptr<Differentiable> x_;
public:
    Neg(std::shared_ptr<Differentiable> x);

    void get_all_parameters(std::vector<std::shared_ptr<Parameter>>& parameters) override;
    void get_equation(std::string &eq) override;
    double operator()() override;
};

#endif // DIFFERENTIABLE_H

