#include"actor.h"
#include "Actor.h"
#include <DxLib.h>

Actor::Actor()
	: location()
	, radius(2.0f)
	, image(NULL)
	, scale(1.0)
	, z_layer(5U)
	, is_destroy(true)
	, actor_state(eActorState::eActive)
{

}

Actor::~Actor()
{

}

void Actor::Initialize()
{
	return;
}

void Actor::Update(float delta_second)
{
	return;
}

void Actor::Draw() const
{
	if (image)
	{
		DrawRotaGraphF(location.x, location.y, scale, 0.0, image, TRUE);
	}
}

void Actor::Finalize()
{
	return;
}

void Actor::OnHitCollisionEnter(Actor* actor)
{
	return;
}

const Vector2& Actor::GetLocation() const
{
	return location;
}

float Actor::GetRadius() const
{
	return radius;
}

eActorState Actor::GetState() const
{
	return actor_state;
}

unsigned char Actor::GetZLayer() const
{
	return z_layer;
}

void Actor::SetLocation(const Vector2& location)
{
	this->location = location;
}

void Actor::DestroyActor(Actor* target)
{
	if (target)
	{
		target->actor_state = eActorState::eDead;
	}
	else
	{
		this->actor_state = eActorState::eDead;
	}
}
