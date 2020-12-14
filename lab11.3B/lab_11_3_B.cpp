#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h> 
#include <fstream>

using namespace std;

enum Specialty { PROGRAMMER, ENGINEER, PHYSICIAN };
string specStr[] = { "КН", "ІК", "ФЛ" };
struct Student
{
    char prizv[64];
    int kurs;
    Specialty spec;
    int physics;
    int math;
    int inform;
    union {
        int inform;
        int NumericalMethods;
        int pedagogy;
    };
};

void Create(char* fname);
void Print(char* fname);
int fRead(fstream& f, const int i);
char SRead(fstream& f, const int i);
int TRead(fstream& f, const int i);
void fChange(fstream& f, const int i, const int j);
void sortBin(char* fname);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char fname[61];
    double MinAve = 0.0;
    int countPhysics = 0;
    int countMathematics = 0;
    int countInformatics = 0;
    int choice;
    do {
        cout << endl << endl;
        cout << "Оберіть номер команди:" << endl;
        cout << "[1] - Створити список студентів" << endl;
        cout << "[2] - Вивести список на екран" << endl;
        cout << "[3] - Фізично впорядкувати файл"
            << endl << endl;
        cout << " [0] - Вийти із програми" << endl << endl;
        cout << "Введіть номер команди: "; cin >> choice;
        cout << endl << endl << endl;

        switch (choice)
        {
        case 1:
            cin.get(); cin.sync();
            cout << "Введіть ім'я файлу: "; cin.getline(fname, sizeof(fname));
            Create(fname);
            break;
        case 2:
            cin.get(); cin.sync();
            cout << "Введіть ім'я файлу: "; cin.getline(fname, sizeof(fname));
            Print(fname);
            break;
        case 3:
            cin.get(); cin.sync();
            cout << "Введіть ім'я файлу: "; cin.getline(fname, sizeof(fname));
            sortBin(fname);
            break;
        default:
            cout << "Ви ввели неправильну команду" << endl;
            break;
        }
    } while (choice != 0);
}

void Create(char* fname)
{
    ofstream f(fname, ios::binary | ios::trunc);
    if (!f)
    {
        cerr << "Неможливо відкрити файл '" << fname << "'" << endl;
        exit(1);
    }

    Student s;
    int spec;
    char ch;
    int i = 0;
    do {
        cout << endl;
        cout << "Студент № " << i + 1 << ":" << endl;
        ++i;
        cin.get(); cin.sync();
        cout << "Прізвище: "; cin.getline( s.prizv, sizeof(s.prizv));
        do {
            cout << "Курс: "; cin >> s.kurs;
        } while (!(s.kurs > 0));
        do {
            cout << "Спеціальність (0 - КН, 1 - ІК, 2 - ФЛ): "; cin >> spec;
            s.spec = (Specialty)spec;
        } while (!(spec >= 0 && spec <= 2));

        cout << "Бал із фізики: "; cin >> s.physics;
        cout << "Бал із математики: "; cin >> s.math;

        switch (s.spec)
        {
        case PROGRAMMER:
            cout << "Бал із інформатики: "; cin >> s.inform;
            break;
        case ENGINEER:
            cout << "Бал із чисельних методів: "; cin >> s.NumericalMethods;
            break;
        case PHYSICIAN:
            cout << "Бал із педагогіки: "; cin >> s.pedagogy;
            break;
        }
        cout << endl;

        if (!f.write((char*)&s, sizeof(Student)))
        {
            cout << " Error of writing " << endl;
        }

        cout << "Хочете додати ще одного студента? (Y/N): "; cin >> ch;

    } while (ch == 'Y' || ch == 'y');

}

void Print(char* fname)
{
    ifstream f(fname, ios::binary | ios::in);
    if (!f)
    {
        cerr << "Неможливо відкрити файл" << endl;
        exit(1);
    }

    Student s;
    int i = 0;

    cout << "============================================================================================================"
        << endl;
    cout << "| № | Прізвище | Курс | Спеціальність | Фізика | Математика | Інформатика | Чисельні методи | Педагогіка   |"
        << endl;
    cout << "------------------------------------------------------------------------------------------------------------"
        << endl;

    while (f.read((char*)&s, sizeof(Student)))
    {


        cout << "| " << setw(2) << right << i + 1
            << "| " << setw(9) << right << s.prizv
            << "| " << setw(5) << right << s.kurs
            << "| " << setw(14) << right << specStr[s.spec];
        switch (s.spec)
        {
        case 0:
            cout << "|" << setw(8) << setprecision(2) << fixed << s.physics << "|" << s.math << setw(12) << right
                << "|" << s.inform << setw(13) << "|" << setw(18) << "|" << setw(15)
                << setprecision(2) << " |" << endl;
            break;
        case 1:
            cout << "|" << setw(8) << setprecision(2) << fixed << s.physics << "|" << s.math << setw(12) << right << "|"
                << setw(14) << "|" << s.NumericalMethods << setw(17) << "|" << setw(15) << "|" << endl;
            break;
        case 2:
            cout << "|" << setw(8) << setprecision(2) << fixed << s.physics << "|" << s.math << setw(12) << right << "|"
                << setw(14) << "|" << setw(18) << "|" << s.pedagogy << setw(14) << "|" << endl;

            break;
        }
        cout << "------------------------------------------------------------------------------------------------------------"
            << endl;
        i++;
    }
    cout << endl;
}

void sortBin(char* fname)
{
    fstream f(fname, ios::binary | ios::in | ios::out);
    if (!f)
    {
        cerr << "Неможливо відкрити файл" << endl;
        exit(1);
    }

    f.seekg(0 * (long)sizeof(Student), ios::end);
    int size = f.tellg() / (sizeof(Student));
    f.seekg(0, ios::beg);
    for (int i0 = 1; i0 < size; i0++)
        for (int i1 = 0; i1 < size - i0; i1++)
        {
            int specialty1 = fRead(f, i1);
            int specialty2 = fRead(f, i1 + 1);
            char prizv1 = SRead(f, i1);
            char prizv2 = SRead(f, i1 + 1);
            int kurs1 = TRead(f, i1);
            int kurs2 = TRead(f, i1 + 1);
            if ((specialty1 > specialty2)
                ||
                (specialty1 == specialty2 && prizv1 < prizv2)
                ||
                (specialty1 == specialty2 && prizv1 == prizv2 && kurs1 > kurs2))
                fChange(f, i1, i1 + 1);
        }
    f.seekp(0, ios::end);
}

int fRead(fstream& f, const int i)
{
    Student student;
    f.seekg(i * 1.*(long)sizeof(Student));
    f.read((char*)&student, sizeof(Student));
    int specialty = student.spec;
    return specialty;
}

char SRead(fstream& f, const int i)
{
    Student student;
    f.seekg(i * 1.*(long)sizeof(Student));
    f.read((char*)&student, sizeof(Student));
    char prizv[64];
    strcpy_s(prizv, sizeof(prizv), student.prizv);
    return *prizv;
}

int TRead(fstream& f, const int i)
{
    Student student;
    f.seekg(i * 1.*(long)sizeof(Student));
    f.read((char*)&student, sizeof(Student));
    int kurs = student.kurs;
    return kurs;
}

void fChange(fstream& f, const int i, const int j)
{
    Student si, sj, temp;
    f.seekg(i * 1.*(long)sizeof(Student));
    f.read((char*)&si, sizeof(Student));
    temp = si;
    f.seekg(j * 1.*(long)sizeof(Student));
    f.read((char*)&sj, sizeof(Student));
    f.seekp(i * 1.*(long)sizeof(Student));
    f.write((char*)&sj, sizeof(Student));
    f.seekp(j * 1.*(long)sizeof(Student));
    f.write((char*)&temp, sizeof(Student));
}