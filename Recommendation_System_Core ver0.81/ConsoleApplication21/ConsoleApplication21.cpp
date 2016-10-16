// ConsoleApplication21.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
//#include "Queue.h"
//#include "Clustering_Manager.h"
//#include "Inter_Interface.h"
//#include "Intra_Interface.h"
//#include "Member_Manager.h"
//#include "Rating_Manager.h"
//#include "Recommendation_Controller.h"
//#include "Recommendation_Manager.h"
//#include "Recommendation_Model_Manager.h"
//#include "Recommender.h"
//#include "Rule_Apllier.h"
//#include "structs.h"

/*

int get_User_num(){
	return 30;
}

int get_Item_num(){
	return 60;
}

MemberVector* get_MemberVector(bool type,int User_id){
	MemberVector* _result=new MemberVector;

	_result->Element[0]=0.145f;
	_result->Element[1]=0.445f;
	_result->Element[2]=0.745f;
	_result->Element[3]=0.045f;
	_result->Element[4]=0.945f;

	return _result;
}

UserProfile* get_UserProfile(int User_id){
	UserProfile* _result=new UserProfile;

	_result->age=20;
	_result->diabetesmellitus=0;
	_result->hyperlipidemia=0;
	_result->hypertension=0;

	return _result;
}

ItemProperty* get_ItemProperty(int Item_id){
	ItemProperty* _result=new ItemProperty;

	_result->to_diabetesmellitus.to50s=0;
	_result->to_diabetesmellitus.to_40s=0;
	_result->to_diabetesmellitus.to_under40s=0;
	_result->to_diabetesmellitus.to_upto60s=0;

	_result->to_hyperlipidemia.to50s=0;
	_result->to_hyperlipidemia.to_40s=0;
	_result->to_hyperlipidemia.to_under40s=0;
	_result->to_hyperlipidemia.to_upto60s=0;

	_result->to_hypertension.to50s=0;
	_result->to_hypertension.to_40s=0;
	_result->to_hypertension.to_under40s=0;
	_result->to_hypertension.to_upto60s=0;

	return _result;
}

int get_Log_num(){
	return 100;
}

Rating_Point get_RatingPoint(int log_num){
	Rating_Point _result;

	_result.item_id=log_num;
	_result.user_id=log_num;
	_result.user_pref=5;
	_result.item_prior=3;

	return _result;
}

*/

int _tmain(int argc, _TCHAR* argv[])
{
	Rec_Controller myRec;

	myRec.Input_Member(true,30);
	myRec.Input_Member(false,60);

	myRec.Input_Rating(20,30,3,5);

	myRec.Time_Interrupt();

	myRec.get_Rec_list(20);

	return 0;
}

