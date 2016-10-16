#pragma once

#pragma comment(lib, "libmysql.dll")

#include "stdafx.h"
#include <Windows.h>

using namespace std;
char* UTF8ToANSI(const char *pszCode)
{
	BSTR    bstrWide;
	char*   pszAnsi;
	int     nLength;
	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, NULL, NULL);
	bstrWide = SysAllocStringLen(NULL, nLength);
	MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide, nLength);
	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	pszAnsi = new char[nLength];
	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString(bstrWide);
	return pszAnsi;
}


int main(void)
{
	int* RecItemList;

	unsigned int count = 0;
	int req_id, req_inputID, req_type = 0;
	double user_locLat, user_locLong, user_locAlt = 0;
	string email_id, password;

	try {

		sql::Driver *driver;
		sql::Connection *con;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://000.000.000.000:3306", "testuser", "3469");
		/* Connect to the MySQL test database */
		con->setSchema("kocca_wellness");

		Rec_Controller rec_controller(con);

		while(1){
			sql::Statement *stmt1 = con->createStatement();
			sql::ResultSet *res1 = stmt1->executeQuery("select * from request_table ORDER BY req_id ASC");
			while(res1->next())
			{
				req_id = res1->getInt(1);
				req_inputID = res1->getInt(2);
				req_type = res1->getInt(3);
				user_locLat = res1->getDouble(4);
				user_locLong = res1->getDouble(5);
				user_locAlt = res1->getDouble(6);
				
				cout << req_inputID << " " << req_type << " " << user_locLat << user_locLong << user_locAlt << endl;

				switch(req_type){
				case 0:{	//추천 요구 일때 작동
					cout << "Request Recommendation!" << endl;
					//작업 요청

					RecItemList=rec_controller.get_Rec_list(req_inputID-1);

					stringstream out;
					string strQuery = "";

					sql::Statement *stmt = con->createStatement();
					sql::ResultSet *res;

					
					int i=0;
					std::cout << endl << endl;
					for(i=0; i<40; i++){		//바꿈
						char *c;
						if(RecItemList[i]==0 || RecItemList[i]==NULL) break;
						strQuery = "select * from wellness_content_table where wc_id = ";
						out << RecItemList[i];
						strQuery += out.str();
						res = stmt->executeQuery(strQuery);
						out.str(std::string());
						while(res->next()){
							std::cout << "=======================================================================================================================" << endl;
							std::cout << "추천 콘텐츠 " << i+1 << endl;
							std::cout << "콘텐츠 ID : " << res->getUInt("wc_id") << endl;
							c = UTF8ToANSI(res->getString("wc_title").c_str());
							std::cout << "이름 : " << c << endl;
							//std::cout << "이름 : " << UTF8ToANSI(res->getString("wc_title").c_str()) << endl;
							std::cout << "콘텐츠 형태 : " << 
								((res->getUInt("wc_type")==0 ? "문화재" : (res->getUInt("wc_type")==1 ? "관광" : "레저"))) << endl;
							c = UTF8ToANSI(res->getString("wc_address").c_str());
							std::cout << "콘텐츠 주소 : " << c << endl;
							//std::cout << "콘텐츠 주소 : " << UTF8ToANSI(res->getString("wc_address").c_str()) << endl;
							std::cout << "콘텐츠 관련 전화번호 : " << res->getString("wc_phonenum") << endl;
							std::cout << "콘텐츠 관련 홈페이지 : " << res->getString("wc_homepage") << endl;
							std::cout << "=======================================================================================================================" << endl;
						}

						if(c != NULL) delete c;
					}
					std::cout << endl << "************* 추천된 콘텐츠 수 ************* : " << i << endl << endl;

					strQuery = "Select * from user_profile_table where up_uid = ";
					out << req_inputID;
					strQuery += out.str();					
					res = stmt->executeQuery(strQuery);
					out.str(std::string());

					while(res->next()){
						std::cout << endl << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						std::cout << "추천 요청한 사용자 번호 : " << req_inputID << endl;
						std::cout << "사용자의 키 : " << res->getDouble(5) << '\t' << "사용자의 몸무게 : " << res->getDouble(6) << endl;
						std::cout << "고혈압   " << (res->getUInt("up_hypertension") ? "있음" : "없음") << '\t' <<
							"당뇨병   " << (res->getUInt("up_diabetes") ? "있음" : "없음") << '\t' <<
							"고지혈증 " << (res->getUInt("up_highcholesterol") ? "있음" : "없음") << endl;
						std::cout << "사용자 5 영역 선호도(신체, 정서, 사회, 지식, 정신) : " << res->getDouble("up_preferPhysical") << ", " <<
							res->getDouble("up_preferEmotion") << ", " << res->getDouble("up_preferSociety") << ", " << 
							res->getDouble("up_preferIntelligence") << ", " << res->getDouble("up_preferSpirit") << endl;
						std::cout << "사용자의 위치(위도) : " << user_locLong << '\t' << 
							"사용자의 위치(경도) : " << user_locLat << '\t' << 
							"사용자의 위치(고도) : " << user_locAlt << endl;
						std::cout << "-----------------------------------------------------------------------------------------------------------------------" << endl << endl << endl << endl;
					}
					//작업 요청 끝 결과 DB에 값을 넘겨줌.

					for(int i=0; i<40; i++){	//20개 -> 40개
						strQuery = "Insert into response_table values (null, ";
						out << req_id;
						strQuery += out.str();
						out.str(std::string());

						strQuery += ", ";
						out << req_inputID;
						strQuery += out.str();
						out.str(std::string());
						strQuery += ", ";

						if(RecItemList[i] > 0) out << RecItemList[i];
						else out << "0";

						strQuery += out.str();
						strQuery += ", ";	
						out.str(std::string());

						out << i+1 << ")";
						strQuery += out.str();
						out.str(std::string());

						stmt->execute(strQuery);
						strQuery = "";
						out.str(std::string());						
					}
					
					cout << "Recommendation operation is finished successfully!!" << endl;
					break;
					   }
				case 1:{	//아이템 추가
					cout << "Item Log Insert!" << endl;
					int type = 0;
					//오준이 작업 요청

					rec_controller.Input_Member(false,req_inputID-1);

					//작업 요청 끝 결과 DB에 값을 넘겨줌.
					break;
					   }
				case 2:{	//사용자 추가
					cout << "User Profile Insert!" << endl;
					int type = 1;
					
					
					//작업 요청
					rec_controller.Input_Member(true,req_inputID-1);

					//작업 요청 끝 결과 DB에 값을 넘겨줌.
					//직접 DB에 사용자를 추가하므로 response table에 결과를 알려줄 필요 없음
					
					
// 					string strQuery = "Insert into response_table values (";
// 					stringstream out;
// 					out << request_id;
// 					strQuery += out.str();
// 					strQuery += ", ";
// 					out.str(std::string());
// 
// 					out << request_type;
// 					strQuery += out.str();
// 					out.str(std::string());
// 					strQuery += ", 'user insert success!')";
// 					stmt->execute(strQuery);
					
					break;
					   }
				case 3:	{	// 사용자 수정 시
					cout << "User profile has been changed!" << endl;
					
					rec_controller.modify_member(true, req_inputID-1);

					break;
					}
				case 4:{	//이용내역 추가(Rating)
					//for(int i=0; i<30; i++)		
					cout << "Usage Log Insert!" << endl;
					//작업 요청

					rec_controller.Input_Rating(req_inputID-1);

					//작업 요청 끝 결과 DB에 값을 넘겨줌.
					//usage_log_table에 직접 로그를 저장하므로 밑의 코드는 불필요.

// 					string strQuery = "Insert into response_table values (";	
// 					stringstream out;
// 					out << request_id;
// 					strQuery += out.str();
// 					strQuery += ", ";
// 					out.str(std::string());
// 
// 					out << request_type;
// 					strQuery += out.str();
// 					out.str(std::string());
// 					strQuery += ", 'usage log insert success!')";
// 					stmt->execute(strQuery);
// 					
					break;
					   }
				default:{
					break;
						}
				}
				
				//작업 끝난뒤 트리거 DB의 해당 레코드 삭제
				stringstream out;
				out << req_id;
				string strQuery = "delete from request_table where req_id = ";
				strQuery += out.str();
				out.str(std::string());								
				stmt1->execute(strQuery);
			}

			//정기작업(클러스터링 작업)
			struct tm *today;
			time_t ltime;

			time(&ltime);
			today = localtime(&ltime);

			if(today->tm_hour == 3 && today->tm_min <= 10){
				//작업 요청

				rec_controller.Time_Interrupt();

				//~~~~~~~~~~~~~~~
			}
			Sleep(1000);
			//cout << count++ << endl;

			delete res1;
			delete stmt1;			
		}

		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	cout << endl;

	return EXIT_SUCCESS;
}
