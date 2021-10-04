#include "oxygine-framework.h"
#include "Board.h"
#include "shared.h"

using namespace oxygine;
Board board;

void preinit()
{

}

void init()
{
	res.loadXML("resources.xml");
	spSprite field = new Sprite;
	field->setResAnim(res.getResAnim("field"));
	field->attachTo(getStage());
	field->setSize(640, 640);
	field->setPosition(160, 0);

	board.init(8, 8);
	spActor view = board.getView();
	view->setScale(getStage()->getHeight() / view->getHeight());
	view->setPosition(getStage()->getSize() / 2 - view->getScaledSize() / 2);
	getStage()->addChild(view);
}

void destroy()
{
	board.free();
	res.free();
}

void update()
{

}