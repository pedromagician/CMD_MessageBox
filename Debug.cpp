﻿#include "Debug.h"

#ifdef _DEBUG

Debug::Debug()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(...);
}

Debug::~Debug()
{
	//_CrtDumpMemoryLeaks();
}

#endif
