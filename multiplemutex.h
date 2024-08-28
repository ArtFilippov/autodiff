#ifndef MULTIPLEMUTEX_H
#define MULTIPLEMUTEX_H

#include <vector>
#include <mutex>

class MultipleMutexGuard
{
    std::vector<std::mutex*> mutexes_;
public:
    MultipleMutexGuard(std::vector<std::mutex*> mutexes) : mutexes_(mutexes)
    {
        lock();
    }

    MultipleMutexGuard(MultipleMutexGuard&&) = default;
    MultipleMutexGuard(const MultipleMutexGuard&) = delete;
    MultipleMutexGuard& operator=(const MultipleMutexGuard&) = delete;

    ~MultipleMutexGuard()
    {
        unlock();
    }



    void lock()
    {
        for (int it = 0; it != mutexes_.size(); ++it) {
            mutexes_[it]->lock();
        }
    }

    void unlock()
    {
        for (int it = 0; it != mutexes_.size(); ++it) {
            mutexes_[it]->unlock();
        }
    }

    bool try_lock()
    {
        bool res = 1;
        int i = 0;
        for (; i < mutexes_.size(); ++i) {
            if (!mutexes_[i]->try_lock()) {
                res = 0;
                break;
            }
        }

        if (!res) {
            for (; i >= 0; --i) {
                mutexes_[i]->unlock();
            }
        }

        return res;
    }
};

#endif // MULTIPLEMUTEX_H
