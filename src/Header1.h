#pragma once
#include <assimp/Importer.hpp>

class MyClass
{
public:
	MyClass();
	~MyClass();

private:
	Assimp::Importer _importer;
};

MyClass::MyClass()
{
}

MyClass::~MyClass()
{
}