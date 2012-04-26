#include <QtCore/QVector>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <iostream>
#include "BandPassOutput.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/data/DataBlob.h"
#include "BandPass.h"
#include "BinMap.h"

namespace pelican {
namespace lofar {


/**
 *@details BandPassOutput 
 */
BandPassOutput::BandPassOutput(const ConfigNode& configNode )
    : AbstractOutputStream( configNode )
{
    QString filename = configNode.getOption("file", "name");
    // initialise file connections
    if( filename != "" )
    {
        addFile( filename );
    }

}

void BandPassOutput::addFile(const QString& filename)
{
    QFile* file = new QFile(filename);
    if( file->open( QIODevice::WriteOnly ) )
    {
        _devices.append( file );
    }
    else {
        std::cerr << "BandPassOutput: unable to open file for writing: " << filename.toStdString() << std::endl;
        delete file;
    }
}

void BandPassOutput::sendStream(const QString& /*streamName*/, const DataBlob* dataBlob)
{
    if( dataBlob->type() == "BandPass" )
    {
        const BandPass* bp = static_cast<const BandPass*>(dataBlob);
        foreach( QIODevice* device, _devices ) {
           float start = bp->primaryMap().startValue();
           float end= bp->primaryMap().endValue();
           int nBins = bp->primaryMap().numberBins();
            
           QTextStream out(device);
           out << "# Generated by BandPassOutput\n"
               << nBins << "\n"
               << start << "\n"
               << end << "\n"
               << bp->primaryMap().width() << "\n"
               << bp->primaryRms() << "\n"
               << bp->primaryMedian() << "\n";
           foreach(const float param, bp->params()) {
               out << param << "\n";
           }
        }
    }
}

/**
 *@details
 */
BandPassOutput::~BandPassOutput()
{
}

} // namespace lofar
} // namespace pelican
