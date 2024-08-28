#include "differentiable.h"
//#include "optimizer.h"

#include <gtest/gtest.h>
#include <memory>
#include <iostream>
#include <cmath>
#include <vector>



TEST(Diff, MakeConst)
{
    Differentiable *c =  new Const(1000.1);
    constexpr double expected_val = 1000.1;
    constexpr double expected_deriv = 0;

    ASSERT_EQ(c->get_value(), expected_val);
    ASSERT_EQ(c->get_derivative(), expected_deriv);
    delete c;
}

TEST(Diff, MakeVar)
{
    std::shared_ptr<Parameter> p = std::make_shared<Parameter>(0, true, "y");
    Differentiable *c =  new Var(p);
    constexpr double expected_val = -1000.1;
    constexpr double expected_deriv = 1;

    p->set_value(-1000.1);
    (*c)();

    ASSERT_EQ(c->get_value(), expected_val);
    ASSERT_EQ(c->get_derivative(), expected_deriv);
    delete c;
}


TEST(Diff, SqwareDeriw)
{
    std::shared_ptr<Parameter> p = std::make_shared<Parameter>(10, true, "x");
    std::shared_ptr<Differentiable> c =  std::make_shared<Var>(p);

    constexpr double expected_val = 100;
    constexpr double expected_deriv = 20;

    auto f = c * c;

    ASSERT_EQ(f->get_value(), expected_val);
    ASSERT_EQ(f->get_derivative(), expected_deriv);
}

TEST(Diff, CosSinIfVar)
{
    for (double x = -3; x < 3; x += 0.1){
        constexpr double expected_val = 1;
        constexpr double expected_deriv = 0;

        std::shared_ptr<Parameter> p = std::make_shared<Parameter>(x, true, "x");
        std::shared_ptr<Differentiable> dx = std::make_shared<Var>(p);

        auto y1 = d_sin(dx) * d_sin(dx);
        auto y2 = d_cos(dx) * d_cos(dx);

        auto y = y1 + y2;


        ASSERT_DOUBLE_EQ(y->get_value(), expected_val);
        ASSERT_DOUBLE_EQ(y->get_derivative(), expected_deriv);
    }
}

TEST(Diff, CosSinConst)
{
    for (double x = -3; x < 3; x += 0.1) {
        double expected_val = std::sin(x) / std::cos(x);
        constexpr double expected_deriv = 0;

        std::shared_ptr<Parameter> p = std::make_shared<Parameter>(x, false, "x");
        std::shared_ptr<Differentiable> dx = std::make_shared<Var>(p);
        auto y = d_sin(dx) / d_cos(dx);

        ASSERT_DOUBLE_EQ(y->get_value(), expected_val);
        ASSERT_DOUBLE_EQ(y->get_derivative(), expected_deriv);
    }
}

TEST(Diff, CosWithSinVar)
{
    for (double x = -3; x < 3; x += 0.1) {
        double expected_val = std::cos(std::sin(x));
        double expected_deriv = -std::sin(std::sin(x)) * std::cos(x);

        std::shared_ptr<Parameter> p = std::make_shared<Parameter>(x, true, "x");
        std::shared_ptr<Differentiable> dx = std::make_shared<Var>(p);
        auto y = d_cos(d_sin(dx));

        ASSERT_DOUBLE_EQ(y->get_value(), expected_val);
        ASSERT_DOUBLE_EQ(y->get_derivative(), expected_deriv);
    }
}

TEST(Diff, Mul)
{
    std::shared_ptr<Parameter> p = std::make_shared<Parameter>(1, true, "x");
    std::shared_ptr<Differentiable> dx = std::make_shared<Var>(p);

    std::shared_ptr<Parameter> pr = std::make_shared<Parameter>(0, false, "y");
    std::shared_ptr<Differentiable> dy = std::make_shared<Var>(pr);

 //   auto z = Mul(std::make_shared<Sin>(dx), std::make_shared<Cos>(dy));
    auto z = d_sin(dx) * d_cos(dy);

    ASSERT_DOUBLE_EQ(z->get_value(), std::sin(1) * std::cos(0));
    ASSERT_DOUBLE_EQ(z->get_derivative(), std::cos(1) * std::cos(0));
}

TEST(Diff, Dev)
{
    std::shared_ptr<Parameter> p = std::make_shared<Parameter>(1, true, "x");
    std::shared_ptr<Differentiable> dx = std::make_shared<Var>(p);

    std::shared_ptr<Parameter> pr = std::make_shared<Parameter>(0, false, "y");
    std::shared_ptr<Differentiable> dy = std::make_shared<Var>(pr);

    //auto z = Dev(std::make_shared<Sin>(dx), std::make_shared<Cos>(dy));
    auto z = d_sin(dx) / d_cos(dy);

    ASSERT_DOUBLE_EQ(z->get_value(), std::sin(1) / std::cos(0));
    ASSERT_DOUBLE_EQ(z->get_derivative(), std::cos(1) / std::cos(0));
}

TEST(Diff, Plus)
{
    std::shared_ptr<Parameter> p = std::make_shared<Parameter>(1, true, "x");
    std::shared_ptr<Differentiable> dx = std::make_shared<Var>(p);

    std::shared_ptr<Parameter> pr = std::make_shared<Parameter>(0, false, "y");
    std::shared_ptr<Differentiable> dy = std::make_shared<Var>(pr);

    //auto z = Plus(std::make_shared<Sin>(dx), std::make_shared<Cos>(dy));
    auto z = d_sin(dx) + d_cos(dy);

    ASSERT_DOUBLE_EQ(z->get_value(), std::sin(1) + std::cos(0));
    ASSERT_DOUBLE_EQ(z->get_derivative(), std::cos(1));
}

TEST(Diff, Minus)
{
    std::shared_ptr<Parameter> p = std::make_shared<Parameter>(1, true, "x");
    std::shared_ptr<Differentiable> dx = std::make_shared<Var>(p);

    std::shared_ptr<Parameter> pr = std::make_shared<Parameter>(0, false, "y");
    std::shared_ptr<Differentiable> dy = std::make_shared<Var>(pr);

    //auto z = Sub(std::make_shared<Sin>(dx), std::make_shared<Cos>(dy));
    auto z = d_sin(dx) - d_cos(dy);

    ASSERT_DOUBLE_EQ(z->get_value(), std::sin(1) - std::cos(0));
    ASSERT_DOUBLE_EQ(z->get_derivative(), std::cos(1));
}

TEST(Diff, GetAllParameters)
{

    std::shared_ptr<Parameter> p = std::make_shared<Parameter>(1, true);
    std::shared_ptr<Differentiable> dx = std::make_shared<Var>(p);

    std::shared_ptr<Parameter> pr = std::make_shared<Parameter>(0, true, "y");
    std::shared_ptr<Differentiable> dy = std::make_shared<Var>(pr);

    auto mul = std::make_shared<Mul>(std::make_shared<Sin>(dx), std::make_shared<Cos>(dy));
    auto minus = std::make_shared<Sub>(mul, std::make_shared<Cos>(dy));
    auto plus = std::make_shared<Plus>(minus, std::make_shared<Cos>(dx));
    auto z = Dev(plus, std::make_shared<Sin>(dx));

    std::vector<std::shared_ptr<Parameter>> Parameters;
    z.get_all_parameters(Parameters);

    ASSERT_EQ(Parameters.size(), 2);
}

TEST(Diff, MakeGrad)
{
    double tx = 2;
    std::shared_ptr<Parameter> p = std::make_shared<Parameter>(tx, false, "x");
    std::shared_ptr<Differentiable> dx = std::make_shared<Var>(p);

    double ty = 3;
    std::shared_ptr<Parameter> pr = std::make_shared<Parameter>(ty, true, "y");
    std::shared_ptr<Differentiable> dy = std::make_shared<Var>(pr);

    auto f = Plus(std::make_shared<Mul>(dx, dx), std::make_shared<Mul>(dy, dy));

    Grad<double> g = f.make_grad();

    ASSERT_DOUBLE_EQ(g[0], 4);
    ASSERT_DOUBLE_EQ(g[1], 6);
}

TEST(Diff, Gradient)
{
    double tx = 2;
    std::shared_ptr<Parameter> p = std::make_shared<Parameter>(tx, false, "x");
    std::shared_ptr<Differentiable> dx = std::make_shared<Var>(p);

    double ty = 3;
    std::shared_ptr<Parameter> pr = std::make_shared<Parameter>(ty, true, "y");
    std::shared_ptr<Differentiable> dy = std::make_shared<Var>(pr);

    auto f = d_pow(dx, std::make_shared<Const>(2)) + d_pow(dy, std::make_shared<Const>(2));

    auto g = f->make_grad();

    p->set_value(20);
    pr->set_value(30);

    auto h = f->make_grad();

    auto t = g / 2 + h * 2 - Grad(std::vector<double>{1, 0});

    ASSERT_DOUBLE_EQ(t[0], 81);
    ASSERT_DOUBLE_EQ(t[1], 123);
}

TEST(Diff, Equation)
{
    std::shared_ptr<Parameter> p = std::make_shared<Parameter>(1, false, "x");
    std::shared_ptr<Differentiable> x = std::make_shared<Var>(p);

    auto f = x * x + CONST(3) * x + CONST(2);

    std::string eq = "";
    f->get_equation(eq);
    ASSERT_EQ(eq, "(x)*(x)+(3.000000)*(x)+2.000000");
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
