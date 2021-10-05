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
	space* getSpace(const Point& pos, bool check = true);

	void createPawns();
	void createRocks();
	void createBishops();
	void createKnights();
	void createQueens();
	void createKings();

	void update(const UpdateState& us);
	spTween move(space& obj, space& nPos);
	void touched(Event*);
	void moved(Event*);

	space* collWithOpFigure(space& obj);

	spSprite _view;
	std::vector<space> _field;
	space* _selected;
	Point _size;
	spTween _current;

	eTurn turn;
};

