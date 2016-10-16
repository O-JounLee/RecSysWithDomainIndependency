#include <iostream>
#include <stdio.h>
#include "Intra_Interface.h"
#include "structs.h"
#include "Recommendation_Model_Manager.h"
#include "Member_Manager.h"
#include "stdafx.h"

using namespace std;

Member_Manager::Member_Manager(int num_user,int num_item){
	num_of_user=num_user;
	num_of_item=num_item;
}

Member_Manager::~Member_Manager(){

}

void Member_Manager::Make_UserList(){
	U_Vectorlist=new MemberVector[num_of_user];
	for(int i=0;i<num_of_user;i++) for(int j=0;j<5;j++) U_Vectorlist[i].Element[j]=get_MemberVector(1,i)->Element[j];

	return;
}

void Member_Manager::Make_ItemList(){
	I_Vectorlist=new MemberVector[num_of_item];
	for(int i=0;i<num_of_item;i++) for(int j=0;j<5;j++) I_Vectorlist[i].Element[j]=get_MemberVector(0,i)->Element[j];

	return;
}

void Member_Manager::learning_U_Vector(int user_id,int item_id,float user_pref){
	MemberVector User,Item;
	for(int i=0;i<5;i++){
		User.Element[i]=get_MemberVector(1,user_id)->Element[i];
		Item.Element[i]=get_MemberVector(0,item_id)->Element[i];
	}
	for(int i=0;i<5;i++){
		U_Vectorlist[user_id].Element[i]=(float)(0.7*User.Element[i]+0.3*user_pref*Item.Element[i]);
	}

	return;
}

void Member_Manager::learning_I_Vector(int user_id,int item_id,int item_prior){
	MemberVector User,Item;
	for(int i=0;i<5;i++){
		User.Element[i]=get_MemberVector(1,user_id)->Element[i];
		Item.Element[i]=get_MemberVector(0,item_id)->Element[i];
	}
	for(int i=0;i<5;i++){
		I_Vectorlist[item_id].Element[i]=(float)(0.7*Item.Element[i]+0.3*item_prior*User.Element[i]/5);
	}

	return;
}

void Member_Manager::add_User(int User_id){
	num_of_user++;
	MemberVector* temp;
	temp=new MemberVector[num_of_user];
	for(int i=0;i<num_of_user;i++) temp[i]=U_Vectorlist[i]; 
	*(temp+(num_of_user-1))->Element=*(get_MemberVector(1,num_of_user-1))->Element;

	delete U_Vectorlist;
	U_Vectorlist=temp;

	return;
}

void Member_Manager::add_Item(int Item_id){
	num_of_item++;
	MemberVector* temp;
	temp=new MemberVector[num_of_item];
	for(int i=0;i<num_of_item;i++) temp[i]=I_Vectorlist[i]; 
	*(temp+(num_of_item-1))->Element=*(get_MemberVector(0,num_of_item-1))->Element;

	delete I_Vectorlist;
	I_Vectorlist=temp;

	return;
}