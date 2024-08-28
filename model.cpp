#include "model.h"
#include "optimizer.h"

#include <thread>
#include <sstream>
#include <iostream>


Model::Model(std::shared_ptr<View> view) : view_(view)
{
    table.insert({"+", std::make_shared<TwoArgFunction<Plus>>()});
    table.insert({"-", std::make_shared<TwoArgFunction<Sub>>()});
    table.insert({"*", std::make_shared<TwoArgFunction<Mul>>()});
    table.insert({"/", std::make_shared<TwoArgFunction<Dev>>()});
    table.insert({"cos", std::make_shared<SingleArgFunction<Cos>>()});
    table.insert({"sin", std::make_shared<SingleArgFunction<Sin>>()});
    table.insert({"^", std::make_shared<TwoArgFunction<Pow>>()});

    range_table.insert({"(", 1});
    range_table.insert({"sin", 2});
    range_table.insert({"cos", 2});
    range_table.insert({"^", 3});
    range_table.insert({"*", 4});
    range_table.insert({"/", 4});
    range_table.insert({"+", 5});
    range_table.insert({"-", 5});
    range_table.insert({")", 6});
}

void Model::add_variables(std::string variables)
{
    auto vars = separate(variables);
    for (auto var : vars) {
        if (table.find(var) != table.end()) {
            continue;
        }
        auto param = std::make_shared<Parameter>(0, true, var);
        table.insert({var, std::make_shared<ParameterClassifier>(param)});
        variables_.push_back(param);
    }
}

void Model::decision_process(std::shared_ptr<Differentiable> equations)
{
    std::cout << "Model::decision_process" << std::endl;
    Optimizer opti(equations);
    opti();
    std::cout << "Model::decision_process after" << std::endl;
    display_answer(equations->operator()());
}

void Model::display_answer(double loss)
{
    if (loss > 1e-5) {
        view_->display_decision("no decision");
    }

    std::string answer = "";
    for (auto v : variables_) {
        answer += std::to_string(v->get_value()) + " ";
    }

    view_->display_decision(answer);
}

void Model::solve(std::string equations)
{
    if (equations.empty()) {
        return;
    }

    std::shared_ptr<Differentiable> equation =  make_equation(equations);

    std::thread t(&Model::decision_process, this, equation);
    t.detach();
}

std::shared_ptr<Differentiable> Model::make_single_equation(std::string equation)
{
    std::vector<std::string> words = separate(equation);
    std::vector<std::string> rpn = rpn_of(words);
    std::stack<std::shared_ptr<Differentiable>> s{};
    for (size_t i = 0; i < rpn.size(); ++i) {
        if (table.find(rpn[i]) != table.end()) {
            table[rpn[i]]->operator()(s);
        } else {
            size_t eptr = 0;
            double c = std::stod(rpn[i], &eptr);
            if (!eptr || eptr != rpn[i].size()) { //строка не пустая || в строке нет мусора
                throw std::string{"invalid const"};
            }
            s.push(std::make_shared<Const>(c));
        }
    }

    if (s.size() != 1) {
        throw std::string{"invalid exp"};
    }

    return s.top();
}

std::shared_ptr<Differentiable> Model::make_equation(std::string equations)
{
    std::vector<std::string> lines{};

    std::istringstream f(equations);

    std::string line;
    while (std::getline(f, line)) {
        if (line[0] != '#') {
            lines.push_back(line);
        }
    }

    std::shared_ptr<Differentiable> result = make_single_equation(lines[0]);
    result = result * result;
    for (std::size_t i = 1; i < lines.size(); ++i) {
        auto addition = make_single_equation(lines[i]);
        addition = addition * addition;
        result = result + addition;
    }

    return result;
}


std::vector<std::string> Model::rpn_of(std::vector<std::string> words)
{
    std::stack<std::string> for_func{};
    std::vector<std::string> rpn;
    while (!words.empty()) {
        int range = range_of_func(words[0]);
        if (range == 0) {
            rpn.push_back(words.front());
            words.erase(words.begin());
        } else if (words[0] == ")") {
            while (!for_func.empty() && for_func.top() != "(") {
                rpn.push_back(for_func.top());
                for_func.pop();
            }

            if (for_func.empty()) {
                throw std::string{"invalid exp"};
            } else {
                for_func.pop();
                words.erase(words.begin());
            }
        } else {
            while (!for_func.empty() && for_func.top() != "(" && range_of_func(for_func.top()) <= range) {
                rpn.push_back(for_func.top());
                for_func.pop();
            }

            for_func.push(words.front());
            words.erase(words.begin());
        }
    }

    while (!for_func.empty() && for_func.top() != "(") {
        rpn.push_back(for_func.top());
        for_func.pop();
    }

    if (!for_func.empty()) {
        throw std::string{"invalid exp"};
    }

    return rpn;
}

int Model::range_of_func(std::string func)
{
    auto res = range_table.find(func);
    if (res == range_table.end()) {
        return 0;
    }
    return res->second;
}

std::vector<std::string> Model::separate(const std::string &s)
{

    std::vector<std::string> words{};
    bool is_word = 0;
    std::string word{};
    for (auto c : s) {
        if (c == ' ' && is_word) {
            words.push_back(word);
            word = "";
            is_word = 0;
        } else if (c != ' ' && is_word) {
            word += c;
        } else if (c != ' ' && !is_word) {
            is_word = 1;
            word += c;
        }
    }

    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}
