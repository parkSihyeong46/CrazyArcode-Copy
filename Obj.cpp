﻿#include "Obj.h"

Obj::Obj()
{
}

Obj::Obj(const string name, const ObjectData::POSITION pos, const ObjectData::SIZE size, HBITMAP hBitmap)
{
	this->name = name;
	this->pos = pos;
	this->size = size;
	this->hBitmap = hBitmap;
}

Obj::~Obj()
{
}

bool Obj::operator==(const Obj& e)
{
	return this->name == e.name;
}

void Obj::SetPosition(ObjectData::POSITION pos)
{
	this->pos = pos;
}

void Obj::SetSize(ObjectData::SIZE size)
{
	this->size = size;
}

void Obj::SetHBitmap(HBITMAP hBitmap)
{
	this->hBitmap = hBitmap;
}

HBITMAP Obj::GetHBitmap()
{
	return hBitmap;
}
