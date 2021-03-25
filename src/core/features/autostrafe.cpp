//#include <math.h>
#include "../../includes.hpp"
#include "features.hpp"

void Features::AutoStrafe::createMove(CUserCmd* cmd) {
    if (Globals::localPlayer) {
        if (CONFIGBOOL("Misc>Misc>Movement>Auto Strafe")) {
            // GetClientEntityFromHandle is being gay
            Weapon* weapon        = (Weapon*) Interfaces::entityList->GetClientEntity((uintptr_t)Globals::localPlayer->activeWeapon() & 0xFFF);
            QAngle viewAngles;
            static bool direction;
            float viewChange      = 0.0f;
            bool isMoving         = cmd->buttons & (1 << 3)  /*Forward*/ \
                                 || cmd->buttons & (1 << 4)  /*Back*/    \
                                 || cmd->buttons & (1 << 9)  /*Left*/    \
                                 || cmd->buttons & (1 << 10) /*Right*/   ;

            if (!( Globals::localPlayer->health() > 0)\
                || Globals::localPlayer->moveType() == 9  /*NoClip*/ \
                || Globals::localPlayer->moveType() == 10 /*Ladder*/ )
                return;

            if (cmd->buttons & (1 << 3) && Globals::localPlayer->velocity().Length() <= 50.0f)
                cmd->forwardmove = 250.0f;
            
            if (Globals::localPlayer->velocity().Length() > 50.f)
                viewChange = 30.0f * fabsf(30.0f / Globals::localPlayer->velocity().Length());
            if (weapon && weapon->ammo() != 0 && cmd->buttons & (1 << 0) /*Attack*/ )
                viewChange = 0.0f;

            Interfaces::engine->GetViewAngles(viewAngles);

            if (!(Globals::localPlayer->flags() & (1 << 0) /*On Ground*/ ) && !isMoving)
            {
                if (direction || cmd->mousedx > 1)
                {
                    // Right
                    Log::log(LOG, "Right");
                    viewAngles.y += viewChange;
                    cmd->sidemove = 250.0f;
                }
                else if (!direction || cmd->mousedx < 1)
                {
                    // Left
                    Log::log(LOG, "Left");
                    viewAngles.y -= viewChange;
                    cmd->sidemove = -250.0f;
                }
                direction = !direction;
            }

            normalizeAngles(viewAngles);
            clampAngles(viewAngles);
            correctMovement(viewAngles, cmd, cmd->forwardmove, cmd->sidemove);
            
            //if (!Settings::AutoStrafe::silent)
            //cmd->viewangles = viewAngles;
        }
    }
}