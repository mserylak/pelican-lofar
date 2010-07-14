#ifndef ADAPTER_SUBBAND_TIME_SERIES_H
#define ADAPTER_SUBBAND_TIME_SERIES_H

#include "pelican/core/AbstractStreamAdapter.h"
#include "LofarUdpHeader.h"
#include <complex>

/**
 * @file AdapterSubbandTimeSeries.h
 */

namespace pelican {

class ConfigNode;

namespace lofar {

class SubbandTimeSeriesC32;

/**
 * @class AdapterSubbandTimeSeries
 *
 * @ingroup pelican_lofar
 *
 * @brief
 * Adapter to deserialise time stream data chunks from a lofar station.
 *
 * @details
 * Stream adapter to deserialise time stream data chunks from a lofar station.
 *
 * \section Configuration:
 *
 * Example configuration node:
 *
 * \verbatim
 *		<AdapterTimeStream name="">
 *			<fixedSizePackets value="true|false"/>
 *			<sampleSize bits=""/>
 *			<samplesPerPacket number=""/>
 *			<packetsPerChunk number=""/>
 *			<samplesPerTimeBlock number=""/>
 *			<subbands number=""/>
 *			<polarisations number=""/>
 *		<\AdapterTimeStream>
 * \verbatim
 *
 * - samplesPerPacket: Number of (time) samples per packet.
 * - fixedSizePackets: Specify if UDP packets are fixed size or not.
 * - sampleSize: Number of bits per sample. (Samples are assumed to be complex
 *               pairs of the number of bits specified).
 * - packetsPerChunk: Number of UDP packets in each input data chunk.
 * - samplesPerTimeBlock: Number of time samples to put in a block.
 * - subbands: Number of sub-bands per packet.
 * - polarisations: Number of polarisations per packet.
 */

class AdapterSubbandTimeSeries : public AbstractStreamAdapter
{
    private:
        friend class AdapterSubbandTimeSeriesTest;

    public:
        /// Constructs a new AdapterTimeStream.
        AdapterSubbandTimeSeries(const ConfigNode& config);

        /// Destroys the AdapterTimeStream.
        ~AdapterSubbandTimeSeries() {}

    protected:
        /// Method to deserialise a LOFAR time stream data.
        void deserialise(QIODevice* in);

    private:
        /// Updates and checks the size of the time stream data.
        void _checkData();

        /// Read the udp packet header from a buffer read from the IO device.
        void _readHeader(UDPPacket::Header& header, char* buffer);

        /// Reads the udp data data section into the data blob data array.
        void _readData(SubbandTimeSeriesC32* data, char* buffer,
                unsigned packetIndex);

        /// Updates dimensions of t	he time stream data being de-serialised.
        void _updateDimensions();

        /// Prints the header to standard out (for debugging).
        void _printHeader(const UDPPacket::Header& header);

    private:
        SubbandTimeSeriesC32* _timeData;
        bool _fixedPacketSize;
        unsigned _nUDPPacketsPerChunk;
        unsigned _nSamplesPerPacket;
        unsigned _nSamplesPerTimeBlock;
        unsigned _nSubbands;
        unsigned _nPolarisations;
        unsigned _sampleBits;
};


PELICAN_DECLARE_ADAPTER(AdapterSubbandTimeSeries)

} // namespace lofar
} // namespace pelican

#endif // ADAPTER_SUBBAND_TIME_STREAM_H
