﻿#include "WaterBallon.h"

void WaterBallon::SetEffectDir(const int x, const int y, const int dir, int& dirCount)
{
	int xCount = 0;		
	int yCount = 0;		
	int direction = 0;	

	switch (dir)
	{
	case Direction::TOP:
		yCount = -1;
		direction = Direction::TOP;
		break;
	case Direction::BOTTOM:
		yCount = 1;
		direction = Direction::BOTTOM;
		break;
	case Direction::RIGHT:
		xCount = 1;
		direction = Direction::RIGHT;
		break;
	case Direction::LEFT:
		xCount = -1;
		direction = Direction::LEFT;
		break;
	}

	for (int n = 1; n <= waterLength; n++)
	{
		//범위 제한
		if (((y + (yCount * n)) < 0) || ((y + (yCount * n)) > 10))
			return;
		else if (((x + (xCount * n)) < 0) || ((x + (xCount * n)) > 14))
			return;


		if (mapData->data[y + (yCount * n)][x + (xCount * n)] != 0)
		{
			hitObjectPos[direction].x = (x + (xCount * n));
			hitObjectPos[direction].y = (y + (yCount * n));

			//피격당한 오브젝트가 물풍선이 아니면 (벽, 블럭)이면 길이 제한
			if(mapData->data[y + (yCount * n)][x + (xCount * n)] != 3)
				return;
		}
		dirCount++;

		//공격범위값저장,,, 시작위치, 위, 우측, 아래, 좌측범위 return문있어가지고 값안들어갈때 있어서 안에다가 넣음
		attackArea.pos.x = mapPos.x;
		attackArea.pos.y = mapPos.y;
		switch (direction)
		{
		case Direction::TOP:
			attackArea.t = dirCount;
			break;
		case Direction::RIGHT:
			attackArea.r = dirCount;
			break;
		case Direction::BOTTOM:
			attackArea.b = dirCount;
			break;
		case Direction::LEFT:
			attackArea.l = dirCount;
			break;
		}
	}
}

WaterBallon::WaterBallon(const int name, const ObjectData::POSITION pos, const ObjectData::SIZE size, int hNumber, int vNumber, HBITMAP hBitmap,const int& waterLength)
			: DynamicObject(name, pos, size, hNumber, vNumber, hBitmap)
{
	charAnimationTick = GetTickCount64();	//생성시간기록
	order = pos.y;

	this->waterLength = waterLength;
}

WaterBallon::~WaterBallon()
{

}

void WaterBallon::Input()
{

}

void WaterBallon::Update()
{
	if (!isAlive)
		return;

	if (isEffect)
	{
		mapData->data[mapPos.y][mapPos.x] = 0;	//물풍선 맵에서 제거

		SetEffectDir(mapPos.x, mapPos.y, Direction::TOP, printDirCount.north);
		SetEffectDir(mapPos.x, mapPos.y, Direction::BOTTOM, printDirCount.south);
		SetEffectDir(mapPos.x, mapPos.y, Direction::RIGHT, printDirCount.east);
		SetEffectDir(mapPos.x, mapPos.y, Direction::LEFT, printDirCount.west);

		if (CheckmDelay(600, deadlineTick))
			isAlive = false;

		if (!CheckmDelay(50, charAnimationTick))
			return;
		printhNumber++;

		if (printhNumber == hNumber)	//시간지나면,, 현재조건 : 8번반복하면
			printhNumber = 0;

		return;
	}

	if (!CheckmDelay(300, charAnimationTick))
		return;

	printhNumber++;					//시간지나면 모양변경
	if (printhNumber == hNumber)	//시간지나면,, 현재조건 : 8번반복하면
	{
		printhNumber = 0;
		isEffect = true;
		deadlineTick = GetTickCount64();	// 이펙트 시작 타임부터 죽음까지 시간 체크
	}
}

void WaterBallon::Render(HDC hDC, HDC memDc)
{
	SelectObject(memDc, hBitmap);

	if (!isEffect)	//일반 출력
	{
		TransparentBlt(hDC,
			pos.x, pos.y,					//출력될 이미지 시작좌표
			imageWidth, imageHeight,		//출력될 이미지크기
			memDc,
			printhNumber * imageWidth, 0,	//이미지에서 출력할 시작위치
			imageWidth, imageHeight,		//이미지에서 출력할 이미지의 크기
			RGB(0, 0, 0));
	}
	else
	{
		//4방향 폭발 출력 + 중앙 폭발
		BoomRender(hDC, memDc, printDirCount.north, Direction::TOP);
		BoomRender(hDC, memDc, printDirCount.south, Direction::BOTTOM);
		BoomRender(hDC, memDc, printDirCount.east, Direction::RIGHT);
		BoomRender(hDC, memDc, printDirCount.west,  Direction::LEFT);
		BoomRender(hDC, memDc, 1, Direction::CENTER);

		printDirCount = { 0, 0, 0, 0 };
	}
}

void WaterBallon::BoomRender(HDC hDC, HDC memDc, const int printBoomImgCount, const int direction)
{
	int printBoomImgPos = 0;
	int addXPos = 0;	//출력위치 수정 변수, x
	int addYPos = 0;	//출력위치 수정 변수, y

	switch (direction)
	{
	case Direction::TOP:
		printBoomImgPos = 5;	addYPos = -1;	break;
	case Direction::BOTTOM:
		printBoomImgPos = 9;	addYPos = 1;	break;
	case Direction::RIGHT:
		printBoomImgPos = 7;	addXPos = 1;	break;
	case Direction::LEFT:
		printBoomImgPos = 3;	addXPos = -1;	break;
	case Direction::CENTER:
		printBoomImgPos = 1;	break;
	}

	for (int n = 1; n <= printBoomImgCount; n++)
	{
		//폭발 맨끝 처리
		if ((n == waterLength) && (printBoomImgPos != 1))
		{
			TransparentBlt(hDC,
				pos.x + (addXPos * n * BLOCK_X), pos.y + (addYPos * n * BLOCK_X),					//출력될 이미지 시작좌표
				imageWidth, imageHeight,		//출력될 이미지크기
				memDc,
				printhNumber * imageWidth, imageHeight * (printBoomImgPos-1),	//이미지에서 출력할 시작위치
				imageWidth, imageHeight,		//이미지에서 출력할 이미지의 크기
				RGB(0, 0, 0));

			return;
		}

		TransparentBlt(hDC,
			pos.x + (addXPos * n * BLOCK_X), pos.y + (addYPos * n * BLOCK_X),					//출력될 이미지 시작좌표
			imageWidth, imageHeight,		//출력될 이미지크기
			memDc,
			printhNumber * imageWidth, imageHeight * printBoomImgPos,	//이미지에서 출력할 시작위치
			imageWidth, imageHeight,		//이미지에서 출력할 이미지의 크기
			RGB(0, 0, 0));
	}
}

void WaterBallon::GetMapData(MapData* mapData)
{
	this->mapData = mapData;

	mapPos.x = ((pos.x + 20) / BLOCK_X) - 1;
	mapPos.y = ((pos.y + 2) / BLOCK_Y) - 1;

	//물풍선 위치 맵에 반영
	mapData->data[mapPos.y][mapPos.x] = 3;
}

const bool WaterBallon::GetIsEffect()
{
	return isEffect;
}

const bool WaterBallon::GetIsAlive()
{
	return isAlive;
}

void WaterBallon::SetIsEffect(const bool isEffect)
{
	this->isEffect = isEffect;
	printhNumber = 0;
	deadlineTick = GetTickCount64();
}

ObjectData::POSITION WaterBallon::GetMapPos()
{
	return mapPos;
}

ObjectData::POSITION* WaterBallon::GetHitObjectPos()
{
	return hitObjectPos;
}

void WaterBallon::SetColor(int color)
{
	this->color = color;
}

const int& WaterBallon::GetColor() const
{
	return color;
}

const AttackArea& WaterBallon::GetAttackArea() const
{
	return attackArea;
}

bool WaterBallon::CheckmDelay(const int delayTime, ULONGLONG& tick)
{
	if (GetTickCount64() > tick + delayTime)
	{
		tick = GetTickCount64();
		return true;
	}
	return false;
}