#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "PatientPriorityQueue.h"

using namespace std;

// Prototypes
void welcome();
void goodbye();
void help();
bool processLine(string, PatientPriorityQueue &);
string delimitBySpace(string &);

void addPatientCmd(string, PatientPriorityQueue &);
void peekNextCmd(PatientPriorityQueue &);
void callNextPatientCmd(PatientPriorityQueue &);
void showPatientListCmd(PatientPriorityQueue &);
void execCommandsFromFileCmd(string, PatientPriorityQueue &);
void editPatientCmd(string, PatientPriorityQueue &);
void removePatientByNameCmd(string, PatientPriorityQueue &);
void handleAbsenceCmd(PatientPriorityQueue &);
void addMedicalHistoryCmd(string, string, PatientPriorityQueue &);
void checkAllergyCmd(string, PatientPriorityQueue &);
void searchPatientInfoCmd(string, PatientPriorityQueue &);
void prescribeMedicineCmd(string, PatientPriorityQueue &);

// =================================================================
//  file_io.c
// Các hàm Đọc / Ghi File CSV chuẩn 5 cột và File Hồ sơ
// =================================================================
void LoadMedicalRecordsFromFile(PatientPriorityQueue &priQueue, const string& filename);
void GhiLichSuKhamMoiCSV(const string& filename, const string& maBN, const string& ngay, const string& chanDoan, const string& dsThuoc);

int globalArrivalOrder = 1;

int main() {
    welcome();
    PatientPriorityQueue priQueue;

    // Nạp sẵn dữ liệu lịch sử y tế từ file medical_records.csv nếu có
    LoadMedicalRecordsFromFile(priQueue, "medical_records.csv");

    string line;
    do {
        cout << "\ntriage> ";
        getline(cin, line);
    } while (processLine(line, priQueue));
    goodbye();
    return 0;
}

bool processLine(string line, PatientPriorityQueue &priQueue) {
    string cmd = delimitBySpace(line);
    if (cmd.empty()) return true;

    if (cmd == "help") help();
    else if (cmd == "add") addPatientCmd(line, priQueue);
    else if (cmd == "peek") peekNextCmd(priQueue);
    else if (cmd == "next") callNextPatientCmd(priQueue);
    else if (cmd == "list") showPatientListCmd(priQueue);
    else if (cmd == "load") execCommandsFromFileCmd(line, priQueue);
    else if (cmd == "edit") editPatientCmd(line, priQueue);
    else if (cmd == "delete") removePatientByNameCmd(line, priQueue);
    else if (cmd == "absent") handleAbsenceCmd(priQueue);
    else if (cmd == "add_illness") addMedicalHistoryCmd(cmd, line, priQueue);
    else if (cmd == "add_allergy") addMedicalHistoryCmd(cmd, line, priQueue);
    else if (cmd == "check_allergy") checkAllergyCmd(line, priQueue);
    else if (cmd == "search") searchPatientInfoCmd(line, priQueue);
    else if (cmd == "prescribe") prescribeMedicineCmd(line, priQueue);
    else if (cmd == "quit") return false;
    else cout << "Loi: Khong nhan dang duoc lenh: " << cmd << endl;
    return true;
}

// =================================================================
// file_io.c
// Hàm nạp thông tin Y tế chuẩn CSV 5 cột
// =================================================================
void LoadMedicalRecordsFromFile(PatientPriorityQueue &priQueue, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty() || line.rfind("Ma_BN", 0) == 0) continue;

        stringstream ss(line);
        string maBN, loaiNode, col3, col4, col5;
        getline(ss, maBN, ',');
        getline(ss, loaiNode, ',');
        getline(ss, col3, ',');
        getline(ss, col4, ',');
        getline(ss, col5, ',');

        Patient* p = priQueue.findPatient(maBN);
        if (p != nullptr) {
            if (loaiNode == "LICH_SU") {
                DATE d = {1, 1, 2026};
                sscanf(col3.c_str(), "%d/%d/%d", &d.day, &d.month, &d.year);
                p->addVisitRecord(d, col4.c_str(), col5.c_str());
            } else if (loaiNode == "MAN_TINH") {
                p->getMedicalHistory().addChronic(col3);
            } else if (loaiNode == "DI_UNG") {
                p->getMedicalHistory().addAllergy(col3);
            }
        }
    }
    file.close();
}

// =================================================================
// [THAY THẾ & TÍCH HỢP TỪ FILE: file_io.c]
// Hàm Ghi bổ sung lịch sử khám mới vào file CSV
// =================================================================
void GhiLichSuKhamMoiCSV(const string& filename, const string& maBN, const string& ngay, const string& chanDoan, const string& dsThuoc) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << maBN << ",LICH_SU," << ngay << "," << chanDoan << "," << dsThuoc << "\n";
        file.close();
    }
}

void addPatientCmd(string line, PatientPriorityQueue &priQueue) {
    string id = delimitBySpace(line);
    string priority_str = delimitBySpace(line);
    string name = line;
    int level;

    if (id.empty() || priority_str.empty() || name.empty()) {
        cout << "Cu phap: add <Mã_BN> <priority> <Tên_Bệnh_Nhân>\n"; return;
    }

    if (priority_str == "do" || priority_str == "1") level = 1;
    else if (priority_str == "cam" || priority_str == "2") level = 2;
    else if (priority_str == "vang" || priority_str == "3") level = 3;
    else if (priority_str == "xanh" || priority_str == "4") level = 4;
    else {
        cout << "Loi: Ma uu tien khong hop le. Dung: do(1), cam(2), vang(3), xanh(4).\n"; return;
    }

    Patient newPatient(id, name, level, globalArrivalOrder++);
    priQueue.RoutePatientToQueue(newPatient);

    cout << "Da them benh nhan [" << id << "] \"" << name << "\" vao hang doi.\n";
}

void callNextPatientCmd(PatientPriorityQueue &priQueue) {
    try {
        Patient p = priQueue.CallNextPatient();
        cout << "\n\n"
             << " [GOI KHAM]: Benh nhan " << p.getName() << " (Ma: " << p.getID() << ")\n"
             << " Muc uu tien: Cap " << p.getPriorityCode() << "\n"
             << "\n";
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
}

// =================================================================
// [THAY THẾ & TÍCH HỢP TỪ FILE: idea.txt - Mục 4 Kê đơn thuốc]
// Kê đơn thuốc tự động kiểm tra dị ứng & Ghi file CSV
// =================================================================
void prescribeMedicineCmd(string line, PatientPriorityQueue &priQueue) {
    string id = delimitBySpace(line);
    string dateStr = delimitBySpace(line);
    string diagnosis = delimitBySpace(line);
    string medicines = line; // Danh sách thuốc cách nhau bởi dấu phẩy

    if (id.empty() || medicines.empty()) {
        cout << "Cu phap: prescribe <Ma_BN> <Ngay> <ChanDoan> <Thuoc1,Thuoc2...>\n";
        return;
    }

    Patient* p = priQueue.findPatient(id);
    if (p == nullptr) {
        cout << "Loi: Khong tim thay benh nhan " << id << endl;
        return;
    }

    cout 
         << "           KE DON THUOC & BAO DI UNG\n"
         << "Benh nhan đang kham: " << p->getName() << " (Ma: " << p->getID() << ")\n"
         << "Thuoc ke don: " << medicines << "\n"
         << "[DANG DUYET CAY TIEN SU & DI UNG THUOC...]\n";

    if (p->getMedicalHistory().isAllergicTo(medicines)) {
        cout << "\n CANH BAO DI UNG TIM THAY!\n"
             << " - Benh nhan " << p->getID() << " bi DI UNG voi thuoc trong danh muc ke don!\n"
             << "--> KE DON THAT BAI! Vui long chon loai thuoc thay the khac.\n";
    } else {
        cout << "\n KIEM TRA THANH CONG: Don thuoc an toan!\n"
             << "   Khong phat hien canh bao di ung nao.\n"
             << "[HE THONG TU DONG CAP NHAT]\n"
             << "  Da tao Node lan kham moi.\n"
             << " Da luu du lieu moi vao file 'medical_records.csv'.\n";

        DATE d = {24, 7, 2026};
        sscanf(dateStr.c_str(), "%d/%d/%d", &d.day, &d.month, &d.year);
        p->addVisitRecord(d, diagnosis.c_str(), medicines.c_str());

        GhiLichSuKhamMoiCSV("medical_records.csv", p->getID(), dateStr, diagnosis, medicines);
    }
}

// =================================================================
// file idea.txt - Mục 3 Tra cứu hồ sơ]
// Tra cứu thông tin bệnh nhân + Cây bệnh lý + Lịch sử khám
// =================================================================
void searchPatientInfoCmd(string line, PatientPriorityQueue &priQueue) {
    string id = line;
    Patient* p = priQueue.findPatient(id);
    if (p == nullptr) {
        cout << "Khong tim thay ho so benh nhan " << id << endl;
        return;
    }

    cout 
         << "        TRA CUU & QUAN LY HO SO BENH NHAN\n"
         << "[THONG TIN BENH NHAN " << p->getID() << "]\n"
         << "- Ho ten: " << p->getName() << "\n"
         << "- Muc uu tien: Cap " << p->getPriorityCode() << "\n";
    
    p->printVisitHistory();

    cout << "  Tien su man tinh: ";
    vector<string> chronic = p->getMedicalHistory().getChronicList();
    for (const string& s : chronic) cout << s << "; ";
    cout << "\n  Di ung thuoc:    ";
    vector<string> allergy = p->getMedicalHistory().getAllergyList();
    for (const string& s : allergy) cout << s << "; ";
}

void peekNextCmd(PatientPriorityQueue &priQueue) {
    try {
        cout << "Benh nhan tiep theo: " << priQueue.peek().getName() << " (" << priQueue.peek().getID() << ")\n";
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
}

void editPatientCmd(string line, PatientPriorityQueue &priQueue) {
    string id = delimitBySpace(line);
    string new_priority_str = line;
    int new_level;

    if (id.empty() || new_priority_str.empty()) {
        cout << "Cu phap: edit <Ma_BN> <new-priority>\n"; return;
    }
    
    if (new_priority_str == "do" || new_priority_str == "1") new_level = 1;
    else if (new_priority_str == "cam" || new_priority_str == "2") new_level = 2;
    else if (new_priority_str == "vang" || new_priority_str == "3") new_level = 3;
    else if (new_priority_str == "xanh" || new_priority_str == "4") new_level = 4;
    else { cout << "Loi: Ma uu tien khong hop le.\n"; return; }
    
    if (priQueue.editPatientPriority(id, new_level)) {
        cout << "Cap nhat thanh cong cho benh nhan " << id << ".\n";
    } else {
        cout << "Loi: Khong tim thay benh nhan " << id << ".\n";
    }
}

void removePatientByNameCmd(string line, PatientPriorityQueue &priQueue) {
    string id = line;
    if (id.empty()) {
        cout << "Cu phap: delete <Ma_BN>\n"; return;
    }
    if (priQueue.removePatient(id)) {
        cout << "Da xoa thanh cong benh nhan " << id << " khoi hang doi.\n";
    } else {
        cout << "Loi: Khong tim thay benh nhan " << id << ".\n";
    }
}

void handleAbsenceCmd(PatientPriorityQueue &priQueue) {
    cout << priQueue.handleAbsence() << endl;
}

void addMedicalHistoryCmd(string cmd, string line, PatientPriorityQueue &priQueue) {
    string id = delimitBySpace(line);
    string detail = line;

    if (id.empty() || detail.empty()) {
        cout << "Cu phap: " << cmd << " <Ma_BN> <chi_tiet>\n"; return;
    }

    Patient* patient = priQueue.findPatient(id);
    if (patient) {
        if (cmd == "add_illness") {
            patient->getMedicalHistory().addChronic(detail);
            cout << "Da them benh man tinh '" << detail << "' cho " << id << ".\n";
        } else {
            patient->getMedicalHistory().addAllergy(detail);
            cout << "Da them di ung '" << detail << "' cho " << id << ".\n";
        }
    } else {
        cout << "Loi: Khong tim thay benh nhan " << id << ".\n";
    }
}

void checkAllergyCmd(string line, PatientPriorityQueue &priQueue) {
    string id = delimitBySpace(line);
    string drug = line;

    Patient* patient = priQueue.findPatient(id);
    if (patient) {
        if (patient->getMedicalHistory().isAllergicTo(drug)) {
            cout << "!!! CANH BAO: Benh nhan " << id << " DI UNG VOI " << drug << " !!!\n";
        } else {
            cout << "Benh nhan " << id << " khong co ghi nhan di ung voi " << drug << ".\n";
        }
    } else {
        cout << "Loi: Khong tim thay benh nhan " << id << ".\n";
    }
}

void showPatientListCmd(PatientPriorityQueue &priQueue) {
    cout << "# Tong so benh nhan dang cho: " << priQueue.size() << endl;
    cout << priQueue.to_string();
}

void execCommandsFromFileCmd(string filename, PatientPriorityQueue &priQueue) {
    ifstream infile(filename);
    if (!infile) {
        cout << "Loi: Khong the mo file: " << filename << endl;
        return;
    }
    string line;
    while (getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue;
        cout << "\n> " << line << endl;
        processLine(line, priQueue);
    }
    infile.close();
}

string delimitBySpace(string &s) {
    auto pos = s.find(' ');
    string token = s.substr(0, pos);
    s.erase(0, (pos != string::npos) ? pos + 1 : s.length());
    return token;
}

void welcome() {
    cout 
         << "   HE THONG DIEU PHOI VA PHAN LUONG KHAM BENH\n"
         << "Go 'help' de xem danh sach cac lenh.\n";
}

void goodbye() {
    cout << "\n KET THUC PHIEN LAM VIEC \n";
}

void help() {
    cout << "DANH SACH LENH HE THONG \n"
         << "  add <Ma_BN> <priority> <Tên>  : Them BN vao hang doi (do/cam/vang/xanh)\n"
         << "  list                          : Hien thi danh sach 4 hang doi\n"
         << "  peek                          : Xem benh nhan tiep theo\n"
         << "  next                          : Goi benh nhan tiep theo vao kham\n"
         << "  absent                        : Bao cao benh nhan dau hang doi vang mat\n"
         << "  search <Ma_BN>                : Tra cuu ho so & Tien su benh ly\n"
         << "  prescribe <Ma_BN> <Ngay> <ChanDoan> <Thuoc1,Thuoc2...> : Ke don & Kiem tra di ung\n"
         << "  edit <Ma_BN> <priority>       : Sua muc do uu tien\n"
         << "  delete <Ma_BN>                : Xoa ho so benh nhan\n"
         << "  add_illness <Ma_BN> <TenBenh> : Them benh man tinh\n"
         << "  add_allergy <Ma_BN> <Thuoc>   : Them di ung thuoc\n"
         << "  check_allergy <Ma_BN> <Thuoc> : Kiem tra di ung thuoc\n"
         << "  load <filename>               : Nap lenh tu file\n"
         << "  quit                          : Luu & Thoat\n";
}