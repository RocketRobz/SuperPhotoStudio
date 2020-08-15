#ifndef COMMON_HPP
#define COMMON_HPP

#ifdef NDS
#include <nds.h>
#else
#include <3ds.h>
#endif

#include "gfx.hpp"
#include "gui.hpp"
#include "screenCommon.hpp"

extern std::unique_ptr<Screen> usedScreen, tempScreen;

#endif