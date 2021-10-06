#pragma once
#include "oxygine-framework.h"

using namespace oxygine;

extern const Vector2 FIGURE_SIZE;

DECLARE_SMART(Figure, spFigure);

enum class eColor
{
	Black,
	White
};

enum class eType
{
	Pawn,
	Rock,
	Bishop,
	Knight,
	Queen,
	King
};

class Figure : public Object
{
public:
	Figure() = default;
	Figure(eColor color, eType t);
	void init();
	void setPos(const float x, const float y);
	void setIsTurn(const bool state);
	spActor getView() const;
	eType getType() const;
	eColor getColor() const;
	bool isExploiding() const;
	bool isDead() const;
	bool isMoving() const;
	bool isSelected() const;
	bool isTurn() const;
	void select();
	void unselect();
	spTween move(const Vector2& pos);
	void explode();
private:
	void moved(Event*);
	void exploded(Event*);

	bool _selected;
	bool _dead;
	bool _moving;
	bool _exploiding;
	bool _isTurn;
	eColor _color;
	eType _type;
	spSprite _view;
};

