#include "StringText.h"
#include <algorithm>
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

using Utils::str2int;

void StringText::calcLineCount()
{
	if (text.getString().isEmpty() == true)
	{
		lineCount = 0;
	}
	else
	{
		lineCount = std::count(text.getString().begin(), text.getString().end(), '\n') + 1;
	}
}

void StringText::calculateDrawPosition()
{
	auto size = Size();
	auto drawPos = GameUtils::getAlignmentPosition(pos, size, horizAlign, vertAlign);
	text.setPosition(drawPos);
}

void StringText::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		return;
	}
	auto size = Size();
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	calculateDrawPosition();
}

bool StringText::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitString(prop, '.');
	if (props.empty() == true)
	{
		return false;
	}
	auto propHash = str2int(props[0].c_str());
	switch (propHash)
	{
	case str2int("lineCount"):
		var = Variable((int64_t)lineCount);
		break;
	case str2int("text"):
		var = Variable(text.getString().toAnsiString());
		break;
	default:
		return GameUtils::getUIObjProp(*this, propHash, props, var);
	}
	return true;
}
