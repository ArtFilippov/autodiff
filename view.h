#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QWidget>
#include <QtWidgets>

#include <string>

class Model;

class View : public QWidget
{
    Q_OBJECT

    QLabel *decision{};
public:
    View(QWidget *parent, QTextEdit *variables, QTextEdit *equations, QPushButton *b_solve);

    void display_decision(std::string answer);
};

#endif // VIEW_H
