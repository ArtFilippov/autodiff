#include "parameter.h"

Parameter::Parameter(double value, bool is_diff, std::string name) : value_(value), is_diff_(is_diff), name_(name) {}


std::string Parameter::get_name()
{
    return name_;
}

void Parameter::make_var()
{
    is_diff_ = true;
}

void Parameter::make_const()
{
    is_diff_ = false;
}

bool Parameter::is_diff()
{
    return is_diff_;
}

double Parameter::get_value()
{
    return value_;
}

void Parameter::set_value(double x)
{
    std::lock_guard lk(mut);
    value_ = x;
}

void Parameter::move_by(double step)
{
    value_ += step;
}
