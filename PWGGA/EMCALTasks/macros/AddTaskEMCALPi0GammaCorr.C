// $Id$
AliAnalysisTaskEMCALPi0GammaCorr* AddTaskEMCALPi0GammaCorr(
  UInt_t      evtTriggerType         = AliVEvent::kEMCEGA, //AliVEvent::kAnyINT,// AliVEvent::kEMCEGA,//..use this type of events to combine gammas(trigger) with hadrons
  UInt_t      evtMixingType          = AliVEvent::kAnyINT,//..use only this type of events to fill your mixed event pool with tracks
  Double_t    trackptcut             = 0.15,              //..
  Double_t    clusptcut              = 3.0,              //..
  Bool_t      SavePool               = 0,                 //..saves a mixed event pool to the output event
  const char *trackName              = "usedefault",
  const char *clusName               = "usedefault",
  const char *taskname               = "AliAnalysisTask",
)
{  
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr)  ::Error("AddTask", "No analysis manager to connect to.");
  AliVEventHandler* handler = mgr->GetInputEventHandler();
  if (!handler) ::Error("AddTask", "This task requires an input event handler");
 
  if(trackName=="usedefault")trackName = "usedefault"; //it was Tracks "tracks"
  if(clusName =="usedefault")clusName  = "usedefault";//"caloClusters"; //CaloClusters"; //caloClusters
  //-------------------------------------------------------
  // Built the name of the Task together
  //-------------------------------------------------------
  TString combinedName;
  combinedName.Form("%s_histos",taskname);
  TString contName(combinedName);

  //-------------------------------------------------------
  // Init the task and do settings
  //------------------------------------------------------
  std::cout << "----#About to start AliAnalysisTaskEMCALPi0GammaCorr task " << std::endl;
  AliAnalysisTaskEMCALPi0GammaCorr* AnalysisTask = new AliAnalysisTaskEMCALPi0GammaCorr(kTRUE);
  std::cout << "----Adding cluster container, track container" << std::endl;
  AnalysisTask->AddClusterContainer("usedefault");
  AliTrackContainer* trackCont = AnalysisTask->AddTrackContainer("usedefault");
  //AliWarning("Setting FilterHybridTracks");
 //trackCont->SetFilterHybridTracks(kTRUE); //gives me Hyprid tracks

  //-------------------------------------------------------
  // Add some selection criteria
  //-------------------------------------------------------
  AnalysisTask->SetOffTrigger(evtTriggerType|evtMixingType); //..select only evets of type evtTriggerType and evtMixingType
  
    //..for Run1 pPb
  AnalysisTask->SetUseManualEvtCuts(kTRUE);
  AnalysisTask->SetUseAliAnaUtils(kTRUE); //this does automatically some vertex selection and pileup suppression
  AnalysisTask->SetVzRange(-10,10);
  AnalysisTask->SetCentRange(0,100.0);
  //..new task for run2
  //AnalysisTask->SetNCentBins(5);
  AnalysisTask->SetUseNewCentralityEstimation(kFALSE); //maybe this is what is required
  if(AnalysisTask->GetTrackContainer(trackName))
  {   
      std::cout << "Setting pt cut of tracks to " << trackptcut;
	  AnalysisTask->GetTrackContainer(trackName)->SetParticlePtCut(trackptcut);
  }

  if(AnalysisTask->GetClusterContainer(clusName))
  {
      std::cout << "Setting cuts for clusters" << std::endl;
	  AnalysisTask->GetClusterContainer(clusName)->SetClusECut(0);                
	  AnalysisTask->GetClusterContainer(clusName)->SetClusPtCut(clusptcut);       
	  AnalysisTask->GetClusterContainer(clusName)->SetClusUserDefEnergyCut(AliVCluster::kHadCorr,0);
	  AnalysisTask->GetClusterContainer(clusName)->SetDefaultClusterEnergy(AliVCluster::kHadCorr);
  }

  AnalysisTask->SetNeedEmcalGeom(kTRUE);
  AnalysisTask->SetSavePool(SavePool);
  AnalysisTask->SetEvtTriggerType(evtTriggerType);   
  AnalysisTask->SetEvtMixType(evtMixingType);       
  
  mgr->AddTask(AnalysisTask);
  // Create containers for input/output
  AliAnalysisDataContainer *cinput1  = mgr->GetCommonInputContainer()  ;
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer(contName.Data(),TList::Class(),
	  	  	  	  	  	  	  	  	  	  	  	  	   AliAnalysisManager::kOutputContainer,
		  	  	  	  	  	  	  	  	  	  	  	  	   Form("%s", AliAnalysisManager::GetCommonFileName()));

  //AliAnalysisDataContainer *coutput2 = mgr->CreateContainer("MyOuputContainer", TList::Class(), 
   //                                                         AliAnalysisManager::kOutputContainer, 
   //                                                         Form("%s", AliAnalysisManager::GetCommonFileName()));
  mgr->ConnectInput  (AnalysisTask, 0,  cinput1 );
  mgr->ConnectOutput (AnalysisTask, 1, coutput1 );
  //mgr->ConnectOutput (AnalysisTask, 2, coutput2 );
  return AnalysisTask;
}
