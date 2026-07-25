#include <iostream>
#include <fstream>
#include <string>
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
void removeNextPatientCmd(PatientPriorityQueue &);
void showPatientListCmd(PatientPriorityQueue &);
void execCommandsFromFileCmd(string, PatientPriorityQueue &);
void editPatientCmd(string, PatientPriorityQueue &);
void removePatientByNameCmd(string, PatientPriorityQueue &);
void handleAbsenceCmd(PatientPriorityQueue &);
void addMedicalHistoryCmd(string, string, PatientPriorityQueue &);
void checkAllergyCmd(string, PatientPriorityQueue &);

int main() {
    welcome();
    PatientPriorityQueue priQueue;
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
    else if (cmd == "next") removeNextPatientCmd(priQueue);
    else if (cmd == "list") showPatientListCmd(priQueue);
    else if (cmd == "load") execCommandsFromFileCmd(line, priQueue);
    else if (cmd == "edit") editPatientCmd(line, priQueue);
    else if (cmd == "delete") removePatientByNameCmd(line, priQueue);
    else if (cmd == "absent") handleAbsenceCmd(priQueue);
    else if (cmd == "add_illness") addMedicalHistoryCmd(cmd, line, priQueue);
    else if (cmd == "add_allergy") addMedicalHistoryCmd(cmd, line, priQueue);
    else if (cmd == "check_allergy") checkAllergyCmd(line, priQueue);
    else if (cmd == "quit") return false;
    else cout << "Loi khong nhan dang duoc lenh: " << cmd << endl;
    return true;
}

void addPatientCmd(string line, PatientPriorityQueue &priQueue) {
    string priority_str = delimitBySpace(line);
    string name = line;
    int level;

    if (priority_str.empty() || name.empty()) {
        cout << "Cu phap: add <priority> <patient-name>\n"; return;
    }

    if (priority_str == "do" || priority_str == "1") level = 1;
    else if (priority_str == "cam" || priority_str == "2") level = 2;
    else if (priority_str == "vang" || priority_str == "3") level = 3;
    else if (priority_str == "xanh" || priority_str == "4") level = 4;
    else {
        cout << "Loi: Ma uu tien khong hop le. Dùng: do(1), cam(2), vang(3), xanh(4).\n"; return;
    }

    priQueue.add(name, level);
    cout << "Da them benh nhan \"" << name << "\" vao he thong.\n";
}

void editPatientCmd(string line, PatientPriorityQueue &priQueue) {
    string name = delimitBySpace(line);
    string new_priority_str = line;
    int new_level;

    if (name.empty() || new_priority_str.empty()) {
        cout << "Cu phap: edit <patient-name> <new-priority-code>\n"; return;
    }
    
    if (new_priority_str == "do" || new_priority_str == "1") new_level = 1;
    else if (new_priority_str == "cam" || new_priority_str == "2") new_level = 2;
    else if (new_priority_str == "vang" || new_priority_str == "3") new_level = 3;
    else if (new_priority_str == "xanh" || new_priority_str == "4") new_level = 4;
    else {
        cout << "Loi: Ma uu tien khong hop le.\n"; return;
    }
    
    if (priQueue.editPatientPriority(name, new_level)) {
        cout << "Cap nhat thanh cong cho benh nhan \"" << name << "\".\n";
    } else {
        cout << "LLoi: Khong tim thay benh nhan \"" << name << "\".\n";
    }
}

void removePatientByNameCmd(string line, PatientPriorityQueue &priQueue) {
    string name = line;
    if (name.empty()) {
        cout << "Cu phap: delete <patient-name>\n"; return;
    }
    // DÒNG NÀY ĐÃ ĐƯỢC SỬA LỖI
    if (priQueue.removePatient(name)) {
        cout << "Da xoa thanh cong benh nhan \"" << name << "\" khoi hang doi.\n";
    } else {
        cout << "LLoi: Khong tim thay benh nhan \"" << name << "\".\n";
    }
}

void handleAbsenceCmd(PatientPriorityQueue &priQueue) {
    cout << priQueue.handleAbsence() << endl;
}

void addMedicalHistoryCmd(string cmd, string line, PatientPriorityQueue &priQueue) {
    string name = delimitBySpace(line);
    string detail = line;

    if (name.empty() || detail.empty()) {
        cout << "Cu phap: " << cmd << " <patient-name> <detail>\n"; return;
    }

    Patient* patient = priQueue.findPatient(name);
    if (patient) {
        if (cmd == "add_illness") {
            patient->getMedicalHistory().addChronic(detail);
            cout << "Da them benh ma nhan '" << detail << "' cho " << name << ".\n";
        } else {
            patient->getMedicalHistory().addAllergy(detail);
            cout << "Da them di ung '" << detail << "' cho " << name << ".\n";
        }
    } else {
        cout << "Loi: Khong tim thay benh nhan \"" << name << "\".\n";
    }
}

void checkAllergyCmd(string line, PatientPriorityQueue &priQueue) {
    string name = delimitBySpace(line);
    string drug = line;

    if (name.empty() || drug.empty()) {
        cout << "Cu phap: check_allergy <patient-name> <drug-name>\n"; return;
    }

    Patient* patient = priQueue.findPatient(name);
    if (patient) {
        if (patient->getMedicalHistory().isAllergicTo(drug)) {
            cout << "!!! Canh bao: Benh nhan " << name << " Di ung voi " << drug << " !!!\n";
        } else {
            cout << "Benh nhan " << name << " khong co ghi nhan di ung voi " << drug << ".\n";
        }
    } else {
        cout << "Loi: Khong tim thay benh nhan \"" << name << "\".\n";
    }
}

void peekNextCmd(PatientPriorityQueue &priQueue) {
    if (priQueue.size() == 0) {
        cout << "Khong co benh nhan trong hang doi.\n";
    } else {
        cout << "Benh nhan tiep theo: " << priQueue.peek().getName() << endl;
    }
}

void removeNextPatientCmd(PatientPriorityQueue &priQueue) {
    if (priQueue.size() == 0) {
        cout << "Khong co benh nhan trong hang doi.\n";
    } else {
        Patient p = priQueue.remove();
        cout << "Benh nhan da duoc goi: " << p.getName() << endl;
    }
}

void showPatientListCmd(PatientPriorityQueue &priQueue) {
    cout << "# So benh nhan dang cho: " << priQueue.size() << endl;
    cout << "  STT Ten     Muc do uu tien                Ten benh nhan\n"
         << "+-----------+-----------------------------+--------------------+\n";
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
        if (line.empty() || line[0] == '#') continue; // Bỏ qua dòng trống và comment
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
    cout << "HE THONG BENH VIEN \n"
         << "Go 'help' de xem danh sach cac lenh.\n";
}

void goodbye() {
    cout << "\nKet thuc phien lam viec \n";
}

void help() {
    cout << "DANH SACH LENH\n"
         << "  add <priority> <name>   : Them BN. priority: do(1), cam(2), vang(3), xanh(4)\n"
         << "  list                    : Hien thi danh sach cho\n"
         << "  peek                    : Xem benh nhan tiep theo\n"
         << "  next                    : Goi benh nhan tiep theo\n"
         << "  absent                  : Bao cao benh nhan dau hang doi vang mat\n"
         << "  edit <name> <priority>  : Sua do uu tien cho benh nhan\n"
         << "  delete <name>           : Xoa benh nhan khoi hang doi\n"
         << "  add_illness <name> <illness> : Them benh ma nhan\n"
         << "  add_allergy <name> <drug>    : Them di ung thuoc\n"
         << "  check_allergy <name> <drug>  : Kiem tra di ung thuoc\n"
         << "  load <filename>         : Chay lenh tu file\n"
         << "  help                    : Hien thi man hinh nay\n"
         << "  quit                    : Thoat chuong trinh\n";
}