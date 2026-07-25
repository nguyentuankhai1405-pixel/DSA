#ifndef _PATIENT_
#define _PATIENT_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "MedicalHistory.h"

using namespace std;

class Patient {
public:
    Patient(string name, int code, int order);
    string to_string() const; // Thêm const ở đây

    // Getters
    string getName() const { return name; }
    int getPriorityCode() const { return priorityCode; }
    int getArrivalOrder() const { return arrivalOrder; }
    int getCallCount() const { return callCount; }
    MedicalHistoryTree& getMedicalHistory() { return medicalHistory; }

    // Setters / Modifiers
    void setPriorityCode(int code) { priorityCode = code; }
    void incrementCallCount() { callCount++; }
    void resetCallCount() { callCount = 0; }

    bool operator>(const Patient &obj) const;

private:
    string name;
    int priorityCode;
    int arrivalOrder;
    int callCount;
    MedicalHistoryTree medicalHistory;
};

Patient::Patient(string name, int code, int order) {
    this->name = name;
    priorityCode = code;
    arrivalOrder = order;
    callCount = 0;
}

// Thêm const ở đây
string Patient::to_string() const {
    stringstream ss;
    ss << "  " << arrivalOrder;
    string formattedString = ss.str();
    while (formattedString.length() < 12) formattedString += " ";
    
    string priorityText;
    if (priorityCode == 1) priorityText = "Cap cuu (Do)";
    else if (priorityCode == 2) priorityText = "Lay nhiem cao (Cam)";
    else if (priorityCode == 3) priorityText = "Trieu chung nhe (Vang)";
    else priorityText = "Kham dinh ky (Xanh)";
    
    formattedString += priorityText;
    while (formattedString.length() < 35) formattedString += " ";
    
    formattedString += name;
    return formattedString;
}

bool Patient::operator>(const Patient &obj) const {
    if (this->priorityCode > obj.priorityCode) {
        return true;
    }
    if (this->priorityCode < obj.priorityCode) {
        return false;
    }
    return this->arrivalOrder > obj.arrivalOrder;
}

#endif