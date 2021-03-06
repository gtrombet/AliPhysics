AliAnalysisTaskEmcalJetSpectra8TeVTriggerQA* AddTaskEmcalJetSpectra8TeVTriggerQA(
                                                                                 const char *ntracks            = "usedefault",
                                                                                 const char *nclusters          = "usedefault",
                                                                                 const char* ncells             = "usedefault",
                                                                                 const char *suffix             = ""
                                                                                 )
{
    // Get the pointer to the existing analysis manager via the static access method.
    //==============================================================================
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    if (!mgr)
    {
        ::Error("AddTaskEmcalJetSpectra8TeVTriggerQA", "No analysis manager to connect to.");
        return 0;
    }

    // Check the analysis type using the event handlers connected to the analysis manager.
    //==============================================================================
    AliVEventHandler* handler = mgr->GetInputEventHandler();
    if (!handler)
    {
        ::Error("AddTaskEmcalJetSpectra8TeVTriggerQA", "This task requires an input event handler");
        return 0;
    }

    enum EDataType_t {
        kUnknown,
        kESD,
        kAOD
    };

    EDataType_t dataType = kUnknown;

    if (handler->InheritsFrom("AliESDInputHandler")) {
        dataType = kESD;
    }
    else if (handler->InheritsFrom("AliAODInputHandler")) {
        dataType = kAOD;
    }

    //-------------------------------------------------------
    // Init the task and do settings
    //-------------------------------------------------------

    TString trackName(ntracks);
    TString clusName(nclusters);
    TString cellName(ncells);

    if (trackName == "usedefault") {
        if (dataType == kESD) {
            trackName = "Tracks";
        }
        else if (dataType == kAOD) {
            trackName = "tracks";
        }
        else {
            trackName = "";
        }
    }

    if (clusName == "usedefault") {
        if (dataType == kESD) {
            clusName = "CaloClusters";
        }
        else if (dataType == kAOD) {
            clusName = "caloClusters";
        }
        else {
            clusName = "";
        }
    }

    if (cellName == "usedefault") {
        if (dataType == kESD) {
            cellName = "EMCALCells";
        }
        else if (dataType == kAOD) {
            cellName = "emcalCells";
        }
        else {
            cellName = "";
        }
    }

    TString name("AliAnalysisTaskEmcalJetSpectra8TeVTriggerQA");
    if (!trackName.IsNull()) {
        name += "_";
        name += trackName;
    }
    if (!clusName.IsNull()) {
        name += "_";
        name += clusName;
    }
    if (!cellName.IsNull()) {
        name += "_";
        name += cellName;
    }
    if (strcmp(suffix,"") != 0) {
        name += "_";
        name += suffix;
    }

    AliAnalysisTaskEmcalJetSpectra8TeVTriggerQA* spectraTaskQA = new AliAnalysisTaskEmcalJetSpectra8TeVTriggerQA(name);
    spectraTaskQA->SetCaloCellsName(cellName);
    spectraTaskQA->SetVzRange(-10,10);

    if (trackName == "mcparticles") {
        AliMCParticleContainer* mcpartCont = spectraTaskQA->AddMCParticleContainer(trackName);
    }
    else if (trackName == "tracks" || trackName == "Tracks") {
        AliTrackContainer* trackCont = spectraTaskQA->AddTrackContainer(trackName);
    }
    else if (!trackName.IsNull()) {
        spectraTaskQA->AddParticleContainer(trackName);
    }
    spectraTaskQA->AddClusterContainer(clusName);


    TString     kEmcalTriggers      = "EmcalTriggers";
    spectraTaskQA->SetCaloTriggerPatchInfoName(kEmcalTriggers.Data());

    //-------------------------------------------------------
    // Final settings, pass to manager and set the containers
    //-------------------------------------------------------

    mgr->AddTask(spectraTaskQA);

    // Create containers for input/output
    AliAnalysisDataContainer *cinput1  = mgr->GetCommonInputContainer()  ;
    TString contname(name);
    contname += "_histos";
    AliAnalysisDataContainer *coutput1 = mgr->CreateContainer(contname.Data(),
                                                              TList::Class(),AliAnalysisManager::kOutputContainer,
                                                              Form("%s", AliAnalysisManager::GetCommonFileName()));
    mgr->ConnectInput  (spectraTaskQA, 0,  cinput1 );
    mgr->ConnectOutput (spectraTaskQA, 1, coutput1 );

    return spectraTaskQA;
}
