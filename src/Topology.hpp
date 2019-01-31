//
//  Topology.hpp
//  CMH_AND
//
//  Created by Avijit Basu on 06/11/18.
//  Copyright © 2018 Juniper. All rights reserved.
//

#ifndef Topology_hpp
#define Topology_hpp

#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <list>
#include <queue>
#include <map>
#include "GlobalConfig.hpp"
#include "InputDataSet.hpp"
#include "Node.hpp"
#include "CMHAndAlgo.hpp"

///
/// @class represents the toplogy of nodes( nodes are part of site).
///
class Topology {
    Topology (void);
    std::map<int, std::shared_ptr<SiteInfo>> _siteList;
    std::map<int, std::shared_ptr<Node>>     _nodeList;
    std::queue <ProbeMsg> _qProbeRx;
public:
    int constructWFG (void);
    static Topology* instance (void) {
        static Topology topo;
        return &topo;
    }
    int detectDeadLock (void);
    int siteId (int pid) {
        int numSites = GlobalConfig::numSite();
        int numNodes = GlobalConfig::numNode();
        int numNodePerSite = numNodes / numSites;
        auto siteId = pid / numNodePerSite;
        return siteId;
    }
    int construstSiteList(void);
    int constructNodeList(void);
    std::shared_ptr<SiteInfo> siteInfo (int siteId) {
        auto it = _siteList.end();
        if (GlobalConfig::numSite() > siteId) {
            it = _siteList.find(siteId);
        }
        if (it != _siteList.end()) {
            return it->second;
        }
        return nullptr;
    }
    std::shared_ptr<Node> node (int nodeId) {
        auto it = _nodeList.end();
        if (GlobalConfig::numNode() > nodeId) {
            it = _nodeList.find(nodeId);
        }
        if (it != _nodeList.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    ///
    /// @brief transmist a probe to a dest in this topology.
    ///
    int transmit (ProbeMsg probe) {
        int status = Error::ErrorId::_EFAIL;
        ///
        /// transmit probe msg to a destination.
        ///  we do a short circuit to send this msg to the dest node.
        ///
        _qProbeRx.push(probe);
        return status;
    }

    ///
    /// Display routines.
    ///
    void printSiteList (ostream& os) {
        for (auto it = _siteList.begin();
             it != _siteList.end();
             it++) {
            if (it->second != nullptr) {
                it->second->display(os);
            }
        }
    }
    void printNodeList (ostream& os) {
        for (auto it = _nodeList.begin();
             it != _nodeList.end();
             it++) {
            if (it->second != nullptr) {
                it->second->display(os);
            }
        }
    }
};

#endif /* Topology_hpp */
