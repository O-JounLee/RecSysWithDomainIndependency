#include <iostream>
#include <stdio.h>
#include "Intra_Interface.h"
#include "Recommendation_Controller.h"
#include "structs.h"
#include "Recommendation_Manager.h"
#include "Rule_Apllier.h"
#include "stdafx.h"

Rule_Applier::Rule_Applier(int num_of_User,int num_of_Item){
	num_of_user=num_of_User;
	num_of_item=num_of_Item;

	return;
}

Rule_Applier::~Rule_Applier(){

}

int* Rule_Applier::rule_applier(int client_id,int* pre_Item_list){
	int* Rec_Item_list=new int[20];
	for(int i=0;i<20;i++) Rec_Item_list[i]=NULL;
	U_Profile.age=get_UserProfile(client_id)->age;
	U_Profile.diabetesmellitus=get_UserProfile(client_id)->diabetesmellitus;
	U_Profile.hyperlipidemia=get_UserProfile(client_id)->hyperlipidemia;
	U_Profile.hypertension=get_UserProfile(client_id)->hypertension;
	Item_list=new ItemProperty[20];
	for(int i=0;i<20;i++){
		Item_list[i].to_diabetesmellitus.to50s=get_ItemProperty(pre_Item_list[i])->to_diabetesmellitus.to50s;
		Item_list[i].to_diabetesmellitus.to_40s=get_ItemProperty(pre_Item_list[i])->to_diabetesmellitus.to_40s;
		Item_list[i].to_diabetesmellitus.to_under40s=get_ItemProperty(pre_Item_list[i])->to_diabetesmellitus.to_under40s;
		Item_list[i].to_diabetesmellitus.to_upto60s=get_ItemProperty(pre_Item_list[i])->to_diabetesmellitus.to_upto60s;

		Item_list[i].to_hyperlipidemia.to50s=get_ItemProperty(pre_Item_list[i])->to_hyperlipidemia.to50s;
		Item_list[i].to_hyperlipidemia.to_40s=get_ItemProperty(pre_Item_list[i])->to_hyperlipidemia.to_40s;
		Item_list[i].to_hyperlipidemia.to_under40s=get_ItemProperty(pre_Item_list[i])->to_hyperlipidemia.to_under40s;
		Item_list[i].to_hyperlipidemia.to_upto60s=get_ItemProperty(pre_Item_list[i])->to_hyperlipidemia.to_upto60s;

		Item_list[i].to_hypertension.to50s=get_ItemProperty(pre_Item_list[i])->to_hypertension.to50s;
		Item_list[i].to_hypertension.to_40s=get_ItemProperty(pre_Item_list[i])->to_hypertension.to_40s;
		Item_list[i].to_hypertension.to_under40s=get_ItemProperty(pre_Item_list[i])->to_hypertension.to_under40s;
		Item_list[i].to_hypertension.to_upto60s=get_ItemProperty(pre_Item_list[i])->to_hypertension.to_upto60s;
	}

	for(int i=0;i<20;i++){
		if(U_Profile.diabetesmellitus){
			if (U_Profile.age<40){
				if(Item_list[i].to_diabetesmellitus.to_under40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_diabetesmellitus.to_under40s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<50){
				if(Item_list[i].to_diabetesmellitus.to_40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_diabetesmellitus.to_40s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<60){
				if(Item_list[i].to_diabetesmellitus.to50s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_diabetesmellitus.to50s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else{
				if(Item_list[i].to_diabetesmellitus.to_upto60s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_diabetesmellitus.to_upto60s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
		}
		else if(U_Profile.hyperlipidemia){
			if (U_Profile.age<40){
				if(Item_list[i].to_hyperlipidemia.to_under40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hyperlipidemia.to_under40s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<50){
				if(Item_list[i].to_hyperlipidemia.to_40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hyperlipidemia.to_40s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<60){
				if(Item_list[i].to_hyperlipidemia.to50s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hyperlipidemia.to50s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else{
				if(Item_list[i].to_hyperlipidemia.to_upto60s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hyperlipidemia.to_upto60s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
		}
		else if(U_Profile.diabetesmellitus){
			if (U_Profile.age<40){
				if(Item_list[i].to_hypertension.to_under40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hypertension.to_under40s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<50){
				if(Item_list[i].to_hypertension.to_40s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hypertension.to_40s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else if(U_Profile.age<60){
				if(Item_list[i].to_hypertension.to50s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hypertension.to50s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
			else{
				if(Item_list[i].to_hypertension.to_upto60s<0) pre_Item_list[i]=NULL;
				else if(Item_list[i].to_hypertension.to_upto60s){
					for(int k=0;k<20;k++) if(Rec_Item_list[k]==NULL) Rec_Item_list[k]=pre_Item_list[i];
				}
			}
		}
	}
	for(int i=0;i<20;i++){
		if(pre_Item_list[i]!=NULL){
			Rec_Item_list[i]=pre_Item_list[i];
			pre_Item_list[i]=NULL;
		}
	}
	return Rec_Item_list;
}