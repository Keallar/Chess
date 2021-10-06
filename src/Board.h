#pragma once
#include "oxygine-framework.h"
#include <vector>
#include "Figure.h"

using namespace oxygine;

DECLARE_SMART(Figure, spFigure);

enum class eTurn
{
	None,
	White,
	Black
};

struct space
{
	Point pos;
	spFigure figure;
};

class Board
{
public:
	Board();
	void init(int w, int h);
	void free();
	spActor getView();
private:
	space* getSpace(const Point& pos);

	void createPawns();
	void createRocks();
	void createBishops();
	void createKnights();
	void createQueens();
	void createKings();

	void update(const UpdateState& us);
	spTween move(space& obj, space& nPos);
	bool collision(Point& spacePos);
	void touched(Event*);
	void moved(Event*);

	space* checkFigure(Point& objPos);
	void changeTurn();
	void checkWin();

	void movePawn(space* sp, bool coll = false);
	void moveRock(space* sp, bool coll = false);

	spSprite _view;
	std::vector<space> _field;
	space* _selected;
	Point _size;
	spTween _current;
	bool _isTurned;
	bool _isEnded;

	eTurn _turn;

	spTextField _text;
};

