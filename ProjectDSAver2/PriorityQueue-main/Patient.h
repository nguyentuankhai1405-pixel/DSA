#ifndef _PATIENT_
#define _PATIENT_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include "MedicalHistory.h"

using namespace std;

// =================================================================
// [THAY THẾ & TÍCH HỢP TỪ FILE: patient.h]
// Cấu trúc ngày tháng & Lịch sử khám bệnh dạng Danh sách liên kết
// =================================================================
struct DATE {
    int day;
    int month;
    int year;
};

struct VisitNode {
    DATE visitDate;
    char chandoanbenh[150];
    char thuockedon[150];
    VisitNode* link;
};

struct VisitList {
    VisitNode* first;
    VisitNode* last;
};

class Patient {
public:
    Patient();
    Patient(string id, string name, int code, int order);
    
    // Clean up bộ nhớ cho Lịch sử khám
    ~Patient();
    Patient(const Patient& other); // Copy Constructor
    Patient& operator=(const Patient& other); // Assignment Operator

    string to_string() const;

    // Getters & Setters
    string getID() const { return id; }
    string getName() const { return name; }
    int getPriorityCode() const { return priorityCode; }
    int getArrivalOrder() const { return arrivalOrder; }
    int getCallCount() const { return callCount; }
    MedicalHistoryTree& getMedicalHistory() { return medicalHistory; }

    void setID(string newId) { id = newId; }
    void setName(string newName) { name = newName; }
    void setPriorityCode(int code) { priorityCode = code; }
    void incrementCallCount() { callCount++; }
    void resetCallCount() { callCount = 0; }

    // =================================================================
    // [THAY THẾ & TÍCH HỢP TỪ FILE: patient_array.c]
    // Quản lý Danh sách liên kết Lịch sử khám
    // =================================================================
    void addVisitRecord(DATE d, const char* chandoan, const char* thuoc);
    void printVisitHistory() const;

    bool operator>(const Patient &obj) const;

private:
    string id;
    string name;
    int priorityCode;   // 1=Đỏ, 2=Cam, 3=Vàng, 4=Xanh
    int arrivalOrder;
    int callCount;      // Tương đương missedCalls
    MedicalHistoryTree medicalHistory;
    VisitList visitHistory; // Lịch sử các lần khám bệnh

    void freeVisitList();
    void copyVisitList(const VisitList& otherList);
};

Patient::Patient() {
    id = "BN000";
    name = "";
    priorityCode = 4;
    arrivalOrder = 0;
    callCount = 0;
    visitHistory.first = visitHistory.last = nullptr;
}

Patient::Patient(string id, string name, int code, int order) {
    this->id = id;
    this->name = name;
    this->priorityCode = code;
    this->arrivalOrder = order;
    this->callCount = 0;
    visitHistory.first = visitHistory.last = nullptr;
}

void Patient::freeVisitList() {
    VisitNode* p = visitHistory.first;
    while (p != nullptr) {
        VisitNode* temp = p;
        p = p->link;
        delete temp;
    }
    visitHistory.first = visitHistory.last = nullptr;
}

void Patient::copyVisitList(const VisitList& otherList) {
    visitHistory.first = visitHistory.last = nullptr;
    VisitNode* p = otherList.first;
    while (p != nullptr) {
        addVisitRecord(p->visitDate, p->chandoanbenh, p->thuockedon);
        p = p->link;
    }
}

Patient::~Patient() {
    freeVisitList();
}

Patient::Patient(const Patient& other) {
    id = other.id;
    name = other.name;
    priorityCode = other.priorityCode;
    arrivalOrder = other.arrivalOrder;
    callCount = other.callCount;
    copyVisitList(other.visitHistory);
}

Patient& Patient::operator=(const Patient& other) {
    if (this != &other) {
        freeVisitList();
        id = other.id;
        name = other.name;
        priorityCode = other.priorityCode;
        arrivalOrder = other.arrivalOrder;
        callCount = other.callCount;
        copyVisitList(other.visitHistory);
    }
    return *this;
}

void Patient::addVisitRecord(DATE d, const char* chandoan, const char* thuoc) {
    VisitNode* p = new VisitNode();
    p->visitDate = d;
    strncpy(p->chandoanbenh, chandoan, sizeof(p->chandoanbenh) - 1);
    p->chandoanbenh[sizeof(p->chandoanbenh) - 1] = '\0';
    strncpy(p->thuockedon, thuoc, sizeof(p->thuockedon) - 1);
    p->thuockedon[sizeof(p->thuockedon) - 1] = '\0';
    p->link = nullptr;

    if (visitHistory.first == nullptr) {
        visitHistory.first = visitHistory.last = p;
    } else {
        visitHistory.last->link = p;
        visitHistory.last = p;
    }
}

void Patient::printVisitHistory() const {
    cout << "  Lich su kham nguyen van:\n";
    VisitNode* cur = visitHistory.first;
    if (cur == nullptr) {
        cout << "    (Chua tung kham lan nao)\n";
        return;
    }
    int i = 1;
    while (cur != nullptr) {
        cout << "    Lan " << i++ << " - Ngay " << cur->visitDate.day << "/" << cur->visitDate.month << "/" << cur->visitDate.year << "\n"
             << "      Chan doan: " << cur->chandoanbenh << "\n"
             << "      Thuoc:     " << cur->thuockedon << "\n";
        cur = cur->link;
    }
}
string Patient::to_string() const {
    stringstream ss;
    ss << "  " << id;
    string formattedString = ss.str();
    while (formattedString.length() < 12) formattedString += " ";
    
    string priorityText;
    if (priorityCode == 1) priorityText = "Cap cuu (Do)";
    else if (priorityCode == 2) priorityText = "Lay nhiem cao (Cam)";
    else if (priorityCode == 3) priorityText = "Trieu chung nhe (Vang)";
    else priorityText = "Kham dinh ky (Xanh)";
    
    formattedString += priorityText;
    while (formattedString.length() < 32) formattedString += " ";
    
    formattedString += name + " (Vang: " + to_string(callCount) + "/3)";
    return formattedString;
}
bool Patient::operator>(const Patient &obj) const {
    if (this->priorityCode > obj.priorityCode) return true;
    if (this->priorityCode < obj.priorityCode) return false;
    return this->arrivalOrder > obj.arrivalOrder;
}

#endif