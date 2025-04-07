#include "toolbox.h"
#include <advanlys.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "proj.h"
//==============================================================================
// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1

//................

//==============================================================================
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
int nPeaks=0;
double *waveData = 0;
double *amplitudeEnvelope=0;
double *peaks=0;
double *peakAmplitudes=0;
double *peakSecondDerivatives=0;
static int panelHandle;
static int panelHandle2;
int bitmapID;
double start=0, end=1;
static int currentPanel;
double* autoSpectrum;
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "proj.uir", PANEL)) < 0)
		return -1;
	currentPanel=panelHandle;
	if ((panelHandle2 = LoadPanel (0, "proj.uir", PANEL_2)) < 0)
		return -1;
	DisplayPanel (currentPanel);
	RunUserInterface ();
	DiscardPanel (currentPanel);
	return 0;
}

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			;
			
	}
	return 0;
}

void countZeroCrossings(double* values, int length, int* crossings) {
    (*crossings) = 0; 
    for (int i = 1; i < length; i++) {
        if ((values[i-1] < 0 && values[i] > 0) || (values[i-1] > 0 && values[i] < 0)) {
            (*crossings)++; 
        }
    }
}

void ApplyFilter2(double *signal, double *filt, int length, double alpha) {
    filt[0] = signal[0]; 

    for (int i = 1; i < length; i++) {
        filt[i] = (1 - alpha) * filt[i - 1] + alpha * signal[i];
    }
}

void ApplyFilter1(double* signal, double* filt, int average, int num_elements) {
    int start, end;
    for (int i = 0; i < num_elements; i++) {
        start = (i - average / 2 < 0) ? 0 : i - average / 2;
        end = (i + average / 2 >= num_elements) ? num_elements : i + average / 2 + 1;
        double sum = 0.0;
        for (int j = start; j < end; j++) {
            sum += signal[j];
        }
        filt[i] = sum / (end - start);
    }
}

void DerivativeForward(double *signal, double *derivative, int npoints) {
    for (int i = 0; i < npoints - 1; i++) {
        derivative[i] = (signal[i + 1] - signal[i]); 
    }
}

int CVICALLBACK Command (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	double minVal, maxVal, mean, variance, median;
    int minIndex, maxIndex, crossings;
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			//executa script python pentru conversia unui fisierului .wav in .txt
			//LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			//Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("waveInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			double *filter1 = (double *) calloc(npoints, sizeof(double));
			double* derivative=(double *) calloc(npoints, sizeof(double));
			autoSpectrum=(double *) calloc(npoints/2, sizeof(double));
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare pe grapf
			DeleteGraphPlot( panel, PANEL_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
			PlotY(panel, PANEL_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			char fileName[256];
			
			
	
			//1
			MaxMin1D(waveData, npoints, &maxVal, &maxIndex, &minVal, &minIndex);
			SetCtrlVal(panel, PANEL_NUMERIC_1, minVal);
			SetCtrlVal(panel, PANEL_NUMERIC_2, minIndex);
			SetCtrlVal(panel, PANEL_NUMERIC_3, maxVal);
			SetCtrlVal(panel, PANEL_NUMERIC_4, maxIndex);
			Variance(waveData,npoints, &mean, &variance);
			SetCtrlVal(panel, PANEL_NUMERIC_5, variance);
			//Mean(waveData,npoints, &mean);
			Median(waveData,npoints,&median);
			SetCtrlVal(panel, PANEL_NUMERIC_6, mean);
			SetCtrlVal(panel, PANEL_NUMERIC_7, median);
			countZeroCrossings(waveData, npoints, &crossings);
			SetCtrlVal(panel, PANEL_NUMERIC_8, crossings);
			double* histogram=0;
			int nbins=30;
			histogram = (double *) calloc(nbins, sizeof(double));
			double Width = (maxVal - minVal) / nbins;
			int binIndex;
    		for (int i = 0; i < npoints; i++) 
			{    
				binIndex = (int)((waveData[i] - minVal) / Width);
       			if (binIndex >= 0 && binIndex < nbins) 
				{
        			histogram[binIndex]++;
    			} 
				else if (binIndex == nbins) 
				{
        			histogram[nbins-1]++;
    			}
    		}
			DeleteGraphPlot( panel, PANEL_HISTO, -1, VAL_IMMEDIATE_DRAW );
			PlotY(panel, PANEL_HISTO, histogram, nbins, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			sprintf(fileName, "grafic histograma ");
			char str[5]={0};
			GetSystemDate(&month, &day, &year);GetSystemTime(&hour, &minute, &second);
			sprintf(str, "%d", year);strcat(fileName, str);strcat(fileName, ".");
			sprintf(str, "%d", month);strcat(fileName, str);strcat(fileName, ".");
			sprintf(str, "%d", day);strcat(fileName, str);strcat(fileName, "_");
			sprintf(str, "%d", hour);strcat(fileName, str);strcat(fileName, "-");
			sprintf(str, "%d", minute);strcat(fileName, str);strcat(fileName, "-");
			sprintf(str, "%d", second);strcat(fileName, str);strcat(fileName, ".jpg");
			GetCtrlDisplayBitmap(panel, PANEL_HISTO, 1, &bitmapID);
			SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
			
			
			
    		//2 a
			int type;
			GetCtrlVal(panel, PANEL_RINGSLIDE, &type);
			DeleteGraphPlot( panel, PANEL_FILTER_1, -1, VAL_IMMEDIATE_DRAW );
			DerivativeForward(waveData, derivative, npoints);
			if(type==0)
			{
				int elements;
				GetCtrlVal(panel, PANEL_NUMERIC_9, &elements);
				ApplyFilter1(waveData, filter1,elements, npoints);
				GetCtrlVal(panel, PANEL_NUMERIC_13,&start);
				GetCtrlVal(panel, PANEL_NUMERIC_14,&end);
				int startIndex = (int)((start / 6) * npoints);  
				int endIndex = (int)((end / 6) * npoints);   
				int zoomPointsCount = endIndex - startIndex + 1;
				double* zoomedFilter1;
				zoomedFilter1=(double *) calloc(zoomPointsCount, sizeof(double));
				double* zoomedFilterD;
				zoomedFilterD=(double *) calloc(zoomPointsCount, sizeof(double));
				for(int i=0; i<zoomPointsCount; i++)
				{
    				if ((startIndex + i) < npoints) 
					{ 
						zoomedFilter1[i]=filter1[startIndex+i]; 
						zoomedFilterD[i]=derivative[startIndex+i];
					}
				}
				int d;
				GetCtrlVal(panel, PANEL_BINARYSWITCH, &d);
				if(d==1)
				{
					PlotY(panel, PANEL_FILTER_1, zoomedFilterD, zoomPointsCount, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
				}
				else
				{
					PlotY(panel, PANEL_FILTER_1, zoomedFilter1, zoomPointsCount, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				}
				sprintf(fileName, "grafic filtru mediere ");
				GetSystemDate(&month, &day, &year);GetSystemTime(&hour, &minute, &second);
				sprintf(str, "%d", year);strcat(fileName, str);strcat(fileName, ".");
				sprintf(str, "%d", month);strcat(fileName, str);strcat(fileName, ".");
				sprintf(str, "%d", day);strcat(fileName, str);strcat(fileName, "_");
				sprintf(str, "%d", hour);strcat(fileName, str);strcat(fileName, "-");
				sprintf(str, "%d", minute);strcat(fileName, str);strcat(fileName, "-");
				sprintf(str, "%d", second);strcat(fileName, str);strcat(fileName, ".jpg");
				GetCtrlDisplayBitmap(panel, PANEL_FILTER_1, 1, &bitmapID);
				SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
			}
			else
			{
				double alpha;
				GetCtrlVal(panel, PANEL_NUMERIC_10, &alpha);
				ApplyFilter2(waveData, filter1, npoints, alpha);
				GetCtrlVal(panel, PANEL_NUMERIC_13,&start);
				GetCtrlVal(panel, PANEL_NUMERIC_14,&end);
				int startIndex = (int)((start / 6) * npoints);  
				int endIndex = (int)((end / 6) * npoints);   
				int zoomPointsCount = endIndex - startIndex + 1;
				double* zoomedFilter1;
				zoomedFilter1=(double *) calloc(zoomPointsCount, sizeof(double));
				double* zoomedFilterD;
				zoomedFilterD=(double *) calloc(zoomPointsCount, sizeof(double));
				for(int i=0; i <zoomPointsCount; i++) {
    				if ((startIndex + i) < npoints) 
					{ 
						zoomedFilter1[i]=filter1[startIndex+i]; 
						zoomedFilterD[i]=derivative[startIndex+i];
					}
				}
				
				int d;
				GetCtrlVal(panel, PANEL_BINARYSWITCH, &d);
				if(d==1)
				{
					PlotY(panel, PANEL_FILTER_1, zoomedFilterD, zoomPointsCount, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
				}			
				else
				{
					PlotY(panel, PANEL_FILTER_1, zoomedFilter1, zoomPointsCount, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				}
				sprintf(fileName, "grafic filtru cu un element de ordin 1 ");
				GetSystemDate(&month, &day, &year);GetSystemTime(&hour, &minute, &second);
				sprintf(str, "%d", year);strcat(fileName, str);strcat(fileName, ".");
				sprintf(str, "%d", month);strcat(fileName, str);strcat(fileName, ".");
				sprintf(str, "%d", day);strcat(fileName, str);strcat(fileName, "_");
				sprintf(str, "%d", hour);strcat(fileName, str);strcat(fileName, "-");
				sprintf(str, "%d", minute);strcat(fileName, str);strcat(fileName, "-");
				sprintf(str, "%d", second);strcat(fileName, str);strcat(fileName, ".jpg");
				GetCtrlDisplayBitmap(panel, PANEL_FILTER_1, 1, &bitmapID);
				SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
			}
			
			
			
			//2 b
			double skewness, kurtosis;
			Moment(waveData, 256, 3, &skewness);
			Moment(waveData, 256, 4, &kurtosis);
			SetCtrlVal(panel, PANEL_NUMERIC_11, skewness);
			SetCtrlVal(panel, PANEL_NUMERIC_12, kurtosis);
			
			//2 c
			amplitudeEnvelope = (double *) calloc(npoints, sizeof(double));
			peaks = (double *) calloc(npoints, sizeof(double));
			peakAmplitudes=(double *) calloc(npoints, sizeof(double));
			peakSecondDerivatives=(double *) calloc(npoints, sizeof(double));
			FileToArray("amplitudeEnvelope.txt", amplitudeEnvelope, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			PlotY(panel, PANEL_GRAPH_RAW_DATA, amplitudeEnvelope, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			sprintf(fileName, "grafic "); 
			GetSystemDate(&month, &day, &year);GetSystemTime(&hour, &minute, &second);
			sprintf(str, "%d", year);strcat(fileName, str);strcat(fileName, ".");
			sprintf(str, "%d", month);strcat(fileName, str);strcat(fileName, ".");
			sprintf(str, "%d", day);strcat(fileName, str);strcat(fileName, "_");
			sprintf(str, "%d", hour);strcat(fileName, str);strcat(fileName, "-");
			sprintf(str, "%d", minute);strcat(fileName, str);strcat(fileName, "-");
			sprintf(str, "%d", second);strcat(fileName, str);strcat(fileName, ".jpg");
			GetCtrlDisplayBitmap(panel, PANEL_GRAPH_RAW_DATA, 1, &bitmapID);
			SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
			break;
	}
	return 0;
}

int CVICALLBACK Next (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, PANEL_NUMERIC_13,&start);
			if (start + 1.0 <= 5.0) 
			{  
        		SetCtrlVal(panel, PANEL_NUMERIC_13,start+1.0);
				SetCtrlVal(panel, PANEL_NUMERIC_14,start+2.0);
    		} 
			break;
	}
	return 0;
}

int CVICALLBACK Prev (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, PANEL_NUMERIC_13,&start);
			if (start - 1.0 >= 0.0) 
			{  
        		SetCtrlVal(panel, PANEL_NUMERIC_13,start-1.0);
				SetCtrlVal(panel, PANEL_NUMERIC_14,start);
    		} 
			break;
	}
	return 0;
}

int CVICALLBACK OnTimer (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	WindowConst winConst;
	double df=0.0;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			if(npoints>0)
			{
				ScaledWindowEx(waveData, npoints, RECTANGLE_, 0, &winConst);
				AutoPowerSpectrum(waveData, npoints,1.0/sampleRate, autoSpectrum, &df);
				PowerFrequencyEstimate(autoSpectrum, npoints/2, sampleRate, winConst, df, frequencyInterval, freqPeak, powerPeak));
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitch (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	int val;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, control, &val);
			if(val==0)
			{
				DisplayPanel (panelHandle);
				currentPanel=panelHandle;
				RunUserInterface ();
				DiscardPanel (panelHandle2);
			}
			else
			{
				DisplayPanel (panelHandle2);
				currentPanel=panelHandle2;
				SetCtrlVal(panelHandle2, PANEL_2_BINARYSWITCH, 0);
				RunUserInterface ();
				DiscardPanel (panelHandle);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitch_2 (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	int val;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, control, &val);
			if(val==0)
			{
				DisplayPanel (panelHandle2);
				currentPanel=panelHandle2;
				RunUserInterface ();
				DiscardPanel (panelHandle);
			}
			else
			{
				DisplayPanel (panelHandle);
				currentPanel=panelHandle;
				SetCtrlVal(panelHandle, PANEL_BINARYSWITCH_2, 0);
				RunUserInterface ();
				DiscardPanel (panelHandle2);
				SetCtrlVal(panelHandle, PANEL_2_BINARYSWITCH, &val);
			}
			break;
	}
	return 0;
}
