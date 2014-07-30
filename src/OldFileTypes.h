#ifndef FileTypes_hh_seen
#define FileTypes_hh_seen
#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <TProfile.h>

#ifndef NODATA
#define NODATA 1024
#endif //NODATA

#ifndef CHMAX
#define CHMAX  2
#endif //CHMAX

class BaseData{
    public:
	BaseData():fTree(NULL),fIsRead(true){};
	virtual ~BaseData(){};
	// Called when we create a file that manages this data
	virtual Int_t InitForRead(TFile* aFile,TTree* aTree){return -2;};
	// Called when the file managing this data is closed
	virtual Int_t FinishRead(){return -2;};
	// Called when we create a file that manages this data
	virtual Int_t InitForWrite(TFile* aFile,TTree* aTree){return -2;};
	// Called when the file managing this data is closed
	virtual Int_t FinishWrite(){return -2;};
	// Called by the file before an event is written to the file
	virtual Int_t Write(){return -2;};
	// Called by the file after an event is read from the file
	virtual Int_t Read(const Int_t& i=-1 ){return -2;};

	TTree* GetTree()const {return fTree;};
	void SetTree(TTree* t) {fTree=t;};
	void SetRead(bool val=true){fIsRead=val;};
	bool IsRead()const{return fIsRead;};
	bool IsWrite()const{return !fIsRead;};
	//void SetWrite(){fIsRead=false;};

    private:
	TTree* fTree; //!
	bool fIsRead;

};

// Base type of file, can only be constructed from an InputFile or OutputFile type
class BaseFile{
    protected:
	enum State_t {kGood=0,kBad=1};
	BaseFile(bool isRead=true,const TString& name="",const TString& mode="READ"):
	    fFilename(name),fTreename("t1"),fOpenMode(mode),fHasTree(true),
	    fFile(NULL),fTree(NULL),fState(kBad),fIsRead(isRead){};
    public:
	virtual ~BaseFile();

	TString GetFileName()const{return fFilename;};
	TString GetTreeName()const{return fTreename;};
	TString GetOpenMode()const{return TString(fOpenMode);};
	void SetFileName(const TString& name){fFilename=name;};
	void SetTreeName(const TString& name){fTreename=name;};
	void SetOpenMode(const TString& mode){fOpenMode=mode;};
	bool IsOk()const{return fState==kGood;};

	TFile* GetFile()const {return fFile;};
	TTree* GetTree()const {return fTree;};
	void SetFile(TFile* f) { fFile=f;};
	void SetTree(TTree* t) { fTree=t;};
	virtual int OpenFile(const TString& name="");
	virtual int CloseFile(){ return 0;};
	void FileHasNoTree(bool val=true){ fHasTree=!val;};

	void AddData(BaseData* data);
	void AddData(BaseData& data){AddData(&data);};
	virtual void InitData(BaseData* data){};
	virtual void FinishData(BaseData* data){};

    protected: 
	void FlagBad(){fState=kBad;};
	void FlagGood(){fState=kGood;};
	bool HasTree()const{return fHasTree;};

    protected: 
	// Comments like '//!' tell rootcint to ignore these fields
	TString fFilename; //!
	TString fTreename; //!
	TString fOpenMode; //!
	bool fHasTree; //!

	TFile* fFile; //!
	TTree* fTree; //!

	State_t fState; //!

	typedef std::vector<BaseData*> DataList_t;
	DataList_t fData; //!
	bool fIsRead;
};

class InputFile:public BaseFile{
    public:
	InputFile(const TString& name=""):BaseFile(true,name,"READ"),fNumEvents(0),fCurrentEvent(0){};
	virtual ~InputFile(){};

	virtual int OpenFile(const TString& name="");
	Int_t ReadEvent(const Int_t& i_entry);
	void AlertData(BaseData* data){ data->Read(fCurrentEvent);};
	void InitData(BaseData* data){ if(data->InitForRead(GetFile(),GetTree())==-2) std::cout<<"undefined InitForRead)"<<std::endl;};
	void FinishData(BaseData* data){ data->FinishRead();};

	size_t GetNumEvents()const{return fNumEvents;};
    private:
	void SetNumEvents(size_t n){fNumEvents=n;};
	size_t fNumEvents; //!
	Int_t fCurrentEvent;
};

class OutputFile:public BaseFile{
    public:
	OutputFile(const TString& name=""):BaseFile(false,name,"RECREATE"),fNumEvents(0){};
	virtual ~OutputFile(){};

	virtual int OpenFile(const TString& name="");
	virtual int CloseFile();
	Int_t WriteEvent();
	void AlertData(BaseData* data){ data->Write();};
	void InitData(BaseData* data){ if(data->InitForWrite(GetFile(),GetTree())==-2) std::cout<<"undefined InitForWrite)"<<std::endl;};
	void FinishData(BaseData* data){ data->FinishWrite();};

	size_t GetNumEvents()const{return fNumEvents;};
    private:
	void StepNumEvents(const int& step=1){fNumEvents+=step;};
	void SetNumEvents(size_t n){fNumEvents=n;};
	size_t fNumEvents; //!
};

class MeasuredData:public BaseData{
    public:
	MeasuredData():BaseData(),fFileID(-1){SetChannel(0);};
	~MeasuredData(){};
	Int_t InitForRead(TFile* aFile,TTree* aTree);

    public:
	const Float_t* GetX0()const{ return &fX0[0];}
	const Float_t* GetX1()const{ return &fX1[0];}
	const Float_t* GetY0()const{ return &fY0[0];}
	const Float_t* GetY1()const{ return &fY1[0];}

	Float_t GetX(const Int_t& ch,const Int_t& index)const{
	    return ch==0? fX0[index] : fX1[index];
	}
	Float_t GetY(const Int_t& ch,const Int_t& index)const{
	    return ch==0? fY0[index] : fY1[index];
	}
	Float_t GetX(const Int_t& index)const{
	    return fX[index];
	}
	Float_t GetY(const Int_t& index)const{
	    return fY[index];
	}
	Int_t GetNumData()const {return NoData;};
	void SetChannel(const int& ch){
	    fCh=ch;
	    fX= (ch==0)? &fX0[0]: &fX1[0];
	    fY= (ch==0)? &fY0[0]: &fY1[0];
	}
	Bool_t ShouldSkip()const;

    private:
	static const int NoData=NODATA;
	Float_t fX0[NoData];
	Float_t fX1[NoData];
	Float_t fY0[NoData];
	Float_t fY1[NoData];

	int fCh; //!
	Float_t *fX, *fY; //!
	int fFileID;//!
	Float_t fMin_t0; //!
};

#ifndef MAX_NOISE_SAMPLES
#define MAX_NOISE_SAMPLES 400
#endif //MAX_NOISE_SAMPLES
class NoiseBank:public BaseData{
    public:
	struct Data{
	    static const int max_num_samples=MAX_NOISE_SAMPLES;
	    int NSamples;
	    Float_t start_x;
	    Float_t mean;
	    Float_t sigma;
	    Float_t x[max_num_samples];
	    Float_t y[max_num_samples];
	};
	
    public:
	NoiseBank(const char* name=""):BaseData(),fTree(NULL),fName(name),fBankPtr(&fBank){};
	~NoiseBank(){};
	Int_t InitForWrite(TFile* aFile,TTree* aTree){SetTree(aTree);Update();return 0;};
	Int_t InitForRead(TFile* aFile,TTree* aTree);

	Data* GetData() {return &fBank;};
	Float_t* GetX(){ return &fBank.x[0];}
	Float_t* GetY(){ return &fBank.y[0];}
	Float_t GetSigma()const {return fBank.sigma;};
	Float_t GetMean()const {return fBank.mean;};
	Int_t GetNSamples()const {return fBank.NSamples;};

	const char* GetName()const{return fName.Data();};
	void SetName(const TString&);
	void SetChannel(const int&);
	void Update(){
	    //cout<<IsRead()<<endl;
	    if(IsRead()){
		// For some reason root only accepts a pointer to connect up a branch
		fTree->SetBranchAddress(fName,&fBankPtr);
	    }else{
		fTree->Branch(fName,&fBank);
		//fTree->Branch(fName,&fBank,"max_num_samples/I:NSamples/I:start_x/F:mean/F:sigma/F:x[1024]/F:y[1024]/F");
	    }
	};
	void SetTree(TTree* f){fTree=f;};
	TTree* GetTree(){return fTree;};

    private:
	TTree* fTree; //!
	TString fName;//!
	Data fBank;
	Data * fBankPtr; // For some reason root only accepts a pointer to connect up a branch
};
typedef NoiseBank::Data NoiseBank_t;

class TemplatePulse:public BaseData{
    public:
    struct Data{
	Data():ns_per_tick(0.){};
	static const int max_num_samples=NODATA;
	Float_t ns_per_tick;
	Float_t x[max_num_samples];
	Float_t y[max_num_samples];
    };
    public:
	TemplatePulse(const char* name=""):BaseData(),fName(name),fDataPtr(&fData){};
	virtual ~TemplatePulse(){};
	Int_t InitForRead(TFile* aFile,TTree* aTree);
	Int_t InitForWrite(TFile* aFile,TTree* aTree){SetTree(aTree);Update();return 0;};
	Int_t Update();
	void SetAndScaleData();
	const char* GetName()const{return fName.Data();};
	void SetName(const TString&);
	Float_t GetNsPerTick()const {return fData.ns_per_tick;};
	void SetNsPerTick(const double& v) {fData.ns_per_tick = v;};
	void SetChannel(const int&);
	//Int_t GetNSamples()const {return fTemplate->GetEntries();};
	Int_t GetCapacity()const {return Data::max_num_samples;};
	void SetPoint(const Int_t& index,const Float_t& x,const Float_t& y) {
	    fData.x[index]=x;
	    fData.y[index]=y;
	};
	Float_t GetScaledX(const int& i)const{return fScaledData.x[i];};
	Float_t GetScaledY(const int& i)const{return fScaledData.y[i];};
	Float_t GetX(const int& i)const{return fData.x[i];};
	Float_t GetY(const int& i)const{return fData.y[i];};
	void GetPoint(const Int_t& index,Float_t& x, Float_t& y)const {
	    x= fData.x[index];
	    y= fData.y[index];
	};
	void GetPointNormed(const Int_t& index,Float_t& x, Float_t& y)const {
	    x= fScaledData.x[index];
	    y= fScaledData.y[index];
	};
	Float_t MaxADC()const {return 949.5;};
	Double_t GetADCScaled(const Double_t& t)const;
	Int_t GetScaledData(Float_t*& x,Float_t*& y){
	    x=fScaledData.x;
	    y=fScaledData.y;
	    return fScaledData.max_num_samples;
	}
	Int_t GetData(const Int_t& size, Float_t*& x,Float_t*& y){
	    x=fData.x;
	    y=fData.y;
	    return fData.max_num_samples;
	}
	void Draw(const char* o="AL"){GetTree()->Draw(Form("Ch%d.y:Ch%d.x",fChannel,fChannel),"",o);};
    private:
	TString fName;//!
	Data fData;
	Data fScaledData;
	Data * fDataPtr; // For some reason root only accepts a pointer to connect up a branch
	int fChannel;
};


class SignalPulse:public BaseData{
    public:
    struct Data{
	static const int max_num_samples=4*NODATA;
	static const int ticks_per_ns=2;
	//static const Float_t ns_per_tick=0.5;
	Float_t x[max_num_samples];
	Double_t y[max_num_samples];
    };
    public:
	SignalPulse(const char* name=""):BaseData(),fName(name){};
	~SignalPulse(){};
	Int_t InitForRead(TFile* aFile,TTree* aTree);
	Int_t Update();
	void SetAndScaleData();
	const char* GetName()const{return fName.Data();};
	void SetName(const TString&);
	void SetChannel(const int&);
	void SetFile(TFile* f){fFile=f;};
	TFile* GetFile(){return fFile;};
	Int_t GetNSamples()const {return fSignal->GetEntries();};
	Int_t GetCapacity()const {return fData.max_num_samples;};
	void GetPoint(const Int_t& index,Float_t& x, Float_t& y)const {
	    x= fSignal->GetBinCenter(index);
	    y= fSignal->GetArray()[index];
	};
	void GetPointNormed(const Int_t& index,Float_t& x, Float_t& y)const {
	    x= fData.x[index];
	    y= fData.y[index];
	};
	Float_t MaxADC()const {return 949.5;};
	Double_t GetADCScaled(const Double_t& t)const;
	Int_t GetData(const Int_t& size, Float_t* x,Float_t* y)const;
	Int_t GetNormedData(const Int_t& size, Float_t*& x,Double_t*& y){
	    x=fData.x;
	    y=fData.y;
	    return fData.max_num_samples;
	}
	void Draw(const char* o="AL"){fSignal->Draw(o);};
    private:
	TString fName;//!
	TFile* fFile;//!
	TProfile* fSignal;
	Data fData;
};

class FakeEvent:public BaseData{
    public:
    struct Data{
	Data():NSamples(0),pileUp_offset(0){};
	static const size_t max_num_samples=1500;
	size_t NSamples;
	int pileUp_offset;
	Float_t x[max_num_samples];
	Float_t y[max_num_samples];
	Float_t noise[max_num_samples];
	Float_t pileUp[max_num_samples];
    };
    public:
	FakeEvent():BaseData(),fDataPtr(&fData){};
	~FakeEvent(){};
	Int_t InitForRead(TFile* aFile,TTree* aTree){aTree->SetBranchAddress(GetName(),&fDataPtr);return 0;};
	Int_t InitForWrite(TFile* aFile,TTree* aTree){aTree->Branch(GetName(),&fData);return 0;};
	void SetChannel(Int_t ch){ 
	    //fData.pileUp_channel=ch; 
	};
	Float_t* GetX(){ return fData.x;}
	Float_t* GetY(){ return fData.y;}
	Float_t* GetPileUp(){ return fData.pileUp;}
	Float_t* GetNoise(){ return &fData.noise[0];}
	const Float_t* GetX()const{ return &fData.x[0];}
	const Float_t* GetY()const{ return &fData.y[0];}
	const Float_t* GetPileUp()const{ return fData.pileUp;}
	const Float_t* GetNoise()const{ return &fData.noise[0];}
	Float_t GetX(const Int_t& i)const{ return fData.x[i];}
	Float_t GetY(const Int_t& i)const{ return fData.y[i];}
	Float_t GetPileUp(const Int_t& i)const{ return fData.pileUp[i];}
	Float_t GetNoise(const Int_t& i)const{ return fData.noise[i];}
	Int_t GetCapacity()const{ return fData.max_num_samples;}
	size_t GetNSamples()const {return fData.NSamples;};
	void SetNSamples(const double& v) {fData.NSamples=v;};
	void SuperposeNoise(){ for(size_t i=0; i<fData.max_num_samples;i++) fData.y[i]+=fData.noise[i];};
	void SuperposePileup(){ for(size_t i=0; i<fData.max_num_samples;i++) fData.y[i]+=fData.pileUp[i];};
	void ScaleSignal(const double& scale){ 
	  for(size_t i=0; i<fData.max_num_samples;i++)
	      fData.y[i]*=scale;
	  };
	void ResetData(){ fData=Data();	};
	//int GetFileID(){return fData.pileUp_fileID;};
	Data* GetData() {return &fData;};
	const char* GetName()const{return fName.Data();};
	TString GetName(){return fName;};
	void SetName(const TString& n){ fName=n;};
	int GetPileUpOffset(){
	return fData.pileUp_offset;
	}
	void SetPileUpOffset(const Int_t& off){
	    fData.pileUp_offset=off;
	};
    private:
	Data fData;
	Data* fDataPtr;
	TString fName;
};
#endif //FileTypes_hh_seen
