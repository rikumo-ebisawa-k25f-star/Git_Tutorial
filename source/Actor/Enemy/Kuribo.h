
#pragma once

enum class KuriboState
{
    eSave,
    eDeath
};

class Kuribo
{
public:
    Kuribo();
    ~Kuribo();

    void Init(float startX, float startY, float speed);
    bool Update(float delta_time, float cameraX, float screenWidth);
    void Draw(float cameraX);

private:
    int Kuribo_image;

    float Kx;
    float Ky;
    float Kspeed;

    KuriboState Kstate;
};
