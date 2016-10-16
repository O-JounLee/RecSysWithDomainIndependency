#include "structs.h"
#include "stdafx.h"
#include "mysql_connection.h"

void encryptor(sql::Connection *con){
	sql::Statement *stmt = con->createStatement();

}

int get_Item_num(sql::Connection *con){
	sql::Statement *stmt = con->createStatement();
	sql::ResultSet *res = stmt->executeQuery("select * from wellness_content_table");
	int x = res->rowsCount();
	delete res;
	delete stmt;
	return x;
}

int get_User_num(sql::Connection *con){
	sql::Statement *stmt = con->createStatement();
	sql::ResultSet *res = stmt->executeQuery("select * from user_profile_table");
	int immm = res->rowsCount();
	delete res;
	delete stmt;
	return immm;
}

int get_Log_num(sql::Connection *con){
	sql::Statement *stmt = con->createStatement();
	sql::ResultSet *res = stmt->executeQuery("select * from usage_log_table");
	int x = res->rowsCount();
	delete res;
	delete stmt;
	return x;
}

MemberVector* get_MemberVector(sql::Connection *con, bool type, int id, MemberVector* mv){ // type 1 : user, 0 : content	
	int use_id = ++id;
	string strQuery = "";
	sql::ResultSet *res = NULL;
	sql::Statement *stmt = con->createStatement();
	stringstream out;
	out << use_id;	

	if(id == NULL){
		delete stmt;
		delete res;
		return mv;
	}
	else if(type == 0){
		strQuery = "select wc_scorePhysical, wc_scoreEmotion, wc_scoreSociety, wc_scoreIntelligence, wc_scoreSpirit from wellness_content_table where wc_id = ";
		strQuery += out.str();
		res = stmt->executeQuery(strQuery);
	}
	else if(type == 1){
		strQuery = "select up_preferPhysical, up_preferEmotion, up_preferSociety, up_preferIntelligence, up_preferSpirit from user_profile_table where up_uid = ";
		strQuery += out.str();
		res = stmt->executeQuery(strQuery);
	}
	while(res->next()){
		for(int i = 1; i < 6; i++){
			mv->Element[i-1] = res->getDouble(i);
		}
	}
	delete stmt;
	delete res;
	return mv;
}

UserProfile* get_UserProfile(sql::Connection *con, int id, UserProfile* up){
	int use_id = ++id;
	string strQuery = "";
	sql::ResultSet *res = NULL;
	sql::Statement *stmt = con->createStatement();
	stringstream out;
	out << use_id;
	struct tm *today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	if(id > 0){
		strQuery = "select up_birthdate, up_hypertension, up_diabetes, up_highcholesterol from user_profile_table where up_uid = ";
		strQuery += out.str();
		res = stmt->executeQuery(strQuery);

		while(res->next()){
			up->age = (today->tm_year + 1900) - (res->getInt(1)) + 1;		
			up->hypertension = res->getInt(2);
			up->diabetesmellitus = res->getInt(3);
			up->hyperlipidemia = res->getInt(4);
		}
	}
	else{
		delete stmt;
		delete res;
		return up;
	}

	delete stmt;
	delete res;

	return up;
}

ItemProperty*  get_ItemProperty(sql::Connection *con, int Item_id, ItemProperty* ip){
	int it_id = ++Item_id;
	string strQuery = "";
	sql::ResultSet *res = NULL;
	sql::Statement *stmt = con->createStatement();
	stringstream out;
	out << it_id;

	if(it_id > 0){
		strQuery = "select wc_hypertension_1, wc_hypertension_2, wc_diabetes_1, wc_diabetes_2, wc_highcholesterol_1, wc_highcholesterol_2 from wellness_content_table where wc_id = ";
		strQuery += out.str();
		res = stmt->executeQuery(strQuery);

		while(res->next()){
			ip->to_hypertension.to_under40s = res->getInt(1);
			ip->to_hypertension.to_40s = res->getInt(2);
			ip->to_hypertension.to50s = res->getInt(2);
			ip->to_hypertension.to_upto60s = res->getInt(2);
			ip->to_diabetesmellitus.to_under40s = res->getInt(3);
			ip->to_diabetesmellitus.to_40s = res->getInt(4);
			ip->to_diabetesmellitus.to50s = res->getInt(4);
			ip->to_diabetesmellitus.to_upto60s = res->getInt(4);
			ip->to_hyperlipidemia.to_under40s = res->getInt(5);
			ip->to_hyperlipidemia.to_40s = res->getInt(6);
			ip->to_hyperlipidemia.to50s = res->getInt(6);
			ip->to_hyperlipidemia.to_upto60s = res->getInt(6);
		}
	}
	else{
		delete stmt;
		delete res;
		return ip;
	}
	delete stmt;
	delete res;
	return ip;
}


Rating_Point* get_RatingPoint(sql::Connection *con, int log_num, Rating_Point* rp){
	int log_id = ++log_num;
	string strQuery = "";
	sql::ResultSet *res = NULL;
	sql::Statement *stmt = con->createStatement();
	stringstream out;
	out << log_id;

	if(log_id > 0){
		strQuery = "select ul_uid, ul_cid, ul_Satisfication, ul_Usefulness from usage_log_table where ul_lid = ";
		strQuery += out.str();
		res = stmt->executeQuery(strQuery);

		while(res->next()){
			rp->user_id = res->getInt(1)-1;
			rp->item_id = res->getInt(2)-1;
			rp->user_pref = (res->getDouble(3) * 2);
			rp->item_prior = (res->getDouble(4) * 2);
		}
	}
	else{
		delete stmt;
		delete res;
		return rp;
	}
	delete stmt;
	delete res;
	return rp;
}