#pragma once

#include "framework.h"
#include <assert.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <fbxsdk.h>

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#include <DirectXMath.h>

using namespace DirectX;

#include "struct.h"
#include "enum.h"

#include <vector>
using std::vector;

#include <fstream>
using std::ifstream;

#include <map>
using std::map;

#include <queue>
using std::queue;

#include <string>
using std::string;
using std::wstring;
using std::pair;
using std::make_pair;

#include "AlignedAllocationPolicy.h"