#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"
#include "view.h"

#include <QtWidgets>

#include <memory>

class Controller : public QWidget
{
    Q_OBJECT

    QTextEdit *variables{};
    QTextEdit *equations{};
    QPushButton *b_solve{};

    std::shared_ptr<Model> model{nullptr};
    std::shared_ptr<View> view;
public:
    Controller(QWidget *parent = nullptr);

public slots:
    void solve_equations();
};


#endif // CONTROLLER_H
