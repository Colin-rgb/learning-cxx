#include "../exercise.h"
#include <cstring>

// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        for (int i = 0; i < 4; i++)
        {
            shape[i] = shape_[i];
            size *= shape[i];
        }
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    // Tensor4D &operator+=(Tensor4D const &others) {
    //     unsigned int flag_idx[4] = {0};
    //     //找出需要广播的维度
    //     for (int m = 0; m < 4; m++)
    //     {
    //         if (this->shape[m] != others.shape[m] && (others.shape[m] == 1))
    //         {
    //             flag_idx[m] = 1;
    //         }
    //     }
    //     // 确保维度数相同或者可以广播
    //     if (this->shape[0] != others.shape[0] && others.shape[0] != 1) {
    //         throw std::invalid_argument("Dimensions are not compatible for broadcasting.");
    //     }
    //     if (this->shape[1] != others.shape[1] && others.shape[1] != 1) {
    //         throw std::invalid_argument("Dimensions are not compatible for broadcasting.");
    //     }
    //     if (this->shape[2] != others.shape[2] && others.shape[2] != 1) {
    //         throw std::invalid_argument("Dimensions are not compatible for broadcasting.");
    //     }
    //     if (this->shape[3] != others.shape[3] && others.shape[3] != 1) {
    //         throw std::invalid_argument("Dimensions are not compatible for broadcasting.");
    //     }

    //     // 计算实际的大小
    //     unsigned int size = 1;
    //     for (int i = 0; i < 4; i++) {
    //         size *= this->shape[i];
    //     }

    //     // 进行广播加法操作
    //     for (unsigned int i = 0; i < size; i++) {
    //         // 在广播的维度上进行相应的索引映射
    //         unsigned int idx = i;
    //         unsigned int shape_idx[4] = {0};
            
    //         // 计算此位置在各维度的索引
    //         for (int dim = 3; dim >= 0; dim--) {
    //             shape_idx[dim] = idx % this->shape[dim];
    //             idx /= this->shape[dim];
    //         }

    //         // 在广播的维度上进行相应的索引映射
    //         for (int j = 0; j < 4; j++)
    //         {
    //             if (flag_idx[j] == 1)
    //             {
    //                 shape_idx[j] = 0;
    //             }
    //         }
    //         this->data[i] += others.data[shape_idx[0] * others.shape[1] * others.shape[2] * others.shape[3] +
    //                                     shape_idx[1] * others.shape[2] * others.shape[3] +
    //                                     shape_idx[2] * others.shape[3] +
    //                                     shape_idx[3]];

    
    //     }
    //     return *this;
    // }


    Tensor4D &operator+=(Tensor4D const &others) {
    // Validate broadcasting compatibility
        for (int m = 0; m < 4; m++) {
            if (this->shape[m] != others.shape[m] && others.shape[m] != 1) {
                throw std::invalid_argument("Dimensions are not compatible for broadcasting.");
            }
        }

        // Calculate total size
        unsigned int size = 1;
        for (int i = 0; i < 4; i++) {
            size *= this->shape[i];
        }

        // Perform broadcasting addition
        for (unsigned int i = 0; i < size; i++) {
            // Calculate indices for current tensor
            unsigned int idx = i;
            unsigned int shape_idx[4] = {0};
            
            for (int dim = 3; dim >= 0; dim--) {
                shape_idx[dim] = idx % this->shape[dim];
                idx /= this->shape[dim];
            }

            // Calculate corresponding indices for others tensor
            unsigned int others_idx[4];
            for (int dim = 0; dim < 4; dim++) {
                others_idx[dim] = (others.shape[dim] == 1) ? 0 : shape_idx[dim];
            }

            // Calculate linear index for others tensor
            unsigned int others_linear_idx = 
                others_idx[0] * others.shape[1] * others.shape[2] * others.shape[3] +
                others_idx[1] * others.shape[2] * others.shape[3] +
                others_idx[2] * others.shape[3] +
                others_idx[3];

            // Perform addition
            this->data[i] += others.data[others_linear_idx];
        }
        return *this;
    }
};


// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
