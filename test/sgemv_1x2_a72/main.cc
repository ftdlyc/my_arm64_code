#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <chrono>
#include <random>
using namespace std::chrono;

#define ALGIN_BITS 16

const uint64_t kInputSize = 1024 * 1024 * 16;

extern "C" void sgemv_1x2_a72(const float* biases, const float* input, const float* kernel, uint64_t size, float* output);

int main() {
  float *input, *kernel;
  float __attribute__((aligned(ALGIN_BITS))) biases[2]{1., 1.};
  float __attribute__((aligned(ALGIN_BITS))) output[2]{0., 0.};
  ::posix_memalign((void**)&input, ALGIN_BITS, kInputSize * sizeof(float));
  ::posix_memalign((void**)&kernel, ALGIN_BITS, kInputSize * 2 * sizeof(float));

  // std::default_random_engine e(system_clock::now().time_since_epoch().count());
  // std::uniform_real_distribution<float> u(-1, 1);
  // for (uint64_t i = 0; i < kInputSize; ++i) {
  //   input[i]          = u(e);
  //   kernel[2 * i]     = u(e);
  //   kernel[2 * i + 1] = u(e);
  //   if(i % (1024 * 1024) == 0) {
  //     printf("1\n");
  //   }
  // }
  // FILE* fp = fopen("data.raw", "wb");
  // fwrite(input, kInputSize * sizeof(float), 1, fp);
  // fwrite(kernel, kInputSize * 2 * sizeof(float), 1, fp);
  // fclose(fp);

  FILE* fp = fopen("data.raw", "rb");
  fread(input, kInputSize * sizeof(float), 1, fp);
  fread(kernel, kInputSize * 2 * sizeof(float), 1, fp);
  fclose(fp);

  for (int n = 0; n < 5; ++n) {
    auto t1   = system_clock::now();
    output[0] = biases[0];
    output[1] = biases[1];
    for (int i = 0; i < kInputSize; ++i) {
      output[0] += input[i] * kernel[2 * i];
      output[1] += input[i] * kernel[2 * i + 1];
    }
    auto t2 = system_clock::now();
    printf("output: [%f %f], took %.4lf us\n", output[0], output[1], duration_cast<nanoseconds>(t2 - t1).count() / 1000000.);

    auto t3 = system_clock::now();
    sgemv_1x2_a72(biases, input, kernel, kInputSize, output);
    auto t4 = system_clock::now();
    printf("output: [%f %f], took %.4lf us\n", output[0], output[1], duration_cast<nanoseconds>(t4 - t3).count() / 1000000.);
  }

  free(input);
  free(kernel);
}
