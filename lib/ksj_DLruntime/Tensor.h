//
// Created by a1009mys on 25. 4. 15.
//

#ifndef TENSOR_H
#define TENSOR_H

#include <memory>
#include "defines.h"


namespace ksj {
    template< class Type >
    class Tensor {
    private:
        std::unique_ptr<Type> data;
        unsigned int dim_size;
        unsigned int dim[MAX_DIMS];
    public:
        Tensor() : data(nullptr), dim_size(0) {}
        ~Tensor() {}


    };
}





#endif //TENSOR_H
