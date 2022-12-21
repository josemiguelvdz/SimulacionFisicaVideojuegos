#pragma once
#include <limits.h>

struct RigidInfo {
    int pStatus;
    double actLifeTime;
    double maxLifeTime;
    float scale;
};

enum RigidStatus
{
    S_ACTIVE, S_DEAD
};

