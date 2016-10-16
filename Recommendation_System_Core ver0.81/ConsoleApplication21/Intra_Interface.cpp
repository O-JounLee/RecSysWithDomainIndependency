#include "structs.h"
#include "stdafx.h"

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
	return 20;
}

Rating_Point get_RatingPoint(int log_num){
	Rating_Point _result;

	_result.item_id=log_num;
	_result.user_id=log_num;
	_result.user_pref=5;
	_result.item_prior=4;

	return _result;
}