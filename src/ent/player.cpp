#include "player.h"

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
