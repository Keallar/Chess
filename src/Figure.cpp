#include "Figure.h"
#include "shared.h"

const Vector2 FIGURE_SIZE(80, 80);

Figure::Figure(eColor color, eType t) : _dead(false), _moving(false)
{
	_color = color;
	_type = t;
	_view = new Sprite;
	Vector2 center = _view->getSize() / 2;
	center.x = int(center.x);
	center.y = int(center.y);
	_view->setAnchorInPixels(center);
}

void Figure::init()
{
	ResAnim* rs = new ResAnim;
	if (_color == eColor::Black) 
	{
		if (_type == eType::Pawn)
		{
			rs = res.getResAnim("b_p");
		}
		else if (_type == eType::Rock)
		{
			rs = res.getResAnim("b_r");
		}
		else if (_type == eType::Bishop)
		{
			rs = res.getResAnim("b_b");
		}
		else if (_type == eType::Knight)
		{
			rs = res.getResAnim("b_n");
		}
		else if (_type == eType::Queen)
		{
			rs = res.getResAnim("b_q");
		}
		else if (_type == eType::King)
		{
			rs = res.getResAnim("b_k");
		}
	}
	if (_color == eColor::White)
	{
		if (_type == eType::Pawn)
		{
			rs = res.getResAnim("w_p");
		}
		else if (_type == eType::Rock)
		{
			rs = res.getResAnim("w_r");
		}
		else if (_type == eType::Bishop)
		{
			rs = res.getResAnim("w_b");
		}
		else if (_type == eType::Knight)
		{
			rs = res.getResAnim("w_n");
		}
		else if (_type == eType::Queen)
		{
			rs = res.getResAnim("w_q");
		}
		else if (_type == eType::King)
		{
			rs = res.getResAnim("w_k");
		}
	}
	_view->setResAnim(rs);
	_view->setSize(80, 80);
}

spTween Figure::move(const Vector2& pos)
{
	_moving = true;
	spTween tween = _view->addTween(Actor::TweenPosition(pos), 200);
	tween->addDoneCallback(CLOSURE(this, &Figure::moved));
	return tween;
}

void Figure::moved(Event* ev)
{
	_moving = false;
}

void Figure::explode()
{
	_dead = true;
	_view->detach();
}

void Figure::exploded(Event* ev)
{
	_exploiding = false;
}

void Figure::select()
{
	_view->addTween(Actor::TweenRotation(1), 500, -1, true);
}

void Figure::unselect()
{
	_view->removeTweens(false);
	_view->addTween(Actor::TweenRotation(0), 250);
}

void Figure::setPos(const float x, const float y)
{
	_view->setPosition(x, y);
}

bool Figure::isMoving() const
{
	return _moving;
}

bool Figure::isDead() const
{
	return _dead;
}

bool Figure::isExploiding() const
{
	return _exploiding;
}

eType Figure::getType() const
{
	return _type;
}

spActor Figure::getView() const 
{
	return _view;
}

