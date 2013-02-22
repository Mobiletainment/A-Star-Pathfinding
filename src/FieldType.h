#pragma once

enum FieldType
{
	FieldTypeRoad = 1,
	FieldTypeGras = 2,
	FieldTypeWater = 3,
	FieldTypeUnwalkable = 4,
};

enum FieldTypeSpecial
{
	FieldTypeSpecialPositionNone = 0,
	FieldTypeSpecialPositionStart = 64,
	FieldTypeSpecialPositionTarget = 128
};

static FieldType GetFieldTypeByInt(int index)
{
	return (FieldType)index;
}

static int GetIntByFieldType(FieldType type)
{
	return type;
}