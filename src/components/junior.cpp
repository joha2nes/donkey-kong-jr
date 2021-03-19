#include "junior.h"

#include "avancezlib.h"

#include "component.h"
#include "game_object.h"

#include "components/liana.h"
#include "components/platform.h"
#include "components/hazard.h"

#include "data/sprite_animation.h"
#include "data/subject.h"
#include "data/state.h"
#include "glm/glm.hpp"

Direction JuniorController::getDirection() const
{
	return _direction;
}

bool JuniorController::isGrounded() const
{
	return _isGrounded;
}

glm::vec2 JuniorController::getVelocity() const
{
	return _vel;
}

bool JuniorController::IsDead() const
{
	return isDead;
}

ClimbSide JuniorController::GetClimbSide() const
{
	return climbSide;
}

void JuniorController::Create()
{
	_direction = Direction::Right;
	_isGrounded = false;
	isDead = false;

	onGoal = false;

	collider = go->GetComponent<CollideComponent*>();
	stateMachine = new StateMachine<JuniorController*>(this, new RunningAndJumping({ 0, 0 }));
}

void JuniorController::Destroy()
{
	delete stateMachine;
}

void JuniorController::Update(float dt)
{
	if (!isDead)
	{
		auto maxOverlap = glm::vec2(0);
		// TODO: this might cause race conditions
		for (auto c : collider->collisions)
		{
			if (c.other->go->GetComponent<Platform*>() != nullptr)
			{
				if (abs(c.overlap.x) > abs(maxOverlap.x))
					maxOverlap.x = c.overlap.x;

				if (abs(c.overlap.y) > abs(maxOverlap.y))
					maxOverlap.y = c.overlap.y;
			}
		}
		go->position -= maxOverlap;

		auto xmin = 0;
		auto xmax = 224;
		go->position.x = glm::clamp(go->position.x, 0.f, 224.f - collider->size.x);

		auto wasGrounded = _isGrounded;
		_isGrounded = maxOverlap.y > 0 && _vel.y >= 0;

		if (_isGrounded)
			_vel.y = 0.f;

		if (!wasGrounded && _isGrounded)
			Notify(JuniorEvent::Land);

		stateMachine->Update(dt);

		for (auto col : collider->collisions)
		{
			// die
			if (col.other->go->GetComponent<Hazard*>())
			{
				isDead = true;
				Notify(JuniorEvent::Death);
			}
		}

		if (go->position.y > 246)
		{
			isDead = true;
			Notify(JuniorEvent::Death);
		}
	}
}

JuniorController::RunningAndJumping::RunningAndJumping(glm::vec2 velocity)
{
	vel0 = velocity;
}

void JuniorController::RunningAndJumping::Enter()
{
	env->_vel = vel0;
	env->collider->size = { 16, 16 };
	env->state = JuniorState::Running;
}

void JuniorController::RunningAndJumping::Update(float dt)
{
	auto gravity = glm::vec2(0.f, 300.f);

	if (env->_isGrounded)
	{
		auto moveRight = env->input->GetKey(SDL_SCANCODE_RIGHT);
		auto moveLeft = env->input->GetKey(SDL_SCANCODE_LEFT);

		env->_direction =
			moveRight ? Direction::Right :
			moveLeft ? Direction::Left :
			env->_direction;

		env->_vel.x =
			moveRight ? 40.f :
			moveLeft ? -40.f :
			0.f;
	}

	// apply physics
	env->_vel += dt * gravity;
	env->go->position += dt * env->_vel;

	if (env->_isGrounded)
	{
		if (env->input->GetKeyDown(SDL_SCANCODE_SPACE))
		{
			env->_vel.y = -110.f;
			env->Notify(JuniorEvent::Jump);
		}
	}

	// clamp speed
	auto maxSpeed = 200.f;
	auto absVelY = abs(env->_vel.y);
	env->_vel.y = absVelY > maxSpeed ? sgn(env->_vel.y) * maxSpeed : env->_vel.y;

	env->state = env->_isGrounded ? JuniorState::Running : JuniorState::Jumping;

	// Climb?
	for (auto c : env->collider->collisions)
	{
		auto liana = c.other->go->GetComponent<Liana*>();
		if (liana != nullptr)
		{
			auto climbSide = env->collider->GetCenter().x < liana->collider->GetCenter().x ? ClimbSide::Left : ClimbSide::Right;
			Switch(new Climbing(liana, climbSide));
		}
	}
}

JuniorController::Climbing::Climbing(Liana* liana, ClimbSide side)
{
	primaryLiana = liana;
	isStretching = false;
	this->side = side;
}

void JuniorController::Climbing::Enter()
{
	env->_vel = { 0.f, 0.f };
	if (primaryLiana == env->goalLiana)
		env->onGoal = true;
}

bool anyBlockingAt(CollideComponent* collider, const glm::vec2& pos)
{
	std::vector<Collision> cols;
	collider->TestPosition(cols, pos);

	auto anyBlocking = false;
	for (auto c : cols)
	{
		if (c.other->go->GetComponent<Platform*>())
		{
			anyBlocking = true;
			break;
		}
	}
	return anyBlocking;
}

void JuniorController::Climbing::Update(float dt)
{
	if (primaryLiana == nullptr)
	{
		// TODO: check if overlapping another liana
		Switch(new RunningAndJumping({ 0, 0 }));
		return;
	}

	env->climbSide = side;

	auto normalSize = glm::vec2{ 16.f, 16.f };
	auto stretchSize = glm::vec2{ 24.f, 16.f };
	auto size = isStretching ? stretchSize : normalSize;
	env->collider->size = size;

	auto climbUp = env->input->GetKey(SDL_SCANCODE_UP);
	auto climbDown = env->input->GetKey(SDL_SCANCODE_DOWN);
	auto goLeft = env->input->GetKeyDown(SDL_SCANCODE_LEFT);
	auto goRight = env->input->GetKeyDown(SDL_SCANCODE_RIGHT);

	auto lianaPos = primaryLiana->go->position;
	auto lianaCenterX = primaryLiana->collider->GetCenter().x;
	auto lianaSize = primaryLiana->go->GetComponent<CollideComponent*>()->size;

	auto xpos = side == ClimbSide::Left ? lianaCenterX - size.x : lianaCenterX;
	
	if (isStretching)
	{
		env->state = JuniorState::ClimbingStretched;

		Liana* secondaryLiana = nullptr;
		for (auto c : env->collider->collisions)
		{
			auto liana = c.other->go->GetComponent<Liana*>();
			if (liana != nullptr && liana != primaryLiana)
			{
				secondaryLiana = liana;
				break;
			}
		}

		if (secondaryLiana != nullptr)
		{
			auto yvel = -40.f * climbUp + 40.f * climbDown;
			auto yposGoal = env->go->position.y + dt * yvel;

			auto ymin= glm::max(
				primaryLiana->go->position.y,
				secondaryLiana->go->position.y);
			auto ymax = glm::min(
				primaryLiana->go->position.y + primaryLiana->collider->size.y,
				secondaryLiana->go->position.y + secondaryLiana->collider->size.y)
				- 4.f;
			auto ypos = glm::clamp(yposGoal, ymin, ymax);

			env->_vel.y = yvel;

			env->go->position = { xpos, ypos };

			if (side == ClimbSide::Left && goLeft)
			{
				isStretching = false;
				side = ClimbSide::Right;
				primaryLiana = secondaryLiana;
			}
			if (side == ClimbSide::Right && goRight)
			{
				isStretching = false;
				side = ClimbSide::Left;
				primaryLiana = secondaryLiana;
			}
		}
		else
		{
			env->go->position = { xpos, env->go->position.y };

			env->_vel.y = 0.f;

			if ((side == ClimbSide::Left && goLeft) ||
				(side == ClimbSide::Right && goRight))
			{
				size = normalSize;
				env->go->position.x = goLeft ? lianaPos.x - size.x : lianaPos.x + lianaSize.x;
				Switch(new RunningAndJumping({ 0, 0 }));
			}

		}

		if ((side == ClimbSide::Right && goLeft) ||
			(side == ClimbSide::Left && goRight))
		{
			isStretching = false;
		}
	}
	else
	{
		env->state = JuniorState::Climbing;

		auto yvel = -16.f * climbUp + 70.f * climbDown;
		auto ymin = lianaPos.y;
		auto ypos = glm::max(env->go->position.y + dt * yvel, ymin);

		env->_vel.y = yvel;
		
		if (ypos > lianaPos.y + lianaSize.y)
		{
			Switch(new RunningAndJumping({ 0, yvel }));
			return;
		}

		env->go->position = { xpos, ypos };

		if ((goLeft && side == ClimbSide::Left) ||
			(goRight && side == ClimbSide::Right))
		{
			isStretching = true;
			env->collider->size = stretchSize;
		}
		else if (goRight && side == ClimbSide::Left)
		{
			if (!anyBlockingAt(env->collider, { lianaCenterX, ypos }))
				side = ClimbSide::Right;
		}
		else if (goLeft && side == ClimbSide::Right)
		{
			if (!anyBlockingAt(env->collider, { lianaCenterX - size.x, ypos }))
				side = ClimbSide::Left;
		}
	}
}
