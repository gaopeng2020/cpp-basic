#include "TaxStategy.h"
#include "ShangHaiTaxStategy.h"
#include <iostream>

int main() {
	TaxStategy* taxStategy = new ShangHaiTaxStategy();
	TaxStategy::InsuranceAndHousingFund insuranceHousingFund;
	insuranceHousingFund.endowmentInsuranceRatio = 0.08;
	insuranceHousingFund.housingFundRatio = 0.07;
	insuranceHousingFund.medicalInsuranceRatio = 0.02;
	insuranceHousingFund.unemploymentInsuranceRatio = 0.005;
	insuranceHousingFund.maternityInsuranceRatio = 0.0;
	insuranceHousingFund.injuryInsuranceRatio = 0.0;

	TaxStategy::SpecialAdditionalDedcution specialDedcution;
	specialDedcution.adultEducationDedcution = 0;
	specialDedcution.childrensEducationDedcution = 1000;
	specialDedcution.careForElderlyDedcution = 1000;
	specialDedcution.housingLoansInterestDedcution = 1000;
	specialDedcution.HousingRentsDedcution = 0;
	specialDedcution.treatmentForSeriousDiseaseDedcution = 0;

	double tax = taxStategy->calculateTax(30000, 12, insuranceHousingFund, specialDedcution);
	std::cout << tax << std::endl;

}