#include "GameObject.h"
#include "Core/include.h"
#include "Transform.h"
#include "Render/Shape.h"


class UI : public GameObject
{
public:
	UI() = default;
	virtual ~UI() = default;

	/*void Initialize(gcle::Shapes shape) override;
	void Update(float32 dt) override;*/

	void OnActivate() {};

};