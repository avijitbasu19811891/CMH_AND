//
//  main.cpp
//  CMH_AND
//
//  Created by Avijit Basu on 05/11/18.
//  Copyright © 2018 Juniper. All rights reserved.
//

#include <iostream>
#include "GlobalConfig.hpp"
#include "InputDataSet.hpp"
#include "Topology.hpp"

///
/// @brief main routine
///
///    Usage: progm_name <numProcess> <numSites> <initiator>
///
/// This read the config file WFG.txt and then creates the WFG.
///
/// Launch the algorithm
///
int main(int argc, const char * argv[]) {

    if (argc < 4) {
        std::cout << "Imporper argument" << std::endl;
        std::cout << "Usage: progm_name <numProcess> <numSites> <initiator>" << std::endl;
        return 0;
    }

    GlobalConfig* global = GlobalConfig::instance();
    if (global == nullptr) {
        std::cout << "Failed to create gloabl state" << std::endl;
    }

    global->config(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));

    ///
    /// Initial num of sites and num of process listed.
    ///
    std::cout << "Num of site : " << atoi(argv[2])
    << " Num of nodes : " << atoi(argv[1])
    << std::endl;
    ///
    /// Read config and form the toplogy.
    ///
    std::cout << "Constructing the topology" << std::endl;
    DataSet* dataSet = DataSet::instance();
    if (dataSet != nullptr) {
        dataSet->description(std::cout);
    }
    Topology *topo = Topology::instance();
    topo->constructWFG();
    std::cout << std::endl << std::endl;
    std::cout << "/////////  Display of constructed topology /////////" << std::endl;
    topo->printNodeList(std::cout);
    std::cout << "/////////                                   /////////" << std::endl;
    std::cout << std::endl << std::endl;
    ///
    /// Run the algorithm
    ///
    
    topo->detectDeadLock();
    return 0;
}
