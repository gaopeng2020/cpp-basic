#pragma once
#include <cstdint>
class TaxStategy
{
public:
	struct InsuranceAndHousingFund
	{
		float housingFundRatio = 0.07;
		float endowmentInsuranceRatio;
		float unemploymentInsuranceRatio;
		float medicalInsuranceRatio;
		float maternityInsuranceRatio;
		float injuryInsuranceRatio;
	}insuranceAndHousingFund;

	struct SpecialAdditionalDedcution
	{
		uint16_t childrensEducationDedcution = 0;
		uint16_t adultEducationDedcution = 0;
		uint16_t treatmentForSeriousDiseaseDedcution = 0;
		uint16_t housingLoansInterestDedcution = 0;
		uint16_t HousingRentsDedcution = 0;
		uint16_t careForElderlyDedcution = 0;
	}specialAdditionalDedcution;

	virtual double calculateTax(const double amount, uint8_t mounth,
		InsuranceAndHousingFund insuranceAndHousingFund, SpecialAdditionalDedcution specialAdditionalDedcution) = 0;
	double insuranceAndHousingFundSum(const double amount);
	double specialAdditionalDedcutionSum(const double amount);


};

