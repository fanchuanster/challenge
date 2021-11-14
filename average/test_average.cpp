#include "average.h"

#include <limits>
#include <iostream>

using namespace std;

#include <gtest/gtest.h>

float get_random(float min, float max) {
 return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

TEST(TestAverage, Basic) {

  auto average = new Average();

  EXPECT_EQ(0, average->get());

  average->add_value(0);
  average->add_value(0);
  average->add_value(0);
  EXPECT_EQ(0.0f, average->get());

  average->add_value(1);
  EXPECT_EQ(0.25f, average->get());

  average->add_value(-1);
  EXPECT_EQ(0.0f, average->get());
}

TEST(TestAverage, Extreme) {

  auto average = new Average();

  average->add_value(numeric_limits<float>::min());
  average->add_value(numeric_limits<float>::min());
  EXPECT_EQ(numeric_limits<float>::min(), average->get());

  average->add_value(-numeric_limits<float>::min());
  average->add_value(-numeric_limits<float>::min());
  EXPECT_EQ(0, average->get());

  EXPECT_GT(numeric_limits<float>::min(), 0);

  average->add_value(numeric_limits<float>::max());
  average->add_value(numeric_limits<float>::max());
  EXPECT_EQ(numeric_limits<float>::max() / 3, average->get());

  average->add_value(-numeric_limits<float>::max());
  EXPECT_EQ(numeric_limits<float>::max() / 7, average->get());

  average->add_value(-numeric_limits<float>::max());
  EXPECT_EQ(0, average->get());
}


TEST(TestAverage, Extreme_2) {

  auto average = new Average();

  const int NUM = 2000;
  double sum = 0;
  for (auto i=0; i<NUM; ++i) {
      auto val = get_random(numeric_limits<float>::max() - 1, numeric_limits<float>::max());
      average->add_value(val);

      sum += val;
  }

  EXPECT_LT(average->get() - numeric_limits<float>::max() - 1, 1);
  EXPECT_LT(numeric_limits<float>::max() - average->get(), 1);

  EXPECT_EQ(sum/NUM, average->get());
}


TEST(TestAverage, Negatives) {

  auto average = new Average(-10, 10);

  const int NUM = 2000000;
  double sum = 0;
  for (auto i=0; i<NUM; ++i) {
      auto val = get_random(-10, 10);
      average->add_value(val);

      sum += val;
  }


  EXPECT_LT(fabs(sum / NUM - average->get()), FLT_EPSILON*10) << "sum/NUM:"<<sum/NUM<<endl<<"average->get():"<<average->get()<<endl;;
}