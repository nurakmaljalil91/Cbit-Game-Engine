#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton final
{
protected:
    Singleton()
    {
    }

    ~Singleton() { ; }

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static T& get()
    {
        static T instance;
        return instance;
    };
};

#endif
