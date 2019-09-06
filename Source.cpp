#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cctype>
//#pragma warning(disable : 4996)
using namespace std;

int daysPerMonth[48] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
						31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
						31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
						31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };



struct Date {
	Date(int iDay, int iMonth, int iYear) :
		day(iDay),
		month(iMonth),
		year(iYear)
	{}
	Date() :
		day(0),
		month(0),
		year(0)
	{}

	int day, month, year;
};

Date GetBirthdate(Date today);

int CalculateAge(Date today, Date birthDate);

bool CheckBirthday(Date birthDate);

string CalculateBirthday(Date birthDate);

string MakeLowerCase(string text);

bool MathTest(int age);

bool ArtTest(int age);

int * DetermineFactors();

int main() {
	//ophalen huidige tijd
	int day, month, year;
	tm s;
	time_t t;
	time(&t);
	s = *localtime(&t);
	day = s.tm_mday;
	month = s.tm_mon + 1; // 0 voor januari
	year = s.tm_year + 1900; // vanaf 1900

	Date today(day, month, year);
	
	srand(t);
	

	cout << "Het huidige jaar is " << today.year << endl;

	Date userBirthday = GetBirthdate(today);

	cout << "Uw geboortejaar is " << userBirthday.year << endl;
	cout << "Uw geboortemaand is " << userBirthday.month << endl;
	cout << "Uw geboortedag is " << userBirthday.day << endl;

	int age = CalculateAge(today, userBirthday);

	cout << "Uw leeftijd is " << age << " maanden" << endl;
	cout << "Ofwel " << (age - (age % 12)) / 12 << " jaar en " << age % 12 << " maanden" << endl;

	if (!CheckBirthday(userBirthday)) {
		return 0;
	}

	MathTest(age);	

	return 0;
}

Date GetBirthdate(Date today) {

	string invalidInput = "Deze invoer is ongeldig. Het programma wordt afgesloten.";
	string invalidAge = "Uw leeftijd is ongeschikt. Het programma wordt afgesloten.";


	Date birthDate;

	cout << "Geef uw geboortejaar op: ";
	cin >> birthDate.year;

	if (cin.fail() or birthDate.year > today.year) {
		cout << invalidInput;
		exit(0);
	}
	else if (birthDate.year > (today.year - 10) or birthDate.year < (today.year - 101)) {
		cout << invalidAge;
		exit(0);
	};

	cout << "Geef uw geboortemaand op: ";
	cin >> birthDate.month;

	if (cin.fail() or birthDate.month < 1 or birthDate.month > 12) {
		cout << invalidInput;
		exit(0);
	}
	else if ((birthDate.year == (today.year - 10)) and today.month < birthDate.month) {
		cout << invalidAge;
		exit(0);
	}
	else if ((birthDate.year == (today.year - 101)) and today.month > birthDate.month) {
		cout << invalidAge;
		exit(0);
	};

	cout << "Geef uw geboortedag op: ";
	cin >> birthDate.day;

	if (cin.fail() or birthDate.day < 1 or birthDate.day > daysPerMonth[birthDate.month - 1 + (12 * (birthDate.year % 4))]) {
		cout << invalidInput;
		exit(0);
	}
	else if ((birthDate.year == (today.year - 10)) and today.month == birthDate.month and today.day < birthDate.day) {
		cout << invalidAge;
		exit(0);
	}
	else if ((birthDate.year == (today.year - 101)) and today.month == birthDate.month and today.day >= birthDate.day) {
		cout << invalidAge;
		exit(0);
	};

	if (birthDate.year == today.year and birthDate.month > today.month) {
		cout << invalidInput;
		exit(0);
	};

	if (birthDate.year == today.year and birthDate.month == today.month and birthDate.day > today.day) {
		cout << invalidInput;
		exit(0);
	};

	if (birthDate.day == today.day) {
		cout << "Gefeliciteerd met uw vermaandag!" << endl;
		if (birthDate.month == today.month) {
			cout << "... en uw verjaardag!" << endl;
		};
	};

	return birthDate;

}

int CalculateAge(Date today, Date birthDate) {

	int age;

	age = today.month - birthDate.month + 12 * (today.year - birthDate.year);

	if (birthDate.day > today.day) {
		age -= 1;
	};

	return age;
}

bool CheckBirthday(Date birthDate) {

	string answer;
	string secondSymbol;

	cout << "Voer de eerste letter van de dag waarop u bent geboren in: ";
	cin >> answer;
	answer = MakeLowerCase(answer);

	if (answer == "d" or answer == "z") {

		cout << "Voer de tweede letter van de dag waarop u bent geboren in: ";
		cin >> secondSymbol;
		answer += secondSymbol;

	};

	if (answer == CalculateBirthday(birthDate)) {

		cout << "De opgegeven geboortedag is juist." << endl;
		return true;

	} else {
		cout << "De opgegeven geboortedag is onjuist. Het programma wordt afgesloten.";
		return false;
	};

}

string CalculateBirthday(Date birthDate) {

	string weekDays[7] = { "m", "di", "w", "do", "v", "za", "zo" };
	int numberOfDays = birthDate.day - 1;
	int differenceYears = birthDate.year - 1901;
	int differenceMonths = birthDate.month - 1;
	string birthday;

	for (int i = 0; i < differenceYears * 12 + differenceMonths; i++) {
		numberOfDays += daysPerMonth[(12 + i)%48];
	};

	birthday = weekDays[(numberOfDays % 7) + 1];


	return birthday;

}

bool MathTest(int age){

		int * factors;
		int product;
		int answer;
		int diff;
		bool older = (age > 360);
				
		factors = DetermineFactors();
		product = factors[0] * factors[1];
		
		if(older){
			cout << "U krijgt nu een wiskundige oefening om uw vaardigheden te beproeven." << endl;
		} else {
			cout << "Laten we even kijken hoeveel breincellen er bij jou nog over zijn!" << endl;
		}

		cout << "Wat is " << factors[0] << " * " << factors[1] << ": ";
		
		cin >> answer;
		
		diff = abs(answer-product);

		if(product == 0){
			return (diff == 0);
		} else {
			double epsilon = (double)diff / (double)product;
			return (epsilon < 0.1);
		}

		cout << "Hier hoort het programma nooit te komen?" << endl;
		return;
}

int * DetermineFactors(){
	
	static int factors[2];
	
	factors[0] = rand() % 1000;
	factors[1] = (rand() % 10 ? rand() % 1000 : 0);
	
	return factors;
}

string MakeLowerCase(string text){
	
	for(int i = 0; i<text.size(); i++){
		text[i] = tolower(text[i]);
	}
	
	return text;

}