#include <iostream>
#include <algorithm>
#include "OldFileTypes.h"
#include <TProfile.h>
//#include "DescribeData.C" // provides data namespace

using std::cout;
using std::endl;

int BaseFile::OpenFile(const TString& name){
   if(GetFile() && IsOk()) {
       if(name == GetFileName() ){
	   std::cout<<"Requested file, '"<<name<<"' is already open"<<std::endl;
	   return 0;
       }
       //std::cout<<"Closing file"<<std::endl;
	for(DataList_t::iterator it=fData.begin();it!=fData.end();it++){
	    InitData(*it);
	}
       CloseFile();
   }
   // Actually open the file
   FlagBad();
   if(name!="") SetFileName(name);
   if(GetFileName()=="") {
       cout<<"No filename given"<<endl;
       return 2;
   }
   SetFile(TFile::Open(GetFileName().Data(),GetOpenMode()));
   if (!GetFile()) {
       std::cout<<"Problem opening file '"<<GetFileName()<<"'"<<std::endl;
       return 1;
   }
   FlagGood();
   return 0;
}

BaseFile::~BaseFile(){
    for(DataList_t::iterator it=fData.begin();
	it!=fData.end();it++){
	if(*it) FinishData(*it);
    }
    if(fFile) fFile->Close();
    FlagBad();
}

void BaseFile::AddData(BaseData* data){ 
    if(IsOk()){
	InitData(data);
    }
    //cout<<GetFileName()<<'\t'<<fIsRead<<endl;
    data->SetRead(fIsRead);
    fData.push_back(data);
}

int InputFile::OpenFile(const TString& name){
   int retVal=BaseFile::OpenFile(name);
   if(retVal!=0) return retVal;

   FlagBad();
   if(HasTree()){
       // Get the tree from the file
       TObject* inObj= GetFile()->Get(GetTreeName().Data());
       if(!inObj) {
	   std::cout<<"Problem finding '"<<GetTreeName()<<"' in file '"<<GetFile()->GetName()<<"'"<<std::endl;
	   GetFile()->ls();
	   return 2;
       }
       // Did we really get a tree?
       if(inObj->IsA()!= TTree::Class()) {
	   std::cout<<"Object '"<<GetTreeName()<<"' in file '"<<GetFile()->GetName()<<"' is not a TTree"<<std::endl;
	   GetFile()->ls();
	   return 3;
       }
       SetTree((TTree*) inObj);
       SetNumEvents(GetTree()->GetEntries());
       std::cout << "File " << GetFileName() << " is attached with " << GetNumEvents() << " events" << std::endl;
   }else{
       std::cout << "File " << GetFileName() << " has been attached"<< std::endl;
   }
   // Tell each data object that we have a new tree
   for(DataList_t::iterator it=fData.begin();it!=fData.end();it++){
	InitData(*it);
   }
   FlagGood();
   return 0;
}

Int_t InputFile::ReadEvent(const Int_t& i_entry){
    if (i_entry<-1 || i_entry > (Int_t) GetNumEvents()) {
	cout<<"Requested event, "<<i_entry<<", is out of range [0,"<<GetNumEvents()<<"]."<<endl;
	return -1;
    }
    if (!fTree) {
	cout<<GetFileName()<<": Cannot read an event without a tree"<<endl;
	return -1;
    }
    if(i_entry==-1) fCurrentEvent++;
    else fCurrentEvent=i_entry;
    int read= fTree->GetEntry(fCurrentEvent);
    if(read>0) {
	for(DataList_t::iterator it=fData.begin();it!=fData.end();it++){
	    AlertData(*it);
	}
    }
    return read;
}

int OutputFile::OpenFile(const TString& name){
   // create file
   int retVal=BaseFile::OpenFile(name);
   if(retVal!=0) return retVal;
   FlagBad();
   cout<<"Created output file: "<<GetFileName()<<endl;

   if(HasTree()){
       // create the tree
       SetTree(new TTree(GetTreeName().Data(),"All data"));
       if(!GetTree()) {
	   cout<<"Problem creating tree, "<<GetTreeName()<<endl;
	   return -4;
       }
       cout<<"Added tree, "<<GetTreeName()<<endl;
   }

   // Tell each data object that we have a new tree
   for(DataList_t::iterator it=fData.begin();it!=fData.end();it++){
	InitData(*it);
   }
   FlagGood();
   return 0;
}

int OutputFile::CloseFile(){
    if(GetTree()){
	GetTree()->Write();
	GetTree()->Print();
    }
    BaseFile::CloseFile();
    SetTree(NULL);
    SetFile(NULL);
    FlagBad();
    cout<<"Closed output file: "<<GetFileName()<<endl;
    return 0;
}

Int_t OutputFile::WriteEvent(){
    if( HasTree() && ! GetTree() ){
	cout<<"No tree available to write data to!"<<endl;
	return 1;
    }
    for(DataList_t::iterator it=fData.begin();it!=fData.end();it++){
	AlertData(*it);
    }
    GetTree()->Fill();
    StepNumEvents();
    return 0;
}


Int_t MeasuredData::InitForRead(TFile* aFile,TTree* aTree){
    if(!aTree) return 1;
    SetTree(aTree);
    aTree->SetBranchAddress("xc0", fX0);
    aTree->SetBranchAddress("xc1", fX1);
    aTree->SetBranchAddress("yc0", fY0);
    aTree->SetBranchAddress("yc1", fY1);
    fMin_t0 = 0;//data::Get_min_t0(aFile->GetName(),fCh);
    return 0;
}

Bool_t MeasuredData::ShouldSkip()const{
    // Is the first x value less than the minimum allowed for this event?
    return fMin_t0 > GetX(0);
}

void NoiseBank::SetName(const TString& name ){
    if(name != fName){
	fName=name;
	if(fTree) Update();
    }
}
void NoiseBank::SetChannel(const int& ch){
    TString name="Ch";
    name+=TString::Itoa(ch,10);
    SetName(name.Data());
}

Int_t NoiseBank::InitForRead(TFile* aFile,TTree* aTree){
    if(!aTree) return 1;
    SetTree(aTree);
    Update();
    return 0;
}

Int_t SignalPulse::InitForRead(TFile* aFile,TTree* aTree){
   if(!aFile) return 1;
   SetFile(aFile);
   return Update();
}

Int_t TemplatePulse::InitForRead(TFile* aFile,TTree* aTree){
   if(!aTree) return 1;
   SetTree(aTree);
   return Update();
}

Int_t TemplatePulse::Update(){
   if(!GetTree()) return 1;
   if(IsRead()){
	// For some reason root only accepts a pointer to connect up a branch
	GetTree()->SetBranchAddress(fName,&fDataPtr);
   }else{
	GetTree()->Branch(fName,&fData);
   }
   return 0;
}


void TemplatePulse::SetAndScaleData(){
   //fScaledData.ns_per_tick=fData.ns_per_tick;
   int num_bins=GetCapacity();
   //cout<<"SignalPulse::SetAndScaleData(): num_bins="<<num_bins<<endl;
   int max_bin=0;
   Float_t maxADC=0;
   Float_t pedestal=0;
   int startPedestal=10;
   int stopPedestal=110;
   Double_t x,y;
   for(int i=0;i<num_bins;i++){
       x=fData.x[i];
       y=fData.y[i];
       if(i%100==0) cout<<"SignalPulse::SetAndScaleData(): x="<<x<<", y="<<y<<endl;
       if( (i >startPedestal) && (i <stopPedestal) ){ 
	   pedestal+= y;
       }
       if(y > maxADC){
	   maxADC=y;
	   max_bin=i;
       }
       fScaledData.x[i]=x;
       fScaledData.y[i]=y;
   }
   pedestal/= stopPedestal - startPedestal;
   // average the max around the peak
   printf("Pedestal=%g\n",pedestal);
   printf("maxADC=%g ->",maxADC);
   maxADC=0;
   const int max_width=6;
   for (int i=max_bin-max_width;i<max_bin+max_width;i++) {
       maxADC += fData.y[i];
   }
   maxADC/=max_width*2;
   printf("%g \n",maxADC);
   const Float_t range=maxADC-pedestal;
   // Now scale everything so that the pedestal sits at 0 and the peak sits at 1
   for(int i=0;i<num_bins;i++){
       fScaledData.y[i]=(fScaledData.y[i] - pedestal)/range;
   }
   printf("scale=%g, pedestal=%g\n",range,pedestal);
}

Double_t TemplatePulse::GetADCScaled(const Double_t& t0)const{
    // Check t is in range
    double t=t0 - GetScaledX(0);
    if(t<0) return 0;
    // Find corresponding tick for t
    int tick = t/GetNsPerTick()+0.5;
    //int tick = t;
    // Check tick is in range
    if(tick >= GetCapacity() || tick <0) return 0;
    //// make sure we've found the best tick
    //int direction=0;
    //if( GetScaledX(tick) < t) direction=+1;
    //else direction=-1;
    //int deltaTick=tick;
    //while (std::fabs(t-GetScaledX(tick))>1) tick+=direction;
    //if(direction<0) tick--;
    //deltaTick-=tick;
    //printf("%g => %d (%d, %g); ",t,tick,deltaTick, GetNsPerTick());

    // Work out where t is between ticks
    //Float_t dT = t - tick*GetNsPerTick();

    // Linear interpolation
    double ret=fScaledData.y[tick];
    //ret+= (fScaledData.y[tick+1]- fScaledData.y[tick]) / GetNsPerTick() * dT;

    //printf("%g->%g(%d/%d, %g): ",t,ret,tick,GetCapacity(), GetNsPerTick());
    return ret;
}

void TemplatePulse::SetChannel(const int& ch){
    TString name="Ch";
    name+=TString::Itoa(ch,10);
    SetName(name.Data());
    fChannel=ch;
}

void TemplatePulse::SetName(const TString& name ){
    if(name != fName){
	fName=name;
	Update();
    }
}

void SignalPulse::SetAndScaleData(){
   int num_bins=fSignal->GetNbinsX();
   //cout<<"SignalPulse::SetAndScaleData(): num_bins="<<num_bins<<endl;
   int max_bin=0;
   Float_t maxADC=0;
   Float_t pedestal=0;
   Float_t startPedestal=40;
   Float_t stopPedestal=110;
   Double_t x,y;
   for(int i=1;i<=num_bins;i++){
       x=fSignal->GetBinCenter(i);
       y=fSignal->GetBinContent(i);
       if(i%100==0) cout<<"SignalPulse::SetAndScaleData(): x="<<x<<", y="<<y<<endl;
       if( (x >startPedestal) && (x <stopPedestal) ){ 
	   pedestal+= y;
       }
       if(y > maxADC){
	   maxADC=y;
	   max_bin=i;
       }
       fData.x[i]=x;
       fData.y[i]=y;
   }
   printf("Pedestal=%g\n",pedestal);
   pedestal/= stopPedestal - startPedestal;
   // average the max around the peak
   printf("Max: me=%g, root=%g\n",maxADC,fSignal->GetMaximum());
   printf("Max_bin: me=%d, root=%d\n",max_bin,fSignal->GetMaximumBin());
   printf("Pedestal=%g\n",pedestal);
   maxADC=0;
   const int max_width=6;
   for (int i=max_bin-max_width;i<max_bin+max_width;i++) {
       maxADC += fData.y[i];
   }
   maxADC/=max_width;
   // Now scale everything so that the pedestal sits at 0 and the peak sits at 1
   for(int i=0;i<num_bins;i++){
       fSignal->SetBinContent(i,fSignal->GetArray()[i] - pedestal);
   }
   const Float_t range=maxADC-pedestal;
   fSignal->Scale(2/range);
   printf("scale=%g, pedestal=%g\n",range,pedestal);
}

void SignalPulse::SetChannel(const int& ch){
    TString name="profile_";
    name+=TString::Itoa(ch,10);
    SetName(name.Data());
}

void SignalPulse::SetName(const TString& name ){
    if(name != fName){
	fName=name;
	Update();
    }
}

Int_t SignalPulse::Update(){
   if(!GetFile()) return 0;
   TObject* inObj= fFile->Get(GetName());
   if(!inObj) {
       std::cout<<"Problem finding '"<<GetName()<<"' in file '"<<fFile->GetName()<<"'"<<std::endl;
       fFile->ls();
       return 2;
   }
   // Did we really get a TProfile?
   if(inObj->IsA()!= TProfile::Class()) {
       std::cout<<"Object '"<<GetName()<<"' in file '"<<fFile->GetName()<<"' is not a TProfile"<<std::endl;
       fFile->ls();
       return 3;
   }
   fSignal=(TProfile*)inObj;
   SetAndScaleData();
   return 0;
}


Int_t SignalPulse::GetData(const Int_t& size, Float_t* x,Float_t* y)const{
    // Make sure we don't go out of the limits of the input arrays
    const Int_t nSamples=GetNSamples();
    const Int_t limit = nSamples < size? nSamples:size;
    // loop over the profile
    for(Int_t i=0; i< limit; i++){
	GetPoint(i+1,x[i],y[i]);
    }
    return nSamples;
}

Double_t SignalPulse::GetADCScaled(const Double_t& t)const{
    // Find corresponding tick for t
    return fSignal->Interpolate(t);
}

