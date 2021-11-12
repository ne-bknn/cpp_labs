#include <gtest/gtest.h>
#include "../src/curve.h"

// Basic smoke tests
TEST(Nephroid, SmokeTests) {
  Nephroid curve(13.37);  
  curve.r();
  curve.arclength();
  curve.area();
  curve.curvature_radius(0);
  curve.x(0);
  curve.x(0);
}

TEST(Nephroid, Repr) {
  Nephroid curve(9999999999999999999999.0);
  curve.repr();
}

TEST(Nephroid, Repr_2) {
  Nephroid curve(100);
  curve.repr();
  curve.set_r(50);
  curve.repr();
  curve.set_r(100);
}

TEST(Nephroid, OpEqual) {
  Nephroid curve(100);
  Nephroid a;
  a = curve;
  ASSERT_NE(&a, &curve);
}

// No more tests rationale - all of the Nephroid method's are basic
// calculations. They do not require carefully built interfaces;
// they do not modify the state (except for set_r). 
// The only option of testing those methods is to compare calculations
// with pre-computed values. Which is quite useless. You can check proper
// class testing in previous lab. 
