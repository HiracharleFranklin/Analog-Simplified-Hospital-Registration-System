#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>
#include <cstdlib>
#include "person.h"
#include "fifo.h"
#include "fifo.cpp"
#include "fibheap.h"
#include "report.h"
#include "alist.cpp"
#include "alist.h"
#include "BPlus_tree.h"
#include "BPlus_tree.cpp"
#include "BPlus_node.h"
#include "BPlus_node.cpp"
#include "hashset.cpp"
#include "B_Tree.h"


using namespace std;

//Functions declared for read in the file
string *split(string i);                                                        //split the long string into different elements
string *get_birth(string i);                                                    //read the date of birth into a list with three element
person *find_person(FibHeap<long long> &Fib, person ***hospital, long long id); // find a person by id
person *find_person(FibHeap<long long> &Fib, person ***hospital, AList<person *> middle_risk_archive, AList<person *> withdraw_reinsertion_archive, long long id); // find a person by id in the whole system
person *find_person(FibHeap<long long> &Fib, person ***hospital, AList<person *> archive, AList<person *> middle_risk_archive, AList<person *> withdraw_reinsertion_archive, long long id);
person *Fib_find_person(FibHeap<long long> &Fib, long long id);                // find a person by id in FibHeap
person *Hospital_find_person(person ***hospital, long long id);                // find a person by id in hospital
person *archive_find_person(AList<person *> archive, long long id);            // find a person by id in archive
bool whether_in_archive(AList<person *> &archive, long long id);             // check whether a person is in the archive by id
bool removePatient(FibHeap<long long> &Fib, person ***hospital, long long id); // remove a person by id from either heap or hospital
bool removePatient(FibHeap<long long> &Fib, person ***hospital, AList<person *> &risk_2_archive, AList<person *> &withdraw_reinsertion_archive, long long id); // remove a person by id from either heap or hospital
bool removePatient(FibHeap<long long> &Fib, person ***hospital, AList<person *> &archive, AList<person *> &risk_2_archive, AList<person *> &withdraw_reinsertion_archive, long long id); // remove a person by id from either heap or hospital
bool removeFromHeap(FibHeap<long long> &Fib, long long id);                    // remove a person by id from heap
bool removeFromHospital(person ***hospital, long long id);                     // remove a person by id from hospital
void archive_remove_by_id(AList<person *> &archive, long long id);              // remove a person by id in an archive
void print_head();                                                             // print head information
int hospital_num(person ***hospital);                                          // return the number of people in the hospital

// main function
int main()
{
    /*****************************************************************/
    //                           Preparation                         //
    /*****************************************************************/

    print_head();

    // Function variables
    int withdraw_mod = 0;

    // Time variables
    int half_day = 0; //Initialize the timeline recorder half_day
    int week = 1;     //Initialize the timeline recorder week
    int month = 1;    //Initialize the timeline recorder week
    // number markers for reporting
    int reginum = 0;       //Initialize the number of registers
    int waitnum = 0;       //Initialize the number of waitings
    int waitnum_total = 0; //Initialize the number of waitings in total
    int appointnum = 0;    //Initialize the number of appointments
    int withdrawnum = 0;   //Initialize the number of withdraws
    int averagetime = 0;   //Initialize the average waiting time
    // statistics
    const int registry_capacity = 500; // the capacity of each registration
    fifo<person> registry[5];          //list of fifo to represent registries
    fifo<person> risk_2;
    FibHeap<long long> Fib; //The centrial fibonacci heap

    // Start with an interactive user interface

    //check for execution operation or quiting operation
    char command;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
    cout << "Press c to continue, and q to quit: ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

    cin >> command;

    //If quit, press q and stop
    if (command == 'q')
    {
        //print out quiting information
        cout << "Quit" << endl;
        cout << "Logging out";
        for (auto _ = 0; _ < 6; _++)
        {
            Sleep(500);
            cout << ".";
        }
        cout << endl;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);

        cout << endl;
        cout << endl;
        cout << endl;
        Sleep(200);
        cout<< "   _______  _                    _              ______               _    _       _                  _ \n";
        Sleep(200);
        cout<< "  |__   __|| |                  | |            |  ____|             | |  | |     (_)                | |\n";
        Sleep(200);
        cout<< "     | |   | |__    __ _  _ __  | | __ ___     | |__  ___   _ __    | |  | | ___  _  _ __    __ _   | |\n";
        Sleep(200);
        cout<< "     | |   | '_ \\  / _` || '_ \\ | |/ // __|    |  __|/ _ \\ | '__|   | |  | |/ __|| || '_ \\  / _` |  | |\n";
        Sleep(200);
        cout<< "     | |   | | | || (_| || | | ||   < \\__ \\    | |  | (_) || |      | |__| |\\__ \\| || | | || (_| |  |_|\n";
        Sleep(200);
        cout<< "     |_|   |_| |_| \\__,_||_| |_||_|\\_\\|___/    |_|   \\___/ |_|       \\____/ |___/|_||_| |_| \\__, |  (_)\n";
        Sleep(200);
        cout<< "                                                                                             __/ |     \n";
        Sleep(200);
        cout<< "                                                                                            |___/     \n";
        Sleep(200);
        cout<< endl;
        cout<< endl;
        cout<< endl;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        return 0;
    }
    if (command != 'q' && command != 'c')
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
        cout << "Wrong Command, please type again: ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

        cin >> command;
        while (command != 'q' && command != 'c')
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
            cout << "Wrong Command, please type again: ";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

            cin >> command;
        }
    }
    //If execution, press p and cont'd
    else if (command == 'c')
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
        cout << "Type 1 to use the functions of withdraw, information update, re-registry a patient or submit a deadline letter.\nOr 0 to shut down these mods and check the basic functions of this system.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
        cout << "We recommand to type 0 for the first time when using this system.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

        cin >> withdraw_mod;
        while (withdraw_mod < 0 || withdraw_mod > 1)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
            cout << "Please type in 0 or 1:" << endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            cin >> withdraw_mod;
        }
        long long startyear, startmonth, startday;
        //The user should type in the start year,month abd day
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
        cout << "Please type in a year after 2000 and before 2500 as the start year:" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

        cin >> startyear;
        while (startyear < 2000 || startyear >= 2500)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
            cout << "Please type in a year after 2000 and before 2500 again:" << endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

            cin >> startyear;
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
        cout << "Please type in a month as the start month:" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

        cin >> startmonth;
        while (startmonth < 1 || startmonth >= 13)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
            cout << "Please type in the month again:" << endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

            cin >> startmonth;
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
        cout << "Please type in a day less than 31 as the start day:" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

        cin >> startday;
        while (startday < 1 || startday >= 31)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
            cout << "Please type in a day between 1 and 30:" << endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

            cin >> startday;
        }
        //

        /*****************************************************************/
        //                          step1. input                         //
        /*****************************************************************/

        //

        // read the 5 files of registries and put the five people into the 5 corresponding queues

        // These are  registries that
        person registry_1[registry_capacity];
        person registry_2[registry_capacity];
        person registry_3[registry_capacity];
        person registry_4[registry_capacity];
        person registry_5[registry_capacity];

        // 还要构建三个医院，用指针数组表示。每个医院有有8个时间段，每个时间段只能1个人就诊。
        person **hospital[3];
        for (int i = 0; i < 3; i++)
        {
            hospital[i] = new person *[8];
            for (int j = 0; j < 8; j++)
                hospital[i][j] = nullptr;
        }

        AList<person *> *temp = new AList<person *>;

        if (temp == NULL)
            cout << "Cannot allocate memory for archive.\n";

        AList<person *> archive = *temp;

        AList<person *> *temp2 = new AList<person *>;

        if (temp2 == NULL)
            cout << "Cannot allocate memory for withdraw_archive.\n";

        AList<person *> withdraw_archive = *temp2;

        AList<person *> *temp3 = new AList<person *>;

        if (temp3 == NULL)

            cout << "Cannot allocate memory for middle_risk_archive.\n";

        AList<person *> middle_risk_archive = *temp3;

        AList<person *> *temp4 = new AList<person *>;

        if (temp4 == NULL)

            cout << "Cannot allocate memory for middle_risk_archive.\n";

        AList<person *> withdraw_reinsertion_archive = *temp4;


        //CA2 B+ tree, B tree and hash table construct

        //B+ tree construction
        CBPlusTree<int, PERSON> person_tree;
        CBPlusTree<int, MEDICAL_STATUS> medical_tree;
        CBPlusTree<int, REGISTRATION> registration_tree;
        CBPlusTree<int, TREATMENT> treatment_tree;

        //B tree construction
        B_Tree<Key_tuple> B_search_tree;
        hashset<MEDICAL_STATUS> hash_of_risk;

        // 一个变量
        int loop_amount = 0;

        // 每个登记处有一个专属文件，文件里是病人们的信息
        ifstream myfile1;
        ifstream myfile2;
        ifstream myfile3;
        ifstream myfile4;
        ifstream myfile5;

        // 打开五个登记处的文件
        myfile1.open("registry_1.csv");
        myfile2.open("registry_2.csv");
        myfile3.open("registry_3.csv");
        myfile4.open("registry_4.csv");
        myfile5.open("registry_5.csv");
        if (!myfile1.is_open())
        {
            cout << "The file registry_1.csv can not be opened" << endl;
            return 0;
        }
        if (!myfile2.is_open())
        {
            cout << "The file registry_2.csv can not be opened" << endl;
            return 0;
        }
        if (!myfile3.is_open())
        {
            cout << "The file registry_3.csv can not be opened" << endl;
            return 0;
        }
        if (!myfile4.is_open())
        {
            cout << "The file registry_4.csv can not be opened" << endl;
            return 0;
        }
        if (!myfile5.is_open())
        {
            cout << "The file registry_5.csv can not be opened" << endl;
            return 0;
        }

        for (int k = 0; k < 500; k++) // k代表第几个半天
        {
            // 每一个半天......
            half_day++;
            // STEP ONE!
            // 提取文件数据
            if ((archive.getlength() + withdraw_archive.getlength() + middle_risk_archive.getlength()) >= 2500) //|| (month == 3 && week >=14))
            {
                cout << "All patients have been treated." << endl;
                myfile1.close();
                myfile2.close();
                myfile3.close();
                myfile4.close();
                myfile5.close();
                cout << "Logging out";
                for (auto _ = 0; _ < 6; _++)
                {
                    Sleep(500);
                    cout << ".";
                }
                cout << endl;

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);

                    cout << endl;
                    cout << endl;
                    cout << endl;
                    Sleep(200);
                    cout<< "   _______  _                    _              ______               _    _       _                  _ \n";
                    Sleep(200);
                    cout<< "  |__   __|| |                  | |            |  ____|             | |  | |     (_)                | |\n";
                    Sleep(200);
                    cout<< "     | |   | |__    __ _  _ __  | | __ ___     | |__  ___   _ __    | |  | | ___  _  _ __    __ _   | |\n";
                    Sleep(200);
                    cout<< "     | |   | '_ \\  / _` || '_ \\ | |/ // __|    |  __|/ _ \\ | '__|   | |  | |/ __|| || '_ \\  / _` |  | |\n";
                    Sleep(200);
                    cout<< "     | |   | | | || (_| || | | ||   < \\__ \\    | |  | (_) || |      | |__| |\\__ \\| || | | || (_| |  |_|\n";
                    Sleep(200);
                    cout<< "     |_|   |_| |_| \\__,_||_| |_||_|\\_\\|___/    |_|   \\___/ |_|       \\____/ |___/|_||_| |_| \\__, |  (_)\n";
                    Sleep(200);
                    cout<< "                                                                                             __/ |     \n";
                    Sleep(200);
                    cout<< "                                                                                            |___/     \n";
                    Sleep(200);
                    cout<< endl;
                    cout<< endl;
                    cout<< endl;

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

                return 1;
            }





            for (int j = 0; j < 5; j++) // j代表第几个人
            {
                loop_amount += 1;
                if (5 * k + j >= registry_capacity || (archive.getlength() + withdraw_archive.getlength())>=2500)
                {
                    break;
                }
                //这部分把五个个体录入registry_n中
                string input1;
                myfile1 >> input1;
                string *current_line1 = split(input1);
                if (atoll(current_line1[8].c_str()) == 2)
                {
                    person *risk2store = new person;
                    if (risk2store == NULL)
                    {
                        cout << "Can not allocate memory\n";
                        return 0;
                    }
                    risk2store->id = atoll(current_line1[0].c_str());
                    risk2store->name = current_line1[1];
                    risk2store->CONTACT_DETAIL[0] = current_line1[2];
                    risk2store->CONTACT_DETAIL[1] = current_line1[3];
                    risk2store->CONTACT_DETAIL[2] = current_line1[4];
                    risk2store->CONTACT_DETAIL[3] = current_line1[5];
                    risk2store->profession = atoll(current_line1[6].c_str());
                    risk2store->birth[1] = atoll((get_birth(current_line1[7])[0].c_str())); //month
                    risk2store->birth[2] = atoll((get_birth(current_line1[7])[1].c_str())); //day
                    risk2store->birth[0] = atoll((get_birth(current_line1[7])[2].c_str())); //year
                    risk2store->risk = atoll(current_line1[8].c_str());
                    risk2store->registration = 1;
                    risk2store->hospital = atoi(current_line1[2].c_str());
                    risk2store->calculate_registertime(startyear, startmonth, startday, 0, half_day);
                    risk2store->mark_regist_halfday(half_day);
                    risk2store->withdraw = 0;
                    risk2store->ddlletter = 0;
                    risk2store->risk2halfday = half_day;
                    
                    //New added part for CA2
                    risk2store->Euclidean_address[0] = atoi(current_line1[10].c_str());
                    risk2store->Euclidean_address[1] = atoi(current_line1[11].c_str());
                    risk2store->health_care_card = atoi(current_line1[12].c_str());
                    risk2store->treatment = atoll(current_line1[13].c_str());

                    risk2store->calculate_key();
                    middle_risk_archive.append(risk2store);
                    registry_1[5 * k + j].id = -1;
                }
                else
                {
                    registry_1[5 * k + j].id = atoll(current_line1[0].c_str());
                    registry_1[5 * k + j].name = current_line1[1];
                    registry_1[5 * k + j].CONTACT_DETAIL[0] = current_line1[2];
                    registry_1[5 * k + j].CONTACT_DETAIL[1] = current_line1[3];
                    registry_1[5 * k + j].CONTACT_DETAIL[2] = current_line1[4];
                    registry_1[5 * k + j].CONTACT_DETAIL[3] = current_line1[5];
                    registry_1[5 * k + j].profession = atoll(current_line1[6].c_str());
                    registry_1[5 * k + j].birth[1] = atoll((get_birth(current_line1[7])[0].c_str())); //month
                    registry_1[5 * k + j].birth[2] = atoll((get_birth(current_line1[7])[1].c_str())); //day
                    registry_1[5 * k + j].birth[0] = atoll((get_birth(current_line1[7])[2].c_str())); //year
                    registry_1[5 * k + j].risk = atoll(current_line1[8].c_str());
                    registry_1[5 * k + j].registration = 1;
                    registry_1[5 * k + j].hospital = atoi(current_line1[2].c_str());
                    registry_1[5 * k + j].calculate_registertime(startyear, startmonth, startday, 0, half_day);
                    registry_1[5 * k + j].mark_regist_halfday(half_day);
                    registry_1[5 * k + j].withdraw = 0;
                    registry_1[5 * k + j].ddlletter = 0;
                    //New added part for CA2
                    registry_1[5 * k + j].Euclidean_address[0] = atoi(current_line1[10].c_str());
                    registry_1[5 * k + j].Euclidean_address[1] = atoi(current_line1[11].c_str());
                    registry_1[5 * k + j].health_care_card = atoi(current_line1[12].c_str());
                    registry_1[5 * k + j].treatment = atoll(current_line1[13].c_str());

                    registry_1[5 * k + j].calculate_key();
                    
                }

                string input2;
                myfile2 >> input2;
                string *current_line2 = split(input2);
                if (atoll(current_line2[8].c_str()) == 2)
                {
                    person *risk2store = new person;
                    if (risk2store == NULL)
                    {
                        cout << "Can not allocate memory\n";
                        return 0;
                    }
                    risk2store->id = atoll(current_line2[0].c_str());
                    risk2store->name = current_line2[1];
                    risk2store->CONTACT_DETAIL[0] = current_line2[2];
                    risk2store->CONTACT_DETAIL[1] = current_line2[3];
                    risk2store->CONTACT_DETAIL[2] = current_line2[4];
                    risk2store->CONTACT_DETAIL[3] = current_line2[5];
                    risk2store->profession = atoll(current_line2[6].c_str());
                    risk2store->birth[1] = atoll((get_birth(current_line2[7])[0].c_str())); //month
                    risk2store->birth[2] = atoll((get_birth(current_line2[7])[1].c_str())); //day
                    risk2store->birth[0] = atoll((get_birth(current_line2[7])[2].c_str())); //year
                    risk2store->risk = atoll(current_line2[8].c_str());
                    risk2store->registration = 1;
                    risk2store->hospital = atoi(current_line2[2].c_str());
                    risk2store->calculate_registertime(startyear, startmonth, startday, 0, half_day);
                    risk2store->mark_regist_halfday(half_day);
                    risk2store->withdraw = 0;
                    risk2store->ddlletter = 0;
                    risk2store->risk2halfday = half_day;
                    
                    //New added part for CA2
                    risk2store->Euclidean_address[0] = atoi(current_line2[9].c_str());
                    risk2store->Euclidean_address[1] = atoi(current_line2[10].c_str());
                    risk2store->health_care_card = atoi(current_line2[11].c_str());
                    risk2store->treatment = atoll(current_line2[12].c_str());

                    risk2store->calculate_key();
                    middle_risk_archive.append(risk2store);
                    registry_2[5 * k + j].id = -1;
                }
                else
                {
                    person *risk2store = new person;
                    if (risk2store == NULL)
                    {
                        cout << "Can not allocate memory\n";
                        return 0;
                    }
                    registry_2[5 * k + j].id = atoll(current_line2[0].c_str());
                    registry_2[5 * k + j].name = current_line2[1];
                    registry_2[5 * k + j].CONTACT_DETAIL[0] = current_line2[2];
                    registry_2[5 * k + j].CONTACT_DETAIL[1] = current_line2[3];
                    registry_2[5 * k + j].CONTACT_DETAIL[2] = current_line2[4];
                    registry_2[5 * k + j].CONTACT_DETAIL[3] = current_line2[5];
                    registry_2[5 * k + j].profession = atoll(current_line2[6].c_str());
                    registry_2[5 * k + j].birth[1] = atoll((get_birth(current_line2[7])[0].c_str())); //month
                    registry_2[5 * k + j].birth[2] = atoll((get_birth(current_line2[7])[1].c_str())); //day
                    registry_2[5 * k + j].birth[0] = atoll((get_birth(current_line2[7])[2].c_str())); //year
                    registry_2[5 * k + j].risk = atoll(current_line2[8].c_str());
                    registry_2[5 * k + j].registration = 2;
                    registry_2[5 * k + j].hospital = atoi(current_line2[2].c_str());
                    registry_2[5 * k + j].calculate_registertime(startyear, startmonth, startday, 0, half_day);
                    registry_2[5 * k + j].mark_regist_halfday(half_day);
                    registry_2[5 * k + j].withdraw = 0;
                    registry_2[5 * k + j].ddlletter = 0;
                    //New added part for CA2
                    registry_2[5 * k + j].Euclidean_address[0] = atoi(current_line2[10].c_str());
                    registry_2[5 * k + j].Euclidean_address[1] = atoi(current_line2[11].c_str());
                    registry_2[5 * k + j].health_care_card = atoi(current_line2[12].c_str());
                    registry_2[5 * k + j].treatment = atoll(current_line2[13].c_str());

                    registry_2[5 * k + j].calculate_key();
                }

                string input3;
                myfile3 >> input3;
                string *current_line3 = split(input3);
                if (atoll(current_line3[8].c_str()) == 2)
                {
                    person *risk2store = new person;
                    if (risk2store == NULL)
                    {
                        cout << "Can not allocate memory\n";
                        return 0;
                    }
                    risk2store->id = atoll(current_line3[0].c_str());
                    risk2store->name = current_line3[1];
                    risk2store->CONTACT_DETAIL[0] = current_line3[2];
                    risk2store->CONTACT_DETAIL[1] = current_line3[3];
                    risk2store->CONTACT_DETAIL[2] = current_line3[4];
                    risk2store->CONTACT_DETAIL[3] = current_line3[5];
                    risk2store->profession = atoll(current_line3[6].c_str());
                    risk2store->birth[1] = atoll((get_birth(current_line3[7])[0].c_str())); //month
                    risk2store->birth[2] = atoll((get_birth(current_line3[7])[1].c_str())); //day
                    risk2store->birth[0] = atoll((get_birth(current_line3[7])[2].c_str())); //year
                    risk2store->risk = atoll(current_line3[8].c_str());
                    risk2store->registration = 1;
                    risk2store->hospital = atoi(current_line3[2].c_str());
                    risk2store->calculate_registertime(startyear, startmonth, startday, 0, half_day);
                    risk2store->mark_regist_halfday(half_day);
                    risk2store->withdraw = 0;
                    risk2store->ddlletter = 0;
                    risk2store->risk2halfday = half_day;
                    
                    //New added part for CA2
                    risk2store->Euclidean_address[0] = atoi(current_line3[10].c_str());
                    risk2store->Euclidean_address[1] = atoi(current_line3[11].c_str());
                    risk2store->health_care_card = atoi(current_line3[12].c_str());
                    risk2store->treatment = atoll(current_line3[13].c_str());

                    risk2store->calculate_key();
                    middle_risk_archive.append(risk2store);
                    registry_3[5 * k + j].id = -1;
                }
                else
                {
                    registry_3[5 * k + j].id = atoll(current_line3[0].c_str());
                    registry_3[5 * k + j].name = current_line3[1];
                    registry_3[5 * k + j].CONTACT_DETAIL[0] = current_line3[2];
                    registry_3[5 * k + j].CONTACT_DETAIL[1] = current_line3[3];
                    registry_3[5 * k + j].CONTACT_DETAIL[2] = current_line3[4];
                    registry_3[5 * k + j].CONTACT_DETAIL[3] = current_line3[5];
                    registry_3[5 * k + j].profession = atoll(current_line3[6].c_str());
                    registry_3[5 * k + j].birth[1] = atoll((get_birth(current_line3[7])[0].c_str())); //month
                    registry_3[5 * k + j].birth[2] = atoll((get_birth(current_line3[7])[1].c_str())); //day
                    registry_3[5 * k + j].birth[0] = atoll((get_birth(current_line3[7])[2].c_str())); //year
                    registry_3[5 * k + j].risk = atoll(current_line3[8].c_str());
                    registry_3[5 * k + j].registration = 3;
                    registry_3[5 * k + j].hospital = atoi(current_line3[2].c_str());
                    registry_3[5 * k + j].calculate_registertime(startyear, startmonth, startday, 0, half_day);
                    registry_3[5 * k + j].mark_regist_halfday(half_day);
                    registry_3[5 * k + j].withdraw = 0;
                    registry_3[5 * k + j].ddlletter = 0;
                    
                    //New added part for CA2
                    registry_3[5 * k + j].Euclidean_address[0] = atoi(current_line3[10].c_str());
                    registry_3[5 * k + j].Euclidean_address[1] = atoi(current_line3[11].c_str());
                    registry_3[5 * k + j].health_care_card = atoi(current_line3[12].c_str());
                    registry_3[5 * k + j].treatment = atoll(current_line3[13].c_str());

                    registry_3[5 * k + j].calculate_key();
                }

                string input4;
                myfile4 >> input4;
                string *current_line4 = split(input4);
                if (atoll(current_line4[8].c_str()) == 2)
                {
                    person *risk2store = new person;
                    if (risk2store == NULL)
                    {
                        cout << "Can not allocate memory\n";
                        return 0;
                    }
                    risk2store->id = atoll(current_line4[0].c_str());
                    risk2store->name = current_line4[1];
                    risk2store->CONTACT_DETAIL[0] = current_line4[2];
                    risk2store->CONTACT_DETAIL[1] = current_line4[3];
                    risk2store->CONTACT_DETAIL[2] = current_line4[4];
                    risk2store->CONTACT_DETAIL[3] = current_line4[5];
                    risk2store->profession = atoll(current_line4[6].c_str());
                    risk2store->birth[1] = atoll((get_birth(current_line4[7])[0].c_str())); //month
                    risk2store->birth[2] = atoll((get_birth(current_line4[7])[1].c_str())); //day
                    risk2store->birth[0] = atoll((get_birth(current_line4[7])[2].c_str())); //year
                    risk2store->risk = atoll(current_line4[8].c_str());
                    risk2store->registration = 1;
                    risk2store->hospital = atoi(current_line4[2].c_str());
                    risk2store->calculate_registertime(startyear, startmonth, startday, 0, half_day);
                    risk2store->mark_regist_halfday(half_day);
                    risk2store->withdraw = 0;
                    risk2store->ddlletter = 0;
                    risk2store->risk2halfday = half_day;
                    
                    //New added part for CA2
                    risk2store->Euclidean_address[0] = atoi(current_line4[10].c_str());
                    risk2store->Euclidean_address[1] = atoi(current_line4[11].c_str());
                    risk2store->health_care_card = atoi(current_line4[12].c_str());
                    risk2store->treatment = atoll(current_line4[13].c_str());

                    risk2store->calculate_key();
                    middle_risk_archive.append(risk2store);
                    registry_4[5 * k + j].id = -1;
                }
                else
                {
                    registry_4[5 * k + j].id = atoll(current_line4[0].c_str());
                    registry_4[5 * k + j].name = current_line4[1];
                    registry_4[5 * k + j].CONTACT_DETAIL[0] = current_line4[2];
                    registry_4[5 * k + j].CONTACT_DETAIL[1] = current_line4[3];
                    registry_4[5 * k + j].CONTACT_DETAIL[2] = current_line4[4];
                    registry_4[5 * k + j].CONTACT_DETAIL[3] = current_line4[5];
                    registry_4[5 * k + j].profession = atoll(current_line4[6].c_str());
                    registry_4[5 * k + j].birth[1] = atoll((get_birth(current_line4[7])[0].c_str())); //month
                    registry_4[5 * k + j].birth[2] = atoll((get_birth(current_line4[7])[1].c_str())); //day
                    registry_4[5 * k + j].birth[0] = atoll((get_birth(current_line4[7])[2].c_str())); //year
                    registry_4[5 * k + j].risk = atoll(current_line4[8].c_str());
                    registry_4[5 * k + j].registration = 4;
                    registry_4[5 * k + j].hospital = atoi(current_line4[2].c_str());
                    registry_4[5 * k + j].calculate_registertime(startyear, startmonth, startday, 0, half_day);
                    registry_4[5 * k + j].mark_regist_halfday(half_day);
                    registry_4[5 * k + j].withdraw = 0;
                    registry_4[5 * k + j].ddlletter = 0;
                    //New added part for CA2
                    registry_4[5 * k + j].Euclidean_address[0] = atoi(current_line4[10].c_str());
                    registry_4[5 * k + j].Euclidean_address[1] = atoi(current_line4[11].c_str());
                    registry_4[5 * k + j].health_care_card = atoi(current_line4[12].c_str());
                    registry_4[5 * k + j].treatment = atoll(current_line4[13].c_str());

                    registry_4[5 * k + j].calculate_key();
                }

                string input5;
                myfile5 >> input5;
                string *current_line5 = split(input5);
                if (atoll(current_line5[8].c_str()) == 2)
                {
                    person *risk2store = new person;
                    if (risk2store == NULL)
                    {
                        cout << "Can not allocate memory\n";
                        return 0;
                    }
                    risk2store->id = atoll(current_line5[0].c_str());
                    risk2store->name = current_line5[1];
                    risk2store->CONTACT_DETAIL[0] = current_line5[2];
                    risk2store->CONTACT_DETAIL[1] = current_line5[3];
                    risk2store->CONTACT_DETAIL[2] = current_line5[4];
                    risk2store->CONTACT_DETAIL[3] = current_line5[5];
                    risk2store->profession = atoll(current_line5[6].c_str());
                    risk2store->birth[1] = atoll((get_birth(current_line5[7])[0].c_str())); //month
                    risk2store->birth[2] = atoll((get_birth(current_line5[7])[1].c_str())); //day
                    risk2store->birth[0] = atoll((get_birth(current_line5[7])[2].c_str())); //year
                    risk2store->risk = atoll(current_line5[8].c_str());
                    risk2store->registration = 1;
                    risk2store->hospital = atoi(current_line5[2].c_str());
                    risk2store->calculate_registertime(startyear, startmonth, startday, 0, half_day);
                    risk2store->mark_regist_halfday(half_day);
                    risk2store->withdraw = 0;
                    risk2store->ddlletter = 0;
                    risk2store->risk2halfday = half_day;
                    
                    //New added part for CA2
                    risk2store->Euclidean_address[0] = atoi(current_line5[10].c_str());
                    risk2store->Euclidean_address[1] = atoi(current_line5[11].c_str());
                    risk2store->health_care_card = atoi(current_line5[12].c_str());
                    risk2store->treatment = atoll(current_line5[13].c_str());

                    risk2store->calculate_key();
                    middle_risk_archive.append(risk2store);
                    registry_5[5 * k + j].id = -1;
                }
                else
                {
                    registry_5[5 * k + j].id = atoll(current_line5[0].c_str());
                    registry_5[5 * k + j].name = current_line5[1];
                    registry_5[5 * k + j].CONTACT_DETAIL[0] = current_line5[2];
                    registry_5[5 * k + j].CONTACT_DETAIL[1] = current_line5[3];
                    registry_5[5 * k + j].CONTACT_DETAIL[2] = current_line5[4];
                    registry_5[5 * k + j].CONTACT_DETAIL[3] = current_line5[5];
                    registry_5[5 * k + j].profession = atoi(current_line5[6].c_str());
                    registry_5[5 * k + j].birth[1] = atoll((get_birth(current_line5[7])[0].c_str())); //month
                    registry_5[5 * k + j].birth[2] = atoll((get_birth(current_line5[7])[1].c_str())); //day
                    registry_5[5 * k + j].birth[0] = atoll((get_birth(current_line5[7])[2].c_str())); //year
                    registry_5[5 * k + j].risk = atoll(current_line5[8].c_str());
                    registry_5[5 * k + j].registration = 5;
                    registry_5[5 * k + j].hospital = atoi(current_line5[2].c_str());
                    registry_5[5 * k + j].calculate_registertime(startyear, startmonth, startday, 0, half_day);
                    registry_5[5 * k + j].mark_regist_halfday(half_day);
                    registry_5[5 * k + j].withdraw = 0;
                    registry_5[5 * k + j].ddlletter = 0;

                    //New added part for CA2
                    registry_5[5 * k + j].Euclidean_address[0] = atoi(current_line5[10].c_str());
                    registry_5[5 * k + j].Euclidean_address[1] = atoi(current_line5[11].c_str());
                    registry_5[5 * k + j].health_care_card = atoi(current_line5[12].c_str());
                    registry_5[5 * k + j].treatment = atoll(current_line5[13].c_str());
                    
                    registry_5[5 * k + j].calculate_key();
                }
            }

            // STEP TWO!
            //从五个lists倒进queue
            // queue 1
            int real_counter = 0; //The number of fake people that are put in each day.
            if (5 * k + 4 < registry_capacity)
            {
                // queue1
                for (int loop = 0; loop < 5; loop++)
                {
                    if (registry_1[5 * k + loop].id > 0)
                    {
                        registry[0].pushback(registry_1[5 * k + loop]);
                        real_counter++;
                    }
                }

                // queue 2
                for (int loop = 0; loop < 5; loop++)
                {
                    if (registry_2[5 * k + loop].id > 0)
                    {
                        registry[1].pushback(registry_2[5 * k + loop]);
                        real_counter++;
                    }
                }
                // queue 3
                for (int loop = 0; loop < 5; loop++)
                {
                    if (registry_3[5 * k + loop].id > 0)
                    {
                        registry[2].pushback(registry_3[5 * k + loop]);
                        real_counter++;
                    }
                }
                // queue 4
                for (int loop = 0; loop < 5; loop++)
                {
                    if (registry_4[5 * k + loop].id > 0)
                    {
                        registry[3].pushback(registry_4[5 * k + loop]);
                        real_counter++;
                    }
                }
                // queue 5
                for (int loop = 0; loop < 5; loop++)
                {
                    if (registry_5[5 * k + loop].id > 0)
                    {
                        registry[4].pushback(registry_5[5 * k + loop]);
                        real_counter++;
                    }
                }
                reginum += 25;

                // STEP THREE!
                // 从五个queue倒一部分人进斐波那契堆
                // find some people
                person people[25]; // initialize an array for 25 person
                int counter_for_queue = 0;
                int counter_for_person = 0;
                while (counter_for_person != real_counter) // while 25 people are not all found
                {
                    for (int i = 0; i <= 4; i++) // for each queue
                    {
                        if (!registry[i].isempty()) // check whether the queue is empty
                        {
                            people[counter_for_person] = registry[counter_for_queue].popfront(); // if not, a person is found
                            counter_for_person += 1;                                             // find next person
                            counter_for_queue += 1;                                              // check next queue
                            counter_for_queue = counter_for_queue % 5;
                            if (counter_for_person == real_counter)
                            {
                                break; // when all of the 25 people are found, break the for loop
                            }
                        }
                        else // if the queue is empty, just check next queue
                        {
                            counter_for_queue += 1;
                            counter_for_queue = counter_for_queue % 5;
                        }
                    }
                }
                // 25 people found. Calculate their key values and add this 25 people to the FibHeap
                node<long long> *nodes = new node<long long>[25];
                for (int i = 0; i < real_counter; i++)
                {
                    // char x;
                    nodes[i].p = people[i];
                    nodes[i].n = people[i].key;
                    nodes[i].ID_recorder = nodes[i].p.id; //Put the value of the person's id into the ID_recorder
                    Fib.Insert_ptr(&(nodes[i]));
                }
            }

                if (withdraw_reinsertion_archive.getlength() != 0)
                {
                    for (int looper = 0; looper < withdraw_reinsertion_archive.getlength(); looper++)
                    {
                        if ((half_day - withdraw_reinsertion_archive[looper + 1]->withdraw_halfday) >= 28)
                        {
                            node<long long> *withdraw_reinsert = new node<long long>;
                            withdraw_reinsert->p = *withdraw_reinsertion_archive[looper + 1];
                            withdraw_reinsert->n = withdraw_reinsertion_archive[looper + 1]->key;
                            withdraw_reinsert->ID_recorder = withdraw_reinsertion_archive[looper + 1]->id; //Put the value of the person's id into the ID_recorder
                            Fib.Insert_ptr(withdraw_reinsert);
                            withdraw_reinsertion_archive.remove(looper + 1);
                        }
                    }
                }

                if (middle_risk_archive.getlength() != 0)
                {
                    for (int looper2 = 0; looper2 < middle_risk_archive.getlength(); looper2++)
                    {
                        if ((half_day - middle_risk_archive[looper2 + 1]->risk2halfday) >= 60)
                        {
                            node<long long> *risk2_insert = new node<long long>;
                            risk2_insert->p = *middle_risk_archive[looper2 + 1];
                            risk2_insert->n = middle_risk_archive[looper2 + 1]->key;
                            risk2_insert->ID_recorder = middle_risk_archive[looper2 + 1]->id; //Put the value of the person's id into the ID_recorder
                            Fib.Insert_ptr(risk2_insert);
                            middle_risk_archive.remove(looper2 + 1);
                        }
                    }
                }
            //The following part is to givw the user a chance to make the withdraw, information update, re-registry a patient or submit a deadline letter.

            if (half_day % 2 != 0)
            {
                if (withdraw_mod == 1)
                {
                    int id;
                    if (1) //half_day % 20 == 0 && k <= 100)
                    {
                        int whether_continue = 1;
                        while (whether_continue)
                        {
                            cout << endl;
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                            cout << "Checkpoint1! Now you have a chance to make the withdraw, information update, re-registry a patient or submit a deadline letter.\n";
                            cout << "Please enter the following number to choose the mode:" << endl;
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                            cout << "1 withdraw a patient" << endl;
                            cout << "2 re-registry a patient who has withdrawn" << endl;
                            cout << "3 update the information of a given person" << endl;
                            cout << "4 submit a deadline letter" << endl;
                            cout << "5 to see a person's information" << endl;
                            cout << "6 continue without do anything" << endl;
                            // CA2
                            cout << "7 to check a person's information by his or her health care card"  << endl;
                            cout << "8 to check all the person with the same risk status." << endl;
                            
                            int choice; //1 for withdraw, 2 re-registry a patient who has withdrawn
                                        //for 3 for information update, 4 for DDL letter, 5 for information, 6 for quit, 7,8  for CA2

                            person *store = new person;
                            if (store == NULL)
                            {
                                cout << "Memory allocation failed. Error." << endl;
                                exit(EXIT_FAILURE);
                            }
                            cin >> choice;

                            if (choice < 1 || choice > 8)
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Invalid choice, please choose again." << endl;
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                continue;
                            }
                            switch (choice)
                            {
                            case 1:
                                cout << "Here are all the patients' ID who are still waiting without appointment: " << endl;
                                Fib.Display();
                                cout << "Here are all the patients' ID who have been appointed: " << endl;
                                hospital_display_id(hospital, half_day);
                                cout << "Here are all the people in the risk 2 buffer. " << endl;
                                middle_risk_archive.risk_report_all(half_day);
                                cout << "Here are all the people who have withdrawed before reinserted in the reinsert buffer." << endl;
                                withdraw_reinsertion_archive.reinsert_report_all(half_day);

                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Please choose an ID to withdraw: ";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cin >> id;
                                while (find_person(Fib, hospital, middle_risk_archive, withdraw_reinsertion_archive, id) == NULL)
                                {
                                    cout << "This person does not exist.\n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cout << "Please choose an ID to withdraw again:\n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> id;
                                }
                                store = find_person(Fib, hospital, middle_risk_archive, withdraw_reinsertion_archive, id); //The pointer to the withdrawn person
                                store->withdraw = 1;
                                store->withdraw_halfday = half_day;
                                withdrawnum++;
                                withdraw_archive.append(store);
                                removePatient(Fib, hospital, middle_risk_archive, withdraw_reinsertion_archive, id);

                                break;
                            case 2:
                            {
                                cout << "Here are all the patients' ID who have withdrawn: " << endl;
                                int size = withdraw_archive.getlength();
                                cout << "[ ";
                                for (int i = 0; i < size; i++)
                                    cout << withdraw_archive[i + 1]->id << " ";
                                cout << "]" << endl;

                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Please choose an ID to re-registry: ";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cin >> id;

                                for (int i = 0; i < size; i++)
                                {
                                    if (withdraw_archive[i + 1]->id == id)
                                    {
                                        withdraw_archive[i + 1]->reinsert_halfday = half_day;
                                        withdraw_reinsertion_archive.append(withdraw_archive[i + 1]);
                                        withdraw_archive.remove(i + 1);
                                        break;
                                    }
                                }

                                cout << "Here are all the patients' ID who are going to be re-inserted: " << endl;
                                cout << "[ ";
                                for (int i = 0; i < withdraw_reinsertion_archive.getlength(); i++)
                                    cout << withdraw_reinsertion_archive[i + 1]->id << " ";
                                cout << "]" << endl;
                                break;
                            }
                            case 3:
                            {
                                int choice_3 = 0;
                                int initial_position = - 1; // 1 for Fib, 2 for hospital, 3 for treated archive, 4 for risk 2 buffer, 5 for reinsert buffer
                                cout << "Here are all the patients' ID who are still waiting without appointment: " << endl;
                                Fib.Display();
                                cout << "Here are all the patients' ID who have been appointed: " << endl;
                                hospital_display_id(hospital, half_day);
                                cout << "Here are all the people who have been treated. " << endl;
                                archive.treat_report_all(half_day);
                                cout << "Here are all the people in the risk 2 buffer. " << endl;
                                middle_risk_archive.risk_report_all(half_day);
                                cout << "Here are all the people who have withdrawed before reinserted in the reinsert buffer." << endl;
                                withdraw_reinsertion_archive.reinsert_report_all(half_day);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Please type the ID of a person to change his or her information: ";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cin >> id;
                                person *change_person = find_person(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                while (change_person == NULL)
                                {
                                    cout << "This person does not exist. \n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                    cout << "Please choose an valid ID: ";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> id;
                                    change_person = find_person(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                }
                                if (Fib_find_person(Fib, id) != NULL)
                                {
                                    initial_position = 1;
                                }
                                else if (Hospital_find_person(hospital, id) != NULL)
                                {
                                    initial_position = 2;
                                }
                                else if (archive_find_person(archive, id) != NULL)
                                {
                                    initial_position = 3;
                                }
                                else if (archive_find_person(middle_risk_archive, id) != NULL)
                                {
                                    initial_position = 4;
                                }
                                else if (archive_find_person(withdraw_reinsertion_archive, id) != NULL)
                                {
                                    initial_position = 5;
                                }

                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Please choose which information to change.\n";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cout << "1 for profession, 2 for risk status, 3 for contact details.\n";

                                cin >> choice_3;
                                while (choice_3 < 1 || choice_3 > 3)
                                {
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                    cout << "Please type in a valid number, 1, 2 or 3.";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> choice_3;
                                }
                                switch (choice_3)
                                {
                                case 1:
                                {
                                    int new_profession = 0;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                    cout << "Please type the new profession for this person." << endl;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> new_profession;
                                    while (new_profession < 1 || new_profession > 8)
                                    {
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                        cout << "Please type in a valid profession, 1, 2, 3, 4, 5, 6, 7 or 8.";
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                        cin >> new_profession;
                                    }
                                    removePatient(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                    change_person->profession = new_profession;
                                    (*change_person).calculate_key();

                                    if (initial_position == 1)
                                    {
                                        node<long long> *change_node = new node<long long>;
                                        change_node->p = *change_person;
                                        change_node->n = change_person->key;
                                        change_node->ID_recorder = change_person->id; //Put the value of the person's id into the ID_recorder
                                        Fib.Insert_ptr(change_node);
                                        cout << "New node inserted." << endl;
                                        cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                    }
                                    else if (initial_position == 2)
                                    {
                                        for (int l = 0; l < 8; l++)
                                        {
                                            if (hospital[change_person->hospital - 1][l] == NULL)
                                            {
                                                hospital[change_person->hospital - 1][l] = change_person;
                                                break;
                                            }
                                        }

                                    }
                                    else if (initial_position == 3)
                                    {
                                        archive.append(change_person);
                                    }
                                    else if (initial_position == 4)
                                    {
                                        middle_risk_archive.append(change_person);
                                    }
                                    else if (initial_position == 5)
                                    {
                                        withdraw_reinsertion_archive.append(change_person);
                                    }
                                    break;
                                }
                                case 2:
                                {
                                    int new_risk_status = -1;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cout << "Please type the new risk status for this person." << endl;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> new_risk_status;
                                    while (new_risk_status < 0 || new_risk_status > 3)
                                    {
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                        cout << "Please type in a valid risk status, 0, 1, 2 or 3.";
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                        cin >> new_risk_status;
                                    }
                                    if (change_person->risk != 2)
                                    {
                                        if (new_risk_status != 2)
                                        {
                                            removePatient(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                            change_person->risk = new_risk_status;
                                            (*change_person).calculate_key();
                                            if (initial_position == 1)
                                            {
                                                node<long long> *change_node = new node<long long>;
                                                change_node->p = *change_person;
                                                change_node->n = change_person->key;
                                                change_node->ID_recorder = change_person->id; //Put the value of the person's id into the ID_recorder
                                                Fib.Insert_ptr(change_node);
                                                cout << "New node inserted." << endl;
                                                cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                            }
                                            else if (initial_position == 2)
                                            {
                                                for (int l = 0; l < 8; l++)
                                                {
                                                    if (hospital[change_person->hospital - 1][l] == NULL)
                                                    {
                                                        hospital[change_person->hospital - 1][l] = change_person;
                                                        break;
                                                    }
                                                }

                                            }
                                            else if (initial_position == 3)
                                            {
                                                archive.append(change_person);
                                            }
                                            else if (initial_position == 4)
                                            {
                                                middle_risk_archive.append(change_person);
                                            }
                                            else if (initial_position == 5)
                                            {
                                                withdraw_reinsertion_archive.append(change_person);
                                            }
                                        }
                                        else
                                        {
                                            removePatient(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                            change_person->risk = new_risk_status;
                                            (*change_person).calculate_key();
                                            if (initial_position == 1)
                                            {
                                                middle_risk_archive.append(change_person);
                                                cout << "Person inserted into the risk 2 archive" << endl;
                                            }
                                            else if (initial_position == 2)
                                            {
                                                for (int l = 0; l < 8; l++)
                                                {
                                                    if (hospital[change_person->hospital - 1][l] == NULL)
                                                    {
                                                        hospital[change_person->hospital - 1][l] = change_person;
                                                        break;
                                                    }
                                                }

                                            }
                                            else if (initial_position == 3)
                                            {
                                                archive.append(change_person);
                                            }
                                            else if (initial_position == 4)
                                            {
                                                middle_risk_archive.append(change_person);
                                            }
                                            else if (initial_position == 5)
                                            {
                                                middle_risk_archive.append(change_person);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if (new_risk_status == 2){}
                                        else            // the changing person's risk status is 2 initially, and will be changed into a number not 2
                                        {
                                            removePatient(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                            change_person->risk = new_risk_status;
                                            (*change_person).calculate_key();
                                            if (initial_position == 1)
                                            {
                                                node<long long> *change_node = new node<long long>;
                                                change_node->p = *change_person;
                                                change_node->n = change_person->key;
                                                change_node->ID_recorder = change_person->id; //Put the value of the person's id into the ID_recorder
                                                Fib.Insert_ptr(change_node);
                                                cout << "New node inserted." << endl;
                                                cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                            }
                                            else if (initial_position == 2)
                                            {
                                                for (int l = 0; l < 8; l++)
                                                {
                                                    if (hospital[change_person->hospital - 1][l] == NULL)
                                                    {
                                                        hospital[change_person->hospital - 1][l] = change_person;
                                                        break;
                                                    }
                                                }

                                            }
                                            else if (initial_position == 3)
                                            {
                                                archive.append(change_person);
                                            }
                                            else if (initial_position == 4)
                                            {
                                                node<long long> *change_node = new node<long long>;
                                                change_node->p = *change_person;
                                                change_node->n = change_person->key;
                                                change_node->ID_recorder = change_person->id; //Put the value of the person's id into the ID_recorder
                                                Fib.Insert_ptr(change_node);
                                                cout << "New node inserted." << endl;
                                                cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                            }
                                            else if (initial_position == 5)
                                            {
                                                withdraw_reinsertion_archive.append(change_person);
                                            }
                                        }


                                    }

                                    break;
                                }

                                case 3:
                                {
                                    int which_contact;
                                    string new_contact;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cout << "Please choose which contact information to change. 1 for address, 2 for phone, 3 for WeChat and 4 for email." << endl;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> which_contact;
                                    while (which_contact < 1 || which_contact > 4)
                                    {
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                        cout << "Please type in a valid number, 1, 2, 3 or 4.";
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                        cin >> which_contact;
                                    }
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                    cout << "Please type in the new contact information." << endl;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> new_contact;
                                    long long key_to_delete = change_person->key;
                                    cout << "Before deletion, the number of node is : " << Fib.numnode << endl;
                                    Fib.Delete_key(key_to_delete);
                                    cout << "After deletion, the number of node is : " << Fib.numnode << endl;
                                    change_person->CONTACT_DETAIL[which_contact - 1] = new_contact;
                                    if (initial_position == 1)
                                    {
                                        node<long long> *change_node = new node<long long>;
                                        change_node->p = *change_person;
                                        change_node->n = change_person->key;
                                        change_node->ID_recorder = change_person->id; //Put the value of the person's id into the ID_recorder
                                        Fib.Insert_ptr(change_node);
                                        cout << "New node inserted." << endl;
                                        cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                    }
                                    else if (initial_position == 2)
                                    {
                                        for (int l = 0; l < 8; l++)
                                        {
                                            if (hospital[change_person->hospital - 1][l] == NULL)
                                            {
                                                hospital[change_person->hospital - 1][l] = change_person;
                                                break;
                                            }
                                        }

                                    }
                                    else if (initial_position == 3)
                                    {
                                        archive.append(change_person);
                                    }
                                    else if (initial_position == 4)
                                    {
                                        middle_risk_archive.append(change_person);
                                    }
                                    else if (initial_position == 5)
                                    {
                                        withdraw_reinsertion_archive.append(change_person);
                                    }
                                    break;
                                }
                                }

                                break;
                            }
                            case 4:
                            {
                                cout << "Here are all the patients' ID who are still waiting without appointment: " << endl;
                                Fib.Display();
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Please type in an ID of a person to submit a deadline letter : ";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cin >> id;
                                person *ddl_person = Fib_find_person(Fib, id);
                                while (ddl_person == NULL)
                                {
                                    cout << "This person does not exist. \n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cout << "Please choose an ID to withdraw again: \n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> id;
                                    ddl_person = Fib_find_person(Fib, id);
                                }
                                long long key_to_delete = ddl_person->key;
                                cout << "Before deletion, the number of node is : " << Fib.numnode << endl;
                                Fib.Delete_key(key_to_delete);
                                cout << "After deletion, the number of node is : " << Fib.numnode << endl;
                                ddl_person->ddlletter = 1;
                                (*ddl_person).calculate_key();
                                node<long long> *ddl_node = new node<long long>;
                                ddl_node->p = *ddl_person;
                                ddl_node->n = ddl_person->key;
                                ddl_node->ID_recorder = ddl_person->id; //Put the value of the person's id into the ID_recorder
                                Fib.Insert_ptr(ddl_node);

                                cout << "The key of this person is recalculated.\n Now he is inserted again with highest priority temporarily." << endl;
                                cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                break;
                            }
                            case 5:
                            {
                                cout << "Here are all the patients' ID who are still waiting without appointment: " << endl;
                                Fib.Display();
                                cout << "Here are all the patients' ID who have been appointed: " << endl;
                                hospital_display_id(hospital, half_day);
                                cout << "Here are all the people who have been treated. " << endl;
                                archive.treat_report_all(half_day);
                                cout << "Here are all the people in the risk 2 buffer. " << endl;
                                middle_risk_archive.risk_report_all(half_day);
                                cout << "Here are all the people who have withdrawed before reinserted in the reinsert buffer." << endl;
                                withdraw_reinsertion_archive.reinsert_report_all(half_day);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                cout << "Please choose an ID to see a person's information: ";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cin >> id;
                                person *person_to_print = find_person(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                while (person_to_print == NULL)
                                {
                                    cout << "This person does not exist.\n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cout << "Please choose an valid ID again:\n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> id;
                                    person_to_print = find_person(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                }
                                print_person(*(person_to_print));
                                break;
                            }
                            case 6:
                            {
                                whether_continue = 0;
                                break;
                            }
                            case 7:
                            {
                                cout << "Here are all card numbers you can choose: " <<endl;
                                B_search_tree.display();
                                cout << "B+ tree: " << endl;
                                person_tree.print();
                                cout << "Please enter the person's health care card ID: " << endl;
                                int health_care_card;
                                cin >> health_care_card;
                                cout << "which part of information do you want to view? " << endl;
                                cout << "1 for personal information, 2 for medical status, 3 for registration, 4 for treatment." << endl;
                                int view;
                                cin >> view;
                                while (view < 1 || view > 4)
                                {
                                    cout << "Please enter a number from 1 to 4." << endl;
                                    cin >> view;
                                }
                                if (view == 1)
                                {
                                    int identity = B_find_id(B_search_tree, health_care_card);
                                    if (identity == -1)
                                    {
                                        cout << "invalid health care card." << endl;
                                    }
                                    else
                                    {
                                        B_report_a_person(B_search_tree, person_tree, health_care_card);
                                    }
                                }
                                if (view == 2)
                                {
                                    int identity = B_find_id(B_search_tree, health_care_card);
                                    if (identity == -1)
                                    {
                                        cout << "invalid health care card." << endl;
                                    }
                                    else
                                    {
                                        B_report_a_medical_status(B_search_tree, medical_tree, health_care_card);
                                    }
                                }
                                if (view == 3)
                                {
                                    int identity = B_find_id(B_search_tree, health_care_card);
                                    if (identity == -1)
                                    {
                                        cout << "invalid health care card." << endl;
                                    }
                                    else
                                    {
                                        B_report_a_registration(B_search_tree, registration_tree, health_care_card);
                                    }
                                }
                                if (view == 4)
                                {
                                    int identity = B_find_id(B_search_tree, health_care_card);
                                    if (identity == -1)
                                    {
                                        cout << "invalid health care card." << endl;
                                    }
                                    else
                                    {
                                        B_report_a_treatment(B_search_tree, treatment_tree, health_care_card);
                                    }
                                }
                                break;
                            }
                            case 8:{
                                cout << "choose a risk status, 0, 1, 2 or 3." << endl;
                                int risk;
                                cin >> risk;
                                while (risk < 0 || risk > 3)
                                {
                                    cout << "risk status can only be 0, 1, 2, or 3." << endl;
                                    cin >> risk;
                                }
                                hash_of_risk.output_same_risk(risk);
                                break;
                            }
                            }
                    
                        }
                    }
                }
            }


            // STEP FOUR!
            // Put the people from the fib_heap into the hospital
            if (half_day % 2 == 0) // 每一天过去之后才执行以下操作
            {
                // 提取24个人出来，安排进他们分别想进的医院里
                cout << "Patients are being popped";
                for (auto _ = 0; _ < 6; _++)
                {
                    Sleep(100);
                    cout << ".";
                }
                cout << endl;

                // 指向空缺的时间槽
                int hos_vacancy_ptr[3] = {0, 0, 0};
                // 暂存装不进去的病人，过一会全部弄回肥波堆里
                fifo<node<long long> *> temp_storage;

                // 直到把三个医院的全部时间槽都填满
                while (!(hos_vacancy_ptr[0] == 8 && hos_vacancy_ptr[1] == 8 && hos_vacancy_ptr[2] == 8))
                {
                    // 优先级最高的病人
                    node<long long> *p_to_node = Fib.Extract_Min();
                    if (p_to_node == NULL)
                    {
                        cout << "FibHeap is empty!\n";
                        break;
                    }

                    // 新建一个person 指针
                    person *p_to_a_person = new person;
                    if (p_to_a_person == NULL)
                    {
                        cout << "Memory allocation failed. Exit." << endl;
                        exit(EXIT_FAILURE);
                    }
                    // 把那个被提取出来的病人赋值给新建出来的person
                    *p_to_a_person = p_to_node->p;
                    //更新appointnum

                    cout << "The person with ID: " << p_to_a_person->id << " is popped out from the heap." << endl;

                    int which_hospital = p_to_a_person->hospital - 1;
                    int that_counter = hos_vacancy_ptr[which_hospital];
                    if (that_counter < 8)
                    {
                        hospital[which_hospital][that_counter] = p_to_a_person;
                        hos_vacancy_ptr[which_hospital]++;
                        appointnum += 1;
                    }
                    else
                    {
                        cout << "The person with ID: " << p_to_a_person->id << " is stored in the temporary storage." << endl;
                        temp_storage.pushback(p_to_node);
                    }
                }

                if (withdraw_mod == 1)
                {
                    int id;
                    if (1) //half_day % 20 == 0 && k <= 100)
                    {
                        int whether_continue = 1;
                        while (whether_continue)
                        {
                            cout << endl;
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                            cout << "Checkpoint2! Now you have a chance to make the withdraw, information update, re-registry a patient or submit a deadline letter.\n";
                            cout << "Please enter the following number to choose the mode:" << endl;
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                            cout << "1 withdraw a patient" << endl;
                            cout << "2 re-registry a patient who has withdrawn" << endl;
                            cout << "3 update the information of a given person" << endl;
                            cout << "4 submit a deadline letter" << endl;
                            cout << "5 to see a person's information" << endl;
                            cout << "6 continue without do anything" << endl;
                            // CA2
                            cout << "7 to check a person's information by his or her health care card"  << endl;
                            cout << "8 to check all the person with the same risk status." << endl;

                            int choice; //1 for withdraw, 2 re-registry a patient who has withdrawn
                                        //for 3 for information update, 4 for DDL letter, 5 for information, 6 for quit, 7,8 for CA2

                            person *store = new person;
                            if (store == NULL)
                            {
                                cout << "Memory allocation failed. Error." << endl;
                                exit(EXIT_FAILURE);
                            }
                            cin >> choice;

                            if (choice < 1 || choice > 8)
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Invalid choice, please choose again." << endl;
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                continue;
                            }
                            switch (choice)
                            {
                            case 1:
                                cout << "Here are all the patients' ID who are still waiting without appointment: " << endl;
                                Fib.Display();
                                cout << "Here are all the patients' ID who have been appointed: " << endl;
                                hospital_display_id(hospital, half_day);
                                cout << "Here are all the people in the risk 2 buffer. " << endl;
                                middle_risk_archive.risk_report_all(half_day);
                                cout << "Here are all the people who have withdrawed before reinserted in the reinsert buffer." << endl;
                                withdraw_reinsertion_archive.reinsert_report_all(half_day);

                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Please choose an ID to withdraw: ";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cin >> id;
                                while (find_person(Fib, hospital, middle_risk_archive, withdraw_reinsertion_archive, id) == NULL)
                                {
                                    cout << "This person does not exist.\n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cout << "Please choose an ID to withdraw again:\n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> id;
                                }
                                store = find_person(Fib, hospital, middle_risk_archive, withdraw_reinsertion_archive, id); //The pointer to the withdrawn person
                                store->withdraw = 1;
                                store->withdraw_halfday = half_day;
                                withdrawnum++;
                                withdraw_archive.append(store);
                                removePatient(Fib, hospital, middle_risk_archive, withdraw_reinsertion_archive, id);

                                break;
                            case 2:
                            {
                                cout << "Here are all the patients' ID who have withdrawn: " << endl;
                                int size = withdraw_archive.getlength();
                                cout << "[ ";
                                for (int i = 0; i < size; i++)
                                    cout << withdraw_archive[i + 1]->id << " ";
                                cout << "]" << endl;

                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Please choose an ID to re-registry: ";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cin >> id;

                                for (int i = 0; i < size; i++)
                                {
                                    if (withdraw_archive[i + 1]->id == id)
                                    {
                                        withdraw_archive[i + 1]->reinsert_halfday = half_day;
                                        withdraw_reinsertion_archive.append(withdraw_archive[i + 1]);
                                        withdraw_archive.remove(i + 1);
                                        break;
                                    }
                                }

                                cout << "Here are all the patients' ID who are going to be re-inserted: " << endl;
                                cout << "[ ";
                                for (int i = 0; i < withdraw_reinsertion_archive.getlength(); i++)
                                    cout << withdraw_reinsertion_archive[i + 1]->id << " ";
                                cout << "]" << endl;
                                break;
                            }
                            case 3:
                            {
                                int choice_3 = 0;
                                int initial_position = - 1; // 1 for Fib, 2 for hospital, 3 for treated archive, 4 for risk 2 buffer, 5 for reinsert buffer
                                cout << "Here are all the patients' ID who are still waiting without appointment: " << endl;
                                Fib.Display();
                                cout << "Here are all the patients' ID who have been appointed: " << endl;
                                hospital_display_id(hospital, half_day);
                                cout << "Here are all the people who have been treated. " << endl;
                                archive.treat_report_all(half_day);
                                cout << "Here are all the people in the risk 2 buffer. " << endl;
                                middle_risk_archive.risk_report_all(half_day);
                                cout << "Here are all the people who have withdrawed before reinserted in the reinsert buffer." << endl;
                                withdraw_reinsertion_archive.reinsert_report_all(half_day);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Please type the ID of a person to change his or her information: ";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cin >> id;
                                person *change_person = find_person(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                while (change_person == NULL)
                                {
                                    cout << "This person does not exist. \n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                    cout << "Please choose an valid ID: ";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> id;
                                    change_person = find_person(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                }
                                if (Fib_find_person(Fib, id) != NULL)
                                {
                                    initial_position = 1;
                                }
                                else if (Hospital_find_person(hospital, id) != NULL)
                                {
                                    initial_position = 2;
                                }
                                else if (archive_find_person(archive, id) != NULL)
                                {
                                    initial_position = 3;
                                }
                                else if (archive_find_person(middle_risk_archive, id) != NULL)
                                {
                                    initial_position = 4;
                                }
                                else if (archive_find_person(withdraw_reinsertion_archive, id) != NULL)
                                {
                                    initial_position = 5;
                                }

                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Please choose which information to change.\n";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cout << "1 for profession, 2 for risk status, 3 for contact details.\n";

                                cin >> choice_3;
                                while (choice_3 < 1 || choice_3 > 3)
                                {
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                    cout << "Please type in a valid number, 1, 2 or 3.";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> choice_3;
                                }
                                switch (choice_3)
                                {
                                case 1:
                                {
                                    int new_profession = 0;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                    cout << "Please type the new profession for this person." << endl;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> new_profession;
                                    while (new_profession < 1 || new_profession > 8)
                                    {
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                        cout << "Please type in a valid profession, 1, 2, 3, 4, 5, 6, 7 or 8.";
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                        cin >> new_profession;
                                    }
                                    removePatient(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                    change_person->profession = new_profession;
                                    (*change_person).calculate_key();

                                    if (initial_position == 1)
                                    {
                                        node<long long> *change_node = new node<long long>;
                                        change_node->p = *change_person;
                                        change_node->n = change_person->key;
                                        change_node->ID_recorder = change_person->id; //Put the value of the person's id into the ID_recorder
                                        Fib.Insert_ptr(change_node);
                                        cout << "New node inserted." << endl;
                                        cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                    }
                                    else if (initial_position == 2)
                                    {
                                        for (int l = 0; l < 8; l++)
                                        {
                                            if (hospital[change_person->hospital - 1][l] == NULL)
                                            {
                                                hospital[change_person->hospital - 1][l] = change_person;
                                                break;
                                            }
                                        }

                                    }
                                    else if (initial_position == 3)
                                    {
                                        archive.append(change_person);
                                    }
                                    else if (initial_position == 4)
                                    {
                                        middle_risk_archive.append(change_person);
                                    }
                                    else if (initial_position == 5)
                                    {
                                        withdraw_reinsertion_archive.append(change_person);
                                    }
                                    break;
                                }
                                case 2:
                                {
                                    int new_risk_status = -1;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cout << "Please type the new risk status for this person." << endl;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> new_risk_status;
                                    while (new_risk_status < 0 || new_risk_status > 3)
                                    {
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                        cout << "Please type in a valid risk status, 0, 1, 2 or 3.";
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                        cin >> new_risk_status;
                                    }
                                    if (change_person->risk != 2)
                                    {
                                        if (new_risk_status != 2)
                                        {
                                            removePatient(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                            change_person->risk = new_risk_status;
                                            (*change_person).calculate_key();
                                            if (initial_position == 1)
                                            {
                                                node<long long> *change_node = new node<long long>;
                                                change_node->p = *change_person;
                                                change_node->n = change_person->key;
                                                change_node->ID_recorder = change_person->id; //Put the value of the person's id into the ID_recorder
                                                Fib.Insert_ptr(change_node);
                                                cout << "New node inserted." << endl;
                                                cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                            }
                                            else if (initial_position == 2)
                                            {
                                                for (int l = 0; l < 8; l++)
                                                {
                                                    if (hospital[change_person->hospital - 1][l] == NULL)
                                                    {
                                                        hospital[change_person->hospital - 1][l] = change_person;
                                                        break;
                                                    }
                                                }

                                            }
                                            else if (initial_position == 3)
                                            {
                                                archive.append(change_person);
                                            }
                                            else if (initial_position == 4)
                                            {
                                                middle_risk_archive.append(change_person);
                                            }
                                            else if (initial_position == 5)
                                            {
                                                withdraw_reinsertion_archive.append(change_person);
                                            }
                                        }
                                        else
                                        {
                                            removePatient(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                            change_person->risk = new_risk_status;
                                            (*change_person).calculate_key();
                                            if (initial_position == 1)
                                            {
                                                middle_risk_archive.append(change_person);
                                                cout << "Person inserted into the risk 2 archive" << endl;
                                            }
                                            else if (initial_position == 2)
                                            {
                                                for (int l = 0; l < 8; l++)
                                                {
                                                    if (hospital[change_person->hospital - 1][l] == NULL)
                                                    {
                                                        hospital[change_person->hospital - 1][l] = change_person;
                                                        break;
                                                    }
                                                }

                                            }
                                            else if (initial_position == 3)
                                            {
                                                archive.append(change_person);
                                            }
                                            else if (initial_position == 4)
                                            {
                                                middle_risk_archive.append(change_person);
                                            }
                                            else if (initial_position == 5)
                                            {
                                                middle_risk_archive.append(change_person);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if (new_risk_status == 2){}
                                        else            // the changing person's risk status is 2 initially, and will be changed into a number not 2
                                        {
                                            removePatient(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                            change_person->risk = new_risk_status;
                                            (*change_person).calculate_key();
                                            if (initial_position == 1)
                                            {
                                                node<long long> *change_node = new node<long long>;
                                                change_node->p = *change_person;
                                                change_node->n = change_person->key;
                                                change_node->ID_recorder = change_person->id; //Put the value of the person's id into the ID_recorder
                                                Fib.Insert_ptr(change_node);
                                                cout << "New node inserted." << endl;
                                                cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                            }
                                            else if (initial_position == 2)
                                            {
                                                for (int l = 0; l < 8; l++)
                                                {
                                                    if (hospital[change_person->hospital - 1][l] == NULL)
                                                    {
                                                        hospital[change_person->hospital - 1][l] = change_person;
                                                        break;
                                                    }
                                                }

                                            }
                                            else if (initial_position == 3)
                                            {
                                                archive.append(change_person);
                                            }
                                            else if (initial_position == 4)
                                            {
                                                node<long long> *change_node = new node<long long>;
                                                change_node->p = *change_person;
                                                change_node->n = change_person->key;
                                                change_node->ID_recorder = change_person->id; //Put the value of the person's id into the ID_recorder
                                                Fib.Insert_ptr(change_node);
                                                cout << "New node inserted." << endl;
                                                cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                            }
                                            else if (initial_position == 5)
                                            {
                                                withdraw_reinsertion_archive.append(change_person);
                                            }
                                        }


                                    }

                                    break;
                                }

                                case 3:
                                {
                                    int which_contact;
                                    string new_contact;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cout << "Please choose which contact information to change. 1 for address, 2 for phone, 3 for WeChat and 4 for email." << endl;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> which_contact;
                                    while (which_contact < 1 || which_contact > 4)
                                    {
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                        cout << "Please type in a valid number, 1, 2, 3 or 4.";
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                        cin >> which_contact;
                                    }
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                    cout << "Please type in the new contact information." << endl;
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> new_contact;
                                    long long key_to_delete = change_person->key;
                                    cout << "Before deletion, the number of node is : " << Fib.numnode << endl;
                                    Fib.Delete_key(key_to_delete);
                                    cout << "After deletion, the number of node is : " << Fib.numnode << endl;
                                    change_person->CONTACT_DETAIL[which_contact - 1] = new_contact;
                                    if (initial_position == 1)
                                    {
                                        node<long long> *change_node = new node<long long>;
                                        change_node->p = *change_person;
                                        change_node->n = change_person->key;
                                        change_node->ID_recorder = change_person->id; //Put the value of the person's id into the ID_recorder
                                        Fib.Insert_ptr(change_node);
                                        cout << "New node inserted." << endl;
                                        cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                    }
                                    else if (initial_position == 2)
                                    {
                                        for (int l = 0; l < 8; l++)
                                        {
                                            if (hospital[change_person->hospital - 1][l] == NULL)
                                            {
                                                hospital[change_person->hospital - 1][l] = change_person;
                                                break;
                                            }
                                        }

                                    }
                                    else if (initial_position == 3)
                                    {
                                        archive.append(change_person);
                                    }
                                    else if (initial_position == 4)
                                    {
                                        middle_risk_archive.append(change_person);
                                    }
                                    else if (initial_position == 5)
                                    {
                                        withdraw_reinsertion_archive.append(change_person);
                                    }
                                    break;
                                }
                                }

                                break;
                            }
                            case 4:
                            {
                                cout << "Here are all the patients' ID who are still waiting without appointment: " << endl;
                                Fib.Display();
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                                cout << "Please type in an ID of a person to submit a deadline letter : ";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cin >> id;
                                person *ddl_person = Fib_find_person(Fib, id);
                                while (ddl_person == NULL)
                                {
                                    cout << "This person does not exist. \n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cout << "Please choose an ID to withdraw again: \n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> id;
                                    ddl_person = Fib_find_person(Fib, id);
                                }
                                long long key_to_delete = ddl_person->key;
                                cout << "Before deletion, the number of node is : " << Fib.numnode << endl;
                                Fib.Delete_key(key_to_delete);
                                cout << "After deletion, the number of node is : " << Fib.numnode << endl;
                                ddl_person->ddlletter = 1;
                                (*ddl_person).calculate_key();
                                node<long long> *ddl_node = new node<long long>;
                                ddl_node->p = *ddl_person;
                                ddl_node->n = ddl_person->key;
                                ddl_node->ID_recorder = ddl_person->id; //Put the value of the person's id into the ID_recorder
                                Fib.Insert_ptr(ddl_node);

                                cout << "The key of this person is recalculated.\n Now he is inserted again with highest priority temporarily." << endl;
                                cout << "After insertion, the number of node is : " << Fib.numnode << endl;
                                break;
                            }
                            case 5:
                            {
                                cout << "Here are all the patients' ID who are still waiting without appointment: " << endl;
                                Fib.Display();
                                cout << "Here are all the patients' ID who have been appointed: " << endl;
                                hospital_display_id(hospital, half_day);
                                cout << "Here are all the people who have been treated. " << endl;
                                archive.treat_report_all(half_day);
                                cout << "Here are all the people in the risk 2 buffer. " << endl;
                                middle_risk_archive.risk_report_all(half_day);
                                cout << "Here are all the people who have withdrawed before reinserted in the reinsert buffer." << endl;
                                withdraw_reinsertion_archive.reinsert_report_all(half_day);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                cout << "Please choose an ID to see a person's information: ";
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                cin >> id;
                                person *person_to_print = find_person(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                while (person_to_print == NULL)
                                {
                                    cout << "This person does not exist.\n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cout << "Please choose an valid ID again:\n";
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

                                    cin >> id;
                                    person_to_print = find_person(Fib, hospital, archive, middle_risk_archive, withdraw_reinsertion_archive, id);
                                }
                                print_person(*(person_to_print));
                                break;
                            }
                            case 6:
                            {
                                whether_continue = 0;
                                break;
                            }
                            case 7:
                            {
                                cout << "Here are all card number you can choose: " << endl;
                                B_search_tree.display();
                                cout << "B+ tree: " << endl;
                                person_tree.print();
                                cout << "Please enter the person's health care card ID: " << endl;
                                int health_care_card;
                                cin >> health_care_card;
                                cout << "which part of information do you want to view? " << endl;
                                cout << "1 for personal information, 2 for medical status, 3 for registration, 4 for treatment." << endl;
                                int view;
                                cin >> view;
                                while (view < 1 || view > 4)
                                {
                                    cout << "Please enter a number from 1 to 4." << endl;
                                    cin >> view;
                                }
                                if (view == 1)
                                {
                                    int identity = B_find_id(B_search_tree, health_care_card);
                                    if (identity == -1)
                                    {
                                        cout << "invalid health care card." << endl;
                                    }
                                    else
                                    {
                                        B_report_a_person(B_search_tree, person_tree, health_care_card);
                                    }
                                }
                                if (view == 2)
                                {
                                    int identity = B_find_id(B_search_tree, health_care_card);
                                    if (identity == -1)
                                    {
                                        cout << "invalid health care card." << endl;
                                    }
                                    else
                                    {
                                        B_report_a_medical_status(B_search_tree, medical_tree, health_care_card);
                                    }
                                }
                                if (view == 3)
                                {
                                    int identity = B_find_id(B_search_tree, health_care_card);
                                    if (identity == -1)
                                    {
                                        cout << "invalid health care card." << endl;
                                    }
                                    else
                                    {
                                        B_report_a_registration(B_search_tree, registration_tree, health_care_card);
                                    }
                                }
                                if (view == 4)
                                {
                                    int identity = B_find_id(B_search_tree, health_care_card);
                                    if (identity == -1)
                                    {
                                        cout << "invalid health care card." << endl;
                                    }
                                    else
                                    {
                                        B_report_a_treatment(B_search_tree, treatment_tree, health_care_card);
                                    }
                                }
                                break;
                            }
                            case 8:{
                                cout << "choose a risk status, 0, 1, 2 or 3." << endl;
                                int risk;
                                cin >> risk;
                                while (risk < 0 || risk > 3)
                                {
                                    cout << "risk status can only be 0, 1, 2, or 3." << endl;
                                    cin >> risk;
                                }
                                hash_of_risk.output_same_risk(risk);
                                break;
                            }
                            }
                        }
                    }
                }

                // cout << "All the hospitals are full." << endl;

                while (!temp_storage.isempty())
                {
                    node<long long> *p_to_node = temp_storage.popfront();
                    if (p_to_node != NULL)
                    {
                        p_to_node->ID_recorder = p_to_node->p.id; //Put the value of the person's id into the ID_recorder
                        Fib.Insert_ptr(p_to_node);
                    }
                    else
                    {
                        cout << "Error, oh my god." << endl;
                        exit(EXIT_FAILURE);
                    }
                }

                // 更新waitnum以及waitnum_total
                waitnum = Fib.numnode + middle_risk_archive.getlength() + withdraw_reinsertion_archive.getlength();
                waitnum_total = waitnum + hospital_num(hospital);

                // 持续治病中......
                cout << "Treating these guys";
                for (auto _ = 0; _ < 6; _++)
                {
                    Sleep(100);
                    cout << ".";
                }
                cout << endl;

                // 人都治好了！
                cout << "Patients are treated!" << endl;

                // 把人存进“已治疗”的档案室里
                for (int hos = 0; hos < 3; hos++)
                {
                    for (int time_slot = 0; time_slot < 8; time_slot++)
                    {
                        if (hospital[hos][time_slot] != NULL)
                        {
                            person *a_patient = hospital[hos][time_slot];
                            a_patient->mark_treat_halfday(half_day);
                            archive.append(a_patient);
                            hospital[hos][time_slot] = NULL;

                            //CA2 put these people also into the B+ tree
                            //******** B+ tree 1
                            PERSON *personstore = new PERSON;
                            if (personstore == NULL)
                            {
                                cout << "Can not allocate memory when putting people into B+ tree 1\n";
                                return 0;
                            }
                            personstore->id = int(a_patient->id);
                            personstore->name = a_patient->name;
                            personstore->profession = a_patient->profession;
                            personstore->health_care_card = a_patient->health_care_card;
                            personstore->birth[0] = a_patient->birth[0];
                            personstore->birth[1] = a_patient->birth[1];
                            personstore->birth[2] = a_patient->birth[2];
                            personstore->CONTACT_DETAIL[0] = a_patient->CONTACT_DETAIL[0];
                            personstore->CONTACT_DETAIL[1] = a_patient->CONTACT_DETAIL[1];
                            personstore->CONTACT_DETAIL[2] = a_patient->CONTACT_DETAIL[2];
                            personstore->CONTACT_DETAIL[3] = a_patient->CONTACT_DETAIL[3];
                            person_tree.insert(personstore->id, *personstore);

                            //******** B+ tree 2
                            MEDICAL_STATUS *medicalstore = new MEDICAL_STATUS;
                            if (medicalstore == NULL)
                            {
                                cout << "Can not allocate memory when putting people into B+ tree 2\n";
                                return 0;
                            }
                            medicalstore->id = int(a_patient->id);
                            medicalstore->risk = a_patient->risk;
                            medical_tree.insert(medicalstore->id, *medicalstore);

                            //******** B+ tree 3
                            REGISTRATION *registrationstore = new REGISTRATION;
                            if (registrationstore == NULL)
                            {
                                cout << "Can not allocate memory when putting people into B+ tree 3\n";
                                return 0;
                            }
                            registrationstore->id = int(a_patient->id);
                            registrationstore->registration = a_patient->registration;
                            registrationstore->register_time[0] = a_patient->register_time[0];
                            registrationstore->register_time[1] = a_patient->register_time[1];
                            registrationstore->register_time[2] = a_patient->register_time[2];
                            registrationstore->register_time[3] = a_patient->register_time[3];
                            registrationstore->register_time[4] = a_patient->register_time[4];
                            registrationstore->hospital = a_patient->hospital;
                            registrationstore->regist_halfday = a_patient->regist_halfday;
                            registrationstore->withdraw = a_patient->withdraw;
                            registrationstore->withdraw_halfday = a_patient->withdraw_halfday;
                            registrationstore->reinsert_halfday = a_patient->reinsert_halfday;
                            registrationstore->risk2halfday = a_patient->risk2halfday;
                            registration_tree.insert(registrationstore->id, *registrationstore);
                            
                            //******** B+ tree 4
                            TREATMENT *treatmentstore = new TREATMENT;
                            if (treatmentstore == NULL)
                            {
                                cout << "Can not allocate memory when putting people into B+ tree 4\n";
                                return 0;
                            }
                            treatmentstore->id = int(a_patient->id);
                            treatmentstore->treatment = a_patient->treatment;
                            treatmentstore->treat_halfday = a_patient->treat_halfday;
                            treatment_tree.insert(treatmentstore->id, *treatmentstore);
                                                   
                            // //******** B+ tree 5 ************//
                            // DISTANCE *distancestore = new DISTANCE;
                            // if (distancestore == NULL)
                            // {
                            //     cout << "Can not allocate memory when putting people into B+ tree 5\n";
                            //     return 0;
                            // }
                            // distancestore->id = int(a_patient->id);
                            // distancestore->treatment = a_patient->treatment;
                            // distancestore->Euclidean_address[0] = a_patient->Euclidean_address[0];
                            // distancestore->Euclidean_address[1] = a_patient->Euclidean_address[1];
                            // distancestore->neighbour_number = a_patient->neighbour_number;
                            // distance_tree.insert(distancestore->id, *distancestore);

                            //B tree
                            Key_tuple *B_search_store = new Key_tuple(a_patient->health_care_card,int(a_patient->id));
                            if (B_search_store != NULL)
                            {
                                B_search_tree.insert(*B_search_store);
                            }
                            
                            //Hash table
                            MEDICAL_STATUS *hashstore = new MEDICAL_STATUS;
                            if (hashstore == NULL)
                            {
                                cout << "Can not allocate memory when putting people into hash table.\n";
                                return 0;
                            }
                            hashstore->id = int(a_patient->id);
                            hashstore->risk = a_patient->risk;
                            hash_of_risk.add_risk_status(*hashstore);

                        }
                    }
                }
                averagetime = archive.average_halfday(half_day);
            }

            // 一个半天结束了，稍微睡一会，睡三秒钟
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
            cout << "Half day passed, press any key to continue\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            system("pause");

            cout << endl
                 << endl;

            //Step5:判断时间长短，选择是否打印周报告或月报告
            if (half_day % 14 == 0)
            {
                week++;
                cout << "Now the variable half_day = " << half_day << endl
                     << "One week passed." << endl
                     << "The current week is week: " << week << endl
                     << "Generating weekly report: "
                     << endl
                     << endl;

                string week_number = to_string(week -1);
                string filename = "week " + week_number + " report.txt";

                ofstream outfile;
                outfile.open(filename,ios::app);


                //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);

                outfile << "+------------------------------------------------------------------------+" << endl
                     << "|                                                                        |" << endl
                     << "|                             Week " << week - 1 << " Report                              |" << endl
                     << "|                                                                        |" << endl
                     << "+------------------------------------------------------------------------+" << endl
                     << endl;
                
                //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                outfile.close();

                weekly_report(archive, hospital, Fib, middle_risk_archive, withdraw_reinsertion_archive, half_day, filename);

                //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
                



                outfile.open(filename, ios::app);
                outfile << "+------------------------------------------------------------------------+" << endl
                     << "|                                                                        |" << endl
                     << "|                            Week " << week - 1 << " Report Ends                          |" << endl
                     << "|       CITY MEDICINE AUTO-REGISTRATION AND APPOINTMENT SYSTEM           |" << endl
                     << "|                                                                        |" << endl
                     << "+------------------------------------------------------------------------+" << endl
                     << endl;
                outfile.close();
                //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

                cout << "+------------------------------------------------------------------------+" << endl
                     << "|                                                                        |" << endl
                     << "|      Week " << week - 1 << " Report have been printed into the file, please check.      |" << endl
                     << "|                                                                        |" << endl
                     << "+------------------------------------------------------------------------+" << endl
                     << endl;

                // CA2 weekly risk report based on hash table
                risk_status_report_week(week, person_tree, medical_tree, registration_tree, treatment_tree, hash_of_risk);
                cout << "+------------------------------------------------------------------------+" << endl
                     << "|                                                                        |" << endl
                     << "|      Week " << week - 1 << " Risk Report have been printed into the file, please check.      |" << endl
                     << "|                                                                        |" << endl
                     << "+------------------------------------------------------------------------+" << endl
                     << endl;


                
                for (auto _ = 0; _ < 6; _++)
                {
                    Sleep(500);
                    cout << "*";
                    cout << endl;
                }
                system("pause");
            }
            if (half_day % 60 == 0)
            {
                month++;

                for (auto _ = 0; _ < 6; _++)
                {
                    Sleep(300);
                    cout << ".";
                    cout << endl;
                }
                cout << "Now the variable half_day = " << half_day << endl
                     << "One month passed." << endl
                     << "The current m is week: " << week << endl
                     << "Generating monthly report: "
                     << endl
                     << endl;

                string month_number = to_string(month -1);
                string filename2 = "month " + month_number + " report.txt";

                ofstream outfile2;
                outfile2.open(filename2);
                //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);

                outfile2 << "+------------------------------------------------------------------------+" << endl
                     << "|                                                                        |" << endl
                     << "|                           Month " << month - 1 << " Report                               |" << endl
                     << "|                                                                        |" << endl
                     << "+------------------------------------------------------------------------+" << endl
                     << endl;

                //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                outfile2.close();
                monthly_report(reginum, waitnum, waitnum_total, appointnum, withdrawnum, averagetime, filename2);

                //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
                outfile2.open(filename2,ios::app);
                outfile2 << endl;
                outfile2.close();
                outfile2.open(filename2,ios::app);
                outfile2 << "+------------------------------------------------------------------------+" << endl
                     << "|                                                                        |" << endl
                     << "|                            Report Ends                                 |" << endl
                     << "|       CITY MEDICINE AUTO-REGISTRATION AND APPOINTMENT SYSTEM           |" << endl
                     << "|                                                                        |" << endl
                     << "+------------------------------------------------------------------------+" << endl;
                outfile2 << endl;

                outfile2.close();

                cout << "+------------------------------------------------------------------------+" << endl
                     << "|                                                                        |" << endl
                     << "|      Month " << month - 1 << " Report have been printed into the file, please check.     |" << endl
                     << "|                                                                        |" << endl
                     << "+------------------------------------------------------------------------+" << endl
                     << endl;


                // CA2 monthly risk report based on hash table
                risk_status_report_month(month, person_tree, medical_tree, registration_tree, treatment_tree, hash_of_risk);
                cout << "+------------------------------------------------------------------------+" << endl
                     << "|                                                                        |" << endl
                     << "|      Month " << month - 1 << " Risk Report have been printed into the file, please check.     |" << endl
                     << "|                                                                        |" << endl
                     << "+------------------------------------------------------------------------+" << endl
                     << endl;
                //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

                reginum = 0;
                waitnum = 0;
                waitnum_total = 0;
                appointnum = 0;
                withdrawnum = 0;
                averagetime = 0;
                for (auto _ = 0; _ < 6; _++)
                {
                    Sleep(500);
                    cout << ".";
                    cout << endl;
                }
                system("pause");
            }
        }

        myfile1.close();
        myfile2.close();
        myfile3.close();
        myfile4.close();
        myfile5.close();

        cout << "All patients in the Source file are treated." << endl;
        cout << "Now we check the number of node in the fibonacci heap: " << endl;
        cout << "There are " << Fib.numnode << " nodes left in Fib-Heap." << endl;
        cout << endl
             << endl;
        cout << "Logging out";
        for (auto _ = 0; _ < 6; _++)
        {
            Sleep(500);
            cout << ".";
        }
        cout << endl;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);

        cout << endl;
        cout << endl;
        cout << endl;
        Sleep(200);
        cout<< "   _______  _                    _              ______               _    _       _                  _ \n";
        Sleep(200);
        cout<< "  |__   __|| |                  | |            |  ____|             | |  | |     (_)                | |\n";
        Sleep(200);
        cout<< "     | |   | |__    __ _  _ __  | | __ ___     | |__  ___   _ __    | |  | | ___  _  _ __    __ _   | |\n";
        Sleep(200);
        cout<< "     | |   | '_ \\  / _` || '_ \\ | |/ // __|    |  __|/ _ \\ | '__|   | |  | |/ __|| || '_ \\  / _` |  | |\n";
        Sleep(200);
        cout<< "     | |   | | | || (_| || | | ||   < \\__ \\    | |  | (_) || |      | |__| |\\__ \\| || | | || (_| |  |_|\n";
        Sleep(200);
        cout<< "     |_|   |_| |_| \\__,_||_| |_||_|\\_\\|___/    |_|   \\___/ |_|       \\____/ |___/|_||_| |_| \\__, |  (_)\n";
        Sleep(200);
        cout<< "                                                                                             __/ |     \n";
        Sleep(200);
        cout<< "                                                                                            |___/     \n";
        Sleep(200);
        cout<< endl;
        cout<< endl;
        cout<< endl;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        return 1;
    }
    else
    {
        cout << "Invalid Command! Exit" << endl;
        for (auto _ = 0; _ < 6; _++)
        {
            Sleep(500);
            cout << ".";
        }
        cout << endl;
        exit(EXIT_FAILURE);
        return 0;
    }
}

/********************/
// helper functions //
/********************/

void print_head()
{
    cout << endl
         << endl
         << endl
         << " ------ Welcome to Auto-Hospital Registration and Appointment Organization System ! ------ "
         << endl
         << endl
         << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);

    Sleep(500);

    cout << "Loading";
    for (auto _ = 0; _ < 6; _++)
    {
        Sleep(250);
        cout << ".";
    }

    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "                            ________  ______    ___   _____                          \n";
    Sleep(250);
    cout << "                           / ____/  |/  /   |  /   | / ___/                          \n";
    Sleep(250);
    cout << "                          / /   / /|_/ / /| | / /| | \\__ \\                           \n";
    Sleep(250);
    cout << "                         / /___/ /  / / ___ |/ ___ |___/ /                           \n";
    Sleep(250);
    cout << "                         \\____/_/  /_/_/  |_/_/  |_/____/                            \n";
    Sleep(250);
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);

    cout << "Development Unit: CS225 Computing Assignment 1" << endl;
    cout << "Developers: Group 4" << endl;
    cout << "\tKong Zitai      3190110306" << endl;
    cout << "\tLi Zihao        3190110098" << endl;
    cout << "\tSong Yifei      3190110099" << endl;
    cout << "\tQi Zhenting     3190112155" << endl
         << endl
         << endl
         << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    cout << "*********************************** NOTICE ******************************************" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    cout << "   This program just analogs a simplified real situation of a city's medicine system.\n";
    cout << "The prepared input files are registry_n.csv, since the program is established on these,\n";
    cout << "please do not change these files. Also, the program will run automatically once you\n";
    cout << "execute it. It will give you several chances to test it by giving withdraw proposal,\n";
    cout << "ddl letter, and change properties. The weekly and monthly reports will be shown in the\n";
    cout << "txt files in the ame directory.\n\n";

    cout << "    During the process of running, the fibonacci heap will be visualized in the form of \na vertical forest";
    cout << "The first column is the root list and the second column are the children of\n the nodes in the root list and so on so forth.\n\n";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    cout << "********************************** SETTINGS ****************************************" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    cout << "   There are 2500 patients, 3 hospitals and 5 registries in total. Every 12 hours \n";
    cout << "there will be 5 patients making registrations in each registry, 25 in total. Each hospital\n";
    cout << "can treat 8 person per day.\n";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    cout << "************************************************************************************\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    cout << endl
         << endl
         << endl;
}
//Main function ends

string *get_birth(string i)
{
    string *ret = new string[3];
    int counter = 0;
    for (int j = 0; j < 3; j++)
    {
        string temp = "";
        while (i[counter] != '\0')
        {
            if (i[counter] == '/')
            {
                counter++;
                break;
            }
            temp += i[counter];
            counter++;
        }
        ret[j] = temp;
    }
    return ret;
}

string *split(string i)
{
    string *ret = new string[20];
    int counter = 0;
    for (int j = 0; j < 14; j++)
    {
        string temp = "";
        while (i[counter] != '\0')
        {
            if (i[counter] == ',')
            {
                counter++;
                break;
            }
            temp += i[counter];
            counter++;
        }
        ret[j] = temp;
    }
    return ret;
}

int hospital_num(person ***hospital)
{
    int count = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (hospital[i][j] != NULL)
            {
                count++;
            }
        }
    }
    return count;
}

person *find_person(FibHeap<long long> &Fib, person ***hospital, long long id)
{
    if (Fib_find_person(Fib, id) != NULL)
    {
        return Fib_find_person(Fib, id);
    }
    if (Hospital_find_person(hospital, id) != NULL)
    {
        return Hospital_find_person(hospital, id);
    }
    return NULL;
}

person *find_person(FibHeap<long long> &Fib, person ***hospital, AList<person *> archive, AList<person *> risk_2_archive, AList<person *> withdraw_reinsertion_archive, long long id)
{
    if (Fib_find_person(Fib, id) != NULL)
    {
        return Fib_find_person(Fib, id);
    }
    else if (Hospital_find_person(hospital, id) != NULL)
    {
        return Hospital_find_person(hospital, id);
    }
    else if (archive_find_person(archive, id) != NULL)
    {
        return archive_find_person(archive, id);
    }
    else if (archive_find_person(risk_2_archive, id) != NULL)
    {
        return archive_find_person(risk_2_archive, id);
    }
    else if (archive_find_person(withdraw_reinsertion_archive, id) != NULL)
    {
        return archive_find_person(withdraw_reinsertion_archive, id);
    }
    return NULL;
}

person *find_person(FibHeap<long long> &Fib, person ***hospital, AList<person *> risk_2_archive, AList<person *> withdraw_reinsertion_archive, long long id)
{
    if (Fib_find_person(Fib, id) != NULL)
    {
        return Fib_find_person(Fib, id);
    }
    else if (Hospital_find_person(hospital, id) != NULL)
    {
        return Hospital_find_person(hospital, id);
    }
    else if (archive_find_person(risk_2_archive, id) != NULL)
    {
        return archive_find_person(risk_2_archive, id);
    }
    else if (archive_find_person(withdraw_reinsertion_archive, id) != NULL)
    {
        return archive_find_person(withdraw_reinsertion_archive, id);
    }
    return NULL;
}

person *Fib_find_person(FibHeap<long long> &Fib, long long id)
{
    if (Fib.Find_ID(Fib.H, id) == NULL)
    {
        return NULL;
    }
    return &((Fib.Find_ID(Fib.H, id))->p);
}

person *Hospital_find_person(person ***hospital, long long id)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (hospital[i][j] != NULL)
            {
                if ((*(hospital[i][j])).id == id)
                {
                    return hospital[i][j];
                }
            }
        }
    }
    return NULL;
}

person *archive_find_person(AList<person *> archive, long long id)
{
    for (int i = 0; i < archive.getlength(); i++)
    {
        if (archive[i + 1]->id == id)
        {
            return archive[i + 1];
        }
    }
    return NULL;
}

bool removePatient(FibHeap<long long> &Fib, person ***hospital, long long id)
{
    bool check;

    check = removeFromHeap(Fib, id);
    if (check)
    {
        cout << "Find the guy with id: " << id << " in Fib-Heap, drive him away." << endl;
        return true;
    }
    else
    {
        check = removeFromHospital(hospital, id);
        if (check)
        {
            cout << "Find the guy with id: " << id << " in hospital, drive him away." << endl;
            return true;
        }
        else
        {
            cout << "Cannot find the guy with id: " << id << endl;
            return false;
        }
    }
}

bool removePatient(FibHeap<long long> &Fib, person ***hospital, AList<person *> &middle_risk_archive, AList<person *> &withdraw_reinsertion_archive, long long id)
{
    if (removeFromHeap(Fib, id))
    {
        cout << "Find the guy with id: " << id << " in Fib-Heap, drive him away." << endl;
        return true;
    }
    else if (removeFromHospital(hospital, id))
    {
        cout << "Find the guy with id: " << id << " in hospital, drive him away." << endl;
        return true;
    }
    else if (whether_in_archive(middle_risk_archive, id))
    {
        cout << "Find the guy with id: " << id << " in the risk 2 buffer, drive him away." << endl;
        archive_remove_by_id(middle_risk_archive, id);
        return true;
    }
    else if (whether_in_archive(withdraw_reinsertion_archive, id))
    {
        cout << "Find the guy with id: " << id << " in the reinsertion buffer, drive him away." << endl;
        archive_remove_by_id(withdraw_reinsertion_archive, id);
        return true;
    }
    else
    {
        cout << "Cannot find the guy with id: " << id << endl;
        return false;
    }
}

bool removePatient(FibHeap<long long> &Fib, person ***hospital, AList<person *> &archive, AList<person *> &middle_risk_archive, AList<person *> &withdraw_reinsertion_archive, long long id)
{
    if (removeFromHeap(Fib, id))
    {
        cout << "Find the guy with id: " << id << " in Fib-Heap, drive him away." << endl;
        return true;
    }
    else if (removeFromHospital(hospital, id))
    {
        cout << "Find the guy with id: " << id << " in hospital, drive him away." << endl;
        return true;
    }
    else if (whether_in_archive(archive, id))
    {
        cout << "Find the guy with id: " << id << " in the treated archive, drive him away." << endl;
        archive_remove_by_id(archive, id);
        return true;
    }
    else if (whether_in_archive(middle_risk_archive, id))
    {
        cout << "Find the guy with id: " << id << " in the risk 2 buffer, drive him away." << endl;
        archive_remove_by_id(middle_risk_archive, id);
        return true;
    }
    else if (whether_in_archive(withdraw_reinsertion_archive, id))
    {
        cout << "Find the guy with id: " << id << " in the reinsertion buffer, drive him away." << endl;
        archive_remove_by_id(withdraw_reinsertion_archive, id);
        return true;
    }
    else
    {
        cout << "Cannot find the guy with id: " << id << endl;
        return false;
    }
}

bool removeFromHeap(FibHeap<long long> &Fib, long long id)
{
    node<long long> *his_node = Fib.Find_ID(Fib.H, id);
    if (his_node == nullptr)
    {
        return false;
    }

    long long his_key = his_node->p.key;
    Fib.Delete_key(his_key);
    return true;
}

bool removeFromHospital(person ***hospital, long long id)
{
    person *the_guy = Hospital_find_person(hospital, id);
    if (the_guy == NULL)
    {
        return false;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (hospital[i][j] == the_guy)
            {
                hospital[i][j] = NULL;
                return true;
            }
        }
    }
    return true;
}

bool whether_in_archive(AList<person *> &archive, long long id)
{
    bool check = false;
    for (int i = 0; i < archive.getlength(); i++)
    {
        if (archive[i+1]->id == id)
        {
        check = true;
        }
    }
    return check;
}

void archive_remove_by_id(AList<person *> &archive, long long id)
{
    for (int i = 0; i < archive.getlength(); i++)
    {
        if (archive[i+1]->id == id)
        {
        archive.remove(i+1);
        break;
        }
    }
}