#include "view.h"


View::View(QWidget *parent, QTextEdit *variables, QTextEdit *equations, QPushButton *b_solve) : QWidget(parent)
{

    decision = new QLabel("Decision: ");

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(new QLabel("Variables: "));
    layout->addWidget(variables);
    layout->addWidget(new QLabel("Equations: "));
    layout->addWidget(equations);
    layout->addWidget(b_solve);
    layout->addWidget(decision);

    this->setLayout(layout);
}

void View::display_decision(std::string answer)
{
    decision->setText("Decision: " + QString::fromStdString(answer));
}
