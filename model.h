#ifndef MODEL_H
#define MODEL_H

#include "stackprocessor.h"
#include "view.h"

#include <memory>
#include <string>
#include <map>
#include <vector>


class Model
{

    std::map<std::string, std::shared_ptr<StackProcessor>> table{};
    std::vector<std::shared_ptr<Parameter>> variables_{};
    std::map<std::string, int> range_table{};

    std::shared_ptr<View> view_;
public:
    Model(std::shared_ptr<View> view);

    void add_variables(std::string variables);
    void solve(std::string equations);
private:
    void display_answer(double loss);
    void decision_process(std::shared_ptr<Differentiable> equations);
    std::vector<std::string> separate(const std::string &s);
    std::shared_ptr<Differentiable> make_equation(std::string equations);
    std::shared_ptr<Differentiable> make_single_equation(std::string equation);
    int range_of_func(std::string func);
    std::vector<std::string> rpn_of(std::vector<std::string> words);
};

#endif // MODEL_H
