#include "player.h"

#include "bullet.h"
#include "../game.h"
#include "../constant.h"

// *************** CONSTRUCTOR & DESTRUCTOR ******************** //
Player::Player(int x, int y) : Entity(x, y) {
  this->spr->set_scale(1, 3);
  this->is_shooting = false;
}

Player::~Player() {
  this->destroy();
}

// ************************ METHOD ***************************** //
void Player::pre_update() {
  Entity::pre_update();
  if (has_collide_()) { this->destroy(); }

  if (Game::controller->key_up) { dy -= flight_spd_; }
  if (Game::controller->key_down) { dy += flight_spd_; }
  if (Game::controller->key_left) { dx -= flight_spd_; }
  if (Game::controller->key_right) { dx += flight_spd_; }
  check_shoot_state_();
  if (Game::controller->key_fire && shoot_delay_.get_time() == 0) {
    is_shooting = true;
    shoot_delay_.start();
  }
}

void Player::update() {
  Entity::update();
  // Check bounding
  x_pos = (cx_+xr_)*kGrid;
  if (x_pos <= 0) {
    x_pos = 0;
    cx_ = 0;
    xr_ = 0;
  } else if (x_pos >= (kScreenWidth - this->spr->get_w())) {
    x_pos = kScreenWidth - this->spr->get_w();
    cx_ = x_pos/kGrid;
    xr_ = 0;
  }

  y_pos = (cy_+yr_)*kGrid;
  if (y_pos <= 0) {
    y_pos = 0;
    cy_ = 0;
    yr_ = 0;
  } else if (y_pos >= (kScreenHeight - this->spr->get_h())) {
    y_pos = kScreenHeight - this->spr->get_h();
    cy_ = y_pos/kGrid;
    yr_ = 0;
  }
}

void Player::post_update() {
  Entity::post_update();

  spr->set_pos(x_pos, y_pos);
  spr->set_scale(dir * spr->get_scale_x(), 1); // Direction
  this->spr->play();
  this->spr->update();
}

void Player::check_shoot_state_() {
  if ((shoot_delay_.get_time()/1000.f) >= 0.5) { shoot_delay_.stop(); }
}

// TODO(nghialam): Add a game framework to support utils work like this
template<typename Base, typename T>
bool Player::is_bullet_(const T* ptr) {
  return dynamic_cast<const Base*>(ptr) != nullptr;
}

bool Player::has_collide_() {
  for (auto* e : *Entity::ALL) {
    if (e != this && !is_bullet_<Bullet>(e))
      if (this->x_pos < e->x_pos + e->spr->get_w() &&
          this->x_pos + this->spr->get_w() > e->x_pos &&
          this->y_pos < e->y_pos + e->spr->get_h() &&
          this->y_pos + this->spr->get_h() > e->y_pos)
        return true;
  }
  return false;
}
