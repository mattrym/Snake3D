#include <map>
#include "game\Snake.h"

namespace game
{
	glm::vec3 Snake::moveVectors[4] = 
	{
		glm::vec3(0, 0, -1),
		glm::vec3(-1, 0, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(1, 0, 0)
	};

	void Snake::MoveForward()
	{
		std::vector<Segment>::iterator segment_it;
		std::list<Node>::iterator node_it;

		node_it = nodes.begin();
		for (segment_it = segments.begin(); segment_it != segments.end(); ++segment_it)
		{
			for (node_it = nodes.begin(); node_it != nodes.end(); ++node_it)
			{
				if (segment_it->Position() == node_it->Position() && segment_it->ID() + node_it->TTL() == length)
				{
					segment_it->ChangeDirection(node_it->Direction());
					node_it->DecrementTTL();
					if (!node_it->TTL())
					{
						node_it = nodes.erase(node_it);
					}
					break;
				}
			}
			segment_it->MoveForward();
		}
	}

	void Snake::ChangeDirection(DIR dir)
	{
		direction = dir;
		Node node(Position(), Direction(), length);
		nodes.push_front(node);
	}

	gfx::Model* Snake::HeadModel() const
	{
		if (segments.empty())
		{
			return nullptr;
		}
		return segments[0].Model();
	}

	glm::ivec2 Snake::Position() const
	{
		if (segments.empty())
		{
			return glm::ivec2();
		}
		return segments[0].Position();
	}

	void Snake::Segment::MoveForward()
	{
		switch (direction)
		{
		case game::NORTH:
			position.y++;
			break;
		case game::WEST:
			position.x--;
			break;
		case game::SOUTH:
			position.y--;
			break;
		case game::EAST:
			position.x++;
			break;
		default:
			break;
		}
		model->Translate(moveVectors[direction]);
	}
}