//#ifdef RUNNING_ON_NOVA
//REGISTER_ALGORITHM (_206039984_a)
//#endif
// Created by Oz Zafar on 17/05/2020.
//

#include "_206039984_c.h"
#include "../interfaces/AlgorithmRegistration.h"
#include "../common/utils/Errors.h"

#ifdef RUNNING_ON_NOVA
REGISTER_ALGORITHM (_206039984_c)
#endif

int _206039984_c::getLoadInstructions(const string &input_path, const string &output_path) {
    (void)input_path;
    (void)output_path;
    return (int)Error::SUCCESS;
}
