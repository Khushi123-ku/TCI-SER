#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <direct.h>
#include <windows.h>

using namespace std;

// ============================================================
// SER SECURITY MANAGEMENT SYSTEM
// Companies:
// 1. Reliable
// 2. Prakash
// 3. G&G
// 4. Sumangla
//
// Compatible with older GCC / MinGW
// No <filesystem>
// ============================================================

const string DATA_FOLDER = "data";
const string WEBSITE_FOLDER = "website";
const string REPORT_FOLDER = "reports";

const string VEHICLE_FILE = "data/vehicle_records.csv";
const string VISITOR_FILE = "data/visitor_records.csv";


// ============================================================
// STRUCTURES
// ============================================================

struct VehicleRecord
{
    string vehicleNumber;
    string company;
    string driverName;
    string driverMobile;
    string purpose;

    string entryDate;
    string entryTime;

    string exitDate;
    string exitTime;

    string status;
    string securityGuard;
    string remarks;
};


struct VisitorRecord
{
    string visitorName;
    string mobile;
    string idType;
    string idNumber;

    string company;
    string personToMeet;
    string department;
    string purpose;

    string entryDate;
    string entryTime;

    string exitDate;
    string exitTime;

    string status;
    string securityGuard;
    string remarks;
};


vector<VehicleRecord> vehicles;
vector<VisitorRecord> visitors;


// ============================================================
// UTILITY FUNCTIONS
// ============================================================

string trim(const string &s)
{
    size_t start = s.find_first_not_of(" \t\r\n");

    if (start == string::npos)
        return "";

    size_t end = s.find_last_not_of(" \t\r\n");

    return s.substr(start, end - start + 1);
}


string csvSafe(string text)
{
    for (size_t i = 0; i < text.length(); i++)
    {
        if (text[i] == ',')
            text[i] = ' ';
    }

    return text;
}


string htmlSafe(string text)
{
    string result;

    for (size_t i = 0; i < text.length(); i++)
    {
        char c = text[i];

        if (c == '&')
            result += "&amp;";
        else if (c == '<')
            result += "&lt;";
        else if (c == '>')
            result += "&gt;";
        else if (c == '"')
            result += "&quot;";
        else
            result += c;
    }

    return result;
}


string currentDate()
{
    time_t now = time(0);
    tm *local = localtime(&now);

    stringstream ss;

    ss << setfill('0')
       << setw(2) << local->tm_mday
       << "-"
       << setw(2) << local->tm_mon + 1
       << "-"
       << local->tm_year + 1900;

    return ss.str();
}


string currentTime()
{
    time_t now = time(0);
    tm *local = localtime(&now);

    stringstream ss;

    int hour = local->tm_hour;
    int minute = local->tm_min;

    string ampm = "AM";

    if (hour >= 12)
        ampm = "PM";

    int displayHour = hour % 12;

    if (displayHour == 0)
        displayHour = 12;

    ss << setfill('0')
       << setw(2) << displayHour
       << ":"
       << setw(2) << minute
       << " "
       << ampm;

    return ss.str();
}


bool validMobile(string mobile)
{
    if (mobile.length() != 10)
        return false;

    if (mobile[0] == '0')
        return false;

    for (size_t i = 0; i < mobile.length(); i++)
    {
        if (mobile[i] < '0' || mobile[i] > '9')
            return false;
    }

    return true;
}


bool isValidCompany(string company)
{
    return company == "Reliable" ||
           company == "Prakash" ||
           company == "G&G" ||
           company == "Sumangla";
}


string chooseCompany()
{
    int choice;

    while (true)
    {
        cout << "\n----------------------------------------\n";
        cout << " SELECT COMPANY\n";
        cout << "----------------------------------------\n";
        cout << "1. Reliable\n";
        cout << "2. Prakash\n";
        cout << "3. G&G\n";
        cout << "4. Sumangla\n";
        cout << "Enter choice: ";

        cin >> choice;
        cin.ignore(10000, '\n');

        if (choice == 1) return "Reliable";
        if (choice == 2) return "Prakash";
        if (choice == 3) return "G&G";
        if (choice == 4) return "Sumangla";

        cout << "Invalid choice. Try again.\n";
    }
}


string inputLine(string message)
{
    string value;

    cout << message;
    getline(cin, value);

    return trim(value);
}


// ============================================================
// CREATE FOLDERS
// ============================================================

void createFolders()
{
    _mkdir(DATA_FOLDER.c_str());
    _mkdir(WEBSITE_FOLDER.c_str());
    _mkdir(REPORT_FOLDER.c_str());
}


// ============================================================
// CREATE CSV FILES
// ============================================================

void createCSVFiles()
{
    ifstream vehicleCheck(VEHICLE_FILE.c_str());

    if (!vehicleCheck.good())
    {
        ofstream file(VEHICLE_FILE.c_str());

        file << "Vehicle Number,Company,Driver Name,Driver Mobile,"
             << "Purpose,Entry Date,Entry Time,Exit Date,Exit Time,"
             << "Status,Security Guard,Remarks\n";

        file.close();
    }

    vehicleCheck.close();


    ifstream visitorCheck(VISITOR_FILE.c_str());

    if (!visitorCheck.good())
    {
        ofstream file(VISITOR_FILE.c_str());

        file << "Visitor Name,Mobile,ID Type,ID Number,"
             << "Company,Person To Meet,Department,Purpose,"
             << "Entry Date,Entry Time,Exit Date,Exit Time,"
             << "Status,Security Guard,Remarks\n";

        file.close();
    }

    visitorCheck.close();
}


// ============================================================
// CSV SPLIT
// ============================================================

vector<string> splitCSV(string line)
{
    vector<string> result;
    string item;
    stringstream ss(line);

    while (getline(ss, item, ','))
    {
        result.push_back(trim(item));
    }

    return result;
}


// ============================================================
// LOAD VEHICLE RECORDS
// ============================================================

void loadVehicles()
{
    vehicles.clear();

    ifstream file(VEHICLE_FILE.c_str());

    if (!file.is_open())
        return;

    string line;

    getline(file, line);

    while (getline(file, line))
    {
        if (trim(line).empty())
            continue;

        vector<string> data = splitCSV(line);

        if (data.size() < 12)
            continue;

        VehicleRecord v;

        v.vehicleNumber = data[0];
        v.company = data[1];
        v.driverName = data[2];
        v.driverMobile = data[3];
        v.purpose = data[4];
        v.entryDate = data[5];
        v.entryTime = data[6];
        v.exitDate = data[7];
        v.exitTime = data[8];
        v.status = data[9];
        v.securityGuard = data[10];
        v.remarks = data[11];

        vehicles.push_back(v);
    }

    file.close();
}


// ============================================================
// LOAD VISITOR RECORDS
// ============================================================

void loadVisitors()
{
    visitors.clear();

    ifstream file(VISITOR_FILE.c_str());

    if (!file.is_open())
        return;

    string line;

    getline(file, line);

    while (getline(file, line))
    {
        if (trim(line).empty())
            continue;

        vector<string> data = splitCSV(line);

        if (data.size() < 15)
            continue;

        VisitorRecord v;

        v.visitorName = data[0];
        v.mobile = data[1];
        v.idType = data[2];
        v.idNumber = data[3];
        v.company = data[4];
        v.personToMeet = data[5];
        v.department = data[6];
        v.purpose = data[7];
        v.entryDate = data[8];
        v.entryTime = data[9];
        v.exitDate = data[10];
        v.exitTime = data[11];
        v.status = data[12];
        v.securityGuard = data[13];
        v.remarks = data[14];

        visitors.push_back(v);
    }

    file.close();
}


// ============================================================
// SAVE ALL VEHICLES
// ============================================================

void saveVehicles()
{
    ofstream file(VEHICLE_FILE.c_str());

    file << "Vehicle Number,Company,Driver Name,Driver Mobile,"
         << "Purpose,Entry Date,Entry Time,Exit Date,Exit Time,"
         << "Status,Security Guard,Remarks\n";

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        VehicleRecord &v = vehicles[i];

        file << csvSafe(v.vehicleNumber) << ","
             << csvSafe(v.company) << ","
             << csvSafe(v.driverName) << ","
             << csvSafe(v.driverMobile) << ","
             << csvSafe(v.purpose) << ","
             << csvSafe(v.entryDate) << ","
             << csvSafe(v.entryTime) << ","
             << csvSafe(v.exitDate) << ","
             << csvSafe(v.exitTime) << ","
             << csvSafe(v.status) << ","
             << csvSafe(v.securityGuard) << ","
             << csvSafe(v.remarks)
             << "\n";
    }

    file.close();
}


// ============================================================
// SAVE ALL VISITORS
// ============================================================

void saveVisitors()
{
    ofstream file(VISITOR_FILE.c_str());

    file << "Visitor Name,Mobile,ID Type,ID Number,"
         << "Company,Person To Meet,Department,Purpose,"
         << "Entry Date,Entry Time,Exit Date,Exit Time,"
         << "Status,Security Guard,Remarks\n";

    for (size_t i = 0; i < visitors.size(); i++)
    {
        VisitorRecord &v = visitors[i];

        file << csvSafe(v.visitorName) << ","
             << csvSafe(v.mobile) << ","
             << csvSafe(v.idType) << ","
             << csvSafe(v.idNumber) << ","
             << csvSafe(v.company) << ","
             << csvSafe(v.personToMeet) << ","
             << csvSafe(v.department) << ","
             << csvSafe(v.purpose) << ","
             << csvSafe(v.entryDate) << ","
             << csvSafe(v.entryTime) << ","
             << csvSafe(v.exitDate) << ","
             << csvSafe(v.exitTime) << ","
             << csvSafe(v.status) << ","
             << csvSafe(v.securityGuard) << ","
             << csvSafe(v.remarks)
             << "\n";
    }

    file.close();
}


// ============================================================
// VEHICLE ENTRY
// ============================================================

void addVehicle()
{
    VehicleRecord v;

    cout << "\n========================================\n";
    cout << "          VEHICLE / TRUCK ENTRY\n";
    cout << "========================================\n";

    v.vehicleNumber = inputLine("Vehicle / Truck Number: ");

    if (v.vehicleNumber.empty())
    {
        cout << "Vehicle number cannot be empty.\n";
        return;
    }

    v.company = chooseCompany();

    v.driverName = inputLine("Driver Name: ");

    while (true)
    {
        v.driverMobile = inputLine("Driver Mobile (10 digits): ");

        if (validMobile(v.driverMobile))
            break;

        cout << "Invalid mobile number. Please enter 10 digits and first digit cannot be 0.\n";
    }

    v.purpose = inputLine("Purpose: ");

    cout << "\nUse current date/time?\n";
    cout << "1. Yes\n";
    cout << "2. Enter manually\n";
    cout << "Choice: ";

    int choice;
    cin >> choice;
    cin.ignore(10000, '\n');

    if (choice == 1)
    {
        v.entryDate = currentDate();
        v.entryTime = currentTime();
    }
    else
    {
        v.entryDate = inputLine("Entry Date (DD-MM-YYYY): ");
        v.entryTime = inputLine("Entry Time (HH:MM AM/PM): ");
    }

    v.exitDate = "";
    v.exitTime = "";

    v.status = "INSIDE";

    v.securityGuard = inputLine("Security Guard Name: ");

    v.remarks = inputLine("Remarks: ");

    vehicles.push_back(v);

    saveVehicles();

    cout << "\nVehicle entry saved successfully.\n";
}


// ============================================================
// VEHICLE EXIT
// ============================================================

void vehicleExit()
{
    string number = inputLine("\nEnter Vehicle / Truck Number for EXIT: ");

    bool found = false;

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        if (vehicles[i].vehicleNumber == number &&
            vehicles[i].status == "INSIDE")
        {
            vehicles[i].exitDate = currentDate();
            vehicles[i].exitTime = currentTime();
            vehicles[i].status = "EXITED";

            saveVehicles();

            cout << "\nVehicle EXIT recorded successfully.\n";
            cout << "Exit Date : " << vehicles[i].exitDate << "\n";
            cout << "Exit Time : " << vehicles[i].exitTime << "\n";

            found = true;
            break;
        }
    }

    if (!found)
        cout << "\nNo INSIDE vehicle found with this number.\n";
}


// ============================================================
// VISITOR ENTRY
// ============================================================

void addVisitor()
{
    VisitorRecord v;

    cout << "\n========================================\n";
    cout << "          VISITOR / UNKNOWN ENTRY\n";
    cout << "========================================\n";

    v.visitorName = inputLine("Visitor Name: ");

    if (v.visitorName.empty())
    {
        cout << "Visitor name cannot be empty.\n";
        return;
    }

    while (true)
    {
        v.mobile = inputLine("Mobile (10 digits): ");

        if (validMobile(v.mobile))
            break;

        cout << "Invalid mobile number. Please enter 10 digits and first digit cannot be 0.\n";
    }

    v.idType = inputLine("ID Type (Aadhaar/DL/PAN/Other): ");
    v.idNumber = inputLine("ID Number: ");

    v.company = chooseCompany();

    v.personToMeet = inputLine("Person To Meet: ");
    v.department = inputLine("Department: ");
    v.purpose = inputLine("Purpose: ");

    cout << "\nUse current date/time?\n";
    cout << "1. Yes\n";
    cout << "2. Enter manually\n";
    cout << "Choice: ";

    int choice;

    cin >> choice;
    cin.ignore(10000, '\n');

    if (choice == 1)
    {
        v.entryDate = currentDate();
        v.entryTime = currentTime();
    }
    else
    {
        v.entryDate = inputLine("Entry Date (DD-MM-YYYY): ");
        v.entryTime = inputLine("Entry Time (HH:MM AM/PM): ");
    }

    v.exitDate = "";
    v.exitTime = "";

    v.status = "INSIDE";

    v.securityGuard = inputLine("Security Guard Name: ");

    v.remarks = inputLine("Remarks: ");

    visitors.push_back(v);

    saveVisitors();

    cout << "\nVisitor entry saved successfully.\n";
}


// ============================================================
// VISITOR EXIT
// ============================================================

void visitorExit()
{
    string name = inputLine("\nEnter Visitor Name for EXIT: ");

    bool found = false;

    for (size_t i = 0; i < visitors.size(); i++)
    {
        if (visitors[i].visitorName == name &&
            visitors[i].status == "INSIDE")
        {
            visitors[i].exitDate = currentDate();
            visitors[i].exitTime = currentTime();
            visitors[i].status = "EXITED";

            saveVisitors();

            cout << "\nVisitor EXIT recorded successfully.\n";
            cout << "Exit Date : " << visitors[i].exitDate << "\n";
            cout << "Exit Time : " << visitors[i].exitTime << "\n";

            found = true;
            break;
        }
    }

    if (!found)
        cout << "\nNo INSIDE visitor found with this name.\n";
}


// ============================================================
// SHOW VEHICLES
// ============================================================

void showVehicles()
{
    cout << "\n============================================================\n";
    cout << "                  VEHICLE REGISTER\n";
    cout << "============================================================\n";

    if (vehicles.empty())
    {
        cout << "No vehicle records found.\n";
        return;
    }

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        VehicleRecord &v = vehicles[i];

        cout << "\nRecord #" << i + 1 << "\n";
        cout << "Vehicle       : " << v.vehicleNumber << "\n";
        cout << "Company       : " << v.company << "\n";
        cout << "Driver        : " << v.driverName << "\n";
        cout << "Mobile        : " << v.driverMobile << "\n";
        cout << "Purpose       : " << v.purpose << "\n";
        cout << "Entry         : " << v.entryDate << " " << v.entryTime << "\n";
        cout << "Exit          : ";

        if (v.exitDate.empty())
            cout << "-\n";
        else
            cout << v.exitDate << " " << v.exitTime << "\n";

        cout << "Status        : " << v.status << "\n";
        cout << "Security Guard: " << v.securityGuard << "\n";
        cout << "Remarks       : " << v.remarks << "\n";
    }
}


// ============================================================
// SHOW VISITORS
// ============================================================

void showVisitors()
{
    cout << "\n============================================================\n";
    cout << "                  VISITOR REGISTER\n";
    cout << "============================================================\n";

    if (visitors.empty())
    {
        cout << "No visitor records found.\n";
        return;
    }

    for (size_t i = 0; i < visitors.size(); i++)
    {
        VisitorRecord &v = visitors[i];

        cout << "\nRecord #" << i + 1 << "\n";
        cout << "Visitor       : " << v.visitorName << "\n";
        cout << "Mobile        : " << v.mobile << "\n";
        cout << "ID Type       : " << v.idType << "\n";
        cout << "ID Number     : " << v.idNumber << "\n";
        cout << "Company       : " << v.company << "\n";
        cout << "Person To Meet: " << v.personToMeet << "\n";
        cout << "Department    : " << v.department << "\n";
        cout << "Purpose       : " << v.purpose << "\n";
        cout << "Entry         : " << v.entryDate << " " << v.entryTime << "\n";
        cout << "Exit          : ";

        if (v.exitDate.empty())
            cout << "-\n";
        else
            cout << v.exitDate << " " << v.exitTime << "\n";

        cout << "Status        : " << v.status << "\n";
        cout << "Security Guard: " << v.securityGuard << "\n";
        cout << "Remarks       : " << v.remarks << "\n";
    }
}


// ============================================================
// SEARCH BY DATE
// ============================================================

void searchByDate()
{
    string date = inputLine("\nEnter Date (DD-MM-YYYY): ");

    bool found = false;

    cout << "\n================ VEHICLE RECORDS ================\n";

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        VehicleRecord &v = vehicles[i];

        if (v.entryDate == date)
        {
            cout << "\nVehicle: " << v.vehicleNumber
                 << " | Company: " << v.company
                 << " | Driver: " << v.driverName
                 << " | Entry: " << v.entryTime
                 << " | Status: " << v.status;

            found = true;
        }
    }

    cout << "\n\n================ VISITOR RECORDS ================\n";

    for (size_t i = 0; i < visitors.size(); i++)
    {
        VisitorRecord &v = visitors[i];

        if (v.entryDate == date)
        {
            cout << "\nVisitor: " << v.visitorName
                 << " | Company: " << v.company
                 << " | Entry: " << v.entryTime
                 << " | Status: " << v.status;

            found = true;
        }
    }

    if (!found)
        cout << "\nNo records found for this date.\n";

    cout << "\n";
}


// ============================================================
// SEARCH VEHICLE
// ============================================================

void searchVehicle()
{
    string number = inputLine("\nEnter Vehicle Number: ");

    bool found = false;

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        if (vehicles[i].vehicleNumber == number)
        {
            VehicleRecord &v = vehicles[i];

            cout << "\nVehicle Number : " << v.vehicleNumber;
            cout << "\nCompany        : " << v.company;
            cout << "\nDriver         : " << v.driverName;
            cout << "\nMobile         : " << v.driverMobile;
            cout << "\nPurpose        : " << v.purpose;
            cout << "\nEntry          : " << v.entryDate << " " << v.entryTime;
            cout << "\nExit           : " << v.exitDate << " " << v.exitTime;
            cout << "\nStatus         : " << v.status;
            cout << "\nSecurity Guard : " << v.securityGuard;
            cout << "\nRemarks        : " << v.remarks << "\n";

            found = true;
        }
    }

    if (!found)
        cout << "\nVehicle not found.\n";
}


// ============================================================
// COMPANY REPORT
// ============================================================

void companyReport()
{
    string company = chooseCompany();

    cout << "\n============================================\n";
    cout << "       " << company << " COMPANY REPORT\n";
    cout << "============================================\n";

    int vehicleCount = 0;
    int vehicleInside = 0;

    int visitorCount = 0;
    int visitorInside = 0;

    cout << "\nVEHICLES:\n";

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        if (vehicles[i].company == company)
        {
            vehicleCount++;

            if (vehicles[i].status == "INSIDE")
                vehicleInside++;

            cout << "\n"
                 << vehicles[i].vehicleNumber
                 << " | "
                 << vehicles[i].driverName
                 << " | "
                 << vehicles[i].entryDate
                 << " "
                 << vehicles[i].entryTime
                 << " | "
                 << vehicles[i].status;
        }
    }

    cout << "\n\nVISITORS:\n";

    for (size_t i = 0; i < visitors.size(); i++)
    {
        if (visitors[i].company == company)
        {
            visitorCount++;

            if (visitors[i].status == "INSIDE")
                visitorInside++;

            cout << "\n"
                 << visitors[i].visitorName
                 << " | "
                 << visitors[i].personToMeet
                 << " | "
                 << visitors[i].entryDate
                 << " "
                 << visitors[i].entryTime
                 << " | "
                 << visitors[i].status;
        }
    }

    cout << "\n\n--------------------------------------------\n";
    cout << "Total Vehicles : " << vehicleCount << "\n";
    cout << "Vehicles Inside: " << vehicleInside << "\n";
    cout << "Total Visitors : " << visitorCount << "\n";
    cout << "Visitors Inside: " << visitorInside << "\n";
}


// ============================================================
// DASHBOARD COUNTS
// ============================================================

int totalVehicles()
{
    return (int)vehicles.size();
}


int vehiclesInside()
{
    int count = 0;

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        if (vehicles[i].status == "INSIDE")
            count++;
    }

    return count;
}


int totalVisitors()
{
    return (int)visitors.size();
}


int visitorsInside()
{
    int count = 0;

    for (size_t i = 0; i < visitors.size(); i++)
    {
        if (visitors[i].status == "INSIDE")
            count++;
    }

    return count;
}


int companyVehicleCount(string company)
{
    int count = 0;

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        if (vehicles[i].company == company)
            count++;
    }

    return count;
}


int companyVisitorCount(string company)
{
    int count = 0;

    for (size_t i = 0; i < visitors.size(); i++)
    {
        if (visitors[i].company == company)
            count++;
    }

    return count;
}


// ============================================================
// GENERATE CSS
// ============================================================

void generateCSS()
{
    ofstream css("website/style.css");

    css << R"CSS(
* {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
}

body {
    font-family: Arial, Helvetica, sans-serif;
    background: #f4f7fb;
    color: #172033;
}

.header {
    background: linear-gradient(135deg, #0b1f3a, #164e8a);
    color: white;
    padding: 30px 5%;
    box-shadow: 0 5px 20px rgba(0,0,0,.15);
}

.header h1 {
    font-size: 30px;
    margin-bottom: 8px;
}

.header p {
    opacity: .85;
}

.container {
    width: 92%;
    max-width: 1500px;
    margin: 30px auto;
}

.cards {
    display: grid;
    grid-template-columns: repeat(4, 1fr);
    gap: 18px;
    margin-bottom: 25px;
}

.card {
    background: white;
    border-radius: 15px;
    padding: 22px;
    box-shadow: 0 5px 18px rgba(0,0,0,.08);
    border-left: 5px solid #164e8a;
}

.card h3 {
    color: #667085;
    font-size: 14px;
    margin-bottom: 12px;
}

.card .number {
    font-size: 30px;
    font-weight: bold;
}

.section {
    background: white;
    padding: 24px;
    border-radius: 15px;
    margin-bottom: 25px;
    box-shadow: 0 5px 18px rgba(0,0,0,.07);
}

.section h2 {
    margin-bottom: 18px;
}

.filters {
    display: grid;
    grid-template-columns: repeat(4, 1fr);
    gap: 12px;
    margin-bottom: 20px;
}

input, select {
    width: 100%;
    padding: 12px;
    border: 1px solid #d5dbe5;
    border-radius: 8px;
    outline: none;
}

input:focus, select:focus {
    border-color: #164e8a;
}

.table-wrapper {
    overflow-x: auto;
}

table {
    width: 100%;
    border-collapse: collapse;
    min-width: 1100px;
}

th {
    background: #0b1f3a;
    color: white;
    padding: 13px;
    text-align: left;
    white-space: nowrap;
}

td {
    padding: 12px;
    border-bottom: 1px solid #e7ebf0;
    white-space: nowrap;
}

tr:hover {
    background: #f7faff;
}

.badge {
    display: inline-block;
    padding: 6px 10px;
    border-radius: 20px;
    font-size: 12px;
    font-weight: bold;
}

.inside {
    background: #dff7e8;
    color: #18743a;
}

.exited {
    background: #eef0f3;
    color: #596273;
}

.company-grid {
    display: grid;
    grid-template-columns: repeat(4, 1fr);
    gap: 15px;
}

.company-box {
    background: #f7f9fc;
    border: 1px solid #e1e6ee;
    border-radius: 12px;
    padding: 18px;
}

.company-box h3 {
    margin-bottom: 10px;
}

.company-box p {
    margin: 6px 0;
    color: #667085;
}

.footer {
    text-align: center;
    padding: 25px;
    color: #667085;
}

.no-print {
    display: block;
}

@media (max-width: 1000px) {
    .cards {
        grid-template-columns: repeat(2, 1fr);
    }

    .filters {
        grid-template-columns: repeat(2, 1fr);
    }

    .company-grid {
        grid-template-columns: repeat(2, 1fr);
    }
}

@media (max-width: 600px) {
    .cards {
        grid-template-columns: 1fr;
    }

    .filters {
        grid-template-columns: 1fr;
    }

    .company-grid {
        grid-template-columns: 1fr;
    }

    .header h1 {
        font-size: 22px;
    }

    .container {
        width: 95%;
    }
}

@media print {
    .no-print {
        display: none !important;
    }

    body {
        background: white;
    }

    .section, .card {
        box-shadow: none;
    }
}
)CSS";

    css.close();
}


// ============================================================
// GENERATE JAVASCRIPT
// ============================================================

void generateJS()
{
    ofstream js("website/script.js");

    js << R"JS(
function filterTables() {

    var search = document.getElementById("globalSearch");

    if (!search)
        return;

    var value = search.value.toLowerCase();

    var rows = document.querySelectorAll("tbody tr");

    rows.forEach(function(row) {

        var text = row.innerText.toLowerCase();

        if (text.indexOf(value) !== -1) {
            row.style.display = "";
        }
        else {
            row.style.display = "none";
        }

    });
}


function filterCompany() {

    var select = document.getElementById("companyFilter");

    if (!select)
        return;

    var company = select.value.toLowerCase();

    var rows = document.querySelectorAll("tbody tr");

    rows.forEach(function(row) {

        var text = row.innerText.toLowerCase();

        if (company === "" || text.indexOf(company) !== -1) {
            row.style.display = "";
        }
        else {
            row.style.display = "none";
        }

    });
}


function filterStatus() {

    var select = document.getElementById("statusFilter");

    if (!select)
        return;

    var status = select.value.toLowerCase();

    var rows = document.querySelectorAll("tbody tr");

    rows.forEach(function(row) {

        var text = row.innerText.toLowerCase();

        if (status === "" || text.indexOf(status) !== -1) {
            row.style.display = "";
        }
        else {
            row.style.display = "none";
        }

    });
}


function clearFilters() {

    var search = document.getElementById("globalSearch");
    var company = document.getElementById("companyFilter");
    var status = document.getElementById("statusFilter");

    if (search)
        search.value = "";

    if (company)
        company.value = "";

    if (status)
        status.value = "";

    var rows = document.querySelectorAll("tbody tr");

    rows.forEach(function(row) {
        row.style.display = "";
    });
}


function printReport() {
    window.print();
}


function applyAllFilters() {

    var searchElement = document.getElementById("globalSearch");
    var companyElement = document.getElementById("companyFilter");
    var statusElement = document.getElementById("statusFilter");

    var search = searchElement ? searchElement.value.toLowerCase() : "";
    var company = companyElement ? companyElement.value.toLowerCase() : "";
    var status = statusElement ? statusElement.value.toLowerCase() : "";

    var rows = document.querySelectorAll("tbody tr");

    rows.forEach(function(row) {

        var text = row.innerText.toLowerCase();

        var searchOK = text.indexOf(search) !== -1;
        var companyOK = company === "" || text.indexOf(company) !== -1;
        var statusOK = status === "" || text.indexOf(status) !== -1;

        row.style.display =
            (searchOK && companyOK && statusOK) ? "" : "none";
    });
}
)JS";

    js.close();
}


// ============================================================
// GENERATE HTML DASHBOARD
// ============================================================

void generateHTML()
{
    ofstream html("website/index.html");

    html << "<!DOCTYPE html>\n";
    html << "<html lang=\"en\">\n";
    html << "<head>\n";
    html << "<meta charset=\"UTF-8\">\n";
    html << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    html << "<title>SER Security Management System</title>\n";
    html << "<link rel=\"stylesheet\" href=\"style.css\">\n";
    html << "</head>\n";

    html << "<body>\n";

    // Header
    html << "<div class=\"header\">\n";
    html << "<h1>SER CAMPUS SECURITY MANAGEMENT SYSTEM</h1>\n";
    html << "<p>Vehicle & Visitor Security Register | Reliable | Prakash | G&G | Sumangla</p>\n";
    html << "</div>\n";

    html << "<div class=\"container\">\n";

    // Dashboard cards
    html << "<div class=\"cards\">\n";

    html << "<div class=\"card\">";
    html << "<h3>TOTAL VEHICLES</h3>";
    html << "<div class=\"number\">" << totalVehicles() << "</div>";
    html << "</div>\n";

    html << "<div class=\"card\">";
    html << "<h3>VEHICLES INSIDE</h3>";
    html << "<div class=\"number\">" << vehiclesInside() << "</div>";
    html << "</div>\n";

    html << "<div class=\"card\">";
    html << "<h3>TOTAL VISITORS</h3>";
    html << "<div class=\"number\">" << totalVisitors() << "</div>";
    html << "</div>\n";

    html << "<div class=\"card\">";
    html << "<h3>VISITORS INSIDE</h3>";
    html << "<div class=\"number\">" << visitorsInside() << "</div>";
    html << "</div>\n";

    html << "</div>\n";


    // Company summary
    html << "<div class=\"section\">\n";
    html << "<h2>Company Summary</h2>\n";
    html << "<div class=\"company-grid\">\n";

    string companies[4] =
    {
        "Reliable",
        "Prakash",
        "G&G",
        "Sumangla"
    };

    for (int i = 0; i < 4; i++)
    {
        html << "<div class=\"company-box\">\n";

        html << "<h3>" << companies[i] << "</h3>";

        html << "<p>Vehicles: "
             << companyVehicleCount(companies[i])
             << "</p>";

        html << "<p>Visitors: "
             << companyVisitorCount(companies[i])
             << "</p>";

        html << "</div>\n";
    }

    html << "</div>\n";
    html << "</div>\n";


    // Filters
    html << "<div class=\"section no-print\">\n";
    html << "<h2>Search & Filters</h2>\n";

    html << "<div class=\"filters\">\n";

    html << "<input id=\"globalSearch\" "
         << "onkeyup=\"applyAllFilters()\" "
         << "placeholder=\"Search vehicle, visitor, driver, mobile...\">";

    html << "<select id=\"companyFilter\" onchange=\"applyAllFilters()\">";
    html << "<option value=\"\">All Companies</option>";
    html << "<option value=\"Reliable\">Reliable</option>";
    html << "<option value=\"Prakash\">Prakash</option>";
    html << "<option value=\"G&G\">G&G</option>";
    html << "<option value=\"Sumangla\">Sumangla</option>";
    html << "</select>";

    html << "<select id=\"statusFilter\" onchange=\"applyAllFilters()\">";
    html << "<option value=\"\">All Status</option>";
    html << "<option value=\"INSIDE\">INSIDE</option>";
    html << "<option value=\"EXITED\">EXITED</option>";
    html << "</select>";

    html << "<button onclick=\"clearFilters()\" "
         << "style=\"border:0;border-radius:8px;background:#0b1f3a;color:white;font-weight:bold;cursor:pointer;\">"
         << "Clear Filters</button>";

    html << "</div>\n";
    html << "</div>\n";


    // Vehicle register
    html << "<div class=\"section\">\n";
    html << "<h2>Vehicle / Truck Register</h2>\n";
    html << "<div class=\"table-wrapper\">\n";

    html << "<table>\n";
    html << "<thead><tr>";

    html << "<th>Vehicle No.</th>";
    html << "<th>Company</th>";
    html << "<th>Driver</th>";
    html << "<th>Mobile</th>";
    html << "<th>Purpose</th>";
    html << "<th>Entry</th>";
    html << "<th>Exit</th>";
    html << "<th>Status</th>";
    html << "<th>Security Guard</th>";
    html << "<th>Remarks</th>";

    html << "</tr></thead>\n";
    html << "<tbody>\n";

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        VehicleRecord &v = vehicles[i];

        html << "<tr>";

        html << "<td>" << htmlSafe(v.vehicleNumber) << "</td>";
        html << "<td>" << htmlSafe(v.company) << "</td>";
        html << "<td>" << htmlSafe(v.driverName) << "</td>";
        html << "<td>" << htmlSafe(v.driverMobile) << "</td>";
        html << "<td>" << htmlSafe(v.purpose) << "</td>";

        html << "<td>"
             << htmlSafe(v.entryDate)
             << "<br>"
             << htmlSafe(v.entryTime)
             << "</td>";

        html << "<td>";

        if (v.exitDate.empty())
            html << "-";
        else
            html << htmlSafe(v.exitDate)
                 << "<br>"
                 << htmlSafe(v.exitTime);

        html << "</td>";

        if (v.status == "INSIDE")
            html << "<td><span class=\"badge inside\">INSIDE</span></td>";
        else
            html << "<td><span class=\"badge exited\">EXITED</span></td>";

        html << "<td>" << htmlSafe(v.securityGuard) << "</td>";
        html << "<td>" << htmlSafe(v.remarks) << "</td>";

        html << "</tr>\n";
    }

    html << "</tbody>\n";
    html << "</table>\n";

    html << "</div>\n";
    html << "</div>\n";


    // Visitor register
    html << "<div class=\"section\">\n";
    html << "<h2>Visitor / Unknown Person Register</h2>\n";
    html << "<div class=\"table-wrapper\">\n";

    html << "<table>\n";
    html << "<thead><tr>";

    html << "<th>Visitor</th>";
    html << "<th>Mobile</th>";
    html << "<th>ID Type</th>";
    html << "<th>ID Number</th>";
    html << "<th>Company</th>";
    html << "<th>Person To Meet</th>";
    html << "<th>Department</th>";
    html << "<th>Purpose</th>";
    html << "<th>Entry</th>";
    html << "<th>Exit</th>";
    html << "<th>Status</th>";
    html << "<th>Security Guard</th>";
    html << "<th>Remarks</th>";

    html << "</tr></thead>\n";
    html << "<tbody>\n";

    for (size_t i = 0; i < visitors.size(); i++)
    {
        VisitorRecord &v = visitors[i];

        html << "<tr>";

        html << "<td>" << htmlSafe(v.visitorName) << "</td>";
        html << "<td>" << htmlSafe(v.mobile) << "</td>";
        html << "<td>" << htmlSafe(v.idType) << "</td>";
        html << "<td>" << htmlSafe(v.idNumber) << "</td>";
        html << "<td>" << htmlSafe(v.company) << "</td>";
        html << "<td>" << htmlSafe(v.personToMeet) << "</td>";
        html << "<td>" << htmlSafe(v.department) << "</td>";
        html << "<td>" << htmlSafe(v.purpose) << "</td>";

        html << "<td>"
             << htmlSafe(v.entryDate)
             << "<br>"
             << htmlSafe(v.entryTime)
             << "</td>";

        html << "<td>";

        if (v.exitDate.empty())
            html << "-";
        else
            html << htmlSafe(v.exitDate)
                 << "<br>"
                 << htmlSafe(v.exitTime);

        html << "</td>";

        if (v.status == "INSIDE")
            html << "<td><span class=\"badge inside\">INSIDE</span></td>";
        else
            html << "<td><span class=\"badge exited\">EXITED</span></td>";

        html << "<td>" << htmlSafe(v.securityGuard) << "</td>";
        html << "<td>" << htmlSafe(v.remarks) << "</td>";

        html << "</tr>\n";
    }

    html << "</tbody>\n";
    html << "</table>\n";

    html << "</div>\n";
    html << "</div>\n";


    // Print
    html << "<div class=\"section no-print\">\n";

    html << "<button onclick=\"printReport()\" "
         << "style=\"padding:13px 22px;border:0;border-radius:8px;"
         << "background:#0b1f3a;color:white;font-weight:bold;cursor:pointer;\">"
         << "Print Report"
         << "</button>";

    html << "</div>\n";


    html << "</div>\n";

    html << "<div class=\"footer\">";
    html << "SER Campus Security Management System | Generated by C++";
    html << "</div>\n";

    html << "<script src=\"script.js\"></script>\n";

    html << "</body>\n";
    html << "</html>\n";

    html.close();
}


// ============================================================
// GENERATE CSV REPORT COPIES
// ============================================================

void generateReports()
{
    // Vehicle report
    ofstream vehicleReport("reports/vehicle_report.csv");

    vehicleReport
        << "Vehicle Number,Company,Driver Name,Driver Mobile,"
        << "Purpose,Entry Date,Entry Time,Exit Date,Exit Time,"
        << "Status,Security Guard,Remarks\n";

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        VehicleRecord &v = vehicles[i];

        vehicleReport
            << csvSafe(v.vehicleNumber) << ","
            << csvSafe(v.company) << ","
            << csvSafe(v.driverName) << ","
            << csvSafe(v.driverMobile) << ","
            << csvSafe(v.purpose) << ","
            << csvSafe(v.entryDate) << ","
            << csvSafe(v.entryTime) << ","
            << csvSafe(v.exitDate) << ","
            << csvSafe(v.exitTime) << ","
            << csvSafe(v.status) << ","
            << csvSafe(v.securityGuard) << ","
            << csvSafe(v.remarks) << "\n";
    }

    vehicleReport.close();


    // Visitor report
    ofstream visitorReport("reports/visitor_report.csv");

    visitorReport
        << "Visitor Name,Mobile,ID Type,ID Number,"
        << "Company,Person To Meet,Department,Purpose,"
        << "Entry Date,Entry Time,Exit Date,Exit Time,"
        << "Status,Security Guard,Remarks\n";

    for (size_t i = 0; i < visitors.size(); i++)
    {
        VisitorRecord &v = visitors[i];

        visitorReport
            << csvSafe(v.visitorName) << ","
            << csvSafe(v.mobile) << ","
            << csvSafe(v.idType) << ","
            << csvSafe(v.idNumber) << ","
            << csvSafe(v.company) << ","
            << csvSafe(v.personToMeet) << ","
            << csvSafe(v.department) << ","
            << csvSafe(v.purpose) << ","
            << csvSafe(v.entryDate) << ","
            << csvSafe(v.entryTime) << ","
            << csvSafe(v.exitDate) << ","
            << csvSafe(v.exitTime) << ","
            << csvSafe(v.status) << ","
            << csvSafe(v.securityGuard) << ","
            << csvSafe(v.remarks) << "\n";
    }

    visitorReport.close();
}


// ============================================================
// GENERATE WEBSITE
// ============================================================

void generateWebsite()
{
    generateCSS();
    generateJS();
    generateHTML();
    generateReports();

    cout << "\n============================================\n";
    cout << " WEBSITE GENERATED SUCCESSFULLY\n";
    cout << "============================================\n";

    cout << "HTML : website/index.html\n";
    cout << "CSS  : website/style.css\n";
    cout << "JS   : website/script.js\n";
    cout << "Reports saved in: reports/\n";
}


// ============================================================
// OPEN WEBSITE
// ============================================================

void openWebsite()
{
    generateWebsite();

    string command = "start \"\" \"website\\index.html\"";

    system(command.c_str());
}


// ============================================================
// DASHBOARD SUMMARY
// ============================================================

void dashboardSummary()
{
    string companies[4] =
    {
        "Reliable",
        "Prakash",
        "G&G",
        "Sumangla"
    };

    cout << "\n================================================\n";
    cout << "             SER DASHBOARD SUMMARY\n";
    cout << "================================================\n";

    cout << "\nTOTAL VEHICLES : " << totalVehicles();
    cout << "\nVEHICLES INSIDE: " << vehiclesInside();

    cout << "\n\nTOTAL VISITORS : " << totalVisitors();
    cout << "\nVISITORS INSIDE: " << visitorsInside();

    cout << "\n\n================================================\n";
    cout << "               COMPANY SUMMARY\n";
    cout << "================================================\n";

    for (int i = 0; i < 4; i++)
    {
        cout << "\n" << companies[i];

        cout << "\n  Vehicles : "
             << companyVehicleCount(companies[i]);

        cout << "\n  Visitors : "
             << companyVisitorCount(companies[i]);

        cout << "\n";
    }

    cout << "\n================================================\n";
}

// ============================================================
// FIXED COMPANY ARRAY FOR SUMMARY
// ============================================================

void printCompanySummary()
{
    string companies[4] =
    {
        "Reliable",
        "Prakash",
        "G&G",
        "Sumangla"
    };

    cout << "\n================================================\n";
    cout << "               COMPANY SUMMARY\n";
    cout << "================================================\n";

    for (int i = 0; i < 4; i++)
    {
        cout << "\n" << companies[i] << "\n";

        cout << "  Vehicles : "
             << companyVehicleCount(companies[i]);

        cout << "\n  Visitors : "
             << companyVisitorCount(companies[i]);

        cout << "\n";
    }
}


// ============================================================
// MAIN MENU
// ============================================================

void showMenu()
{
    cout << "\n\n";
    cout << "============================================================\n";
    cout << "        SER CAMPUS SECURITY MANAGEMENT SYSTEM\n";
    cout << "============================================================\n";
    cout << "       Reliable | Prakash | G&G | Sumangla\n";
    cout << "============================================================\n";

    cout << "\n1.  Vehicle / Truck Entry";
    cout << "\n2.  Vehicle / Truck Exit";
    cout << "\n3.  Visitor / Unknown Person Entry";
    cout << "\n4.  Visitor Exit";
    cout << "\n5.  Show Vehicle Register";
    cout << "\n6.  Show Visitor Register";
    cout << "\n7.  Search Records by Date";
    cout << "\n8.  Search Vehicle Number";
    cout << "\n9.  Company-wise Report";
    cout << "\n10. Dashboard Summary";
    cout << "\n11. Generate HTML + CSS + JS Website";
    cout << "\n12. Open Premium HTML Dashboard";
    cout << "\n13. Generate CSV Reports";
    cout << "\n0.  Exit";

    cout << "\n\nEnter Choice: ";
}


// ============================================================
// MAIN
// ============================================================

int main()
{
    system("cls");

    cout << "============================================================\n";
    cout << "        SER CAMPUS SECURITY MANAGEMENT SYSTEM\n";
    cout << "============================================================\n";
    cout << " Reliable | Prakash | G&G | Sumangla\n";
    cout << "============================================================\n";

    // Automatically create folders
    createFolders();

    // Automatically create CSV files
    createCSVFiles();

    // Load old records
    loadVehicles();
    loadVisitors();

    // Automatically generate website when program starts
    generateWebsite();

    cout << "\nExisting Vehicle Records : "
         << vehicles.size();

    cout << "\nExisting Visitor Records : "
         << visitors.size();

    cout << "\n\nHTML website automatically created.";

    int choice;

    while (true)
    {
        showMenu();

        cin >> choice;
        cin.ignore(10000, '\n');

        switch (choice)
        {
            case 1:
                addVehicle();
                generateWebsite();
                break;

            case 2:
                vehicleExit();
                generateWebsite();
                break;

            case 3:
                addVisitor();
                generateWebsite();
                break;

            case 4:
                visitorExit();
                generateWebsite();
                break;

            case 5:
                showVehicles();
                break;

            case 6:
                showVisitors();
                break;

            case 7:
                searchByDate();
                break;

            case 8:
                searchVehicle();
                break;

            case 9:
                companyReport();
                break;

            case 10:
                dashboardSummary();
                printCompanySummary();
                break;

            case 11:
                generateWebsite();
                break;

            case 12:
                openWebsite();
                break;

            case 13:
                generateReports();
                cout << "\nCSV reports generated successfully.\n";
                break;

            case 0:
                cout << "\nSystem closed.\n";
                return 0;

            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    }

    return 0;
}