#pragma once
#include "pch.h"
#include <iostream>
#include <exception>

namespace Exceptions
{
    class GameInvalid : public std::exception
    {
    public:
        GameInvalid(const char* mensagem) : mensagem_(mensagem) {}

        const char* what() const noexcept override { return mensagem_.c_str(); }

    private:
        std::string mensagem_;
    };
}