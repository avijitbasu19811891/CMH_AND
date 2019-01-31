//
//  CMHAndAlgo.hpp
//  CMH_AND
//
//  Created by Avijit Basu on 06/11/18.
//  Copyright © 2018 Juniper. All rights reserved.
//

#ifndef CMHAndAlgo_hpp
#define CMHAndAlgo_hpp

#include <stdio.h>

#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <list>
#include <map>
#include "GlobalConfig.hpp"

class ProbeMsg {
public:
    int _initiator;
    int _srcNode;
    int _destNode;
    ///
    /// @brief constructor
    ///
    ProbeMsg (int initiator, int srcId, int destId) :
    _initiator{initiator}, _srcNode{srcId}, _destNode{destId} {}
};
class CMHAndAlgo {
    std::list<ProbeMsg> _probeList;
    int _initiator;
    bool _deadlockDetected;
    ///
    /// @brief check if a node is an initiator.
    ///
    bool isInitiator (int nodeId) {
        return (_initiator == nodeId);
    }
    bool detectCyclicProbe (ProbeMsg probe) {
        for (auto oldProbe : _probeList) {
            if ((oldProbe._srcNode == probe._destNode) &&
                (oldProbe._destNode == probe._srcNode) ){
                    return std::true_type::value;
                }
        }
        return std::false_type::value;
    }
public:
    int launch (void);
    ///
    /// @brief Node will visit the algorithm for a received probe.
    ///
    int visit (ProbeMsg probe) {
        _probeList.push_back(probe);
        if (isInitiator(probe._destNode)) {
            _deadlockDetected = std::true_type::value;
            return  Error::ErrorId::_ETEMRINATE;
        }
        ///
        /// Detect if this probe is a cycle.
        ///
        auto cycleDetected = detectCyclicProbe (probe);
        if (cycleDetected) {
            _deadlockDetected = std::true_type::value;
            return  Error::ErrorId::_ETEMRINATE;
        }
        return 0;
    }
private:
    ///
    /// @brief Constructor
    ///
    CMHAndAlgo (void) {
        _initiator = GlobalConfig::initiator();
        _deadlockDetected = std::false_type::value;
        _probeList.clear();
    }
public:
    ///
    /// @brief retrieve singleton instance of the algo
    ///
    static CMHAndAlgo* instance (void) {
        static CMHAndAlgo algo;
        return &algo;
    }
    ///
    /// @brief prepare a probe msg for a give node.
    ///
    ProbeMsg probeNext(int nodeId, int destId) {
        std::cout << "Preparing to probe : " << destId << " srdId : " << nodeId
        << std::endl;
        ProbeMsg probe(_initiator, nodeId, destId);
        return probe;
    }
    
    ///
    /// @brief check if termination detected.
    ///
    bool detectTermiantion (void) {
        return (_deadlockDetected);
    }
    ///
    /// @brief check if termination detected.
    ///
    bool deadlock (void) {
        return (_deadlockDetected);
    }
};
#endif /* CMHAndAlgo_hpp */
