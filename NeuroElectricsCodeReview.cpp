#include <iostream>
#include <vector>
#include <numeric>
class SensorDataProcessor
{
public:
    // Constructor
    SensorDataProcessor() : rawDataBuffer(nullptr), processedData(nullptr),
                            maxBufferSize(1000), filterCoefficient(0.5)
    {
        // Initial allocation (fixed size)
        rawDataBuffer = new double[maxBufferSize];
        processedData = new double[maxBufferSize];
        currentRawSize = 0;       // Tracks actual elements in rawDataBuffer
        currentProcessedSize = 0; // Tracks actual elements in processedData
    }
    // Destructor
    ~SensorDataProcessor()
    {
        delete[] rawDataBuffer;
        delete[] processedData;
    }
    // Adds a new data point to the buffer
    void addDataPoint(double dataPoint)
    {
        if (currentRawSize < maxBufferSize)
        {
            rawDataBuffer[currentRawSize++] = dataPoint;
        }
        else
        {
            // Shift the circular buffer
            for (int i = 0; i < maxBufferSize - 1; ++i)
            {
                rawDataBuffer[i] = rawDataBuffer[i + 1];
            }
            rawDataBuffer[maxBufferSize - 1] = dataPoint;
        }
    }
    // Processes all data in the rawDataBuffer and stores it in processedData
    // This function is called frequently, potentially on every new data point
    double *processAllData()
    {
        currentProcessedSize = 0; // Reset processed data size
        if (currentRawSize > 0)
        {
            processedData[currentProcessedSize++] = rawDataBuffer[0]; // First element
        }
        for (int i = 1; i < currentRawSize; ++i)
        {
            // Simple moving average
            double filteredValue = (rawDataBuffer[i] * filterCoefficient) +
                                   (processedData[i - 1] * (1.0 - filterCoefficient));
            if (currentProcessedSize < maxBufferSize)
            {
                processedData[currentProcessedSize++] = filteredValue;
            }
            else
            {
                std::cerr << "Warning: Processed data buffer full!" << std::endl;
            }
        }
        return processedData;
    }
    // Returns the latest processed data
    double *getLatestProcessedData()
    {
        return processedData;
    }
    // Returns the current size of the processed data
    int getProcessedDataSize() const
    {
        return currentProcessedSize;
    }
    // Sets new configuration parameters
    void setConfig(int newSize, double newCoeff)
    {
        maxBufferSize = newSize;
        filterCoefficient = newCoeff;
    }

private:
    double *rawDataBuffer;
    double *processedData;
    int maxBufferSize;
    double filterCoefficient;
    int currentRawSize;
    int currentProcessedSize;
};
// Example Usage (imagine this is in another part of the system)
int main()
{
    SensorDataProcessor processor;
    for (int i = 0; i < 2000; ++i)
    {
        processor.addDataPoint(i * 0.1 + (i % 10) * 0.5); // Simulate some data
        if (i % 10 == 0)
        { // Process every 10 data points
            double *currentData = processor.processAllData();
            // Imagine this data is then sent to a visualization module
            // For demonstration, print first few elements
            if (processor.getProcessedDataSize() > 0)
            {
                std::cout << "Processed data length: " << processor.getProcessedDataSize() << std::endl;
            }
        }
    }
    // Later, someone tries to change config
    processor.setConfig(500, 1.2);
    return 0;
}
