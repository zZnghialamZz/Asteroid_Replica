#ifndef ASTEROID_ENT_PLAYER_H_
#define ASTEROID_ENT_PLAYER_H_

#include "entity.h"

class Player : public Entity {
 private:
  const float flight_spd_ = 0.1;
   
 public:
  Player(int x, int y);
  virtual ~Player();

  void pre_update() override;
  void update() override;
  void post_update() override;
};

#endif //ASTEROID_ENT_PLAYER_H_
