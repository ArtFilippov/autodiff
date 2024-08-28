#ifndef GRAD_H
#define GRAD_H

#include <vector>

template<typename Num>
class Grad
{
    std::vector<Num> val_;
public:
    Grad(std::vector<Num> val = {0.0}) : val_(val) {}
    const Grad& operator=(const Grad &other)
    {
        val_ = other.val_;
        return *this;
    }

    Num& operator[](std::size_t i)
    {
        return val_[i];
    }

    Grad<Num> operator+(const Grad &other) const
    {
        std::vector<Num> new_val = val_;
        for (int i = 0; i < new_val.size(); ++i) {
            new_val[i] += other.val_[i];
        }

        return Grad(new_val);
    }

    Grad<Num> operator-(const Grad &other) const
    {
        std::vector<Num> new_val = val_;
        for (int i = 0; i < new_val.size(); ++i) {
            new_val[i] -= other.val_[i];
        }

        return Grad(new_val);
    }

    Grad<Num> operator*(Num a) const
    {
        std::vector<Num> new_val = val_;
        for (int i = 0; i < new_val.size(); ++i) {
            new_val[i] *= a;
        }

        return Grad(new_val);
    }

    Grad<Num> operator/(Num a) const
    {
        std::vector<Num> new_val = val_;
        for (int i = 0; i < new_val.size(); ++i) {
            new_val[i] /= a;
        }

        return Grad(new_val);
    }

    Num operator*(const Grad &other)
    {
        Num len = 0;
        for (int i = 0; i < val_.size(); ++i) {
            len += val_[i] * other.val_[i];
        }

        return len;
    }

    Grad<Num> operator-()
    {
        std::vector<Num> new_val = val_;
        for (int i = 0; i < new_val.size(); ++i) {
            new_val[i] *= -1;
        }

        return Grad(new_val);
    }
};

#endif // GRAD_H
