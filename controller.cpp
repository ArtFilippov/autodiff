#include "controller.h"

#include <iostream>

Controller::Controller(QWidget *parent) : QWidget(parent), variables(new QTextEdit), equations(new QTextEdit), b_solve(new QPushButton("SOLVE")),
    view(std::make_shared<View>(parent, variables, equations, b_solve))
{
    model = std::make_shared<Model>(view);
    connect(b_solve, &QPushButton::clicked, this, &Controller::solve_equations);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view.get());
    this->setLayout(layout);
}


void Controller::solve_equations()
{
    std::cout << "Constroller::solve_equations" << std::endl;
    model->add_variables(variables->toPlainText().toStdString());
    model->solve(equations->toPlainText().toStdString());
}
