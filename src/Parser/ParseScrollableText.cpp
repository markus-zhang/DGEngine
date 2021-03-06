#include "ParseAction.h"
#include "GameUtils.h"
#include "ParseScrollableText.h"
#include "ParseText.h"
#include "ScrollableText.h"
#include "Utils.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseScrollableText(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return;
		}

		auto text = parseDrawableTextObj(game, elem);
		if (text == nullptr)
		{
			return;
		}

		auto split = getUIntKey(elem, "splitText");
		if (split > 0)
		{
			auto splitText = Utils::splitInLines(text->getText(), split);
			text->setText(splitText);
		}

		auto speed = sf::milliseconds(getUIntKey(elem, "refresh"));
		auto scrollable = std::make_shared<ScrollableText>(std::move(text), speed);

		auto anchor = getAnchorKey(elem, "anchor");
		scrollable->setAnchor(anchor);
		auto pos = getVector2fKey<sf::Vector2f>(elem, "position");
		auto size = getVector2fKey<sf::Vector2f>(elem, "boxSize");
		if (getBoolKey(elem, "relativeCoords", true) == true)
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		scrollable->Position(pos);
		scrollable->Size(size);
		scrollable->Visible(getBoolKey(elem, "visible", true));

		scrollable->setLoop(getBoolKey(elem, "loop"));
		scrollable->setPause(getBoolKey(elem, "pause"));

		scrollable->reset();
		scrollable->updateViewPort(game);

		if (elem.HasMember("onComplete"))
		{
			scrollable->setAction(parseAction(game, elem["onComplete"]));
		}

		game.Resources().addDrawable(id, scrollable);
	}
}
