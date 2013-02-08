#pragma once
class ResultSet
{
public:
	ResultSet(void);
	~ResultSet(void);
	int numberOfHits;
	int numberOfRanks;
	int primaryRays;
	int generatedRays;

	double totalExecutionTime;
	double gpuTotalTime;
	double gpuTransferTime;
};


