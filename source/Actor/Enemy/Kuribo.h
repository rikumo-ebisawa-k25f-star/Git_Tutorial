
#pragma once

enum class KuriboState
{
    eSave,   // ¶‘¶ó‘Ô
    eDeath   // €–Só‘Ô
};

class Kuribo
{
public:
    Kuribo();
    ~Kuribo();

    void Init(float startX, float startY, float speed);

    bool Update(float delta_time);

    void Draw();

private:
    float x;
    float y;
    float speed;

    KuriboState state;
};
