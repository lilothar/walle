// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <math.h>
#include <stdio.h>
#include <walle/sys/Histogram.h>

namespace walle {
namespace util {

const double Histogram::kBucketLimit[kNumBuckets] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 25, 30, 35, 40, 45,
  50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 250, 300, 350, 400, 450,
  500, 600, 700, 800, 900, 1000, 1200, 1400, 1600, 1800, 2000, 2500, 3000,
  3500, 4000, 4500, 5000, 6000, 7000, 8000, 9000, 10000, 12000, 14000,
  16000, 18000, 20000, 25000, 30000, 35000, 40000, 45000, 50000, 60000,
  70000, 80000, 90000, 100000, 120000, 140000, 160000, 180000, 200000,
  250000, 300000, 350000, 400000, 450000, 500000, 600000, 700000, 800000,
  900000, 1000000, 1200000, 1400000, 1600000, 1800000, 2000000, 2500000,
  3000000, 3500000, 4000000, 4500000, 5000000, 6000000, 7000000, 8000000,
  9000000, 10000000, 12000000, 14000000, 16000000, 18000000, 20000000,
  25000000, 30000000, 35000000, 40000000, 45000000, 50000000, 60000000,
  70000000, 80000000, 90000000, 100000000, 120000000, 140000000, 160000000,
  180000000, 200000000, 250000000, 300000000, 350000000, 400000000,
  450000000, 500000000, 600000000, 700000000, 800000000, 900000000,
  1000000000, 1200000000, 1400000000, 1600000000, 1800000000, 2000000000,
  2500000000.0, 3000000000.0, 3500000000.0, 4000000000.0, 4500000000.0,
  5000000000.0, 6000000000.0, 7000000000.0, 8000000000.0, 9000000000.0,
  1e200,
};

void Histogram::Clear() {
  _min = kBucketLimit[kNumBuckets-1];
  _max = 0;
  _num = 0;
  _sum = 0;
  _sum_squares = 0;
  for (int i = 0; i < kNumBuckets; i++) {
    _buckets[i] = 0;
  }
}

void Histogram::Add(double value) {
  // Linear search is fast enough
  int b = 0;
  while (b < kNumBuckets - 1 && kBucketLimit[b] <= value) {
    b++;
  }
  _buckets[b] += 1.0;
  if (_min > value) _min = value;
  if (_max < value) _max = value;
  _num++;
  _sum += value;
  _sum_squares += (value * value);
}

void Histogram::Merge(const Histogram& other) {
  if (other._min < _min) _min = other._min;
  if (other._max > _max) _max = other._max;
  _num += other._num;
  _sum += other._sum;
  _sum_squares += other._sum_squares;
  for (int b = 0; b < kNumBuckets; b++) {
    _buckets[b] += other._buckets[b];
  }
}

double Histogram::Median() const {
  return Percentile(50.0);
}

double Histogram::Percentile(double p) const {
  double threshold = _num * (p / 100.0);
  double sum = 0;
  for (int b = 0; b < kNumBuckets; b++) {
    sum += _buckets[b];
    if (sum >= threshold) {
      // Scale linearly within this bucket
      double left_point = (b == 0) ? 0 : kBucketLimit[b-1];
      double right_point = kBucketLimit[b];
      double left_sum = sum - _buckets[b];
      double right_sum = sum;
      double pos = (threshold - left_sum) / (right_sum - left_sum);
      double r = left_point + (right_point - left_point) * pos;
      if (r < _min) r = _min;
      if (r > _max) r = _max;
      return r;
    }
  }
  return _max;
}

double Histogram::Average() const {
  if (_num == 0.0) return 0;
  return _sum / _num;
}

double Histogram::StandardDeviation() const {
  if (_num == 0.0) return 0;
  double variance = (_sum_squares * _num - _sum * _sum) / (_num * _num);
  return sqrt(variance);
}

std::string Histogram::ToString() const {
  std::string r;
  char buf[200];
  snprintf(buf, sizeof(buf),
           "Count: %.0f  Average: %.4f  StdDev: %.2f\n",
           _num, Average(), StandardDeviation());
  r.append(buf);
  snprintf(buf, sizeof(buf),
           "Min: %.4f  Median: %.4f  Max: %.4f\n",
           (_num == 0.0 ? 0.0 : _min), Median(), _max);
  r.append(buf);
  r.append("------------------------------------------------------\n");
  const double mult = 100.0 / _num;
  double sum = 0;
  for (int b = 0; b < kNumBuckets; b++) {
    if (_buckets[b] <= 0.0) continue;
    sum += _buckets[b];
    snprintf(buf, sizeof(buf),
             "[ %7.0f, %7.0f ) %7.0f %7.3f%% %7.3f%% ",
             ((b == 0) ? 0.0 : kBucketLimit[b-1]),      // left
             kBucketLimit[b],                           // right
             _buckets[b],                               // count
             mult * _buckets[b],                        // percentage
             mult * sum);                               // cumulative percentage
    r.append(buf);

    // Add hash marks based on percentage; 20 marks for 100%.
    int marks = static_cast<int>(20*(_buckets[b] / _num) + 0.5);
    r.append(marks, '#');
    r.push_back('\n');
  }
  return r;
}
}
}
