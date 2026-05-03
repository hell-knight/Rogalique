\# Combat System



\## Overview

The combat system is built on ECS components and handles damage dealing, character death, and automatic/manual attack control. Key entities:



\- \*\*AttackComponent\*\* – base attack logic (target search, damage, cooldown).

\- \*\*AIAttackComponent\*\* – automatic attack for enemies (attacks when target is in range).

\- \*\*InputAttackComponent\*\* – input-driven attack (for the player).

\- \*\*HealthComponent\*\* – health, armor, death handling.



All components reside in the `MyEngine` namespace.



\---



\## Components



\### 1. AttackComponent

Stores attack parameters and searches for the closest alive target within a given radius.



\*\*Constructor parameters:\*\*

\- `gameObject` – owner of the component.

\- `damage` – base damage (float).

\- `radius` – target search radius (float).

\- `cooldown` – cooldown between attacks in seconds (float).



\*\*Public methods:\*\*

\- `SetDamage(float)` / `SetRadius(float)` / `SetCooldown(float)` – adjust parameters.

\- `bool Attack()` – attempts an attack. Returns `true` if a target was found and damaged, `false` otherwise.



\*\*Internal logic:\*\*

\- `FindTargetInRange()` – iterates over all objects in the scene (excluding self), finds the nearest one with a `HealthComponent` (not dead) within `radius`.

\- On success, calls `ApplyDamage(damage)` on the target and resets the cooldown timer.

\- The timer `timeSinceLastAttack` increases in `Update(deltaTime)`.



\*\*Notes:\*\*

\- The component does not require a pre-assigned target; it searches automatically.

\- If multiple targets are in range, the closest one is chosen.

\- If the cooldown has not expired, `Attack()` returns `false` immediately.



\### 2. AIAttackComponent

Automatically calls `AttackComponent::Attack()` when the assigned target (usually the player) is within the attack range.



\*\*Requirements:\*\* the object must have a `TransformComponent` and an `AttackComponent`.



\*\*Public methods:\*\*

\- `SetTarget(GameObject\* target)` – sets the target; stores its `TransformComponent`.

\- `SetAttackRange(float range)` – sets the distance at which an attack is triggered (default 150).



\*\*Behavior:\*\*

\- In `Update`, computes the distance to the target. If `distance ≤ attackRange`, it calls `attack->Attack()`.

\- If the target or attack component is missing, nothing happens.



\### 3. InputAttackComponent

Binds an attack to player input (e.g., spacebar).



\*\*Requirements:\*\* `InputComponent` and `AttackComponent` on the same object.



\*\*Behavior:\*\*

\- In `Update`, checks `input->IsAttackPressed()`. If true, calls `attack->Attack()`.

\- Holding the key calls the attack every frame, but the actual attack only happens when `AttackComponent` permits it (after the cooldown).



\### 4. HealthComponent

Manages health points, armor, and death.



\*\*Constructor parameters:\*\*

\- `gameObject`, `newHealth` (current and max health), `newArmor` (armor).



\*\*Public methods:\*\*

\- `ApplyDamage(float damage)` – deals damage reduced by armor (`damage - armor`, minimum 0). If health ≤ 0, calls `CheckDeath()`.

\- `Heal(float amount)` – restores health (cannot exceed max).

\- `bool IsDead()` – returns `true` if the object is dead.

\- `SubscribeOnDeath(std::function<void()>)` – subscribes a callback for death (returns subscription ID).

\- `UnsubscribeOnDeath(int id)` – unsubscribes.



\*\*Death event:\*\* when death occurs, all registered callbacks are invoked (e.g., destroy object, play sound, trigger effects).



\---



\## Typical Component Interactions



\### For the player:


GameObject

├── TransformComponent

├── SpriteRendererComponent

├── InputComponent

├── MovementComponent

├── HealthComponent

├── AttackComponent

└── InputAttackComponent ← connects Input and Attack



\### For an enemy (AI):

GameObject

├── TransformComponent

├── SpriteRendererComponent

├── FollowComponent (chasing)

├── HealthComponent

├── AttackComponent

└── AIAttackComponent ← connects following and attacking





\---



\## Setup Examples (from game code)



\### Player with attack

```cpp

auto player = MyEngine::GameWorld::Instance()->CreateGameObject("Player");

// ... initialize transform, sprite, input...



auto attack = player->AddComponent<MyEngine::AttackComponent>(player, 25.f, 150.f, 0.5f);

player->AddComponent<MyEngine::InputAttackComponent>();

auto health = player->AddComponent<MyEngine::HealthComponent>(player, 100.f, 10.f);

health->SubscribeOnDeath(\[player]() {

&#x20;   LOG\_INFO("Player died.");

&#x20;   // possibly restart level

});


### Enemy (Creeper) with attack

```cpp

auto enemy = MyEngine::GameWorld::Instance()->CreateGameObject("Creeper");

// ... transform, sprite, physics, animation...



auto attack = enemy->AddComponent<MyEngine::AttackComponent>(enemy, 15.f, 120.f, 1.5f);

auto aiAttack = enemy->AddComponent<MyEngine::AIAttackComponent>();

aiAttack->SetTarget(player->GetGameObject());

aiAttack->SetAttackRange(120.f);



auto health = enemy->AddComponent<MyEngine::HealthComponent>(enemy, 50.f, 0.f);

health->SubscribeOnDeath(\[enemy]() {

&#x20;   MyEngine::GameWorld::Instance()->DestroyGameObject(enemy);

});

