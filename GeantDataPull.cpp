
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include "TTree.h"
#include "TFile.h"
#include <string>
#include "TH1F.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TSpectrum.h"
#include "TGraph.h"
#include "TF1.h"

using namespace std;

//Begin by defining a lot of global variables


	
	


	

int main()
{
	
	TCanvas *c0=new TCanvas("c0", "BeRingParticles", 800, 800);
	TCanvas *c1=new TCanvas("c1", "BeSectorParticles", 800,800);

	c0->Divide(4,6);
	c1->Divide(2,4);


//If working with particle spectrum.



	TH1F *h0=new TH1F("EarlyParticlePeaks0", "", 24,0,24);
	




/*
	TH1F *h0=new TH1F("EarlyParticlePeaks0", "", 2000,1,2001);
	TH1F *h1=new TH1F("EarlyParticlePeaks1", "", 2000,1,2001);
	TH1F *h2=new TH1F("EarlyParticlePeaks2", "", 2000,1,2001);
	TH1F *h3=new TH1F("EarlyParticlePeaks3", "", 2000,1,2001);
	TH1F *h4=new TH1F("EarlyParticlePeaks4", "", 2000,1,2001);
	TH1F *h5=new TH1F("EarlyParticlePeaks5", "", 2000,1,2001);
	TH1F *h6=new TH1F("EarlyParticlePeaks6", "", 2000,1,2001);
	TH1F *h7=new TH1F("EarlyParticlePeaks7", "", 2000,1,2001);
	TH1F *h8=new TH1F("EarlyParticlePeaks8", "", 2000,1,2001);
	TH1F *h9=new TH1F("EarlyParticlePeaks9", "", 2000,1,2001);
	TH1F *h10=new TH1F("EarlyParticlePeaks10", "", 2000,1,2001);
	TH1F *h11=new TH1F("EarlyParticlePeaks11", "", 2000,1,2001);
	TH1F *h12=new TH1F("EarlyParticlePeaks12", "", 2000,1,2001);
	TH1F *h13=new TH1F("EarlyParticlePeaks13", "", 2000,1,2001);	
	TH1F *h14=new TH1F("EarlyParticlePeaks14", "", 2000,1,2001);
	TH1F *h15=new TH1F("EarlyParticlePeaks15", "", 2000,1,2001);
	TH1F *h16=new TH1F("EarlyParticlePeaks16", "", 2000,1,2001);
	TH1F *h17=new TH1F("EarlyParticlePeaks17", "", 2000,1,2001);
	TH1F *h18=new TH1F("EarlyParticlePeaks18", "", 2000,1,2001);
	TH1F *h19=new TH1F("EarlyParticlePeaks19", "", 2000,1,2001);
	TH1F *h20=new TH1F("EarlyParticlePeaks20", "", 2000,1,2001);
	TH1F *h21=new TH1F("EarlyParticlePeaks21", "", 2000,1,2001);
	TH1F *h22=new TH1F("EarlyParticlePeaks22", "", 2000,1,2001);
	TH1F *h23=new TH1F("EarlyParticlePeaks23", "", 2000,1,2001);
	TH1F *h24=new TH1F("EarlyParticlePeaks24", "", 2000,1,2001);
	TH1F *h25=new TH1F("EarlyParticlePeaks25", "", 2000,1,2001);
	TH1F *h26=new TH1F("EarlyParticlePeaks26", "", 2000,1,2001);
	TH1F *h27=new TH1F("EarlyParticlePeaks27", "", 2000,1,2001);
	TH1F *h28=new TH1F("EarlyParticlePeaks28", "", 2000,1,2001);
	TH1F *h29=new TH1F("EarlyParticlePeaks29", "", 2000,1,2001);
	TH1F *h30=new TH1F("EarlyParticlePeaks30", "", 2000,1,2001);
	TH1F *h31=new TH1F("EarlyParticlePeaks31", "", 2000,1,2001);	

*/

	int add=0;
	int Added=0;
	int SkipEvents=0;
	int Value=0;
	int DetectorID[3];
	double energy[24][8]={0};
	double theta[24][8]={0};
	int ring[24]={0};
	int sector[8]={0};
	double layerEne[5]={0};
	double RingEvents[24]={0};
	int FuzzyEvents=0;
	int SectorOffset=0;
	int RingOffset=0;
	int NoOffset=0;
	int GammaNoSector=0;;
	int GammaNoRing=0;
	int DopplerDet=0;
	int DopplerRing=0;
	int DopplerSector=0;
	int CorrectNum=0;
	float AngleSector=0;
	float AngleRing=0;
	float AngleDet=0;
	double PeakCorrection=1;
	double FullTime=0;
	double RingNum=0;
	int SectIncriment=0;
	int RingIncriment=0;


	int FileNum=0;
	string line;


	
	
		

	
	TFile *EarlyFile=TFile::Open("/afs/crc.nd.edu/group/nsl/activetarget/users/shender4/Codes/GeantOutput/output16mmNewUniformSmalAngledLise.root");
	TTree *t=(TTree*)EarlyFile->Get("t");
		





//Reading from calibrated files


	t->SetBranchAddress("E1_ene", energy);
	t->SetBranchAddress("E1_theta", theta);
	t->SetBranchAddress("E1_ring", ring);
	t->SetBranchAddress("E1_sector", sector);
	t->SetBranchAddress("dead_layer", layerEne);


	//Int_t NumEntries=t->GetEntries();
	//cout<< NumEntries<< endl;



//Now, we begin to set Tree branches that we read from and the ones we want to read into.

/*
	double Energy[120]={0};
	double TimeFull[120]={0};


	t->SetBranchAddress("ene", Energy);
	t->SetBranchAddress("timefull", TimeFull);
*/
	Int_t NumEntries=t->GetEntries();
	cout<<endl;
	cout<<NumEntries<< " "<<FileNum<< endl;



	add=0;
	FuzzyEvents=0;
	Added=0;
	int l=0;
	double FirstTime=0;
	double LastTime=0;

		

	
	
	
	//Now, we perform the sorting algorithm.
	for(int M=0; M<NumEntries; M++)
	{
	
	

		float Percent=M;
		//cout<<"Are we getting stuck?";

		if(M==10000*add)
		{
			add+=1;
			cout<< Percent/NumEntries*100<< " percent done \r";
			cout.flush();
			l=0;
		}


		double CalibratedDetectors[24]={0};
		double ParticleDetectors[32]={0};
		t->GetEntry(M);	


		double RingEvent=0;
		double SectEvent=0;
		int RingDeg=0;
		int SectorDeg=0;
		int SectEventNum=0;
		int RingEventNum=0;
		int SectMultiplicity=0;
		int RingMultiplicity=0;


					
		
		
		

		
		for(int RingNum=0; RingNum<24; RingNum++)
		{
	
			for(int SectNum=0; SectNum<8; SectNum++)
			{
				if(energy[RingNum][SectNum]<32000 && energy[RingNum][SectNum]>26000)
				{		
					//cout<<energy[RingNum][SectNum]<<endl;
						
					RingEvents[RingNum]++;	
					
				}
			}
		}

	}
		

	
	

		


	for(int i=0; i<24; i++)
	{
		h0->Fill(i,RingEvents[i]);
		Value=h0->Integral(i,i);
		cout<<Value<<endl;
	}
	h0->Draw();
	h0->SaveAs("PeakIntegrated16mmSmallAngledNewUniformGeantBeam.root");
	


//Check: When a gamma occured, was there also a particle somewhere?
//if yes, check for a particle event in a sector and a ring. Then, use DopplerCorrection to pull the right angle.
//Use equation E0=E(1+u*costheta)/Beta


	return 0;
}

