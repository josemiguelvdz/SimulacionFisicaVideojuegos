#pragma once
#include <limits.h>

struct RigidInfo {
    int pStatus;
    double actLifeTime;
    double maxLifeTime;
};

enum RigidStatus
{
    S_ACTIVE, S_DEAD
};

