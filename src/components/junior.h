#pragma once

#include "avancezlib.h"

#include "component.h"
#include "game_object.h"

#include "components/liana.h"
#include "components/platform.h"
#include "components/hazard.h"

#include "data/sprite_animation.h"
#include "data/subject.h"
#include "data/state.h"

enum class Direction { Left, Right, Down, Up };
enum class JuniorState { Running, Jumping, Climbing, ClimbingStretched };
enum class ClimbSide { Left, Right };
enum class JuniorEvent { Jump, Land, Death, StartClimb, StopClimb, };

class JuniorController
	: public Component
	, public Subject<JuniorEvent>
{
	glm::vec2 _vel;
	Direction _direction;
	bool _isGrounded;
	bool isDead;
	CollideComponent* collider;
	ClimbSide climbSide;

	StateMachine<JuniorController*>* stateMachine;

public:
	Liana* goalLiana;
	bool onGoal;

	Direction getDirection() const;
	glm::vec2 getVelocity() const;
	bool isGrounded() const;
	bool IsDead() const;
	ClimbSide GetClimbSide() const;

	JuniorState state;

	void Create();
	void Update(float dt);
	void Destroy();

	class Climbing : public State<JuniorController*>
	{
		Liana* primaryLiana;
		bool isStretching;
		ClimbSide side;
	public:
		Climbing(Liana* liana, ClimbSide side);
		void Enter();
		void Update(float dt);
	};

	class RunningAndJumping : public State<JuniorController*>
	{
		glm::vec2 vel0;
		bool isJumping;

	public:
		RunningAndJumping(glm::vec2 velocity);
		void Enter();
		void Update(float dt);

		template <typename T> int sgn(T val) {
			return (T(0) < val) - (val < T(0));
		}
	};
};
