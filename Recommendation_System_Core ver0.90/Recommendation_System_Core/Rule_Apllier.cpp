#include <iostream>
#include <stdio.h>
#include "Intra_Interface.h"
#include "Recommendation_Controller.h"
#include "structs.h"
#include "Recommendation_Manager.h"
#include "Rule_Apllier.h"
#include "stdafx.h"

Rule_Applier::Rule_Applier(sql::Connection* con,int num_of_User,int num_of_Item){
	_con=con;

	num_of_user=num_of_User;
	num_of_item=num_of_Item;

	return;
}

Rule_Applier::~Rule_Applier(){

}

int* Rule_Applier::rule_applier(int client_id,int* pre_Item_list){
	std::cout<<"Start Applying Rules. \n";

	int* Rec_Item_list=new int[40];
	for(int i=0;i<40;i++) Rec_Item_list[i]=NULL;
	UserProfile* up=new UserProfile;
	ItemProperty* ip=new ItemProperty;

	get_UserProfile(_con,client_id,up);

	U_Profile.age=up->age;
	U_Profile.diabetesmellitus=up->diabetesmellitus;
	U_Profile.hyperlipidemia=up->hyperlipidemia;
	U_Profile.hypertension=up->hypertension;

	Item_list=new ItemProperty[40];
	for(int i=0;i<40;i++){
		get_ItemProperty(_con,pre_Item_list[i],ip);

		Item_list[i].to_diabetesmellitus.to50s=ip->to_diabetesmellitus.to50s;
		Item_list[i].to_diabetesmellitus.to_40s=ip->to_diabetesmellitus.to_40s;
		Item_list[i].to_diabetesmellitus.to_under40s=ip->to_diabetesmellitus.to_under40s;
		Item_list[i].to_diabetesmellitus.to_upto60s=ip->to_diabetesmellitus.to_upto60s;

		Item_list[i].to_hyperlipidemia.to50s=ip->to_hyperlipidemia.to50s;
		Item_list[i].to_hyperlipidemia.to_40s=ip->to_hyperlipidemia.to_40s;
		Item_list[i].to_hyperlipidemia.to_under40s=ip->to_hyperlipidemia.to_under40s;
		Item_list[i].to_hyperlipidemia.to_upto60s=ip->to_hyperlipidemia.to_upto60s;

		Item_list[i].to_hypertension.to50s=ip->to_hypertension.to50s;
		Item_list[i].to_hypertension.to_40s=ip->to_hypertension.to_40s;
		Item_list[i].to_hypertension.to_under40s=ip->to_hypertension.to_under40s;
		Item_list[i].to_hypertension.to_upto60s=ip->to_hypertension.to_upto60s;
	}

	for(int i=0;i<40;i++){
		if(U_Profile.diabetesmellitus){
			if (U_Profile.age<40){
				if(Item_list[i].to_diabetesmellitus.to_under40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_diabetesmellitus.to_under40s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<50){
				if(Item_list[i].to_diabetesmellitus.to_40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_diabetesmellitus.to_40s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<60){
				if(Item_list[i].to_diabetesmellitus.to50s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_diabetesmellitus.to50s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else{
				if(Item_list[i].to_diabetesmellitus.to_upto60s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_diabetesmellitus.to_upto60s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
		}
		else if(U_Profile.hyperlipidemia){
			if (U_Profile.age<40){
				if(Item_list[i].to_hyperlipidemia.to_under40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hyperlipidemia.to_under40s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<50){
				if(Item_list[i].to_hyperlipidemia.to_40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hyperlipidemia.to_40s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<60){
				if(Item_list[i].to_hyperlipidemia.to50s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hyperlipidemia.to50s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else{
				if(Item_list[i].to_hyperlipidemia.to_upto60s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hyperlipidemia.to_upto60s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
		}
		else if(U_Profile.diabetesmellitus){
			if (U_Profile.age<40){
				if(Item_list[i].to_hypertension.to_under40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hypertension.to_under40s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<50){
				if(Item_list[i].to_hypertension.to_40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hypertension.to_40s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<60){
				if(Item_list[i].to_hypertension.to50s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hypertension.to50s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else{
				if(Item_list[i].to_hypertension.to_upto60s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hypertension.to_upto60s){
					//for(int k=0;k<40;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
		}
	}

	
	for(int i=0;i<40;i++){
		//if(pre_Item_list[i]!=NULL){
			Rec_Item_list[i]=pre_Item_list[i];
		//	pre_Item_list[i]=NULL;
		//}
	}
	
	/*
	for(int i=38;i>0;i--){
		if(Rec_Item_list[i]==NULL){
			Rec_Item_list[i]=Rec_Item_list[i+1];
			Rec_Item_list[i+1]=NULL;
		}
	}
	*/

	delete pre_Item_list;
	delete up;
	delete ip;

	for(int i=0;i<40;i++) std::cout<<Rec_Item_list[i]<<" ";

	return Rec_Item_list;
}