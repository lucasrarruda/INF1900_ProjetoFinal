#pragma once

#include <Windows.h>
#include <tuple>

namespace DisplayHelper
{
    constexpr float DEFAULT_DPI = 96;

    inline std::tuple<float, float> GetMonitorDpi()
    {
        HDC handleDeviceContext = GetDC(NULL);
        int dpiX = GetDeviceCaps(handleDeviceContext, LOGPIXELSX);
        int dpiY = GetDeviceCaps(handleDeviceContext, LOGPIXELSY);
        ReleaseDC(NULL, handleDeviceContext);

        if (dpiX == 96 || dpiY == 96)
            return std::make_tuple(0.8, 0.8);

        return std::make_tuple(DEFAULT_DPI / dpiX, DEFAULT_DPI / dpiY);
    }
}
