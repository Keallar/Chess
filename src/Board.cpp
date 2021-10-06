#include <iostream>
#include "Board.h"

const Point dirUp(0, -1);
const Point dirDown(0, 1);
const Point dirLeft(-1, 0);
const Point dirRight(1, 0);
const Point dirDiagLeftUp(-1, -1);
const Point dirDiagRightUp(1, -1);
const Point dirDiagLeftDown(-1, 1);
const Point dirDiagRightDown(1, 1);

Board::Board() : _size(0, 0), _selected(0), turn(eTurn::None)
{
}

void Board::init(int w, int h)
{
	_size.x = w;
	_size.y = h;
	_view = new Sprite;
	_field.resize(_size.x * _size.y);
	for (int y = 0; y < _size.y; ++y)
	{
		for (int x = 0; x < _size.x; ++x)
		{
			_field[x + y * _size.x].pos = Point(x, y);
		}
	}
	createPawns();
	createRocks();
	createBishops();
	createKnights();
	createQueens();
	createKings();

	turn = eTurn::White;

	_view->setSize(_size.x * FIGURE_SIZE.x, _size.y * FIGURE_SIZE.y);
	_view->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &Board::touched));
	_view->setTouchChildrenEnabled(false);
	_view->setCallbackDoUpdate(CLOSURE(this, &Board::update));
}

struct swapData : public Object
{
	swapData(space* A, space* B) : a(A), b(B) {}
	space* a;
	space* b;
};

space* Board::getSpace(const Point& pos, bool check)
{
	if (pos.x < 0 || pos.y < 0)
		return 0;
	if (pos.x > _size.x || pos.y > _size.y)
		return 0;
	space& sp = _field[pos.x + pos.y * _size.x];
	if (check)
	{
		if (sp.figure)
		{
			if (sp.figure->isDead())
				return 0;
			if (sp.figure->isExploiding())
				return 0;
			if (sp.figure->isMoving())
				return 0;
		}
	}
	return &sp;
}

space* Board::checkFigure(space& obj)
{
	if (!obj.figure)
		return nullptr;

	Point objPos = obj.pos;
	eColor objColor = obj.figure->getColor();
	for (int y = 0; y < _size.y; ++y)
	{
		for (int x = 0; x < _size.x; ++x)
		{
			space& opSp = _field[x * y + _size.x];
			if (opSp.figure)
			{
				eColor opColor = opSp.figure->getColor();
				if (objColor != opColor)
				{
					Point opPos = opSp.pos;
					if (objPos == opPos)
					{
						return &opSp;
					}
				}
			}
		}
	}
	return nullptr;
}

void Board::update(const UpdateState& us)
{
	//check isDead for figures
	for (int y = 0; y < _size.y; ++y)
	{
		for (int x = 0; x < _size.x; ++x)
		{
			space& sp = _field[x * y + _size.x];
			spFigure tempFig = sp.figure;
			if (!tempFig)
				continue;
			if (tempFig->isDead())
				sp.figure = 0;
		}
	}
	//collision with opponent figures
	for (int y = 0; y < _size.y; ++y)
	{
		for (int x = 0; x < _size.x; ++x)
		{
			space* sp = getSpace(Point(x, y));
			if (!sp)
				continue;
			space* exSp = checkFigure(*sp);
			if (exSp)
				exSp->figure->explode();
		}
	}
}

void Board::touched(Event* event)
{
	TouchEvent* te = safeCast<TouchEvent*>(event);
	Vector2 pos = te->localPosition;
	Point spacePos;
	spacePos.x = (int)(pos.x / FIGURE_SIZE.x);
	spacePos.y = (int)(pos.y / FIGURE_SIZE.y);
	space* sp = getSpace(spacePos);
	if (_selected)
	{
		bool col = collision(spacePos);
		if (!col)
		{
			if (sp)
			{
				_selected->figure->unselect();
				Point dir = _selected->pos - sp->pos;
				eType selectedType = _selected->figure->getType();
				spTween tween = move(*_selected, *sp);
				tween->setDoneCallback(CLOSURE(this, &Board::moved));
				_selected = 0;
				if (selectedType == eType::Pawn)
				{

				}
				else if (selectedType == eType::Rock)
				{

				}
				else if (selectedType == eType::Bishop)
				{

				}
				else if (selectedType == eType::Knight)
				{

				}
				else if (selectedType == eType::Queen)
				{

				}
				else if (selectedType == eType::King)
				{

				}
			}
		}
		else
		{
			eColor selectedColor = _selected->figure->getColor();
			eColor spColor = sp->figure->getColor();
			if (selectedColor != spColor)
			{
				_selected->figure->unselect();
				Point dir = _selected->pos - sp->pos;
				eType selectedType = _selected->figure->getType();
				spTween tween = move(*_selected, *sp);
				tween->setDoneCallback(CLOSURE(this, &Board::moved));
				_selected = 0;
			}
			else
			{
				_selected->figure->unselect();
				_selected = 0;
			}
		}
	}
	else
	{
		if (sp->figure)
		{
			_selected = sp;
			if (sp)
			{
				sp->figure->select();
			}
		}
	}
}

spTween Board::move(space& obj, space& nPos)
{
	spTween tween;
	Point spacePos = nPos.pos;
	spacePos *= 80;
	spacePos.x += 160;
	tween = obj.figure->move(spacePos);
	std::swap(obj.figure, nPos.figure);
	tween->setDataObject(new swapData(&obj, &nPos));
	return tween;
}

void Board::moved(Event* event)
{
	TweenEvent* te = safeCast<TweenEvent*>(event);
	spObject data = te->tween->getDataObject();
	swapData* sw = safeCast<swapData*>(data.get());
	space& a = *sw->a;
	space& b = *sw->b;
	//std::swap(a.pos, b.pos);
}

bool Board::collision(Point& spacePos)
{
	for (int y = 0; y < _size.y; ++y)
	{
		for (int x = 0; x < _size.x; ++x)
		{
			space* sp = getSpace(Point(x, y));
			if (!sp->figure)
				return false;
			if (!_selected)
				return false;
			if (spacePos == sp->pos)
			{
				return true;
			}
		}
	}
	return false;
}

void Board::createPawns()
{
	//create white
	for (int i = 0; i < 8; ++i)
	{
		_field[8 + i].figure = new Figure(eColor::White, eType::Pawn);
		_field[8 + i].figure->init();
		_field[8 + i].figure->setPos(160 + _field[8 + i].pos.x * FIGURE_SIZE.x,
			_field[8 + i].pos.y * FIGURE_SIZE.y);
		_field[8 + i].figure->getView()->attachTo(getStage());
	}
	//create black
	for (int j = 0; j < 8; ++j)
	{
		_field[63 - 8 - j].figure = new Figure(eColor::Black, eType::Pawn);
		_field[63 - 8 - j].figure->init();
		_field[63 - 8 - j].figure->setPos(160 + _field[63 - 8 - j].pos.x * FIGURE_SIZE.x,
			_field[63 - 8 - j].pos.y * FIGURE_SIZE.y);
		_field[63 - 8 - j].figure->getView()->attachTo(getStage());
	}
}

void Board::createRocks()
{
	//create white
	_field[0].figure = new Figure(eColor::White, eType::Rock);
	_field[0].figure->init();
	_field[0].figure->setPos(160 + _field[0].pos.x * FIGURE_SIZE.x,
		_field[0].pos.y * FIGURE_SIZE.y);
	_field[0].figure->getView()->attachTo(getStage());
	_field[7].figure = new Figure(eColor::White, eType::Rock);
	_field[7].figure->init();
	_field[7].figure->setPos(160 + _field[7].pos.x * FIGURE_SIZE.x,
		_field[7].pos.y * FIGURE_SIZE.y);
	_field[7].figure->getView()->attachTo(getStage());
	//create black
	_field[56].figure = new Figure(eColor::Black, eType::Rock);
	_field[56].figure->init();
	_field[56].figure->setPos(160 + _field[56].pos.x * FIGURE_SIZE.x,
		_field[56].pos.y * FIGURE_SIZE.y);
	_field[56].figure->getView()->attachTo(getStage());
	_field[63].figure = new Figure(eColor::Black, eType::Rock);
	_field[63].figure->init();
	_field[63].figure->setPos(160 + _field[63].pos.x * FIGURE_SIZE.x,
		_field[63].pos.y * FIGURE_SIZE.y);
	_field[63].figure->getView()->attachTo(getStage());
}

void Board::createBishops()
{
	//create white
	_field[2].figure = new Figure(eColor::White, eType::Bishop);
	_field[2].figure->init();
	_field[2].figure->setPos(160 + _field[2].pos.x * FIGURE_SIZE.x,
		_field[2].pos.y * FIGURE_SIZE.y);
	_field[2].figure->getView()->attachTo(getStage());
	_field[5].figure = new Figure(eColor::White, eType::Bishop);
	_field[5].figure->init();
	_field[5].figure->setPos(160 + _field[5].pos.x * FIGURE_SIZE.x,
		_field[5].pos.y * FIGURE_SIZE.y);
	_field[5].figure->getView()->attachTo(getStage());
	//create black
	_field[58].figure = new Figure(eColor::Black, eType::Bishop);
	_field[58].figure->init();
	_field[58].figure->setPos(160 + _field[58].pos.x * FIGURE_SIZE.x,
		_field[58].pos.y * FIGURE_SIZE.y);
	_field[58].figure->getView()->attachTo(getStage());
	_field[61].figure = new Figure(eColor::Black, eType::Bishop);
	_field[61].figure->init();
	_field[61].figure->setPos(160 + _field[61].pos.x * FIGURE_SIZE.x,
		_field[61].pos.y * FIGURE_SIZE.y);
	_field[61].figure->getView()->attachTo(getStage());
}

void Board::createKnights()
{
	//create white
	_field[1].figure = new Figure(eColor::White, eType::Knight);
	_field[1].figure->init();
	_field[1].figure->setPos(160 + _field[1].pos.x * FIGURE_SIZE.x,
		_field[1].pos.y * FIGURE_SIZE.y);
	_field[1].figure->getView()->attachTo(getStage());
	_field[6].figure = new Figure(eColor::White, eType::Knight);
	_field[6].figure->init();
	_field[6].figure->setPos(160 + _field[6].pos.x * FIGURE_SIZE.x,
		_field[6].pos.y * FIGURE_SIZE.y);
	_field[6].figure->getView()->attachTo(getStage());
	//create black
	_field[57].figure = new Figure(eColor::Black, eType::Knight);
	_field[57].figure->init();
	_field[57].figure->setPos(160 + _field[57].pos.x * FIGURE_SIZE.x,
		_field[57].pos.y * FIGURE_SIZE.y);
	_field[57].figure->getView()->attachTo(getStage());
	_field[62].figure = new Figure(eColor::Black, eType::Knight);
	_field[62].figure->init();
	_field[62].figure->setPos(160 + _field[62].pos.x * FIGURE_SIZE.x,
		_field[62].pos.y * FIGURE_SIZE.y);
	_field[62].figure->getView()->attachTo(getStage());

}

void Board::createQueens()
{
	//create white
	_field[3].figure = new Figure(eColor::White, eType::Queen);
	_field[3].figure->init();
	_field[3].figure->setPos(160 + _field[3].pos.x * FIGURE_SIZE.x,
		_field[3].pos.y * FIGURE_SIZE.y);
	_field[3].figure->getView()->attachTo(getStage());
	//create black
	_field[59].figure = new Figure(eColor::Black, eType::Queen);
	_field[59].figure->init();
	_field[59].figure->setPos(160 + _field[59].pos.x * FIGURE_SIZE.x,
		_field[59].pos.y * FIGURE_SIZE.y);
	_field[59].figure->getView()->attachTo(getStage());

}

void Board::createKings()
{
	//create white
	_field[4].figure = new Figure(eColor::White, eType::King);
	_field[4].figure->init();
	_field[4].figure->setPos(160 + _field[4].pos.x * FIGURE_SIZE.x,
		_field[4].pos.y * FIGURE_SIZE.y);
	_field[4].figure->getView()->attachTo(getStage());
	//create black
	_field[60].figure = new Figure(eColor::Black, eType::King);
	_field[60].figure->init();
	_field[60].figure->setPos(160 + _field[60].pos.x * FIGURE_SIZE.x,
		_field[60].pos.y * FIGURE_SIZE.y);
	_field[60].figure->getView()->attachTo(getStage());
}

spActor Board::getView()
{
	return _view;
}

void Board::free()
{
	_field.clear();
	_view->detach();
	_view = 0;
	_current = 0;
}
