#pragma once

// Globaly override the new and delete operator to use Memory
// Each dll should include in one cpp file the MemoryOperatorImplementation 
// because the new / delete operator cannot be dynamicly link, so the implementation have to present in each dll

#pragma warning(push)
#pragma warning(disable: 6387)
#pragma warning(disable: 28196)
#pragma warning(disable: 28251)

void* operator new (size_t Size);
void* operator new[](size_t Size);
void operator delete(void* Memory);
void operator delete[](void* Memory);

#pragma warning(pop)
