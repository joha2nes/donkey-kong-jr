#include "blinking_text.h"

#include "avancezlib.h"
#include "glm/vec3.hpp"
#include "game_object.h"
#include "text.h"

void BlinkingText::Create()
{
	if (text == nullptr)
		text = go->GetComponent<Text*>();
	color = text->color;
}

void BlinkingText::Update(float dt)
{
	time += Component::time->GetUnscaledDeltaTime();

	auto blinking = glm::fract(1.8f * time) > 0.5f;

	text->color = (unsigned)blinking * color;
}