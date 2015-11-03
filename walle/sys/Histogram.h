#ifndef WALLE_SYS_HISTOGRAM_H_
#define WALLE_SYS_HISTOGRAM_H_
#include <string>
namespace walle {
namespace util {
class Histogram {
 public:
  Histogram() { }
  ~Histogram() { }

  void Clear();
  void Add(double value);
  void Merge(const Histogram& other);

  std::string ToString() const;
  double Median() const;
  double Percentile(double p) const;
  double Average() const;
  double StandardDeviation() const;
  
 private:
  double _min;
  double _max;
  double _num;
  double _sum;
  double _sum_squares;

  enum { kNumBuckets = 154 };
  static const double kBucketLimit[kNumBuckets];
  double _buckets[kNumBuckets];


};
}
}
#endif
