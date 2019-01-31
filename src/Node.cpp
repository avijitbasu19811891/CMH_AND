//
//  Node.cpp
//  CMH_AND
//
//  Created by Avijit Basu on 06/11/18.
//  Copyright © 2018 Juniper. All rights reserved.
//

#include "Node.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <list>
#include "GlobalConfig.hpp"
#include "InputDataSet.hpp"
#include "Topology.hpp"
#include "CMHAndAlgo.hpp"

Node::Node(int i, std::shared_ptr<SiteInfo> site, list<int> dependentOnList) {
    _nodeId = i;
    _site = site;
    _waitForList.clear();

    if (!dependentOnList.empty()) {
       int dependentNodeId = dependentOnList.front();
        dependentOnList.pop_front();
        _waitForList.push_back(dependentNodeId);
    }
}

int
Node::addDependentNodes (list<int> dependentOnList) {
    if (!dependentOnList.empty()) {
        for ( auto d : dependentOnList) {
            _waitForList.push_back(d);
        }
    }
    return 0;
}

///
/// @brief initiate probe.
///
int Node::startProbe (void) {
    int status = Error::ErrorId::_EFAIL;
    CMHAndAlgo* algo = CMHAndAlgo::instance();
    if (_waitForList.empty()) {
        return status;
    } else {
        for ( auto d : _waitForList) {
            auto probeMsg = algo->probeNext( _nodeId, d);
            Topology* topo = Topology::instance();
            if (topo) {
                status = topo->transmit(probeMsg);
            }
            if (status == Error::ErrorId::_ETEMRINATE) {
                return Error::ErrorId::_ETEMRINATE;
            }
        }
    }
    return 0;
}

///
/// @brief process a probe msg received on a give node.
///
int Node::recvProbe (ProbeMsg probe) {
    int status = Error::ErrorId::_EFAIL;
    CMHAndAlgo* algo = CMHAndAlgo::instance();
    
    if (algo) {
        algo->visit(probe);
    }
    ///
    /// Send probe to next set of dependent nodes.
    ///
    for ( auto d : _waitForList) {
        auto probeMsg = algo->probeNext( _nodeId, d);
        Topology* topo = Topology::instance();
        if (topo) {
            status = topo->transmit(probeMsg);
        }
        if (status == Error::ErrorId::_ETEMRINATE) {
            return Error::ErrorId::_ETEMRINATE;
        }
    }
    return status;
}
