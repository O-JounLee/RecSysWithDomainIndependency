// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include "structs.h"
#include "Queue.h"
#include "Inter_Interface.h"
#include "Intra_Interface.h"
#include "Member_Manager.h"
#include "Rating_Manager.h"
#include "Clustering_Manager.h"
#include "Recommendation_Controller.h"
#include "Recommendation_Manager.h"
#include "Recommendation_Model_Manager.h"
#include "Recommender.h"
#include "Rule_Apllier.h"

#include <stdio.h>
#include <tchar.h>



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <sstream>
#include <ctime>