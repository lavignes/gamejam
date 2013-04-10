#ifndef FIFA_H
#define FIFA_H

#include "game.h"

void ai_goalie1(GameState* gs, int pos, float dt);
void ai_forwardl1(GameState* gs, int pos, float dt);
void ai_forwardr1(GameState* gs, int pos, float dt);
void ai_centerl1(GameState* gs, int pos, float dt);
void ai_centerr1(GameState* gs, int pos, float dt);

void ai_goalie2(GameState* gs, int pos, float dt);
void ai_forwardl2(GameState* gs, int pos, float dt);
void ai_forwardr2(GameState* gs, int pos, float dt);
void ai_centerl2(GameState* gs, int pos, float dt);
void ai_centerr2(GameState* gs, int pos, float dt);

void fifa_step(GameState* gs, float dt);

void fifa_draw(GameState* gs);

#endif /* FIFA_H */