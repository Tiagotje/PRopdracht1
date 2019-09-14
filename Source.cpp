#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

// 14-09-2019
// Programmeermethoden, opdracht 1
// Source.cpp
// Matt van den Nieuwenhuijzen en Tiago Scholten
// compiled with g++ -Wall -o pr1 Source.cpp on Linux
// programmed in gedit on Linux

// Het programma doet het volgende:
// - Het vraagt de gebruiker om zijn/haar leeftijd
// - Kijkt of deze niet te jong/oud is voor uni
// - Laat de gebruiker de weekdag van zijn/haar geboorte raden als "fraudeprotectie"
// - Geeft een productsom als test voor een exacte studie
// - Als correct beantwoord mag de gebruiker deelnemen aan een exacte studie
// - Als het antwoord fout is, gaat het programma door met een literatuur vraag
// - Wanneer correct beantwoord mag de gebruiker deelnemen aan een alfastudie
// - Wanneer fout beantwoord mag de gebruiker niet naar universiteit


/*
TODO:
3. Schrijven verslag: http://liacs.leidenuniv.nl/~kosterswa/pm/pmwc3.php
*/

//struct die de dag, maand en jaar van een datum opslaat
struct Date {
	Date(int iDay, int iMonth, int iYear) :
		day(iDay), month(iMonth), year(iYear)
	{}

	Date() :
		day(0), month(0), year(0)
	{}

	int day, month, year;

	bool operator==(Date o){
		return (year == o.year) && (month == o.month) && (day == o.day);
	}
};

//declaratie van de functies die gebruikt worden
//zie onder main uitgebreider commentaar per functie
int getDayCount(int year, int month);
Date getBirthDate(Date today);
Date getAge(Date today, Date birthDate);
bool checkBirthday(Date birthDate);
string calculateBirthday(Date birthDate);
string makeLowerCase(string text);
bool mathTest(bool older);
bool artTest(bool older);
int * determineFactors();
void infoBlock();

//main
int main() {
	int day, month, year;
	tm s; //een struct dat tijd bijhoudt
	time_t t; //een variabele die een tijdstip opslaat
	bool older; //of de gebruiker minstens 30 jaar oud is

	//ophalen huidige tijd
	time(&t);
	s = *localtime(&t);
	day = s.tm_mday;
	month = s.tm_mon + 1; // 0 voor januari
	year = s.tm_year + 1900; // vanaf 1900
	Date today(day, month, year);

	srand(t); //stel seed vast voor willekeurige getallen

	infoBlock();

	Date birthDate = getBirthDate(today);

	//De geboortedatum is afgekeurd
	if (birthDate == Date(-1, -1, -1))
		return 1;

	cout << "Uw geboortejaar is " << birthDate.year << endl;
	cout << "Uw geboortemaand is " << birthDate.month << endl;
	cout << "Uw geboortedag is " << birthDate.day << endl;

	Date age = getAge(today, birthDate);

	if (age == Date(-1, -1, -1))
		return 0;

	cout << "Uw leeftijd is " << age.month + age.year * 12 << " maanden" << endl;
	cout << "Oftewel " << age.year << " jaar en "
	 		 << age.month << " maanden" << endl;

	if(!checkBirthday(birthDate))
		return 0;

	older = (age.year >= 30);

	if(mathTest(older)){
		cout << "Gefeliciteerd!" << endl;
		cout << (older ? "U " : "Je ") << "mag deelnemen aan een exacte studie!";
		cout << endl;
		return 0;
	}

	cout << "Helaas!" << endl;
	cout << (older ? "U " : "Je ") << "mag niet deelnemen aan een exacte studie!";
	cout << endl;
	cout << endl;

	if(artTest(older)){
		cout << "Gefeliciteerd!" << endl;
		cout << (older ? "U " : "Je ") << "mag deelnemen aan een alfa-studie"
				 << endl;
	} else {
		cout << "Helaas!" << endl;
		cout << (older ? "U " : "Je ") << "mag helaas ook niet deelnemen ";
		cout << "aan een alfa-studie." << endl;
	}

	cout << "Het programma wordt nu afgesloten..." << endl;

	return 0;
}

void infoBlock(){

	cout << "================================" << endl;
	cout << "Welkom bij onze uitwerking van opdracht 1!" << endl;
	cout << "Geschreven door: " << endl;
	cout << "Matt van den Nieuwenhuijzen" << endl;
	cout << "Aankomstjaar: 2017" << endl;
	cout << "Studierichting: Natuurkunde" << endl;
	cout << "Studentnummer: S2042096" << endl;
	cout << "&" << endl;
	cout << "Tiago Scholten" << endl;
	cout << "Aankomstjaar: 2019" << endl;
	cout << "Studierichting: Natuurkunde/Wiskunde/Taakunde" << endl;
	cout << "Studentnummer: S2430479" << endl;
	cout << "================================" << endl << endl;

	cout << "Welkom bij de toelatingstoets van de Universiteit Leiden!" << endl;
	cout << "Middels deze test wordt gekeken of uw leeftijd geschikt is en" << endl;
	cout << " of u de vaardigheden heeft om deel te nemen aan een studie." << endl;

	return;
}

//Vraagt aan de gebruiker een geldige geboortedatum in te vullen
//Gebruikt 'today' om te checken of deze datum niet in de verre toekomst is
//Returnt een datum object (jaar, maand, dag) met de geboortedatum van de gebruiker
//Returnt (-1, -1, -1) in het geval van een invalid input
Date getBirthDate(Date today) {

	string invalidAge = "Uw leeftijd is ongeschikt.";
	string invalidInput = "Deze invoer is ongeldig.";
	string quitProgram = " Het programma wordt afgesloten.";

	Date birthDate;

	cout << "Geef uw geboortejaar op: ";
	cin >> birthDate.year;

	//Check of het jaartal wel minstens 10 jaar geleden is
	if (cin.fail() or birthDate.year > (today.year - 10)
			or birthDate.year < (today.year - 101)) {
		cout << invalidAge << quitProgram << endl;
		return Date(-1, -1, -1);
	}

	cout << "Geef uw geboortemaand op: ";
	cin >> birthDate.month;

	//Check of het een geldige maand is
	if (cin.fail() or birthDate.month < 1 or birthDate.month > 12) {
		cout << invalidInput << quitProgram << endl;
		return Date(-1, -1, -1);
	}

	//Check of niet te jong op maand
	if (birthDate.year == (today.year - 10) and birthDate.month > today.month){
		cout << invalidAge << quitProgram << endl;
		return Date(-1, -1, -1);
	}

	//Check of niet te oud op maand
	if (birthDate.year == (today.year - 100) and birthDate.month < today.month){
		cout << invalidAge << quitProgram << endl;
		return Date(-1, -1, -1);
	}

	cout << "Geef uw geboortedag op: ";
	cin >> birthDate.day;

	//Check of het wel een geldige dag is
	if (cin.fail() or birthDate.day < 1
			or birthDate.day > getDayCount(birthDate.year, birthDate.month)){
		cout << invalidInput << quitProgram << endl;
		return Date(-1, -1, -1);
	}

	return birthDate;
}

//berekent de leeftijd van de gebruiker
//doet dit door het verschil te nemen van 'today' en 'birthDate'
//returnt een Date object waar year en month de leeftijd uitdrukken
//en als de dag 0 is, is de gebruiker jarig
//returnt (-1, -1, -1) bij een onacceptabele datum
Date getAge(Date today, Date birthDate) {

	string invalidInput = "Deze invoer is ongeldig.";
	string invalidAge = "Uw leeftijd is ongeschikt.";
	string quitProgram = "Het programma wordt afgesloten.";

	Date age; //jaar, maand, dag object gebruikt om leeftijd aan te duiden

	age.year = today.year - birthDate.year;
	age.month = today.month - birthDate.month;
	age.day	= today.day - birthDate.day;

	if(age.day < 0){
		age.month -= 1;
		//leeftijd in dagen hoeft niet precies
		//0 = jarig/maandig, niet 0 = overig
		age.day = 1;
	}

	if(age.month < 0){
		age.year -= 1;
		age.month += 12;
	}

	//invalid age:
	if (age.year < 0){
		cout << invalidInput << quitProgram << endl;
		return Date(-1, -1, -1);
	}

	//too young or too old
	if (age.year < 10 or age.year > 100) {
		cout << invalidAge << quitProgram << endl;
		return Date(-1, -1, -1);
	}

	if (age.day == 0) {
		cout << "Gefeliciteerd met uw vermaandag!" << endl;
		if (age.month == 0) {
			cout << "... en uw verjaardag!" << endl;
		}
	}

	return age;
}

//vraagt aan de gebruiker de weekdag van zijn/haar geboorte 'birthDate'
//returnt of de gebruiker het juiste antwoord gegeven heeft
bool checkBirthday(Date birthDate) {

	string answer;
	string secondSymbol;

	cout << "Om te kijken of u wel uw echte leeftijd heeft ingevuld, " << endl;
	cout << "gaan we u testen." << endl;
	cout << "Voer de eerste (kleine!) letter van de dag waarop u bent geboren in: ";
	cin >> answer;

	if (answer == "d" or answer == "z") {

		cout << "Voer de tweede letter van de dag waarop u bent geboren in: ";
		cin >> secondSymbol;
		answer += secondSymbol;

	}

	if (answer == calculateBirthday(birthDate)) {
		cout << "De opgegeven geboortedag is juist." << endl;
		return true;

	} else {
		cout << "De opgegeven geboortedag is onjuist." << endl;
        cout << "Het programma wordt afgesloten." << endl;
		return false;
	}
}

//Geeft voor het jaartal 'year' de hoeveelheid dagen die 'month' heeft.
int getDayCount(int year, int month){
	int daysPerMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if(month == 2){
        return daysPerMonth[month-1] + ((year+1) % 4 == 0 ? 1 : 0);
    } else {
		return daysPerMonth[month-1];
    }
}

//berekent de dag van de week op de gegeven datum 'birthdate'
//returnt met een 1 of 2 letter string de weekdag
string calculateBirthday(Date birthDate) {

	string weekDays[7] = { "di", "w", "do", "v", "za", "zo", "m" };
	int numberOfDays = birthDate.day - 1;
	int differenceYears = birthDate.year - 1901;
	int differenceMonths = birthDate.month - 1;
	string birthday;

	//voor elke maand vanaf 1900 tot de geboortemaand, sommeer het aantal dagen
	for (int i = 0; i < differenceYears * 12 + differenceMonths; i++) {
		numberOfDays += getDayCount(i/12, (i%12) + 1);
	};

	birthday = weekDays[numberOfDays % 7];

	return birthday;
}

//geeft de gebruiker een rekentoets
//de aanspreekwijze hangt ervan af of de gebruiker minstens 30 jaar oud is 'older'
//returnt of de gebruiker geslaagd is
bool mathTest(bool older){

	int * factors;
	int product;
	int answer;
	int diff;
	const double EPSILON = 0.1;

	factors = determineFactors();
	product = factors[0] * factors[1];

	if (older) {
		cout << "U krijgt nu een wiskundige oefening ";
		cout << "om uw vaardigheden te beproeven." << endl;
	} else {
		cout << "Laten we even kijken hoeveel ";
		cout << "breincellen er bij jou nog over zijn!" << endl;
	}

	cout << "Wat is " << factors[0] << " * " << factors[1] << ": ";
	cin >> answer;

	diff = abs(answer-product);

	if(product == 0){
		if (diff == 0)
			return true;
	} else {
		//berekent percentuele verschil van ingevuld antwoord en het juiste
		//kijk het of het lager is dan de maxwaarde (EPSILON)
		double reldiff = (double)diff / (double)product;
		if (reldiff < EPSILON){
			cout << (older ? "Uw " : "Je ") << "antwoord was goed genoeg!" << endl;
			cout << "Het exacte antwoord was " << product << endl;
			return true;
		}
	}

	//als het antwoord fout was, krijgt de gebruiker het juiste antwoord te zien
	cout << "Onjuist. Het juiste antwoord is: " << product << endl;
	return false;
}

//Geeft een literatuurvraag aan de gebruiker
//De vraag hangt ervanaf of de gebruiker ouder is dan 30 'older'
bool artTest(bool older){

	char answer;

	if (older) {
		cout << "U krijgt nu een meerkeuzevraag ";
		cout << "over kunst of literatuur." << endl;
		cout << "Wat is de echte naam van de schrijver die schreef onder het ";
		cout << "pseudoniem Multatuli?" << endl;

		cout << "A. M. Havelaar" << endl << "B. E. Douwes Dekker" << endl;
		cout << "C. J. Schmidt" << endl << "D. K. van Steijn" << endl;
	} else {
		cout << "Misschien dat deze meerkeuzevraag ";
		cout << "over kunst/literatuur je beter ligt!" << endl;
		cout << "Hoe heette het leidsche kunsttijdschrift waar Piet Mondriaan ";
		cout << "een belangrijke rol in heeft gespeeld?" << endl;

		cout << "A. De vorm" << endl << "B. De Stijl" << endl;
		cout << "C. De Kunst" << endl << "D. Leidsche Schoonheid" << endl;
	}

	cin >> answer;

	if (answer == 'B' or answer == 'b'){
		cout << "Dat is het juiste antwoord!" << endl;
		return true;
	} else {
		cout << (older ? "Uw" : "Je") << " antwoord is onjuist." << endl;
		cout << "Het juiste antwoord was B. ";
		cout << (older ? "E. Douwes Dekker." : "De Stijl.") << endl;
	}

	return false;
}

//returnt een pointer naar een array met 2 factoren voor de rekentoets
int * determineFactors(){

	static int factors[2];

	factors[0] = (rand() % 10 ? rand() % 1000 : 0);
	factors[1] = rand() % 1000;

	return factors;
}
