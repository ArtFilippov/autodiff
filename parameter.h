#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <mutex>

class Var;

class Parameter
{
    friend class Var;

    double value_;
    bool is_diff_;
    std::string name_;
    std::mutex mut{};
public:
    Parameter(double value = 0, bool is_diff = 0, std::string name = "x");
    Parameter(const Parameter&) = delete;
    Parameter(const Parameter&&) = delete;
    Parameter& operator=(const Parameter&) = delete;

    bool is_diff();
    void set_value(double value);
    double get_value();
    std::string get_name();

private:
    void move_by(double step);
    void make_const();
    void make_var();
};

#endif // PARAMETER_H
