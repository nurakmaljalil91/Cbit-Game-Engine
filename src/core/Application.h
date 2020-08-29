#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>

namespace ct
{
    class Application
    {
    private:
        /* data */
    public:
        Application(/* args */);
        virtual ~Application();

        void Print(const char *message);
    };

} // namespace ct

#endif // APPLICATION_H