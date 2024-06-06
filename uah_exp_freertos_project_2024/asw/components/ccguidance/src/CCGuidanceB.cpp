#include <public/ccguidance_iface_v1.h>

// ***********************************************************************

// class EDROOM_CTX_Top_0

// ***********************************************************************



	// CONSTRUCTORS***********************************************

CCGuidance::EDROOM_CTX_Top_0::EDROOM_CTX_Top_0(CCGuidance &act,
	 CDEventList & EDROOMpVarVCurrentEvList,
	 CDTMList & EDROOMpVarVCurrentTMList,
	 Pr_Time & EDROOMpVarVNextTimeout,
	 CEDROOMPOOLCDTMList & EDROOMpPoolCDTMList ):

	EDROOMcomponent(act),
	Msg(EDROOMcomponent.Msg),
	MsgBack(EDROOMcomponent.MsgBack),
	TMChannelCtrl(EDROOMcomponent.TMChannelCtrl),
	Guidance(EDROOMcomponent.Guidance),
	Guidance_Timer(EDROOMcomponent.Guidance_Timer),
	VCurrentEvList(EDROOMpVarVCurrentEvList),
	VCurrentTMList(EDROOMpVarVCurrentTMList),
	VNextTimeout(EDROOMpVarVNextTimeout),
	EDROOMPoolCDTMList(EDROOMpPoolCDTMList)
{
}

CCGuidance::EDROOM_CTX_Top_0::EDROOM_CTX_Top_0(EDROOM_CTX_Top_0 &context):

	EDROOMcomponent(context.EDROOMcomponent),
	Msg(context.Msg),
	MsgBack(context.MsgBack),
	TMChannelCtrl(context.TMChannelCtrl),
	Guidance(context.Guidance),
	Guidance_Timer(context.Guidance_Timer),
	VCurrentEvList(context.VCurrentEvList),
	VCurrentTMList(context.VCurrentTMList),
	VNextTimeout(context.VNextTimeout),
	EDROOMPoolCDTMList(context.EDROOMPoolCDTMList )
{

}

	// EDROOMSearchContextTrans********************************************

bool CCGuidance::EDROOM_CTX_Top_0::EDROOMSearchContextTrans(
			TEDROOMTransId &edroomCurrentTrans)
			{

	bool edroomValidMsg=false; 

	 switch(Msg->signal)
	{

		 case ( EDROOMSignalDestroy ): 

			 edroomValidMsg=true;
			 edroomCurrentTrans.distanceToContext = 0 ;
			 edroomCurrentTrans.localId = -1 ;
			 break;

	}

	return(edroomValidMsg);

}

	// User-defined Functions   ****************************

void	CCGuidance::EDROOM_CTX_Top_0::FDo_Guidance()

{
   //Define absolute time
  Pr_Time time;
 
 
VNextTimeout+= Pr_Time(0.1,0); // Add X sec + Y microsec 
time=VNextTimeout; 
 
PUSService129::GuidanceControl();
 
   //Program absolute timer 
   Guidance_Timer.InformAt( time ); 
}



void	CCGuidance::EDROOM_CTX_Top_0::FExecGuidance()

{
   //Handle Msg->data
  CDTCHandler & varSGuidance_TC = *(CDTCHandler *)Msg->data;
 
	
		// Data access
 
CDEventList TCExecEventList;  	
  
PUS_GuidanceExecutor::ExecTC(varSHK_FDIR_TC,VCurrentTMList,TCExecEventList);

}



void	CCGuidance::EDROOM_CTX_Top_0::FInit_Guidance()

{
   //Define absolute time
  Pr_Time time;
 
 
time.GetTime(); // Get current monotonic time   
time+=Pr_Time(0.1,0); // Add X sec + Y microsec    
VNextTimeout=time;
 
 
   //Program absolute timer 
   Guidance_Timer.InformAt( time ); 
}



void	CCGuidance::EDROOM_CTX_Top_0::FInvokeTxTMList()

{
   //Allocate data from pool
  CDTMList * pSTxTM_Data = EDROOMPoolCDTMList.AllocData();
 
	
	*pSTxTM_Data=VCurrentTMList;    
	VCurrentTMList.Clear();
   //Invoke synchronous communication 
   MsgBack=TMChannelCtrl.invoke(STxTM,pSTxTM_Data,&EDROOMPoolCDTMList); 
}



void	CCGuidance::EDROOM_CTX_Top_0::FTriggerEvAction()

{

PUSService19::TriggerEvActionExecution();

}



	//********************************** Pools *************************************

	//CEDROOMPOOLCDTMList

CCGuidance::EDROOM_CTX_Top_0::CEDROOMPOOLCDTMList::CEDROOMPOOLCDTMList(
			TEDROOMUInt32 elemCount,CDTMList* pMem,bool * pMemMarks):
				CEDROOMProtectedMemoryPool(elemCount, pMem, pMemMarks,
					sizeof(CDTMList))
{
}

CDTMList *	CCGuidance::EDROOM_CTX_Top_0::CEDROOMPOOLCDTMList::AllocData()
{
	return(CDTMList*)CEDROOMProtectedMemoryPool::AllocData();
}



// ***********************************************************************

// class EDROOM_SUB_Top_0

// ***********************************************************************



	// CONSTRUCTOR*************************************************

CCGuidance::EDROOM_SUB_Top_0::EDROOM_SUB_Top_0 (CCGuidance&act
	,CEDROOMMemory *pEDROOMMemory):
		EDROOM_CTX_Top_0(act,
			VCurrentEvList,
			VCurrentTMList,
			VNextTimeout,
			EDROOMPoolCDTMList),
		EDROOMPoolCDTMList(
			10, pEDROOMMemory->poolCDTMList,
			pEDROOMMemory->poolMarkCDTMList)
{

}

	//***************************** EDROOMBehaviour ********************************

void CCGuidance::EDROOM_SUB_Top_0::EDROOMBehaviour()
{

	TEDROOMTransId edroomCurrentTrans;

	//Behaviour starts from Init State

	edroomCurrentTrans = EDROOMIArrival();

	do
	{

		//Take next transition

		switch(edroomCurrentTrans.localId)
		{

			//Next Transition is Init
			case (Init):
				//Execute Action 
				FInit_Guidance();
				//Next State is Idle
				edroomNextState = Idle;
				break;
			//Next Transition is ExecTc
			case (ExecTc):
				//Msg->Data Handling 
				FExecGuidance();
				//Invoke Synchronous Message 
				FInvokeTxTMList();
				//Next State is Idle
				edroomNextState = Idle;
				break;
			//Next Transition is Do_Guidance
			case (Do_Guidance):
				//Execute Action 
				FDo_Guidance();
				//Invoke Synchronous Message 
				FInvokeTxTMList();
				//Next State is Idle
				edroomNextState = Idle;
				break;
		}

		//Entry into the Next State 
		switch(edroomNextState)
		{

				//Go to the state I
			case (I):
				//Arrival to state I
				edroomCurrentTrans=EDROOMIArrival();
				break;

				//Go to the state Idle
			case (Idle):
				//Arrival to state Idle
				edroomCurrentTrans=EDROOMIdleArrival();
				break;

		}

		edroomCurrentState=edroomNextState;

	}while(Msg->signal != EDROOMSignalDestroy);

}



	// Context Init**********************************************

void CCGuidance::EDROOM_SUB_Top_0::EDROOMInit()
{

edroomCurrentState=I;

}



//	 ***********************************************************************

//	 SubState I

//	 ***********************************************************************



TEDROOMTransId CCGuidance::EDROOM_SUB_Top_0::EDROOMIArrival()
{

	TEDROOMTransId edroomCurrentTrans;

	//Next transition is  Init
	edroomCurrentTrans.localId = Init;
	edroomCurrentTrans.distanceToContext = 0;
	return(edroomCurrentTrans);

}



	// ***********************************************************************

	// Leaf SubState  Idle

	// ***********************************************************************



TEDROOMTransId CCGuidance::EDROOM_SUB_Top_0::EDROOMIdleArrival()
{

	TEDROOMTransId edroomCurrentTrans;

	bool edroomValidMsg=false;

	do
	{

		EDROOMNewMessage ();

		switch(Msg->signal)
		{

			case (SGuidance_TC): 

				 if (*Msg->GetPInterface() == Guidance)
				{

					//Next transition is  ExecTc
					edroomCurrentTrans.localId= ExecTc;
					edroomCurrentTrans.distanceToContext = 0;
					edroomValidMsg=true;
				 }

				break;

			case (EDROOMSignalTimeout): 

				 if (*Msg->GetPInterface() == Guidance_Timer)
				{

					//Next transition is  Do_Guidance
					edroomCurrentTrans.localId= Do_Guidance;
					edroomCurrentTrans.distanceToContext = 0;
					edroomValidMsg=true;
				 }

				break;

		};

		if (false == edroomValidMsg)
		{
			 edroomValidMsg = EDROOMSearchContextTrans(edroomCurrentTrans);

		}

	} while (false == edroomValidMsg);

	return(edroomCurrentTrans);

}



