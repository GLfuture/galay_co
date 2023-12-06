#ifndef __GALAY_CO_RESULT_H__
#define __GALAY_CO_RESULT_H__

#include <iostream>
#include <memory>
class Result
{
public:
    using Ptr = std::shared_ptr<Result>;
    virtual ~Result(){ }
};

class Net_Result: public Result
{
public:
    Net_Result(ssize_t ret)
    {
        this->ret = ret;
    }
    using Ptr = std::shared_ptr<Net_Result>;
    ssize_t ret = 0;
};

#endif