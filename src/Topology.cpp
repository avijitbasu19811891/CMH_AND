//
//  Topology.cpp
//  CMH_AND
//
//  Created by Avijit Basu on 06/11/18.
//  Copyright © 2018 Juniper. All rights reserved.
//

#include "Topology.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <list>
#include "GlobalConfig.hpp"
#include "InputDataSet.hpp"
#include "Node.hpp"

int Topology::construstSiteList (void) {
    int numSites = GlobalConfig::numSite();
    int numNodes = GlobalConfig::numNode();
    int numNodePerSite = numNodes / numSites;
    int siteRepresenationNode;
    for (auto siteId = 0; siteId < numSites; siteId++) {
        siteRepresenationNode = siteId*numNodePerSite;
        std::shared_ptr<SiteInfo> site = std::make_shared<SiteInfo> (siteId,
                                                                    siteRepresenationNode);
        if (site != nullptr) {
            _siteList.insert(
                             std::pair<int,std::shared_ptr<SiteInfo>>(siteId, site) );
        }
    }
    printSiteList(std::cout);
    return 0;
}
int Topology::constructNodeList (void) {
    int numSites = GlobalConfig::numSite();
    int numNodes = GlobalConfig::numNode();
    int numNodePerSite = numNodes / numSites;
    std::list<int> emptyList = {};
    for (auto nodeId = 0; nodeId < numNodes; nodeId++) {
        
        std::shared_ptr<Node> node = std::make_shared<Node> (nodeId,
                                                             siteInfo(siteId(nodeId)),
                                                              emptyList);
        if (node != nullptr) {
            _nodeList.insert(
                             std::pair<int,std::shared_ptr<Node>>(nodeId, node) );
        }
    }
    printNodeList(std::cout);
    return 0;
}

Topology::Topology (void) {
    construstSiteList();
    constructNodeList();
}
int
Topology::constructWFG (void) {
    DataSet* dataSet = DataSet::instance();
    std::cout << "//////// Constructing WFG ////////" << std::endl;
    if (dataSet != nullptr) {
        std::shared_ptr<WaitForList> waitForList;
        while(waitForList = dataSet->getNextEvent()) {
            if (waitForList->isWaitForGraph()) {
                waitForList->display(std::cout);
                std::cout << "Updating wait for list" << std::endl;
                auto nodeId = waitForList->siteId();
                
                auto nodePtr = node(nodeId);
                if (nodePtr != nullptr) {
                    nodePtr->addDependentNodes(waitForList->_waitForList);
                }
            }
        }
    }
    std::cout << "////////                 ////////" << std::endl;
    printNodeList(std::cout);
    return 0;
}

///
/// @brief detect deadlock.
///
/// launch probe from the initiator node.
/// Check if any node received a probe msg.
/// Terminate if any of following
///  1. Algo detected deadlock.
///  2. There is no pending probe in Rx Queue.
///
int Topology::detectDeadLock (void) {
    int numIteration = 0;
    CMHAndAlgo*algo = CMHAndAlgo::instance();
    
    if (algo == nullptr) {
        return Error::ErrorId::_EFAIL;
    }
    
    std::cout << "////////// Starting deadlock detection ///////////" << std::endl
    << std::endl;
    
    auto initiatorId = GlobalConfig::initiator();

    std::cout << "Initiator node : " << initiatorId << std::endl;
    auto initiatorNode = node(initiatorId);
    
    if (initiatorNode) {
        initiatorNode->startProbe();
    }
    
    ///
    /// While there is any Probe in Recv Q.
    ///
    while(!_qProbeRx.empty()) {
        std::cout << "Processing receive Probe" << std::endl;
        ProbeMsg rxProbe = _qProbeRx.front();
        _qProbeRx.pop();
        auto destId = rxProbe._destNode;
        ///
        /// Trigger a rx event on a node
        ///
        auto destNode = node(destId);
        
        if (destNode != nullptr) {
            destNode->recvProbe(rxProbe);
        }
        if (algo->detectTermiantion()) {
            break;
        }
    }
    std::cout << "////////// End of deadlock detection ///////////" << std::endl
    << std::endl;

    ///
    /// Check if deadlock detected.
    ///
    if (algo->deadlock()) {
        std::cout << "Deadlock detected" << std::endl;
    } else {
        std::cout << "Deadlock not detected" << std::endl;
    }
    return 0;
}
