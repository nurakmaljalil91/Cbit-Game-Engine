#ifndef SINGLETON_H
#define SINGLETON_H
#include <iostream>

namespace Engine
{
    template <typename T>
    class Singleton
    {
    protected:
        Singleton() {}
        virtual ~Singleton() { ; }

    public:
        Singleton(const Singleton &) = delete;
        Singleton &operator=(const Singleton &) = delete;
        Singleton(Singleton &&) = delete;
        Singleton &operator=(Singleton &&) = delete;

        static T &get()
        {
            static T instance;
            return instance;
        };
    };
} // namespace Engine
#endif