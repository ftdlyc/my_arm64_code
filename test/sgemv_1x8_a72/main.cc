#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <chrono>
#include <random>
using namespace std::chrono;

#define ALGIN_BITS 16

const uint64_t kInputSize = 1024 * 1024 * 4;

extern "C" void sgemv_1x8_a72(const float* biases, const float* input, const float* kernel, uint64_t size, float* output);

int main() {
  float *input, *kernel;
  float __attribute__((aligned(ALGIN_BITS))) biases[8]{1, 1, 1, 1, 1, 1, 1, 1};
  float __attribute__((aligned(ALGIN_BITS))) output[8]{0, 0, 0, 0, 0, 0, 0, 0};
  ::posix_memalign((void**)&input, ALGIN_BITS, kInputSize * sizeof(float));
  ::posix_memalign((void**)&kernel, ALGIN_BITS, kInputSize * 8 * sizeof(float));

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
    output[2] = biases[2];
    output[3] = biases[3];
    output[4] = biases[4];
    output[5] = biases[5];
    output[6] = biases[6];
    output[7] = biases[7];
    for (int i = 0; i < kInputSize; ++i) {
      output[0] += input[i] * kernel[8 * i];
      output[1] += input[i] * kernel[8 * i + 1];
      output[2] += input[i] * kernel[8 * i + 2];
      output[3] += input[i] * kernel[8 * i + 3];
      output[4] += input[i] * kernel[8 * i + 4];
      output[5] += input[i] * kernel[8 * i + 5];
      output[6] += input[i] * kernel[8 * i + 6];
      output[7] += input[i] * kernel[8 * i + 7];
    }
    auto t2 = system_clock::now();
    printf("output: [%f %f %f %f %f %f %f %f], took %.4lf ms\n",
           output[0], output[1], output[2], output[3], output[4], output[5], output[6], output[7],
           duration_cast<microseconds>(t2 - t1).count() / 1000.);

    auto t3 = system_clock::now();
    sgemv_1x8_a72(biases, input, kernel, kInputSize, output);
    auto t4 = system_clock::now();
    printf("output: [%f %f %f %f %f %f %f %f], took %.4lf ms\n",
           output[0], output[1], output[2], output[3], output[4], output[5], output[6], output[7],
           duration_cast<microseconds>(t4 - t3).count() / 1000.);
  }

  free(input);
  free(kernel);
}
