#pragma once

#ifndef _SNAKE_H
#define _SNAKE_H

#include <list>
#include <vector>
#include "graphics\models\Model.h"

namespace game
{
	enum DIR
	{
		NORTH,
		WEST,
		SOUTH,
		EAST
	};

	class Snake
	{
	public:
		static glm::vec3 moveVectors[4];
		struct Segment;
		struct Node;

	private:
		int length;
		DIR direction;
		std::vector<Segment> segments;
		std::list<Node> nodes;

	public:
		Snake() { length = 0; }
		virtual ~Snake() { }
		
		void MoveForward();
		void ChangeDirection(DIR dir);
		
		gfx::Model* HeadModel() const;
		glm::ivec2 Position() const;
		DIR Direction() const { return direction; }

		friend class SceneCreator;
	};

	struct Snake::Segment
	{
	private:
		int id;
		DIR direction;
		glm::ivec2 position;
		gfx::Model* model;

	public:
		Segment(int _id, glm::vec2 _position, gfx::Model* _model) : id(_id), position(_position), model(_model) { }
		virtual ~Segment() { }

		void MoveForward();
		void ChangeDirection(DIR dir) { direction = dir; }

		int ID() const { return id; }
		glm::ivec2 Position() const { return position; }
		gfx::Model* Model() const { return model; }
		DIR Direction() const { return direction; }

		friend class SceneCreator;
	};

	struct Snake::Node 
	{
	private:
		glm::ivec2 position;
		DIR direction;
		int ttl;

	public:
		Node(glm::vec2 _position, DIR _direction, int _ttl) : position(_position), direction(_direction), ttl(_ttl) { }
		void DecrementTTL() { --ttl; }

		glm::ivec2 Position() const { return position; }
		DIR Direction() const { return direction; }
		int TTL() const { return ttl; }
	};
}

#endif // !_SNAKE_H