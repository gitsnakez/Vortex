#pragma once

#include "Vector3D.h"
#include "file_reader.h"

#include <map>
#include <string>
#include <vector>

class MapLoader
{
public:
	MapLoader();
	~MapLoader();

	void LoadMap(const wchar_t* filename);
	void LoadReadableMap(const wchar_t* filename);

	Vector3D GetPosOfObject(std::wstring objname);

	static MapLoader* Get();

private:
	std::map<std::wstring, Vector3D> m_positions;
};