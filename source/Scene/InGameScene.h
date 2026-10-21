#pragma once
#pragma once

#include "../Actor/Enemy/Kuribo.h"

class InGameScene
{
public:
    InGameScene() = default;
    ~InGameScene() = default;

    // ‰Šú‰»ˆ—
    void Init();

    // XVˆ—
    void Update();

    // •`‰æˆ—
    void Draw();

private:
    // ‰E‚ÉˆÚ“®‚·‚é“G
    Kuribo enemy;
};
