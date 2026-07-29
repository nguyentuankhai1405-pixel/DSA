#ifndef PATIENT_PRIORITY_QUEUE_H
#define PATIENT_PRIORITY_QUEUE_H

#include <string>
#include <vector>
#include <iostream>
#include "Patient.h"

using namespace std;

// 1. Định nghĩa một Node trong danh sách, chứa dữ liệu bệnh nhân
struct PatientNode {
    Patient data;
    PatientNode* pNext;
};

// 2. Định nghĩa một Hàng đợi (Queue), quản lý bằng con trỏ Head và Tail
struct Queue {
    PatientNode* pHead;
    PatientNode* pTail;
    int count;
};

// --- KẾT THÚC THAY ĐỔI CỐT LÕI ---

class PatientPriorityQueue {
public:
    PatientPriorityQueue(); // Constructor

    // --- CÁC HÀM MỚI THEO YÊU CẦU ---
    void RoutePatientToQueue(const Patient& patientData);
    Patient CallNextPatient();

    // --- CÁC HÀM CŨ ĐƯỢC VIẾT LẠI TRÊN CẤU TRÚC MỚI ---
    Patient peek() const;
    int size() const;
    string to_string() const;
    Patient* findPatient(const string& name);
    bool removePatient(const string& name);
    bool editPatientPriority(const string& name, int newPriority);
    string handleAbsence();

private:
    int nextPatientNumber;

    // --- THAY ĐỔI CỐT LÕI: 4 HÀNG ĐỢI RIÊNG BIỆT ---
    Queue queueDo;   // Priority 1
    Queue queueCam;  // Priority 2
    Queue queueVang; // Priority 3
    Queue queueXanh; // Priority 4

    // --- CÁC HÀM HỖ TRỢ CHO DANH SÁCH LIÊN KẾT (THEO YÊU CẦU) ---
    void InitQueue(Queue& q);
    void Enqueue(Queue& q, const Patient& patientData);
    Patient Dequeue(Queue& q);
    bool IsQueueEmpty(const Queue& q) const;
};

// --- PHẦN IMPLEMENTATION ---

// Constructor: Khởi tạo tất cả 4 hàng đợi
PatientPriorityQueue::PatientPriorityQueue() {
    nextPatientNumber = 1;
    InitQueue(queueDo);
    InitQueue(queueCam);
    InitQueue(queueVang);
    InitQueue(queueXanh);
}

// Hàm khởi tạo một hàng đợi rỗng
void PatientPriorityQueue::InitQueue(Queue& q) {
    q.pHead = NULL;
    q.pTail = NULL;
    q.count = 0;
}

// Hàm kiểm tra hàng đợi rỗng
bool PatientPriorityQueue::IsQueueEmpty(const Queue& q) const {
    return q.pHead == NULL;
}

// Hàm thêm một bệnh nhân vào cuối (Tail) của một hàng đợi
void PatientPriorityQueue::Enqueue(Queue& q, const Patient& patientData) {
    PatientNode* p = new PatientNode();
    p->data = patientData;
    p->pNext = NULL;

    if (IsQueueEmpty(q)) {
        q.pHead = q.pTail = p;
    } else {
        q.pTail->pNext = p;
        q.pTail = p;
    }
    q.count++;
}

// Hàm lấy và xóa bệnh nhân ở đầu (Head) của một hàng đợi
Patient PatientPriorityQueue::Dequeue(Queue& q) {
    if (IsQueueEmpty(q)) {
        throw runtime_error("Cố gắng Dequeue từ hàng đợi rỗng!");
    }

    PatientNode* p = q.pHead;
    Patient patientData = p->data;
    q.pHead = q.pHead->pNext;
    delete p;

    if (q.pHead == NULL) {
        q.pTail = NULL;
    }
    q.count--;
    return patientData;
}

// Hàm Phân luồng: Dựa vào priorityLevel để Enqueue vào đúng Queue
void PatientPriorityQueue::RoutePatientToQueue(const Patient& patientData) {
    int level = patientData.getPriorityCode();
    switch (level) {
        case 1: Enqueue(queueDo, patientData); break;
        case 2: Enqueue(queueCam, patientData); break;
        case 3: Enqueue(queueVang, patientData); break;
        case 4: Enqueue(queueXanh, patientData); break;
    }
}

// Hàm Gọi khám: Xử lý ưu tiên, check từ Đỏ -> Cam -> Vàng -> Xanh
Patient PatientPriorityQueue::CallNextPatient() {
    if (!IsQueueEmpty(queueDo)) return Dequeue(queueDo);
    if (!IsQueueEmpty(queueCam)) return Dequeue(queueCam);
    if (!IsQueueEmpty(queueVang)) return Dequeue(queueVang);
    if (!IsQueueEmpty(queueXanh)) return Dequeue(queueXanh);
    
    throw runtime_error("Không có bệnh nhân nào trong tất cả các hàng đợi.");
}

// Tổng số bệnh nhân = tổng của 4 hàng đợi
int PatientPriorityQueue::size() const {
    return queueDo.count + queueCam.count + queueVang.count + queueXanh.count;
}

// Viết lại hàm peek()
Patient PatientPriorityQueue::peek() const {
    if (!IsQueueEmpty(queueDo)) return queueDo.pHead->data;
    if (!IsQueueEmpty(queueCam)) return queueCam.pHead->data;
    if (!IsQueueEmpty(queueVang)) return queueVang.pHead->data;
    if (!IsQueueEmpty(queueXanh)) return queueXanh.pHead->data;

    throw runtime_error("Không có bệnh nhân nào để xem.");
}

// Viết lại hàm to_string() để hiển thị 4 hàng đợi
string PatientPriorityQueue::to_string() const {
    string result = "";
    PatientNode* p;

    result += "--- HANG DOI DO (UU TIEN 1) ---\n";
    for (p = queueDo.pHead; p != NULL; p = p->pNext) result += p->data.to_string() + "\n";

    result += "--- HANG DOI CAM (UU TIEN 2) ---\n";
    for (p = queueCam.pHead; p != NULL; p = p->pNext) result += p->data.to_string() + "\n";
    
    result += "--- HANG DOI VANG (UU TIEN 3) ---\n";
    for (p = queueVang.pHead; p != NULL; p = p->pNext) result += p->data.to_string() + "\n";

    result += "--- HANG DOI XANH (UU TIEN 4) ---\n";
    for (p = queueXanh.pHead; p != NULL; p = p->pNext) result += p->data.to_string() + "\n";
    
    return result;
}

// Hàm findPatient phải duyệt qua cả 4 danh sách
Patient* PatientPriorityQueue::findPatient(const string& name) {
    PatientNode* p;
    for (p = queueDo.pHead; p != NULL; p = p->pNext) if (p->data.getName() == name) return &(p->data);
    for (p = queueCam.pHead; p != NULL; p = p->pNext) if (p->data.getName() == name) return &(p->data);
    for (p = queueVang.pHead; p != NULL; p = p->pNext) if (p->data.getName() == name) return &(p->data);
    for (p = queueXanh.pHead; p != NULL; p = p->pNext) if (p->data.getName() == name) return &(p->data);
    return nullptr;
}

// Logic xử lý vắng mặt trên cấu trúc mới
string PatientPriorityQueue::handleAbsence() {
    Patient patientData;
    int currentPriority = -1;

    // Tìm xem hàng đợi nào có bệnh nhân đầu tiên
    if (!IsQueueEmpty(queueDo)) { patientData = queueDo.pHead->data; currentPriority = 1; }
    else if (!IsQueueEmpty(queueCam)) { patientData = queueCam.pHead->data; currentPriority = 2; }
    else if (!IsQueueEmpty(queueVang)) { patientData = queueVang.pHead->data; currentPriority = 3; }
    else if (!IsQueueEmpty(queueXanh)) { patientData = queueXanh.pHead->data; currentPriority = 4; }
    else { return "Khong co benh nhan de bao vang."; }

    patientData.incrementCallCount();

    if (patientData.getCallCount() >= 3) {
        // Xóa bệnh nhân khỏi hàng đợi hiện tại của họ
        switch (currentPriority) {
            case 1: Dequeue(queueDo); break;
            case 2: Dequeue(queueCam); break;
            case 3: Dequeue(queueVang); break;
            case 4: Dequeue(queueXanh); break;
        }

        // Hạ độ ưu tiên và reset bộ đếm
        int newPriority = min(4, currentPriority + 1);
        patientData.setPriorityCode(newPriority);
        patientData.resetCallCount();

        // Phân luồng họ vào hàng đợi mới (thấp hơn)
        RoutePatientToQueue(patientData);

        return "Benh nhan \"" + patientData.getName() + "\" vang mat 3 lan. Da ha do uu tien va chuyen xuong hang doi phu hop.";
    } else {
        // Cập nhật lại thông tin bệnh nhân trong Node (vì callCount đã thay đổi)
        switch (currentPriority) {
            case 1: queueDo.pHead->data = patientData; break;
            case 2: queueCam.pHead->data = patientData; break;
            case 3: queueVang.pHead->data = patientData; break;
            case 4: queueXanh.pHead->data = patientData; break;
        }
        return "Da goi benh nhan \"" + patientData.getName() + "\" lan thu " + std::to_string(patientData.getCallCount()) + ".";
    }
}

// Xóa một bệnh nhân bất kỳ (phức tạp hơn trên DSLK)
bool PatientPriorityQueue::removePatient(const string& name) {
    Queue* queues[] = { &queueDo, &queueCam, &queueVang, &queueXanh };
    for (int i = 0; i < 4; ++i) {
        PatientNode *p = queues[i]->pHead;
        PatientNode *prev = NULL;
        while (p != NULL && p->data.getName() != name) {
            prev = p;
            p = p->pNext;
        }
        if (p != NULL) { // Tìm thấy
            if (prev == NULL) { // Node cần xóa là Head
                queues[i]->pHead = p->pNext;
                if (queues[i]->pHead == NULL) queues[i]->pTail = NULL; // Queue trở nên rỗng
            } else {
                prev->pNext = p->pNext;
                if (p == queues[i]->pTail) queues[i]->pTail = prev; // Node cần xóa là Tail
            }
            delete p;
            queues[i]->count--;
            return true;
        }
    }
    return false; // Không tìm thấy trong cả 4 hàng đợi
}

// Sửa độ ưu tiên (Xóa khỏi hàng đợi cũ, thêm vào hàng đợi mới)
bool PatientPriorityQueue::editPatientPriority(const string& name, int newPriority) {
    Patient* p = findPatient(name);
    if (p) {
        Patient patientData = *p; // Tạo bản sao dữ liệu
        removePatient(name);     // Xóa node cũ khỏi hàng đợi hiện tại
        patientData.setPriorityCode(newPriority); // Cập nhật độ ưu tiên trên bản sao
        RoutePatientToQueue(patientData); // Phân luồng lại vào hàng đợi mới
        return true;
    }
    return false;
}

#endif