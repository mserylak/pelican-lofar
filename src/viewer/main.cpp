/*
 * Lofar Data Viewer
 *
 * Attaches to a data stream from the Lofar pipeline to display
 * it in real time.
 *
 * Copyright OerC 2010
 *
 */

#include <iostream>
#include <QtGui/QApplication>
#include <boost/program_options.hpp>
#include "pelican/utility/Config.h"
#include "viewer/LofarDataViewer.h"

namespace opts = boost::program_options;

pelican::Config createConfig(int argc, char** argv)
{
    // Check that argc and argv are nonzero
    if (argc == 0 || argv == NULL)
        throw QString("No command line.");

    // Declare the supported options.
    opts::options_description desc("Allowed options");
    desc.add_options()
        ("help", "Produce help message.")
        ("config,c", opts::value<std::string>(), "Set configuration file.")
    ;

    // Parse the command line arguments.
    opts::variables_map varMap;
    opts::store(opts::parse_command_line(argc, argv, desc), varMap);
    opts::notify(varMap);

    // Check for help message.
    if (varMap.count("help")) {
        std::cout << desc << "\n";
        exit;
    }

    // Get the configuration file name.
    std::string configFilename = "";
    if (varMap.count("config"))
        configFilename = varMap["config"].as<std::string>();

    pelican::Config config(QString(configFilename.c_str()));
    return config;
}


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    pelican::Config config = createConfig(argc,argv);

    pelican::lofar::LofarDataViewer ldv( config.get( pelican::Config::TreeAddress() 
                                     << pelican::Config::NodeId("dataviewer", "") 
                                   ) 
                       );
    ldv.show();
    return app.exec();
}
