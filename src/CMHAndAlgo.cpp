//
//  CMHAndAlgo.cpp
//  CMH_AND
//
//  Created by Avijit Basu on 06/11/18.
//

#include "CMHAndAlgo.hpp"
#include <stdio.h>
#include "GlobalConfig.hpp"
int CMHAndAlgo::launch (void) {
    _initiator = GlobalConfig::initiator();
    _deadlockDetected = std::false_type::value;
    _probeList.clear();
    return Error::ErrorId::_EOK;
}
