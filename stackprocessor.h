#ifndef STACKPROCESSOR_H
#define STACKPROCESSOR_H

#include "parameter.h"
#include "differentiable.h"

#include <stack>
#include <memory>


class StackProcessor
{
public:
    StackProcessor() = default;
    virtual ~StackProcessor() = default;
    virtual void operator()(std::stack<std::shared_ptr<Differentiable>> &s) = 0;
};

class ParameterClassifier : public StackProcessor
{
    std::shared_ptr<Parameter> param_;
public:
    ParameterClassifier(std::shared_ptr<Parameter> param) :  param_(param) {}
    ~ParameterClassifier() = default;
    void operator()(std::stack<std::shared_ptr<Differentiable>> &s) override
    {
        s.push(std::make_shared<Var>(param_));
    }
};

template<typename D>
class SingleArgFunction : public StackProcessor
{
public:
    SingleArgFunction() = default;
    ~SingleArgFunction() = default;
    void operator()(std::stack<std::shared_ptr<Differentiable>> &s) override
    {
        std::shared_ptr<Differentiable> param = s.top();
        s.pop();
        s.push(std::make_shared<D>(param));
    }
};


template<typename D>
class TwoArgFunction : public StackProcessor
{
public:
    TwoArgFunction() = default;
    ~TwoArgFunction() = default;
    void operator()(std::stack<std::shared_ptr<Differentiable>> &s) override
    {
        std::shared_ptr<Differentiable> param_2 = s.top();
        s.pop();
        std::shared_ptr<Differentiable> param_1 = s.top();
        s.pop();
        s.push(std::make_shared<D>(param_1, param_2));
    }
};

#endif // STACKPROCESSOR_H
