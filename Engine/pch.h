#pragma once

#include <winsdkver.h>
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <wrl/client.h>
#include <wrl/event.h>
#include <DirectXMath.h>

#include <cstdint>
#include <cstdio>
#include <cstdarg>
#include <vector>
#include <memory>
#include <string>
#include <cwctype>
#include <exception>

#include <ppltasks.h>
#include <functional>

#include "Utility.h"

class Display;
class Graphics;