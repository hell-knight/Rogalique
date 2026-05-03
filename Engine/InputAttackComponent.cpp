#include "pch.h"
#include "GameObject.h"
#include "InputAttackComponent.h"
#include "InputComponent.h"
#include "AttackComponent.h"
#include "Logger.h"

namespace MyEngine
{
    InputAttackComponent::InputAttackComponent(GameObject* gameObject)
        : Component(gameObject)
    {
        input = gameObject->GetComponent<InputComponent>();
        attack = gameObject->GetComponent<AttackComponent>();
        if (!input || !attack)
        {
            LOG_ERROR("InputAttackComponent: missing Input or Attack component. Removing.");
            gameObject->RemoveComponent(this);
        }
    }

    void InputAttackComponent::Update(float)
    {
        if (input && attack && input->IsAttackPressed())
        {
            attack->Attack();
        }
    }
}