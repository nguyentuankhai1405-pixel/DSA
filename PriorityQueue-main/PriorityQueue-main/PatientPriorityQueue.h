#ifndef PATIENT_PRIORITY_QUEUE_H
#define PATIENT_PRIORITY_QUEUE_H

#include <string> // THÊM DÒNG NÀY
#include <vector>
#include <iostream>
#include <algorithm>
#include "Patient.h"

using namespace std;

class PatientPriorityQueue {
public:
    PatientPriorityQueue();
    void add(string name, int level);
    Patient peek() const;
    Patient remove();
    int size() const;
    string to_string() const; // Thêm const ở đây nữa cho an toàn
    Patient* findPatient(const string& name);
    bool removePatient(const string& name);
    bool editPatientPriority(const string& name, int newPriority);
    string handleAbsence();
private:
    int nextPatientNumber;
    vector<Patient> queue;
    void sortQueue();
};

PatientPriorityQueue::PatientPriorityQueue() : nextPatientNumber(1) {}

void PatientPriorityQueue::sortQueue() {
    for (size_t i = 0; i < queue.size(); ++i) {
        for (size_t j = i + 1; j < queue.size(); ++j) {
            if (queue[i] > queue[j]) {
                swap(queue[i], queue[j]);
            }
        }
    }
}

void PatientPriorityQueue::add(string name, int level) {
    Patient newPatient(name, level, nextPatientNumber++);
    queue.push_back(newPatient);
    sortQueue();
}

Patient PatientPriorityQueue::peek() const {
    if (queue.empty()) {
        throw runtime_error("Hang doi rong!");
    }
    return queue.front();
}

Patient PatientPriorityQueue::remove() {
    if (queue.empty()) {
        throw runtime_error("Hang doi rong!");
    }
    Patient frontPatient = queue.front();
    queue.erase(queue.begin());
    return frontPatient;
}

int PatientPriorityQueue::size() const {
    return queue.size();
}

string PatientPriorityQueue::to_string() const {
    string result = "";
    for (const auto& patient : queue) {
        result += patient.to_string() + "\n";
    }
    return result;
}

Patient* PatientPriorityQueue::findPatient(const string& name) {
    for (auto& patient : queue) {
        if (patient.getName() == name) {
            return &patient;
        }
    }
    return nullptr;
}

bool PatientPriorityQueue::removePatient(const string& name) {
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        if (it->getName() == name) {
            queue.erase(it);
            return true;
        }
    }
    return false;
}

bool PatientPriorityQueue::editPatientPriority(const string& name, int newPriority) {
    Patient* patient = findPatient(name);
    if (patient) {
        patient->setPriorityCode(newPriority);
        sortQueue();
        return true;
    }
    return false;
}

string PatientPriorityQueue::handleAbsence() {
    if (queue.empty()) {
        return "Khong co benh nhan trong hang doi.";
    }
    Patient& patient = queue.front();
    patient.incrementCallCount();
    if (patient.getCallCount() >= 3) {
        string name = patient.getName();
        int newPriority = min(4, patient.getPriorityCode() + 1);
        Patient movedPatient = patient;
        movedPatient.setPriorityCode(newPriority);
        movedPatient.resetCallCount();
        queue.erase(queue.begin());
        queue.push_back(movedPatient);
        sortQueue();
        return "Benh nhan \"" + name + "\" vang mat 3 lan. Giam muc do uu tien va dua vao cuoi hang doi.";
    }
    return "Da goi benh nhan  \"" + patient.getName() + "\" lan thu " + std::to_string(patient.getCallCount()) + ".";
}

#endif