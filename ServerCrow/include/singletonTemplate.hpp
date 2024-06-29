#ifndef _SINGLETON_TEMPLATE_HPP_
#define _SINGLETON_TEMPLATE_HPP_

#include <iostream>

template <typename T>
class SingletonTemplate
{
public:
    SingletonTemplate(const SingletonTemplate&) = delete;
    SingletonTemplate& operator=(const SingletonTemplate&) = delete;

    static T& getInstance()
    {
        static T instance;

        return instance;
    }

protected:
    SingletonTemplate(){}

    ~SingletonTemplate(){}
};

#endif