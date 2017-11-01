
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


using namespace std;

//Begin by defining a lot of global variables


int main(){
string line;
ifstream fInNew("DetectorMap.dat");
getline(fInNew,line);

ifstream fInput("RunFileNames.dat");
char Map1[60];
char Map2[60];
char Map3[60];


int CloverMap[6][4]={0};
int RingMap[24]={0};
int SectorMap[8]={0};
int CloverNum=0;
int LeafNum=0;
int RingNum=0;
int SectorNum=0;
int i=0;
int j=0;
int p=0;
int FileNum=0;
//Cross talk is 6 and 7 for Be, 2 and 3 for Li.
int CrossTalkOne=6;
int CrossTalkTwo=7;

	//Use this to automate the sorting process.
while(FileNum<129){
	char FileName[200];
		///Now, we also want to loop over all the files and add them.
	
	fInput>>FileNum;
	getline(fInput,line);

	



	char NewFileName[100];


	if(FileNum<100)
		{
		sprintf( FileName , "/afs/crc.nd.edu/group/nsl/activetarget/data/7Be_Coulex_March2016/rootfiles/run00%d-00.root", FileNum);
		sprintf(NewFileName, "/afs/crc.nd.edu/group/nsl/activetarget/users/shender4/7BeData/CaliRun00%d-00.root", FileNum);
		}
		else
		{	
		sprintf( FileName , "/afs/crc.nd.edu/group/nsl/activetarget//data/7Be_Coulex_March2016/rootfiles/run0%d-00.root", FileNum);
		sprintf(NewFileName, "/afs/crc.nd.edu/group/nsl/activetarget/users/shender4/7BeData/CaliRun0%d-00.root", FileNum);
		}
	



//Mapping Algorithm. Probably should be a function at this point.

while(i<24)
{
	fInNew>>Map1;
	if(strncmp(Map1, "Clover", 6)==0)
	{
		fInNew>>CloverNum;
		fInNew>>LeafNum;
		fInNew>>CloverMap[CloverNum][LeafNum];
		i++;
	}
	getline(fInNew,line);



}

fInNew.seekg(1, ios::beg); //Sends us back to the beginning of the file.
getline(fInNew,line);



while(j<8)
{

	fInNew>>Map2;

	if(strncmp(Map2, "Sector", 6)==0)
	{
		fInNew>>SectorNum;
		fInNew>>SectorMap[SectorNum];
		j++;

	}
	getline(fInNew,line);




}


fInNew.seekg(1, ios::beg); //Sends us back to the beginning of the file.
getline(fInNew,line);


while(p<24)
{

	fInNew>>Map3;

	if(strncmp(Map3, "Rings", 5)==0)
	{
		fInNew>>RingNum;
		//cout<<RingNum<<endl;
		fInNew>>RingMap[RingNum];
		p++;

	}
	getline(fInNew,line);



}



//Now, we begin the part of the experiment for reading in data fits.

	int add=0;



	TFile *DataFile=TFile::Open(FileName);


	//TFile *DataFile=TFile::Open("/afs/crc.nd.edu/group/nsl/activetarget/data/7Be_Coulex_March2016/rootfiles/run0090-00.root");

	TTree *t=(TTree*)DataFile->Get("t2"); 

	TH1F *h1=new TH1F("EuropiumPeaks", "", 32768,0,32768);
	TH1F *h2=new TH1F("Detector5", "", 32768,0,32768);

	char drawName[50];
	char Hist[10];


//Detector4 and Detector5 have problems. 4 due to cross-talk, 5 since one leaf is missing.

	ifstream fIn("7BeFits.dat");
	int DetectorStart=0;
	for(int Z=0; Z<1+DetectorStart; Z++)
	{
	getline(fIn,line);
	}

	float Fits[24][2]={0};
	float Error[24][2]={0};

	for(int A=0; A<24; A++){
		if(A<10){
			fIn.seekg(1, ios::cur);
		}
		else{
			fIn.seekg(2, ios::cur);
		}
		fIn>>Fits[A][0];
		fIn>>Error[A][0];
		fIn>>Fits[A][1];
		fIn>>Error[A][1];	
		//cout<<Fits[A][0]<< " "<<Fits[A][1]<<endl;

		getline(fIn,line);

	}





//Now, we begin to set Tree branches that we read from and the ones we want to read into.

	double Energy[120]={0};
	double TimeFull[120]={0};


	t->SetBranchAddress("ene", Energy);
	t->SetBranchAddress("timefull", TimeFull);

	Int_t NumEntries=t->GetEntries();
	cout<<endl;
	cout<<NumEntries<< " "<<FileNum<< endl;


	TFile *CaliDetectors=new TFile(NewFileName,"RECREATE");

	TTree *Detectors = new TTree("Detectors", "Holds the 4 leaves and the summed clovers for the 6 Detectors");

	//Maybe make two trees, one with 4 leaves and clover ene, and one with 4 leaves and clover time?

	double Detector0[9]={0};
	double Detector1[9]={0}; 
	double Detector2[9]={0}; 
	double Detector3[9]={0}; 
	double Detector4[9]={0}; 
	double Detector5[9]={0};
	double ParticleRings[48]={0};
	double ParticleSectors[16]={0};



	//Each branch should contain the four calibrated leaves and the fifth is the summed detector. The sixth is the summed timefull.
	Detectors->Branch("Detector0", Detector0, "Detector0[9]/D");
	Detectors->Branch("Detector1", Detector1, "Detector1[9]/D");
	Detectors->Branch("Detector2", Detector2, "Detector2[9]/D");
	Detectors->Branch("Detector3", Detector3, "Detector3[9]/D");
	Detectors->Branch("Detector4", Detector4, "Detector4[9]/D");
	Detectors->Branch("Detector5", Detector5, "Detector5[9]/D");
	Detectors->Branch("ParticleRings", ParticleRings, "ParticleRings[48]/D");
	Detectors->Branch("ParticleSectors", ParticleSectors, "ParticleSectors[16]/D");



	// THIS IS FOR PLACING THE VARIOUS LEAVES INTO ONE CLOVER, THEN MAKING A HISTOGRAM FROM THAT.
	cout<< "Are you okay up to here?"<<endl;



//Now, we perform the sorting algorithm.

	for(int M=0; M<NumEntries; M++)
	{

		float Percent=M;

		if(M==10000*add)
		{
			add+=1;
			cout<< Percent/NumEntries*100<< " percent done \r";
			cout.flush();
			

		}



		DataFile->cd();
		double CalibratedDetectors[24]={0};
		double ParticleDetectors[32]={0};
		double ParticleTimes[32]={0};
		int Count=0;
		t->GetEntry(M);	


		for(int jj=0; jj<24; jj++){
			Count+=Energy[jj];
		}
		for(int k=0; k<6; k++)
		{
			//PLOT SIGMA VS. ENERGY
			CaliDetectors->cd();

			for(int iii=0; iii<24; iii++){
				ParticleRings[iii]=0.0;
				if(iii<16){
					ParticleSectors[iii]=0.0;
				}
				if(iii<9){
					Detector0[iii]=0.0;
					Detector1[iii]=0.0;
					Detector2[iii]=0.0;
					Detector3[iii]=0.0;
					Detector4[iii]=0.0;
					Detector5[iii]=0.0;
				}
			}
			

			double CloverEnergy=0;
			double CloverTime=0;
			//sprintf(Hist, "h%d", k);
			DataFile->cd();
			for(int l=0; l<4; l++)
			{
				
				//This part for copying over particle info. Do we only copy particle info if there's a gamma ray coincidence? No, that comes later





			if(k<2)
			{
				//if(Energy[SectorMap[4*k+l]]<10){


				//	ParticleDetectors[24+4*k+l]=0; //Fill an array instead.
				//}

				if(Energy[SectorMap[4*k+l]]>10)
				{
					
					ParticleDetectors[24+4*k+l]=Energy[SectorMap[4*k+l]];
					ParticleTimes[24+4*k+l]=TimeFull[SectorMap[4*k+l]];
			
				}
			}

				//if(Energy[RingMap[4*k+l]]<10){

				//	ParticleDetectors[4*k+l]=0;
				//}

				if(Energy[RingMap[4*k+l]]>10){

					ParticleDetectors[4*k+l]=Energy[RingMap[4*k+l]];
					ParticleTimes[4*k+l]=TimeFull[RingMap[4*k+l]];
				}
				

				//Now, the actual Clover stuff.

				//if(Energy[CloverMap[k][l]]<10){

				//	CalibratedDetectors[CloverMap[k][l]]=0;

				//}
				
				//This kills the cross-talk detectors in the experiment. 
				if(CloverMap[k][l]==CrossTalkOne || CloverMap[k][l]==CrossTalkTwo){

					CalibratedDetectors[CloverMap[k][l]]=0;

				}
				else if(Energy[CloverMap[k][l]]>10)
				{

				CalibratedDetectors[4*k+l]=Energy[CloverMap[k][l]]*Fits[4*k+l][1]+Fits[4*k+l][0];
				

				CloverEnergy +=CalibratedDetectors[4*k+l];
				}


			}


			CaliDetectors->cd();

			if(k==0)
			{
				
				Detector0[0]=CalibratedDetectors[0+4*k]; 
				Detector0[1]=CalibratedDetectors[1+4*k];
				Detector0[2]=CalibratedDetectors[2+4*k];
				Detector0[3]=CalibratedDetectors[3+4*k];
				Detector0[4]=CloverEnergy;
				Detector0[5]= TimeFull[CloverMap[k][0]]; //Time for first leaf
				Detector0[6]= TimeFull[CloverMap[k][1]];
				Detector0[7]= TimeFull[CloverMap[k][2]];
				Detector0[8]= TimeFull[CloverMap[k][3]]; 

				for(int a=0; a<24; a++)
				{

					ParticleRings[a]=ParticleDetectors[a];
					ParticleRings[24+a]=ParticleTimes[a];
					if(a<8)
					{
					ParticleSectors[a]=ParticleDetectors[24+a];
					ParticleSectors[8+a]=ParticleTimes[24+a];
					}
				}
				
				Detectors->Fill();
				h1->Fill(CloverEnergy);
				
				
				
			}

			
			if(k==1)
			{
				//h2->Fill(CloverEnergy);
				Detector1[4]=CloverEnergy;
				Detector1[0]=CalibratedDetectors[0+4*k];
				Detector1[1]=CalibratedDetectors[1+4*k];
				Detector1[2]=CalibratedDetectors[2+4*k];
				Detector1[3]=CalibratedDetectors[3+4*k];
				Detector1[5]= TimeFull[CloverMap[k][0]]; //Time for first leaf
				Detector1[6]= TimeFull[CloverMap[k][1]];
				Detector1[7]= TimeFull[CloverMap[k][2]];
				Detector1[8]= TimeFull[CloverMap[k][3]]; 

				for(int a=0; a<24; a++)
				{

					ParticleRings[a]=ParticleDetectors[a];
					ParticleRings[24+a]=ParticleTimes[a];
					if(a<8)
					{
					ParticleSectors[a]=ParticleDetectors[24+a];
					ParticleSectors[8+a]=ParticleTimes[24+a];
					}
				}
				Detectors->Fill();



			}

			if(k==2)
			{
				Detector2[4]=CloverEnergy;
				Detector2[0]=CalibratedDetectors[0+4*k];
				Detector2[1]=CalibratedDetectors[1+4*k];
				Detector2[2]=CalibratedDetectors[2+4*k];
				Detector2[3]=CalibratedDetectors[3+4*k];
				Detector2[5]= TimeFull[CloverMap[k][0]]; //Time for first leaf
				Detector2[6]= TimeFull[CloverMap[k][1]];
				Detector2[7]= TimeFull[CloverMap[k][2]];
				Detector2[8]= TimeFull[CloverMap[k][3]]; 

				for(int a=0; a<24; a++)
				{

					ParticleRings[a]=ParticleDetectors[a];
					ParticleRings[24+a]=ParticleTimes[a];
					if(a<8)
					{
					ParticleSectors[a]=ParticleDetectors[24+a];
					ParticleSectors[8+a]=ParticleTimes[24+a];
					}
				}
				Detectors->Fill();
			}

			if(k==3)
			{
				Detector3[4]=CloverEnergy;
				Detector3[0]=CalibratedDetectors[0+4*k];
				Detector3[1]=CalibratedDetectors[1+4*k];
				Detector3[2]=CalibratedDetectors[2+4*k];
				Detector3[3]=CalibratedDetectors[3+4*k];
				Detector3[5]= TimeFull[CloverMap[k][0]]; //Time for first leaf
				Detector3[6]= TimeFull[CloverMap[k][1]];
				Detector3[7]= TimeFull[CloverMap[k][2]];
				Detector3[8]= TimeFull[CloverMap[k][3]]; 

				for(int a=0; a<24; a++)
				{

					ParticleRings[a]=ParticleDetectors[a];
					ParticleRings[24+a]=ParticleTimes[a];
					if(a<8)
					{
					ParticleSectors[a]=ParticleDetectors[24+a];
					ParticleSectors[8+a]=ParticleTimes[24+a];
					}
				}
				Detectors->Fill();
				
			}

			if(k==4)
			{
				Detector4[4]=CloverEnergy;
				Detector4[0]=CalibratedDetectors[0+4*k];
				Detector4[1]=CalibratedDetectors[1+4*k];
				Detector4[2]=CalibratedDetectors[2+4*k];
				Detector4[3]=CalibratedDetectors[3+4*k];
				Detector4[5]= TimeFull[CloverMap[k][0]]; //Time for first leaf
				Detector4[6]= TimeFull[CloverMap[k][1]];
				Detector4[7]= TimeFull[CloverMap[k][2]];
				Detector4[8]= TimeFull[CloverMap[k][3]]; 

				for(int a=0; a<24; a++)
				{

					ParticleRings[a]=ParticleDetectors[a];
					ParticleRings[24+a]=ParticleTimes[a];
					if(a<8)
					{
					ParticleSectors[a]=ParticleDetectors[24+a];
					ParticleSectors[8+a]=ParticleTimes[24+a];
					}
				}
				Detectors->Fill();
				
			}

			if(k==5)
			{
				Detector5[4]=CloverEnergy;
				Detector5[0]=CalibratedDetectors[0+4*k];
				Detector5[1]=CalibratedDetectors[1+4*k];
				Detector5[2]=CalibratedDetectors[2+4*k];
				Detector5[3]=CalibratedDetectors[3+4*k];
				Detector5[5]= TimeFull[CloverMap[k][0]]; //Time for first leaf
				Detector5[6]= TimeFull[CloverMap[k][1]];
				Detector5[7]= TimeFull[CloverMap[k][2]];
				Detector5[8]= TimeFull[CloverMap[k][3]]; 
				//cout<< Detector5[0] << " "<< Detector5[1]<< " "<< Detector5[2]<< " "<< Detector5[3]<< endl;
				
				for(int a=0; a<24; a++)
				{

					ParticleRings[a]=ParticleDetectors[a];
					ParticleRings[24+a]=ParticleTimes[a];
					if(a<8)
					{
					ParticleSectors[a]=ParticleDetectors[24+a];
					ParticleSectors[8+a]=ParticleTimes[24+a];
					}
				}
				Detectors->Fill();
				
			}

		 }



	}
	Detectors->Write();
	CaliDetectors->Close();








	//DRAW 4 leaves DRAWN OVER EACH OTHER.

	// for(int M=DetectorStart; M<DetectorStart+4;M++)
	// {
	// 	//THIS IS FOR CALIBRATING AND ADDING UP HISTOGRAMS INSTEAD OF FILES



	// 	sprintf(drawName , "ene[%d]>>EuropiumPeaks" , M);

	// 	t->Draw(drawName);

	// 	cout<<Fits[M][0]<< " "<< Fits[M][1]<< " "<< endl;
	// 	//Calibrate h1 into h2.

	// 	for(int j=0; j<5000; j++)
	// 	{
	// 		//R1 = (float)rand()/(float)(RAND_MAX);
	// 		if(floor(j*Fits[M][1]+Fits[M][1])>floor(j*Fits[M][1])){
	// 			float Value=h1->Integral(j,j);
	// 			float Weight=(ceil(j*Fits[M][1])-j*Fits[M][1])/(Fits[M][1]);
	// 			h2->Fill(j*Fits[M][1]+Fits[M][0], Value*Weight);
	// 			h2->Fill(j*Fits[M][1]+Fits[M][1]+Fits[M][0], Value*(1-Weight));
	// 		}
	// 		else{
	// 		float Value=h1->Integral(j,j);
	// 		h2->Fill(j*Fits[M][1]+Fits[M][0], Value);
	// 			}
	// 		if(M==DetectorStart){
	// 			float Value=h1->Integral(j,j);
	// 			h3->Fill(j*Fits[M][1]+Fits[M][0], Value);
	// 		}
	// 		if(M==DetectorStart+1){
	// 			float Value=h1->Integral(j,j);
	// 			h4->Fill(j*Fits[M][1]+Fits[M][0], Value);
	// 		}
	// 		if(M==DetectorStart+2){
	// 			float Value=h1->Integral(j,j);
	// 			h5->Fill(j*Fits[M][1]+Fits[M][0], Value);
	// 		}
	// 		if(M==DetectorStart+3){
	// 			float Value=h1->Integral(j,j);
	// 			h6->Fill(j*Fits[M][1]+Fits[M][0], Value);
	// 		}


	// 	}

	// }


	// //h2->Draw("");
	// //h2->SetLineColor(3);
	// h2->GetXaxis()->SetTitle("Energy");
	// h2->GetXaxis()->SetNdivisions(520);
	// h2->GetXaxis()->SetTitleSize(0.05);
	// h2->GetXaxis()->CenterTitle();
	// h2->GetYaxis()->SetTitleSize(0.05);
	// h2->GetYaxis()->SetTitle("Counts");
	// h2->GetYaxis()->SetNdivisions(510);
	// h2->GetYaxis()->CenterTitle();
	// h2->SetTitle("Europium152 Spectrum");
	// h3->Draw();
	// h3->SetLineColor(2);
	// h4->Draw("same");
	// h4->SetLineColor(1);
	// h5->Draw("same");
	// h5->SetLineColor(4);
	// h6->Draw("same");
	// h6->SetLineColor(3);

	








	///THIS IS FOR FINDING PEAKS IF NECESSARY
	// h2->GetXaxis()->SetRangeUser(10,5000);

	// //create an object TSpectrum, specify the max number of peaks to find (here: 30)
	// TSpectrum *s=new TSpectrum(30);

	// Int_t nfound = s->Search(h2,2,"",0.02);

	// Float_t *xpeaks = s->GetPositionX();


	// //puts all of the channels to large numbers, so we can sort.
	// for(int i=0; i<11; i++)
	// {
	// 	cout<<xpeaks[i] <<endl;
	// }
	}
return 0;
}



